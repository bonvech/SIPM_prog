/// \file sipm.cpp
/// \brief Заголовочный файл для класса SIPM_device
/// \class SIPM_device

#ifndef __SIPM_H__
#define __SIPM_H__

#define HOST       1
#define MASTER     2
#define HOST_NEW   4

#define	HEADER_LEN      3   //bytes
#define	ADDR_LEN		4   //bytes
#define	BFT_LEN			2   // BYTES_FOR_TRANSFER length
#define	STAT_LEN		1   //status bytes

#define	HD			    0xFFFF  //header
#define	CMD_RD			0x80
#define	CMD_WR			0x00
#define STAT_OK         0x00

#include "io_check.h"

class SIPM_device: public IO_device
{
  public:
    //char IPname[16];
    //int Port;
    //int active;
    //int Number;
    //int IsConnected;
    //int IPaddr;


    //int HVcode[4];

    //int sd_control;
    //int sd_data;

    int Coin;
    int WinCoin;

    int id;

    /// data from config file
    int Offset[32];
    int Length[32];
    int Smooth[32];
    float Power[32];
    int ActiveCh[32];

    int TH_Low[32];
    int TH_High[32];
    //int TH_Mon[12];
    int Matrix[32];
    //int matrix[32][12];
    //int Matrix_Mon[12];
    //int matrix_Mon[12][12];
    int HVcode[32];  ///< коды высокого для каналов

    /// @todo дописать в конфиг файл:  четыре переменные для выставления питания на 4 семерки на плате для -24 В
    /// Читать из конфиг файлов

    int Level;
    int Delay;
    int Simple;
    int Mode;

    int EventSz;
    int Stop_socket;

    //FILE *fdat, *ferr;
    //char filename[80];
    //char errname[80];

//    unsigned int HIST[32][1024];
    int HIST[32][1024];

    unsigned long NumEventRealy;
    unsigned long NumEvents;
    unsigned long Nevent;
    long long NumBytesFile;

    //int cal[9][1024];

    //int Type;  // types of clusters:
    //  0x01  --  HOST
    //  0x02  --  MASTER
    //  0x04  --  Muon
    //  0x08  --  Radio
    //  0x10  --  Baik
    //  any types can added
    //char TypeName[20];
    int HW_ID;


    //SIPM(int NumClust, int sd);
    /// конструктор класса
    /// \todo в конструктор включить создание файлов? проверки?
    SIPM_device()
    {
        memcpy(IPname, "192.168.1.201", 16);
        strcpy(TypeName, "SIPM");

        Stop_socket=1;
        //Port = 3001;
        //Number=0;
        Level=0;
        Delay=0;
        Simple=0;
        EventSz=0;
        Mode=0;

        //IsConnected=0;

        EventSz=0;
        NumEvents=0L;
        NumBytesFile=0L;
        //drs_num=0;
        /*
        for(int i=0;i<4;i++) {
            HVcode[i]=0;
        }
        */

        for(int i=0; i<32; i++)
        {
            TH_Low[i]=100;
            TH_High[i]=110;
            Power[i]=0;
	    Matrix[i] = 0;
        }


        for(int i=0; i<32; i++)
        {
            Smooth[i]=0;
            Offset[i]=0;
            Length[i]=2044;
            ActiveCh[i]=0;

           //for(int j=0;j<1024;j++) {
           //    cal[i][j]=0;
           //}
        }


    };
    ~SIPM_device() {};

    //==========================================================
    int RequestDisable(int ch);
    int RequestEnable(int ch) ;
    int PolarnostPlus(int ch,int data) ;
    int StopDelay(int ch,int data) ;
    int LowThreshold(int ch,int data) ;
    int HighThreshold(int ch,int data) ;
    int ResetAndStartAnalizator(int ch,int data) ;
    int HistogrammTime(int ch,int data) ;
    int DataSmooth(int ch,int data) ;
    int DataOffset(int ch,int data) ;
    int DataLength(int ch,int data) ;
    int StatusRequest(int ch,int *stat) ;
    int Resolve_Timer(int data) ;
    int Set_Timer ( int t0, int t1, int t2, int t3 );
    int Set_Timer_Zero ( void );
    int Test1(int data) ;
    int GetTest1(int *data) ;
    int Coinsidence(int coin) ;
    int TriggerTopology(int topology) ;
    int Reset_Counter_Events(void) ;
    int CoinsidenceWindow(int wincoin) ;
    int Maska (int maska1,int mask2) ;
    int Time_Store(void) ;
    int SetCurrentTime (word *h, word *m, word *s, word *mls, word *mks, word *dns);
    int ChipNumber(int num) ;
    int ChipData(int data) ;
    int LocalClock (void) ;
    int DMA_Enable (void) ;
    int DMA_Reset (void) ;
    int Xilinx_Reset (void) ;
    int GeneratorClock (void) ;
    int OpticClock (void) ;
    int CheckADC (void) ;
    int  Frequence(int freq) ;
    int  ProgrammTrigger(void) ;
    int  Test2(int data) ;
    int  ApparatTimer(int data) ;
    int TriggerEnable(void) ;
    int TriggerDisable(void) ;
    int DeadTimeTrigger(int data) ;
    int DeadTimeRequest(int data) ;
    int RequestDelay(int data) ;
    int GetCurrentTime (word *h, word *m, word *s, word *mls, word *mks, word *dns);
    int Get_Counter_Trigger(int *data1,int *data2) ;
    int Get_Counter_Request(int *data1,int *data2) ;
    //int Check_Frequence ( int *data );
    int Check_Frequence ( void );
    int Check_Serial_ID ( int *data );
    int Check_Board_ID ( int *data );
    int Check_Programm_ID ( int *data );
    int Check_HW_ID ( int *data );
    //===========================================================================
    //==========================================================
    int InitMinusPower(int RDAC,int comm,int data);
    int SetMinusPower(int RDAC,int comm,int data);
    int InitPlusPower(int RDAC,int comm,int data);
    int SetPlusPower(int RDAC,int comm,int det,int data);
    //==========================================================
    //===========================================================================
    //int DMA_TCP_Blocked ( void );
    int StopData ( void );
    //int DMA_Buffer_Clear (void );
    int ClearData ( void );
    //int MC_Start ( void );
    int StartData ( void );
    int MC_Events_Number ( int data );
    //===========================================================================
    int Get_Proshivka (void);
    int Set_Proshivka_BASE (void);
    int Set_Proshivka_WORK (void);


    int Init(void);
    //====================================================================
    int Time_Amplitude_Analyzer(int time);
    int Start_Amplitude_Analyzer(void);
    int Read_Amplitude_Analyzer_old(int kprint, int kfile, int por);
    int Read_Amplitude_Analyzer(int ch,int kprint, int kfile, int por);
    int Read_Amplitude_Analyzer_by_bytes(void);
    int CountRate(int ch, int time,int kprint, int kfile,int por);
//===========================================================================

    pthread_t fRecvThread;
    //pthread_t fSaveThread;


  private:

    //static void* pRecv(void *parten);
    //static void* pRecv_old(void *parten);
    //static void* pSave(void *parten);

    //byte fHeaderBuffer[1024];

};
#endif
//====================================================================
