#include   <Winsock2.h>
#pragma comment (lib,"ws2_32.lib")
int Start_NDIS(SOCKET *psockfd,WORD Port,DWORD EVENT,HWND Hwnd)
{
char     API_ErrMsg[200];
WSADATA  wsadata;
int      err;
struct   sockaddr_in  udpserver;
BOOL     optval = TRUE;
DWORD    dwBufferLen[10], dwBufferInLen = 1, dwBytesReturned = 0;

  // ±Ò°ÊWinsock
  if ((err=WSAStartup(0x202,&wsadata))!=0)
  {  strcpy(API_ErrMsg,"UDP Server WSAStartup(.) Error !\r\n"); return -1; }

  if ((*psockfd = WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED))==INVALID_SOCKET)
  //if ((*psockfd = WSASocket(AF_INET, SOCK_RAW, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED))==INVALID_SOCKET)
  {  strcpy(API_ErrMsg,"WSASocket() failed: ");  return -1;  }

  // Set Asynchronized Mode
  if ((err=WSAAsyncSelect(*psockfd,Hwnd,EVENT,FD_READ|FD_WRITE))<0)
  {  strcpy(API_ErrMsg,"UDP Server WSAAsyncSelect(.) Error !\r\n"); return -1;  }

  // Socket Naming
  char S1[100];
  hostent *HostIP;
  gethostname(S1,sizeof(S1)-1);
  HostIP=gethostbyname(S1);
  udpserver.sin_family=AF_INET;
  udpserver.sin_port=htons(Port);
  udpserver.sin_addr.s_addr=inet_addr(inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));
  if (bind(*psockfd,(struct sockaddr *)&udpserver,sizeof(udpserver))<0)
  {  strcpy(API_ErrMsg,"UDP Server bind(.) Error !\r\n"); return -1; }

  if (WSAIoctl(*psockfd, _WSAIOW(IOC_VENDOR,1),//SIO_RCVALL,
            &dwBufferInLen, sizeof(dwBufferInLen),
            &dwBufferLen, sizeof(dwBufferLen),
      &dwBytesReturned , NULL , NULL) == SOCKET_ERROR)
  {  strcpy(API_ErrMsg,"WSAIoctl()"); return -1; }

  return 0;
}
