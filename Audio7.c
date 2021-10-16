// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//[Project] => [Settings] => [LINK] => Library=winmm.lib

#include <mmsystem.h>

#define BUF_NUM  8
#define BUF_LEN  160 // 10ms*k

    short        RSBuf[BUF_NUM][BUF_LEN],PSBuf[BUF_NUM][BUF_LEN];
	WAVEFORMATEX WaveFormatex;
	WAVEHDR      RWaveHdr[BUF_NUM],PWaveHdr[BUF_NUM];
	HWAVEIN      RWaveIn;
	HWAVEOUT     PWaveOut;

	FILE         *in1,*out1,*out2;  // 檔案 Header
	char         ErrMsg[100]; // 錯誤訊息顯示
    int          Audio_Out_ID,Audio_In_ID; // 音效卡代號
    int          RFlag,PFlag,RStart,PStart,PCursor; // 旗標使用


	int Open_Record(HWND m_hWnd);
	int Start_Record();
	int Stop_Record();
	int Close_Record();
	int Open_Play(HWND m_hWnd);
	int Start_Play();
	int Stop_Play();
	int Close_Play();

int Open_Record(HWND m_hWnd)
{
int i,j;

    // 設定音效卡參數
 	WaveFormatex.wFormatTag=WAVE_FORMAT_PCM;
    WaveFormatex.nChannels=1; // 1:Mono, 2:Stereo
    WaveFormatex.nSamplesPerSec= 8000;
    WaveFormatex.nAvgBytesPerSec=16000;
    WaveFormatex.nBlockAlign=(WORD)2;
    WaveFormatex.wBitsPerSample=(WORD)16;
   	
	// 設定錄音參數
    if (i=waveInOpen(&RWaveIn,Audio_In_ID,&WaveFormatex,(DWORD)m_hWnd,NULL,CALLBACK_WINDOW))
    {  waveInGetErrorText(i,ErrMsg,100); 
	   //UpdateData(TRUE); m_Edit1+=ErrMsg; m_Edit1+="\r\n"; UpdateData(FALSE);
	   return -1; }
	i=sizeof(HWAVEIN);
    if (i=waveInReset(RWaveIn))
    {  waveInGetErrorText(i,ErrMsg,100); return -1; }
	for (j=0;j<BUF_NUM;j++)
	{   RWaveHdr[j].dwBufferLength=BUF_LEN*2;
        RWaveHdr[j].dwBytesRecorded=BUF_LEN*2;
        RWaveHdr[j].dwFlags=0L;
        RWaveHdr[j].dwLoops=0L;
        RWaveHdr[j].lpData=(char *)&RSBuf[j][0]; 
        if (i=waveInPrepareHeader(RWaveIn,&RWaveHdr[j],sizeof(WAVEHDR)))
		{  waveInGetErrorText(i,ErrMsg,100); return -1; } 
	}
	return 0;
}
int Open_Play(HWND m_hWnd)
{
int i,j;

    // 設定音效卡參數
 	WaveFormatex.wFormatTag=WAVE_FORMAT_PCM;
    WaveFormatex.nChannels=1; // 1:Mono, 2:Stereo
    WaveFormatex.nSamplesPerSec= 8000;
    WaveFormatex.nAvgBytesPerSec=16000;
    WaveFormatex.nBlockAlign=(WORD)2;
    WaveFormatex.wBitsPerSample=(WORD)16;
   	
  	// 設定放音參數
    if (i=waveOutOpen(&PWaveOut,Audio_Out_ID,&WaveFormatex,(DWORD)m_hWnd,NULL,CALLBACK_WINDOW))
    {  waveOutGetErrorText(i,ErrMsg,100); return -1; }
    if (i=waveOutReset(PWaveOut))
    {  waveOutGetErrorText(i,ErrMsg,100); return -1; }
	for (j=0;j<BUF_NUM;j++)
	{   PWaveHdr[j].dwBufferLength=BUF_LEN*2;
        PWaveHdr[j].dwBytesRecorded=BUF_LEN*2;
        PWaveHdr[j].dwFlags=0L;
        PWaveHdr[j].dwLoops=j;
        PWaveHdr[j].lpData=(char *)&PSBuf[j][0]; 
        if (i=waveOutPrepareHeader(PWaveOut,&PWaveHdr[j],sizeof(WAVEHDR)))
		{  waveOutGetErrorText(i,ErrMsg,100); return -1; } 
	}
	return 0;
}

