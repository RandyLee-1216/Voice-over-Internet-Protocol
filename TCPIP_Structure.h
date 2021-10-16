// @@@@@@@@@@@@@@@@@@@@@@@@@ TCPIP @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
struct MAC // MAC[14]
{
unsigned char  DMAC[6];
unsigned char  SMAC[6];
unsigned char  Type[2];
};
struct PPP // PPP[8]
{
unsigned char Ver,Code,ID[2];
unsigned char Len[2];
unsigned char Type[2]; // IPv4=0021
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
struct RTP // RTP(12)
{
unsigned char  Ver,PLT;
unsigned char  Seq[2],TS[4];
unsigned char  SSRC[4];
unsigned char  Payload[128];
};
