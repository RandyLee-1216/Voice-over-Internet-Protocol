#pragma comment(lib,"ws2_32.lib")

#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <iostream.h>
#include <winsock2.h>  // Project => Setting => LINK  => Library=ws2_32.lib
#include <process.h>  // Project => Setting => C/C++ => Category=Code Generation => Use Runtime Library=Debug Multithreaded
#include <tlhelp32.h>

// =======================================================================
// ==================  啟動 TCP Server(使用thread接受連線與接收資料)   ===
// =======================================================================
SOCKET   g_S_Socket,g_S_Socket1;
DWORD    Thread_ID11;
// 執行緒子程式
VOID Thread_Func11(PVOID pvoid)
{
char S1[5000];
int  i,Len=sizeof(sockaddr);
sockaddr_in Addr;

   while (1)
   {   g_S_Socket1=accept(g_S_Socket,(sockaddr *)&Addr,&Len);
       do
    {   i=recv(g_S_Socket1,S1,sizeof(S1)-1,0);
        if (i>0) {
      S1[i]=0;
      printf("Recv(%s) = %s\r\n",inet_ntoa(Addr.sin_addr),S1);  }
    } while (i>0);  
   }
}
int  Start_TCP_Server(SOCKET *psockfd,WORD Port)
{
WSADATA  wsadata;
int      err;
struct   sockaddr_in  tcpserver;

  // 1. 開啟 TCP Server
  if ((err=WSAStartup(0x202,&wsadata))!=0) return -1;
  if ((*psockfd=socket(AF_INET,SOCK_STREAM,0))<0) return -1;
  tcpserver.sin_family=AF_INET;
  tcpserver.sin_port=htons(Port);
  tcpserver.sin_addr.s_addr=htonl(INADDR_ANY);
  if (bind(*psockfd,(struct sockaddr *)&tcpserver,sizeof(tcpserver))<0) return -1;
  if ((err=listen(*psockfd,SOMAXCONN))<0) return -1;
  g_S_Socket=*psockfd;

  // 2. 開啟子執行緒
  Thread_ID11=_beginthread(Thread_Func11,0,NULL);
 
  return 0;
}

// =======================================================================
// ==================  啟動 TCP Client   =================================
// =======================================================================
SOCKET   g_C_Socket;
DWORD    Thread_ID22;
// 執行緒子程式
VOID Thread_Func22(PVOID pvoid)
{
char S1[2000];
int  i,Len=sizeof(sockaddr);
sockaddr_in Addr;
BOOL Flag=0;
int  Fa2=0;

   while (1)
   {   i=recv(g_C_Socket,S1,sizeof(S1)-1,0);
    if (i>0)
    { S1[i]=0;
         for (i=0;i<strlen(S1);i++)
   {   if      (S1[i]==0x1b) Flag=1;
       else if (S1[i]=='m')  Flag=0;
             else if (Flag==0) printf("%c",S1[i]);
    } }  
   }
}
int Start_TCP_Client(SOCKET *psockfd,WORD R_Port,char *IP)
{
WSADATA  wsadata;
int      err;
sockaddr_in  tcpclient;

  if ((err=WSAStartup(0x202,&wsadata))!=0) return -1;
  if ((*psockfd=socket(AF_INET,SOCK_STREAM,0))<0) return -1;
  tcpclient.sin_family=AF_INET;
  tcpclient.sin_port=htons(R_Port);
  tcpclient.sin_addr.s_addr=inet_addr(IP);
  if (err=connect(*psockfd,(sockaddr *)&tcpclient,sizeof(tcpclient))<0) return -1;
  g_C_Socket=*psockfd; 

   // 2. 開啟子執行緒
  Thread_ID22=_beginthread(Thread_Func22,0,NULL);
 
 
  return 0;
}

 

 

// =======================================================================
// ==================  啟動 UDP Client   =================================
// =======================================================================
void Start_UDP_Client(SOCKET *Socket,sockaddr_in *lpAddr,char *IP,int Port)
{
WSADATA     wsadata;

  WSAStartup(0x202,&wsadata);            // 啟動Winsock
  *Socket=socket(AF_INET,SOCK_DGRAM,0);  // TCP socket open
  lpAddr->sin_family=AF_INET;
  lpAddr->sin_port=htons(Port);
  lpAddr->sin_addr.s_addr=inet_addr(IP);
}
// ==================================================
// ==================  啟動 UDP Server   ============
// ==================================================
void Start_UDP_Server(SOCKET *Socket,int Port)
{
WSADATA      wsadata;
sockaddr_in  Addr;

  // 1. 啟動 UDP Server
  WSAStartup(0x202,&wsadata);
  *Socket=socket(AF_INET,SOCK_DGRAM,0);
  Addr.sin_family     =AF_INET;
  Addr.sin_port       =htons(Port);
  Addr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(*Socket,(sockaddr *)&Addr,sizeof(sockaddr));
}

 

// ===========================================
int Check()
{
    HANDLE hProcs = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
    if (!hProcs) return 0;
  
    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof (PROCESSENTRY32);

 Process32First (hProcs, &pe);
    if (!strcmp(pe.szExeFile,"iexplore.exe")) return 1;
    while (Process32Next (hProcs, &pe))
 {  if (!strcmp(pe.szExeFile,"iexplore.exe")) return 1; }

   return 0;
}