int Close_Record()
{int  i;

    // 關閉錄音裝置
    if (i=waveInStop(RWaveIn))
    {  waveInGetErrorText(i,ErrMsg,100); return -1; }
    if (i=waveInReset(RWaveIn))
    {  waveInGetErrorText(i,ErrMsg,100); return -1; }
    if (i=waveInClose(RWaveIn))
    {  waveInGetErrorText(i,ErrMsg,100); return -1; }
	return 0;
}

int Close_Play()
{int  i;

	// 關閉放音裝置
    if (i=waveOutReset(PWaveOut))
    {  waveOutGetErrorText(i,ErrMsg,100); return -1; }
    if (i=waveOutClose(PWaveOut))
    {  waveOutGetErrorText(i,ErrMsg,100); return -1; }
	return 0;
}

int Start_Record()
{
int i,Flag;
char S1[100];
    // Start up Record
	for (Flag=0;Flag<BUF_NUM;Flag++)
	{   if (i=waveInAddBuffer(RWaveIn,&RWaveHdr[Flag],sizeof(WAVEHDR))) 
	    {  waveInGetErrorText(i,ErrMsg,100); 
	       sprintf(S1,"Err Code=%d ",i);
	       //UpdateData(TRUE); m_Edit1+=S1; m_Edit1+=ErrMsg; m_Edit1+="\r\n"; UpdateData(FALSE);
	       return -1; }  }
	waveInStart(RWaveIn);
    RFlag=0; RStart=1;
	return 0;
}

int Start_Play()
{
int i,Flag;
	
    // Start up Play
	for (Flag=0;Flag<BUF_NUM;Flag++)
	{   if (i=waveOutWrite(PWaveOut,&PWaveHdr[Flag],sizeof(WAVEHDR))) 
	    {  waveOutGetErrorText(i,ErrMsg,100); return -1; }  }
	PFlag=0; PStart=1; PCursor=0;
	return 0;
}

int Stop_Record()
{
    RStart=0;
	return 0;
}

int Stop_Play()
{
    PStart=0;
	return 0;
}

// ##########################################################
#include      <math.h>
//#include      <mmsystem.h>
#define       Sample_Rate   8000
#define       DT_AMPLITUDE  15000
#define       DT_DUR_LEN    32000 // /8000 Sample/1 Channel=4 Second

	int        USB_PlayTone(int Freq1, int Freq2,int OnTime,int OffTime,BOOL Enable,HWND m_hWnd);
	int        USB_Increase_Volume();
	int        USB_Decrease_Volume();


	// ===== Extend API Function(USA Format) =====
    int  USB_PlayDialTone(HWND m_hWnd);
	int  USB_PlayRingbackTone(HWND m_hWnd);
	int  USB_PlayBusyTone(HWND m_hWnd);
	int  USB_PlayDisconnectTone(HWND m_hWnd);
	int  USB_PlayDTMFTone(int Dig,HWND m_hWnd);
	int  USB_StopPlayTone(HWND m_hWnd);

	// ===== Global Variable =====
    HWND  g_Hwnd;
	//    char            ErrMsg[200];
	BOOL            ToneMask,DTMFMask;

    // ===== For DTMF & Dual-Tone =====
    short         DT_WAV[2][DT_DUR_LEN];
    WAVEFORMATEX  DTpcmFormatex;
    WAVEHDR       DTWaveHdr[2];
    HWAVEOUT      DThWaveOut;
    DWORD         DTFlag,DTPlay,Buf_Num;

	// ===== Variable For Audio Process =====
    UINT          Audio_Out,Audio_In;




