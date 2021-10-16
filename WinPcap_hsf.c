#define HAVE_REMOTE
#pragma comment(lib,"lib/wpcap.lib")
#pragma comment(lib,"lib/packet.lib")
#include "process.h"
#include "include/pcap.h"
struct MAC // MAC[14]
{
unsigned char  DMAC[6];
unsigned char  SMAC[6];
unsigned char  Type[2];
};
struct IP // IP(20)
{
unsigned char  Ver[2],Len[2];
unsigned char  ID[2], Frag[2];
unsigned char  Time,Protocol,Check[2];
unsigned char  SIP[4],DIP[4];
};
struct TCP // TCP(20)
{
unsigned char  SPort[2],DPort[2];
unsigned char  Seq[4],Ack[4];
unsigned char  Offset,Flag11,Window[2];
unsigned char  Check[2],Urgent[2];
};
struct UDP // UDP(8)
{
unsigned char  SPort[2],DPort[2];
unsigned char  Len[2],  Check[2];
};

VOID Fun(PVOID p)
{
char SS1[2000];
int i,res,Len;
pcap_t *fp;
struct pcap_pkthdr *header;
const u_char *pkt_data;
MAC   *lpMac;
IP    *lpIp;
TCP   *lpTcp;
UDP   *lpUdp;

 fp=(pcap_t *)p;
 while((res=pcap_next_ex(fp,&header, &pkt_data)) >= 0)
 { if (header->caplen>54)
     {  lpIp=(IP *)&pkt_data[14];
        lpTcp=(TCP *)&pkt_data[34];
        if ( (lpIp->Protocol==0x06) &&
       ((lpTcp->DPort[1]==21)||(lpTcp->SPort[1]==21)) )
     {  for (i=54;i<header->caplen;i++) TRACE("%c",pkt_data[i]); TRACE("\r\n"); }
     }
 }
}


void Start_Winpcap()
{
pcap_if_t *alldevs, *d;
pcap_t *fp;
u_int inum, i=0;
char errbuf[PCAP_ERRBUF_SIZE];
int res,Len;
struct pcap_pkthdr *header;
const u_char *pkt_data;
char  S1[2000];

  /* The user didn't provide a packet source: Retrieve the local device list */
  pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf);
  d=alldevs;
  while (d!=0)
  {   sprintf(S1,"Name=%s  Description=%s Next=%lx\r\n",d->name,d->description,d->next);
            //UpdateData(1); m_Edit1+=S1; UpdateData(0);
   d=d->next;  }

  inum=2;
  // Free the device list
  //pcap_freealldevs(alldevs);
  
  // Jump to the selected adapter
  for (d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
  
  // Open the device
  fp= pcap_open(d->name,100, //snaplen
       PCAP_OPENFLAG_PROMISCUOUS, //flags
       20, //read timeout
       NULL, // remote authentication
       errbuf);

  _beginthread(Fun,0,(void *)fp);
 
}