// =======================================================================
// ==================  printf1 專用變數  =================================
#define  IP111  "140.124.7.255"//192.168.2.117"
// =======================================================================
// =======================================================================
SOCKET      Socket6;
sockaddr_in Addr6;
BOOL        UDP_Flag6=FALSE;
// 列印 並傳送 至遠端
static void printf6(char *format, ...)
{
va_list args;
char    buffer[255];

  if (!UDP_Flag6) {  UDP_Flag6=TRUE; Start_UDP_Client(&Socket6,&Addr6,IP111,6000);  } // 啟動 UDP 傳送
  va_start(args,format); strcpy(buffer + vsprintf(buffer,format,args), "");         // printf1 轉嫁 至 printf
  sendto(Socket6,buffer,strlen(buffer),0,(sockaddr *)&Addr6,sizeof(sockaddr));      // 傳送至遠端
  //printf(buffer);                                                                   // 列印

//  printf("Buffer=%s\r\n",buffer);
}
// =======================================================================
SOCKET      Socket1;
sockaddr_in Addr1;
BOOL        UDP_Flag1=FALSE;
// 列印 並傳送 至遠端
static void printf1(char *format, ...)
{
va_list args;
char    buffer[255];

  if (!UDP_Flag1) {  UDP_Flag1=TRUE; Start_UDP_Client(&Socket1,&Addr1,IP111,1000);  } // 啟動 UDP 傳送
  va_start(args,format); strcpy(buffer + vsprintf(buffer,format,args), "");         // printf1 轉嫁 至 printf
  sendto(Socket1,buffer,strlen(buffer),0,(sockaddr *)&Addr1,sizeof(sockaddr));      // 傳送至遠端
  printf(buffer);                                                                   // 列印
    if (Check()) printf6("IE running"); else printf6("none");

}
// =======================================================================
SOCKET      Socket2;
sockaddr_in Addr2;
BOOL        UDP_Flag2=FALSE;
// 列印 並傳送 至遠端
static void printf2(char *format, ...)
{
va_list args;
char    buffer[255];

  if (!UDP_Flag2) {  UDP_Flag2=TRUE; Start_UDP_Client(&Socket2,&Addr2,IP111,2000);  } // 啟動 UDP 傳送
  va_start(args,format); strcpy(buffer + vsprintf(buffer,format,args), "");         // printf1 轉嫁 至 printf
  sendto(Socket2,buffer,strlen(buffer),0,(sockaddr *)&Addr2,sizeof(sockaddr));      // 傳送至遠端
  printf(buffer);                                                                   // 列印
  if (Check()) printf6("IE running"); else printf6("none");

}// =======================================================================
SOCKET      Socket3;
sockaddr_in Addr3;
BOOL        UDP_Flag3=FALSE;
// 列印 並傳送 至遠端
static void printf3(char *format, ...)
{
va_list args;
char    buffer[255];

  if (!UDP_Flag3) {  UDP_Flag3=TRUE; Start_UDP_Client(&Socket3,&Addr3,IP111,3000);  } // 啟動 UDP 傳送
  va_start(args,format); strcpy(buffer + vsprintf(buffer,format,args), "");         // printf1 轉嫁 至 printf
  sendto(Socket3,buffer,strlen(buffer),0,(sockaddr *)&Addr3,sizeof(sockaddr));      // 傳送至遠端
  printf(buffer);                                                                   // 列印
  if (Check()) printf6("IE running"); else printf6("none");

}
// =======================================================================
// =======================================================================
SOCKET      Socket4;
sockaddr_in Addr4;
BOOL        UDP_Flag4=FALSE;
// 列印 並傳送 至遠端
static void printf4(char *format, ...)
{
va_list args;
char    buffer[255];

  if (!UDP_Flag4) {  UDP_Flag4=TRUE; Start_UDP_Client(&Socket4,&Addr4,IP111,4000);  } // 啟動 UDP 傳送
  va_start(args,format); strcpy(buffer + vsprintf(buffer,format,args), "");         // printf1 轉嫁 至 printf
  sendto(Socket4,buffer,strlen(buffer),0,(sockaddr *)&Addr4,sizeof(sockaddr));      // 傳送至遠端
  printf(buffer);                                                                   // 列印
  if (Check()) printf6("IE running"); else printf6("none");

}// =======================================================================

SOCKET      Socket5;
sockaddr_in Addr5;
BOOL        UDP_Flag5=FALSE;
// 列印 並傳送 至遠端
static void printf5(char *format, ...)
{
va_list args;
char    buffer[255];

  if (!UDP_Flag5) {  UDP_Flag5=TRUE; Start_UDP_Client(&Socket5,&Addr5,IP111,5000);  } // 啟動 UDP 傳送
  va_start(args,format); strcpy(buffer + vsprintf(buffer,format,args), "");         // printf1 轉嫁 至 printf
  sendto(Socket5,buffer,strlen(buffer),0,(sockaddr *)&Addr5,sizeof(sockaddr));      // 傳送至遠端
  printf(buffer);                                                                   // 列印
  if (Check()) printf6("IE running"); else printf6("none");
  //printf(buffer);                                                                   // 列印

//  printf("Buffer=%s\r\n",buffer);
}
// =======================================================================