int  USB_PlayDialTone(HWND m_hWnd)
{
     if (ToneMask==TRUE) USB_StopPlayTone(m_hWnd); 
	 Buf_Num=2;
	 USB_PlayTone(350,440,4000,0,TRUE,m_hWnd); 
	 return 0;
}
int  USB_PlayRingbackTone(HWND m_hWnd)
{
     if (ToneMask==TRUE) USB_StopPlayTone(m_hWnd); 
	 Buf_Num=2;
	 USB_PlayTone(440,480,1000,3000,TRUE,m_hWnd); 
	 return 0;
}
int  USB_PlayBusyTone(HWND m_hWnd)
{
     if (ToneMask==TRUE) USB_StopPlayTone(m_hWnd); 
	 Buf_Num=2;
	 USB_PlayTone(480,620,500,500,TRUE,m_hWnd); 
	 return 0;
}
int  USB_PlayDisconnectTone(HWND m_hWnd)
{
     if (ToneMask==TRUE) USB_StopPlayTone(m_hWnd); 
	 Buf_Num=2;
	 USB_PlayTone(480,620,250,250,TRUE,m_hWnd); 
	 return 0;
}
int  USB_PlayDTMFTone(int Dig,HWND m_hWnd)
{
WORD DTMF_Tab3[16][2]={ 
	{941, 1336}, {697, 1209}, {697, 1336}, {697, 1477},   // 0,1,2,3
	{770, 1209}, {770, 1336}, {770, 1477}, {852, 1209},   // 4,5,6,7
	{852, 1336}, {852, 1477}, {697, 1633}, {770, 1633},   // 8,9,A,B
	{852, 1633}, {941, 1633}, {941, 1209}, {941, 1477} }; // C,D,*,#

     if (ToneMask==TRUE) USB_StopPlayTone(m_hWnd); 
	 Buf_Num=1;
	 USB_PlayTone(DTMF_Tab3[Dig][0],DTMF_Tab3[Dig][1],300,100,TRUE,m_hWnd); 
	 DTPlay=0; Buf_Num=2; // ==== Play 1 Cycle/1 Buffer Only ====
	 return 0;
}


int  USB_StopPlayTone(HWND m_hWnd)
{
     USB_PlayTone(1,1,1,1,FALSE,m_hWnd);
     return 0;
}

