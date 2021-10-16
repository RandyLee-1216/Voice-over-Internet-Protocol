#pragma comment(lib,"ws2_32.lib")

// 設定 Winsock Library
// [Project] => [Setting] => [LINK] => [Object/Library] => ws2_32.lib

#include "winsock2.h"
char API_ErrMsg[256];
void WSA_Get_ErrTxt(char *S1) {  }
int  Start_TCP_Server(SOCKET *psockfd,WORD Port,DWORD EVENT,HWND Hwnd)
{
WSADATA  wsadata;
int      err;
struct   sockaddr_in  tcpserver;

  // 啟動Winsock
  if ((err=WSAStartup(0x202,&wsadata))!=0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
 
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_STREAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

  // Set Asynchronized Mode
  if ((err=WSAAsyncSelect(*psockfd,Hwnd,EVENT,FD_ACCEPT|FD_WRITE|FD_READ|FD_CLOSE))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

  // Socket Naming
  tcpserver.sin_family=AF_INET;
  tcpserver.sin_port=htons(Port);
  tcpserver.sin_addr.s_addr=htonl(INADDR_ANY);
  if (bind(*psockfd,(struct sockaddr *)&tcpserver,sizeof(tcpserver))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

  // Wait for Client Login
  if ((err=listen(*psockfd,SOMAXCONN))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

   return 0;
}

int Start_TCP_Client(SOCKET *psockfd,WORD L_Port,WORD R_Port,char *IP,DWORD EVENT,HWND Hwnd)
{
WSADATA  wsadata;
int      err,i,on=1;
struct   sockaddr_in  tcpclient,Addr;
LPHOSTENT HostIP;
char      IP11[100];

  // 啟動Winsock
  if ((err=WSAStartup(0x202,&wsadata))!=0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
 
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_STREAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

//  setsockopt(*psockfd,IPPROTO_TCP,TCP_NODELAY,(char *)&on,sizeof(on));
  // Set Asynchronized Mode
  if ((err=WSAAsyncSelect(*psockfd,Hwnd,EVENT,FD_CONNECT|FD_WRITE|FD_READ|FD_CLOSE/*|FD_ADDRESS_LIST_CHANGE|FD_ROUTING_INTERFACE_CHANGE*/))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1;  }

  // Check IP or Domain Name,
  for (i=0;i<(int)strlen(IP);i++) if ( ((IP[i]<'0')||(IP[i]>'9')) && (IP[i]!='.') ) break;
  if (i<(int)strlen(IP))
  {  if ((HostIP=gethostbyname(IP))==NULL)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
     else strcpy(IP11,inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));  }
  else strcpy(IP11,IP);

  // Socket Naming
  tcpclient.sin_family=AF_INET;
  tcpclient.sin_port=htons(R_Port);
  tcpclient.sin_addr.s_addr=inet_addr(IP11);

  // Socket Naming
  Addr.sin_family=AF_INET;
  Addr.sin_port=htons(L_Port);
  Addr.sin_addr.s_addr=htonl(INADDR_ANY);

  // Set Local's IP/Port
  if (0)//L_Port>100)
  {  if (bind(*psockfd,(struct sockaddr *)&Addr,sizeof(sockaddr))<0)
     {  //int i=WSAGetLastError();
     WSA_Get_ErrTxt(API_ErrMsg); return -1; }
  }
  // else
  // {  Automatically Assign Local Port }

  // Connect to Server
  if (err=connect(*psockfd,(struct sockaddr *)&tcpclient,sizeof(tcpclient))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg);  return -1; }

  return 0;
}

 

int Start_UDP_Server(SOCKET *psockfd,WORD Port,DWORD EVENT,HWND Hwnd)
{
WSADATA  wsadata;
int      err;
struct   sockaddr_in  udpserver;

  // 啟動Winsock
  if ((err=WSAStartup(0x202,&wsadata))!=0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
 
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -2; }

  // Set Asynchronized Mode
  if ((err=WSAAsyncSelect(*psockfd,Hwnd,EVENT,FD_WRITE|FD_READ))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -3;  }

  // Socket Naming
  udpserver.sin_family=AF_INET;
  udpserver.sin_port=htons(Port);
  udpserver.sin_addr.s_addr=htonl(INADDR_ANY);
  if (bind(*psockfd,(struct sockaddr *)&udpserver,sizeof(udpserver))<0)
  {  err=WSAGetLastError();
  WSA_Get_ErrTxt(API_ErrMsg); return -4; }

  return 0;
}
int Start_UDP_Client(SOCKET *psockfd,sockaddr_in *udpclient,WORD L_Port,WORD R_Port,char *IP,DWORD EVENT,HWND Hwnd)
{
WSADATA     wsadata;
int         err,i;
sockaddr_in Addr;
LPHOSTENT   HostIP;
char      IP11[100];

  // 啟動Winsock
  if ((err=WSAStartup(0x202,&wsadata))!=0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
 
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

    // Set Asynchronized Mode
  if ((err=WSAAsyncSelect(*psockfd,Hwnd,EVENT,FD_READ|FD_WRITE))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1;  }

  // Check IP or Domain Name,
  for (i=0;i<(int)strlen(IP);i++) if ( ((IP[i]<'0')||(IP[i]>'9')) && (IP[i]!='.') ) break;
  if (i<(int)strlen(IP))
  {  if ((HostIP=gethostbyname(IP))==NULL)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
     else strcpy(IP11,inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));  }
  else strcpy(IP11,IP);

  // Socket Naming
  udpclient->sin_family=AF_INET;
  udpclient->sin_port=htons(R_Port);
  udpclient->sin_addr.s_addr=inet_addr(IP11);

  // Socket Naming
  Addr.sin_family=AF_INET;
  Addr.sin_port=htons(L_Port);
  gethostname(IP11,sizeof(IP11)-1);
  if ((HostIP=gethostbyname(IP11))==NULL) {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
  else strcpy(IP11,inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));
  Addr.sin_addr.s_addr=inet_addr(IP11);
  //Addr.sin_addr.s_addr=htonl(INADDR_ANY);

  // Set Local's IP/Port
  if (L_Port>100)
  {  if (bind(*psockfd,(struct sockaddr *)&Addr,sizeof(sockaddr))<0)
     {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
  }
  // else
  // {  Automatically Assign Local Port }

  return 0;
}
