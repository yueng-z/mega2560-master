

#include "board.h"
#include "esp32.h"
#include "timer.h"
#include <stdio.h>
#include "queue.h"
#include "string.h"
#include <stdio.h>
#include <iom2560v.h>
#include <macros.h>

#define MAX_ESP_WAIT_TIME 10000
#define ESP_RCV_BUF_MAXLEN 48
#define SERVER_IP   "10.0.0.3"
#define SERVER_PORT   "8080"


QUEUE_ALLOCATE_DATA(uart2Queue, 64);
Queue esp32RcvQueue;
typedef enum {
	ESP_RST,
	ESP_RST_DELAY,
	ESP_RESTORE,
    ESP_WAIT_RESTORE_RESPONSE,   
    ESP_WAIT_ATE0_RESPONSE,
	ESP_SETED_STATION_MODE,
	ESP_WAITING_AP_RESPONSE,
	ESP_CONNECTING_SERVER,
	ESP_INTO_TRAN_MODE,
	ESP_START_TRAN,
    ESP_ERROR,
} ESP_Status;

ESP_Status esp32Status;
U16 esp32ComuTimer, esp32StatusTimer;
char rcvdOnePackage = false;

char esp32RcvBuf[ESP_RCV_BUF_MAXLEN];
char esp32RcvdLen = 0;

//UART2 initialize
// desired baud rate:115200
// actual baud rate: (0.2%)
void uart2_init(void)
{
	UCSR2B = 0x00; // disable while setting baud rate
	UCSR2A = 0x02;
	UCSR2C = 0x06;
	UBRR2L = 0x10; // set baud rate lo
	UBRR2H = 0x00; // set baud rate hi
	UCSR2B = 0x98;
}

void esp32Init(void)
{
	DDRL |=(1<<1);
	PORTL|=(1<<1);  //enable the ESP32
	uart2_init();   //115200 baudrate
	esp32RcvQueue = QUEUE_INITIALISE_DATA(uart2Queue, 64);
	memset(esp32RcvBuf, 0, ESP_RCV_BUF_MAXLEN);
	esp32RcvdLen = 0;
	esp32StatusTimer = Timer_GetTickCount();
	esp32ComuTimer = Timer_GetTickCount();
}

#pragma interrupt_handler uart2_rx_isr:iv_USART2_RXC
void uart2_rx_isr(void)
{
 	char data = UDR2;
#if ESP32_DEBUG
	UDR0 = data;
#else
    if(!Queue_IsFull(esp32RcvQueue))
	{
		Queue_Push(esp32RcvQueue, data);
	}
#endif
	
}

static unsigned char checkIfReceivePackage(void) {
    if (esp32RcvdLen > 2
        && esp32RcvBuf[esp32RcvdLen - 2] == '\r'
        && esp32RcvBuf[esp32RcvdLen - 1] == '\n') {
        return 1;
    }
    return 0;
}


void uart2SendBytes(char * ptr, char len)
{
	while(len--){
		UDR2 = *ptr++;
		while (!(UCSR2A & (1 << 5)));
	}
}
void uart2SendStr(char * ptr)
{
	while(*ptr){
		UDR2 = *ptr++;
		while (!(UCSR2A & (1 << 5)));
	}
}

void esp32RcvPackage(void)
{
	char data;
	if(!Timer_PassedDelay(esp32ComuTimer,10)) return;
	esp32ComuTimer = Timer_GetTickCount();
	CLI();
	while(!rcvdOnePackage &&  !Queue_IsEmpty(esp32RcvQueue)){
		if (esp32RcvdLen < ESP_RCV_BUF_MAXLEN) {
			data = Queue_Pop(esp32RcvQueue);
			if(data == '\r'){
				if('\n' == Queue_Pop(esp32RcvQueue)) {
					rcvdOnePackage = true;
					esp32RcvBuf[esp32RcvdLen] = '\0';
				}
			}else{
        		esp32RcvBuf[esp32RcvdLen] = data;
        		esp32RcvdLen++;
			}
    	}else{
			while(1); //fault
		}
	}
	SEI();
}