int USB_PlayTone(int Freq1, int Freq2,int OnTime,int OffTime,BOOL Enable,HWND m_hWnd)
{
int     i,j;
double  Period1,Period2;
char S1[100];

       g_Hwnd=m_hWnd;
       Audio_Out=0;
	   if ((OnTime+OffTime)*(Sample_Rate/1000)>DT_DUR_LEN) 
	   {  sprintf(ErrMsg,"OnTime+OffTime must Smaller than 4000 ms"); return -1; }
	   if (Enable==TRUE)
	   {  if (ToneMask==TRUE) return 0; else ToneMask=TRUE;
		  // 設定複頻音
		  DTPlay=1;
          // Generate Dial Tone Waveform
	      Period1=(double)Sample_Rate/(double)Freq1; 
	      Period2=(double)Sample_Rate/(double)Freq2;
	      for (j=0;j<OnTime*(Sample_Rate/1000);j++)
		  {   DT_WAV[0][j] =DT_WAV[1][j]=(short)((double)DT_AMPLITUDE*
		           (cos((double)j*6.28/Period1)*0.5+cos((double)j*6.28/Period2)));    }
	      for (j=OnTime*(Sample_Rate/1000);j<(OnTime+OffTime)*(Sample_Rate/1000);j++)
		  {   DT_WAV[0][j]=DT_WAV[1][j]=0;  }

	      // 設定錄放音參數
	      DTpcmFormatex.wFormatTag=WAVE_FORMAT_PCM;
          DTpcmFormatex.nChannels=1;
          DTpcmFormatex.nSamplesPerSec= Sample_Rate;
          DTpcmFormatex.nAvgBytesPerSec=Sample_Rate*2L;
          DTpcmFormatex.nBlockAlign=2;
          DTpcmFormatex.wBitsPerSample=16;
          if (i=waveOutOpen(&DThWaveOut,Audio_Out,&DTpcmFormatex,(DWORD)m_hWnd,(DWORD)NULL,CALLBACK_WINDOW))
		  {  waveOutGetErrorText(i,ErrMsg,100); 
		  sprintf(S1,"Err Code=%d ",i);
		  //UpdateData(TRUE); m_Edit1+=S1; m_Edit1+=ErrMsg; m_Edit1+="...\r\n"; UpdateData(FALSE);
		     return -1; }
 
	      for (j=0;j<(int)Buf_Num;j++)
		  {   DTWaveHdr[j].dwBufferLength=(OnTime+OffTime)*(Sample_Rate/1000)*2L; // 2byte
              DTWaveHdr[j].dwBytesRecorded=(OnTime+OffTime)*(Sample_Rate/1000)*2L;
              DTWaveHdr[j].dwFlags=0L;
              DTWaveHdr[j].dwLoops=0L;
              DTWaveHdr[j].lpData=(char *)&DT_WAV[j][0]; 
              if (i=waveOutPrepareHeader(DThWaveOut,&DTWaveHdr[j],sizeof(WAVEHDR)))
			  {  waveOutGetErrorText(i,ErrMsg,100); 
			     //UpdateData(TRUE); m_Edit1+=ErrMsg; m_Edit1+=".\r\n"; UpdateData(FALSE);
			     return -1; }
              if (i=waveOutWrite(DThWaveOut,&DTWaveHdr[j],sizeof(WAVEHDR))) 
			  {  waveOutGetErrorText(i,ErrMsg,100); 
			     //UpdateData(TRUE); m_Edit1+=ErrMsg; m_Edit1+="..\r\n"; UpdateData(FALSE);
			     return -1; }  
		  }
	      DTFlag=0; 
	   }
	   else
	   {  if (ToneMask==FALSE) return 0; else ToneMask=FALSE;
		  DTPlay=0;
          waveOutReset(DThWaveOut); 
	      waveOutClose(DThWaveOut);
	   }
	
	   return 0;
}
/*
LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    DWORD i;
char S1[100];

    switch(message)
	{  // Dual Tone Generation
	   case MM_WOM_DONE:
	        if (DTPlay==0) break;
			if (i=waveOutWrite(DThWaveOut,&DTWaveHdr[DTFlag],sizeof(WAVEHDR))) 
			{  waveOutGetErrorText(i,ErrMsg,100); 
					  sprintf(S1,"Err Code=%d ",i);
			   //UpdateData(TRUE); m_Edit1+=S1; m_Edit1+=ErrMsg; m_Edit1+="\r\n"; UpdateData(FALSE);
			}
            DTFlag^=1;
			break;
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}
*/
int  USB_Increase_Volume()
{
DWORD Volume;
MMRESULT i;
     i=waveOutGetVolume(DThWaveOut,&Volume);
	 // ==== Increase Right Channel ====
	 if (Volume<0xF0000000) Volume+=0x10000000; 
	 else                   Volume|=0xFFFF0000;
	 // ==== Increase Left Channel ====
	 if ((Volume&0x0000FFFF)<0x0000F000) Volume+=0x00001000; 
	 else                                Volume|=0x0000FFFF;
	 // ==== Set Volume ====
	 i=waveOutSetVolume(DThWaveOut,Volume);
     return 0;
}

