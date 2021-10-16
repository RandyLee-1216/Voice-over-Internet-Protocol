// ###################################################
struct DATA
{
BOOL  Flag;
unsigned char Protocol;
unsigned char SIP[4],DIP[4];
unsigned char SPort[2],DPort[2];
int           UPKT_Num,UPKT_Size;
int           DPKT_Num,DPKT_Size;
int           TPKT_Num,TPKT_Size;
};
#define MAX_DATA  1000
DATA    Data[MAX_DATA];
void Init_Data()
{ 
int i;
    for (i=0;i<MAX_DATA;i++) 
 {   Data[i].Flag=0;
     Data[i].UPKT_Num=Data[i].UPKT_Size=0;
     Data[i].DPKT_Num=Data[i].DPKT_Size=0;
     Data[i].TPKT_Num=Data[i].TPKT_Size=0;
 }
}
int Search_Data(IP *lpIp,int Len)
{
int  i;
unsigned char *lp;
TCP           *lpTcp;
UDP           *lpUdp;
unsigned char *SPort,*DPort;
 
     lp=(unsigned char *)lpIp; lpTcp=(TCP *)(lp+20); lpUdp=(UDP *)(lp+20);
     if      (lpIp->Protocol==0x06) { SPort=lpTcp->SPort; DPort=lpTcp->DPort; }
     else if (lpIp->Protocol==0x11) { SPort=lpUdp->SPort; DPort=lpUdp->DPort; }

     for (i=0;i<MAX_DATA;i++)
  {   if ( (Data[i].Flag) && (Data[i].Protocol==lpIp->Protocol) )
      {  if      ( (!memcmp(Data[i].SIP,  lpIp->SIP,4)) && (!memcmp(Data[i].DIP,lpIp->DIP,4)) &&
                (!memcmp(Data[i].SPort,SPort,2))     && (!memcmp(Data[i].DPort,DPort,2))  )
         {  Data[i].UPKT_Num++; Data[i].UPKT_Size+=Len; 
            Data[i].TPKT_Num++; Data[i].TPKT_Size+=Len; break; }
      else if ( (!memcmp(Data[i].SIP,  lpIp->DIP,4)) && (!memcmp(Data[i].DIP,lpIp->SIP,4)) &&
                (!memcmp(Data[i].SPort,DPort,2))     && (!memcmp(Data[i].DPort,SPort,2))  )
         {  Data[i].DPKT_Num++; Data[i].DPKT_Size+=Len; 
            Data[i].TPKT_Num++; Data[i].TPKT_Size+=Len; break; }
  }   }
  if (i<MAX_DATA) return i; else return -1;
}
int Insert_Data(IP *lpIp,int Len)
{
int  i;
unsigned char *lp;
TCP           *lpTcp;
UDP           *lpUdp;
unsigned char *SPort,*DPort;
 
     lp=(unsigned char *)lpIp; lpTcp=(TCP *)(lp+20); lpUdp=(UDP *)(lp+20);
     if      (lpIp->Protocol==0x06) { SPort=lpTcp->SPort; DPort=lpTcp->DPort; }
     else if (lpIp->Protocol==0x11) { SPort=lpUdp->SPort; DPort=lpUdp->DPort; }

     for (i=0;i<MAX_DATA;i++) if (!Data[i].Flag) break;
  if (i<MAX_DATA)
  {  Data[i].Flag=1;
     Data[i].Protocol=lpIp->Protocol;
  Data[i].UPKT_Num =Data[i].TPKT_Num=1;    Data[i].DPKT_Num=0;
     Data[i].UPKT_Size=Data[i].TPKT_Size=Len; Data[i].DPKT_Size=0;
  memcpy(Data[i].SIP,lpIp->SIP,4);
        memcpy(Data[i].DIP,lpIp->DIP,4);
        memcpy(Data[i].SPort,SPort,2);
     memcpy(Data[i].DPort,DPort,2);
  return i;
  }
  else return -1;
}