static void parseResult(void) {
    unsigned char sendBuf[6] = {0};
    switch (esp32Status) {
		case ESP_RESTORE:
			if (strstr(esp32RcvBuf, "ready") != 0) {//  
            	esp32RcvdLen = 0;
            	memset(esp32RcvBuf, 0, ESP_RCV_BUF_MAXLEN);
				uart2SendStr("AT+RESTORE\r\n");  // turn off echo
				esp32Status = ESP_WAIT_RESTORE_RESPONSE;
			}
		break;
		case ESP_WAIT_RESTORE_RESPONSE:
			if (strstr(esp32RcvBuf, "ready") != 0) {//           
                printf("0.restored!\r\n");
                esp32Status = ESP_WAIT_ATE0_RESPONSE;
                uart2SendBytes("ATE0\r\n", 6);  // turn off echo                
            }           
            break;
        case ESP_WAIT_ATE0_RESPONSE:
            if (strstr(esp32RcvBuf, "OK") != 0) {//Echo is closed              
                printf("1.Echo off\r\n");
                esp32Status = ESP_SETED_STATION_MODE;
                uart2SendStr("AT+CWMODE=1\r\n");
            }
            break;
		case ESP_SETED_STATION_MODE:
		        if (strstr(esp32RcvBuf, "OK") != 0) {// set work mode OK           
                printf("2. work in Station mode\r\n");
                esp32Status = ESP_WAITING_AP_RESPONSE;
                uart2SendStr("AT+CWJAP=\"NETGEAR17\",\"lvyi1599\"\r\n");
            }
		break;

		case ESP_WAITING_AP_RESPONSE:
				if (strstr(esp32RcvBuf, "WIFI CONNECTED") != 0) {// connected AP       
					printf("--WIFI CONNECTED\r\n");
				} else if (strstr(esp32RcvBuf, "WIFI GOT IP") != 0) {// connected AP       
					printf("--WIFI GOT IP\r\n");
				}else if (strstr(esp32RcvBuf, "ERROR") != 0) {// connected AP       
                	esp32Status = ESP_ERROR;
                	esp32StatusTimer = Timer_GetTickCount();
				}else if (strstr(esp32RcvBuf, "OK") != 0) {// connected AP     
				    printf("3.Connected AP\r\n");  
					esp32Status = ESP_CONNECTING_SERVER;//
					uart2SendBytes("AT+CIPSTART=\"TCP\",\"", 19);
					uart2SendBytes( SERVER_IP, strlen(SERVER_IP));
					uart2SendBytes( "\",", 2);
					uart2SendBytes( SERVER_PORT, strlen(SERVER_PORT));
					uart2SendBytes( "\r\n", 2);	
				}
		break;

        case ESP_CONNECTING_SERVER:
            if (strstr( esp32RcvBuf, "CONNECT") != 0) {
				printf("4-1.CONNECT!\r\n");
            } else if(strstr(esp32RcvBuf, "ERROR") != 0) {
                esp32Status = ESP_ERROR;
                esp32StatusTimer = Timer_GetTickCount();
				printf("4.connecting server failed\r\n");
            } else if(strstr(esp32RcvBuf, "OK")){
				printf("4-2.CONNECT server!\r\n");
				esp32Status = ESP_INTO_TRAN_MODE;
				uart2SendStr("AT+CIPMODE=1\r\n");
			}
            break;
		case ESP_INTO_TRAN_MODE:
			if (strstr(esp32RcvBuf, "OK") != 0) {//Echo is closed              
                printf("5.Enter tran mode\r\n");
                esp32Status = ESP_START_TRAN;
                uart2SendStr("AT+CIPSEND\r\n");
                esp32StatusTimer = Timer_GetTickCount();
            }
			break;

		case ESP_START_TRAN:
			break;


    }
    esp32RcvdLen = 0;
    memset(esp32RcvBuf, 0, ESP_RCV_BUF_MAXLEN);
}



void esp32Process(void)
{
	esp32RcvPackage();
	switch(esp32Status)
	{
		case ESP_RST:
			PORTL&=~(1<<1);  //disable the ESP32	
			memset(esp32RcvBuf, 0, ESP_RCV_BUF_MAXLEN);
			esp32RcvdLen = 0;
			esp32StatusTimer = Timer_GetTickCount();
			esp32ComuTimer = Timer_GetTickCount();
			esp32Status = ESP_RST_DELAY;
		break;

		case ESP_RST_DELAY:
			if(Timer_PassedDelay(esp32StatusTimer, 100)){
				esp32StatusTimer = Timer_GetTickCount();	
				PORTL|=(1<<1); 	
				esp32Status = ESP_RESTORE;
			}
		break;

		case ESP_RESTORE:
		case ESP_WAIT_RESTORE_RESPONSE:
        case ESP_WAIT_ATE0_RESPONSE:
        case ESP_SETED_STATION_MODE:
        case ESP_WAITING_AP_RESPONSE:
		case ESP_CONNECTING_SERVER:
		case ESP_INTO_TRAN_MODE:

            if (Timer_PassedDelay(esp32StatusTimer, MAX_ESP_WAIT_TIME)) {
                esp32Status = ESP_ERROR;  // Timeout
				printf("ESP32 connect server failed!\r\n");
                esp32StatusTimer = Timer_GetTickCount();
            }
			if(!rcvdOnePackage) return;
			esp32StatusTimer = Timer_GetTickCount();
			//printf(esp32RcvBuf);
			parseResult();
			rcvdOnePackage = false;
            break;
		case ESP_ERROR: 
			if(Timer_PassedDelay(esp32StatusTimer, 1000)){
				esp32StatusTimer = Timer_GetTickCount();				
			}
		default: break;

	}
}