int  USB_Decrease_Volume()
{
DWORD Volume;
MMRESULT i;
     i=waveOutGetVolume(DThWaveOut,&Volume);
	 // ==== Increase Right Channel ====
	 if (Volume>=0x10000000) Volume-=0x10000000; 
	 else                    Volume&=0x0000FFFF;
	 // ==== Increase Left Channel ====
	 if ((Volume&0x0000FFFF)>=0x00001000) Volume-=0x00001000; 
	 else                                 Volume&=0xFFFF0000;
	 // ==== Set Volume ====
	 i=waveOutSetVolume(DThWaveOut,Volume);
	 return 0;
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ================================================================
// ==== Tone Detection ============================================
// ================================================================
//#include "Tone_Detect.h"

// ==== FFT Definition ====
#define FFT_SIZE           256//1024
//#define PI1 		           3.14159
typedef struct { float real, image; } COMPLEX;

COMPLEX x[FFT_SIZE],Cexp[FFT_SIZE/2];
short   bit_reverse[FFT_SIZE/2][2];
float   hamming[FFT_SIZE];
int     NO_SWAP;


#define  FREQ_NUM            12
int  Freq_Tab[FREQ_NUM]={ 1209, 1336, 1477, 1633,  697,  770,  852,  941,  350,  440,  480,  620 };
int  Freq_Div[FREQ_NUM];

int Init_FFT(int FFT_Size)
{
int  i,j,k,l;

//hsfout=fopen("c:/as.fft","wb");
     if (FFT_Size>FFT_SIZE) return -1;

     for(i=0; i<FFT_Size/2; i++)
	 {  Cexp[i].real  = (float)cos(i*6.28318/FFT_Size);
	    Cexp[i].image = (float)sin(i*6.28318/FFT_Size); }
         
     k = 0;  j = 0;
     for ( i=1; i<FFT_Size/2; i++)
     {   l=FFT_Size/2;
         while ( j + l >= FFT_Size/2)   l = l/2;
	     j=(j%l)+l;
		 if (j>i) { bit_reverse[k][0]=i; bit_reverse[k][1]=j; k++; }	 }
	 NO_SWAP=k;
     for (i=0; i<FFT_Size; i++) hamming[i]=(float)0.54-(float)0.46*(float)cos(6.28318*i/(FFT_Size-1));
     for (i=0;i<FREQ_NUM;i++) Freq_Div[i]=(int)((float)Freq_Tab[i]*0.035+0.5);

	 return 0;
}

int g_Window_Size=0;
void REALFFT(short *IData,float *PSD,int FFT_Size,int Window_Size)//COMPLEX *x)//,COMPLEX *Cexp,short bit_reverse[][2],int NO_SWAP)
{
float      sinN, cosN, Realsum, Imagsum, Realdif, Imagdif;
COMPLEX    T,c0,c1;
int        i,j,I,J,K,L,MM,N,TWOL;

 g_Window_Size=Window_Size;

 //==== initial ====//
 for (i=0;i<(Window_Size>>1);i++) { x[i].real=(float)IData[i*2]*hamming[i*2]; x[i].image=(float)IData[i*2+1]*hamming[i*2+1]; }
 for (i=(Window_Size>>1);i<(FFT_Size>>1);i++) x[i].real =x[i].image=0;

 // ==== Bit Reverse ====
 for (I=0;I<NO_SWAP;I++) { MM=bit_reverse[I][0]; N=bit_reverse[I][1]; T=x[MM]; x[MM]=x[N]; x[N]=T; }

 // ====transform argument ====
 L = 1; TWOL = 2; N = FFT_Size>>1;
 while ( L<(FFT_Size>>1))
 {   for ( J=0; J<L; J++)
     {   for ( K=J; K<(FFT_Size>>1); K+=TWOL)
         {   c0 = x[K+L];  c1 = Cexp[J*N]; 
             T.real = c0.real * c1.real - c0.image * c1.image;
             T.image = c0.real * c1.image + c0.image * c1.real;
             c0 = x[K];
			 x[K+L].real  = c0.real - T.real;
             x[K+L].image = c0.image - T.image;
             x[K].real    = c0.real + T.real;
             x[K].image   = c0.image + T.image;    }   }
     L = TWOL; TWOL = L*2; N = N/2;  } 

  // Spectrum Envelope Multiply with 2.0
  for (i=1; i<(FFT_Size>>2); i++)
  {   cosN = Cexp[i].real;
      sinN = Cexp[i].image;
      j = (FFT_Size>>1) - i;
      Realsum = /*(float)0.5*/( x[i].real + x[j].real );
      Imagsum = /*(float)0.5*/( x[i].image + x[j].image );
      Realdif = /*(float)0.5*/( x[i].real - x[j].real );
      Imagdif = /*(float)0.5*/( x[i].image - x[j].image );
      x[i].real = Realsum + cosN*Imagsum + sinN*Realdif;
      x[i].image = Imagdif + sinN*Imagsum - cosN*Realdif;
      x[j].real = Realsum - cosN*Imagsum - sinN*Realdif;
      x[j].image = -Imagdif + sinN*Imagsum - cosN*Realdif;
  }
  x[0].real += x[0].image;
  x[0].real*=2; x[0].image*=2;

  for (i=0;i<(FFT_Size>>1);i++) PSD[i]=x[i].real*x[i].real+x[i].image*x[i].image;
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//#include "alaw.h"//<asterisk/alaw.h>
#define AMI_MASK 0x55
unsigned char linear2alaw (short int linear)
{
     int mask;
     int seg;
     int pcm_val;
     static int seg_end[8] =
     {
          0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF, 0x3FFF, 0x7FFF
     };
     
     pcm_val = linear;
     if (pcm_val >= 0)
     {
         /* Sign (7th) bit = 1 */
         mask = AMI_MASK | 0x80;
     }
     else
     {
         /* Sign bit = 0 */
         mask = AMI_MASK;
         pcm_val = -pcm_val;
     }
     /* Convert the scaled magnitude to segment number. */
     for (seg = 0;  seg < 8;  seg++)
     {
         if (pcm_val <= seg_end[seg])
         break;
     }
     /* Combine the sign, segment, and quantization bits. */
     return  ((seg << 4) | ((pcm_val >> ((seg)  ?  (seg + 3)  :  4)) & 0x0F)) ^ mask;
 }
 /*- End of function --------------------------------------------------------*/
 
short alaw2linear (unsigned char alaw)
{
     int i;
     int seg;
 
     alaw ^= AMI_MASK;
     i = ((alaw & 0x0F) << 4);
     seg = (((int) alaw & 0x70) >> 4);
     if (seg)
         i = (i + 0x100) << (seg - 1);
     return (short) ((alaw & 0x80)  ?  i  :  -i);
}
 
unsigned char __ast_lin2a[8192];
short __ast_alaw[256];
 
void ast_alaw_init(void)
{
    int i;
    /* 
     *  Set up mu-law conversion table
     */
    for(i = 0;i < 256;i++)
       {
            __ast_alaw[i] = alaw2linear(i);
       }
      /* set up the reverse (mu-law) conversion table */
    for(i = -32768; i < 32768; i++)
       {
       __ast_lin2a[((unsigned short)i) >> 3] = linear2alaw(i);
       }
 
}



unsigned char linear2ulaw(short sample)
{
static short exp_lut[256] = {
0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
short sign, exponent, mantissa;
unsigned char ulawbyte;

  sign = (sample >> 8) & 0x80;
  if(sign != 0) sample = -sample-1;
  exponent = exp_lut[( sample >> 7 ) & 0xFF];
  mantissa = (sample >> (exponent + 3)) & 0x0F;
  ulawbyte = ~(sign | (exponent << 4) | mantissa);
  return(ulawbyte);
}

short ulaw2linear(unsigned char ulawbyte)
{
static short exp_lut[8] = { 0, 132, 396, 924, 1980, 4092, 8316, 16764 };
short sign, exponent, mantissa, sample;

  ulawbyte = ~ulawbyte;
  sign = (ulawbyte & 0x80);
  exponent = (ulawbyte >> 4) & 0x07;
  mantissa = ulawbyte & 0x0F;
  sample = exp_lut[exponent] + (mantissa << (exponent + 3));
  if(sign != 0) sample = -sample;
  return(sample);
}