#if 0
        case ESP_CHECK_CONNECT_STATUS:
            if (strstr(receiveBuf, "STATUS:3") != 0 //3: The ESP32 Station has created a TCP or UDP transmission
                || strstr((char const *) receiveBuf, "STATUS:1") != 0) {//1: The ESP32 station is idle.
                esp32Status = ESP_LOGIN;
                sendBuf[0] = 0x01;
                sendBuf[1] = 0x02;
                sendBuf[2] = deviceId >> 8;
                sendBuf[3] = deviceId;
                esp32SendWifiData(sendBuf, 4);
            } else if (strstr((char const *) receiveBuf, "STATUS:5") != 0) {
                esp32Status = ESP_IDLE;   //not connected AP 
                esp32StatusTimer = Timer_GetTickCount();
				printf("No Wifi!\r\n");
            } else {
                esp32Status = ESP_CONNECT_TO_SERVER;//connected AP, no tcp
                uart2SendBytes("AT+CIPSTART=\"TCP\",\"", 19);
                uart2SendBytes( SERVER_IP, strlen(SERVER_IP));
                uart2SendBytes( "\",", 2);
                uart2SendBytes( SERVER_PORT, strlen(SERVER_PORT));
                uart2SendBytes( "\r\n", 2);				
            }
            break;



        case ESP_LOGIN:
        case ESP_WAIT_WIFI_DATA:
            if (receiveBufLength >= 8 && strstr((char const *) receiveBuf, "+IPD")) {
                wifiData = parseWifiData(receiveBuf + 7, &wifiDataLength);
                //接收到数据
                if (status == ESP_LOGIN && wifiData[0] == 0x02 && wifiData[1] == 0x01 && wifiData[2] == 0x01) {
                    status = ESP_WAIT_WIFI_DATA;
                    clear_screen();
                    display_GB2312_string(0, 2, "登录成功", 0);
                } else if (status == ESP_WAIT_WIFI_DATA && wifiData[0] == 0x03) {
                    flashLedByIndex(12);
                    unlockDoor();
                    clear_screen();
                    display_GB2312_string(0, 2, "开锁成功", 0);
                }
            } else if (strstr((char const *) receiveBuf, "CLOSE") != 0) {
                status = ESP_INIT;
            }
            break;

        case ESP_INTO_STATION_MODE:
            if (strstr((char const *) receiveBuf, "OK") != 0) {
                status = ESP_ENABLE_AUTO_CONNECT_AP;
                sendBytes(USART3, "AT+CWAUTOCONN=1\r\n", 17);//使能上电自动连接ap
            } else {
                sendBytes(USART3, "AT+CWMODE=1\r\n", 13);//重复发送进入Station mode命令
            }
            break;

        case ESP_ENABLE_AUTO_CONNECT_AP:
            if (strstr((char const *) receiveBuf, "OK") != 0) {
                status = ESP_STOP_SMART_CONFIG;
                sendBytes(USART3, "AT+CWSTOPSMART\r\n", 16);// 先释放一下ESP32内部快连所占内存
            } else {
                sendBytes(USART3, "AT+CWAUTOCONN=1\r\n", 17);//重复发送使能上电自动连接ap命令
            }
            break;

        case ESP_STOP_SMART_CONFIG:
            if (strstr((char const *) receiveBuf, "OK") != 0) {
                status = ESP_START_SMART_CONFIG;
                sendBytes(USART3, "AT+CWSTARTSMART=3\r\n", 19);// ESP-Touch和Airkiss智能配网
            } else {
                sendBytes(USART3, "AT+CWSTOPSMART\r\n", 16);// 完成配网,释放ESP32内部快连所占内存
            }
            break;

        case ESP_START_SMART_CONFIG:
            if (strstr((char const *) receiveBuf, "WIFI GOT IP") != 0) {//配网成功
                clear_screen();
                display_GB2312_string(0, 2, "配网成功", 0);
                status = ESP_INIT;//重新初始化
                sendBytes(USART3, "AT+CWSTOPSMART\r\n", 16);// 完成配网,释放ESP32内部快连所占内存
            }
            break;

#endif





