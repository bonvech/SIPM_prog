/// \file sipm-lib.cpp
/// \brief Реализация функций для класса SIPM_device
//==========================================================
int SIPM_device::RequestDisable(int ch)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x0, 0x4, 2); // Request Disable
}
//==========================================================
int SIPM_device::RequestEnable(int ch)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x0, 0x0, 2); // Request Enable
}
//==========================================================
int SIPM_device::PolarnostPlus(int ch,int data)
{
//  data=32 - positive pulse
// data=0  - negative pulse
    return	WR_IO(3000, 0x80000+(ch<<12)+0x0, data, 2);
}
//==========================================================
int SIPM_device::StopDelay(int ch,int data)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0xE, data, 2);
}
//==========================================================
int SIPM_device::LowThreshold(int ch,int data)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x10, data+2048, 2);
}
//==========================================================
int SIPM_device::HighThreshold(int ch,int data)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x20, data+2048, 2);
}
//==========================================================
int SIPM_device::ResetAndStartAnalizator(int ch,int data)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x18, data, 2);
}
//==========================================================
int SIPM_device::HistogrammTime(int ch,int data)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x26, data, 2);
}
//==========================================================
int SIPM_device::DataSmooth(int ch,int data)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x28, data, 2);
}
//==========================================================
int SIPM_device::DataOffset(int ch,int data)
{
    return	WR_IO(3000, 0x80000+(ch<<12)+0x40, data+8, 2);
}
//==========================================================
int SIPM_device::DataLength(int ch,int data)
{
    if (data>2048)
    {
        printf("Too Huge Data Length value (>2048): %i\n",data);
        abort();
    }

    WR_IO(3000, 0x80000+(ch<<12)+0x44, data, 2);
//press_any_key();
    return 1;
}
//==========================================================
int SIPM_device::StatusRequest(int ch,int *stat)
{
    int res=0,dat=0;
    res=RD_IO(3001, 0x80000+(ch<<12)+0x0, &dat, 2);
    *stat = (int)dat;
    return res;
}
//==========================================================
int SIPM_device::Resolve_Timer(int data)
{
    return WR_IO(3000, 0x200002, data, 2);
}
//===========================================================================
int SIPM_device::Set_Timer ( int t0, int t1, int t2, int t3 )
{
    int res=0;
    res+=WR_IO ( 3000, 0x200000+0x10, t0, 2 );
    res+=WR_IO ( 3000, 0x200000+0x12, t1, 2 );
    res+=WR_IO ( 3000, 0x200000+0x14, t2, 2 );
    res+=WR_IO ( 3000, 0x200000+0x16, t3, 2 );
    return res;
}
//===========================================================================
int SIPM_device::Set_Timer_Zero (void)
{
    int res=0;
    res+=WR_IO ( 3000, 0x200010, 0, 2 );
    res+=WR_IO ( 3000, 0x200012, 0, 2 );
    res+=WR_IO ( 3000, 0x200014, 0, 2 );
    res+=WR_IO ( 3000, 0x200016, 0, 2 );
    return res;
}
//==========================================================
int SIPM_device::Test1(int data)
{
    printf("Set  Test 1 = 0x%X   (%i)\n",(unsigned int)data,(unsigned int)data);
    return WR_IO(3000, 0x200004, data, 2);
}
//==========================================================
int SIPM_device::GetTest1(int *data)
{
    int dat;
    int res=RD_IO ( 3001, 0x200004, &dat, 2);
    printf("Get  Test 1 = 0x%X   (%i)\n",(unsigned int)dat,(unsigned int)dat);
    *data = (int)dat;
    return res;
}
//==========================================================
int SIPM_device::Coinsidence(int coin)
{
    return	WR_IO(3000, 0x200006, coin, 2);
}
//==========================================================
int SIPM_device::TriggerTopology(int topology)
{
    return	WR_IO(3000, 0x200007, topology, 2);
}
//===========================================================================
int SIPM_device::Reset_Counter_Events(void)
{
    int res=0;
    res+=WR_IO(3000, 0x200008, 0x3, 2);  //  обязательно вместе
//	kk+=WR_IO(3000, 0x200008, 0x1, 2);  //  сбос сигналов внешний запрос (внутренний триггер)
//	kk+=WR_IO(3000, 0x200008, 0x2, 2);  //  сбос сигналов внешний триггер
    return res;
}
//==========================================================
int SIPM_device::CoinsidenceWindow(int wincoin)
{
    return	WR_IO(3000, 0x20000a, wincoin, 2); // окно совпадений
}
//==========================================================
int SIPM_device::Maska (int maska1,int maska2)
{
    int res=0;
    res+=WR_IO(3000,0x20000C, maska1, 2); //маска каналов
    res+=WR_IO(3000,0x20000d, maska2, 2); //маска каналов
    return res;
}
//==========================================================
int SIPM_device::Time_Store(void)
{
    return WR_IO(3000, 0x20000E, 0x1, 2);
}
//==========================================================
int SIPM_device::SetCurrentTime (word *h, word *m, word *s, word *mls, word *mks, word *dns)
{
    word res=0,data=0;

    res = WR_IO ( 3000, 0x200010, data, 2 );

    *dns = ( data & 0x7f )*10;
    *mks = ( data & 0xff80 ) >> 7;

    res += WR_IO ( 3000, 0x200012, data, 2 );

    *mks |= (data & 1) << 9;
    *mls = ( data & 0x7fe ) >> 1;
    *s   = ( data & 0xf800 ) >> 11;

    res += WR_IO ( 3000, 0x200014, data, 2 );

    *s |= (data & 1) << 5;
    *m  = ( data & 0x7e ) >> 1;
    *h = ( data & 0xf80 ) >> 7;

    res += WR_IO ( 3000, 0x200016, data, 2 );

//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", *h,*m,*s,*mls,*mks,*dns);
    return res;
}
//==========================================================
int SIPM_device::ChipNumber(int num)
{
    return WR_IO(3000, 0x200018, num, 2);
}
//==========================================================
int SIPM_device::ChipData(int data)
{
    return WR_IO(3000, 0x20001A, data, 2);
}
//==========================================================
int SIPM_device::LocalClock (void)
{
    return	WR_IO(3000,0x20001C, 0x1, 2);
}
//==========================================================
int SIPM_device::DMA_Enable (void)
{
    return	WR_IO(3000,0x200020, 0x1, 2);
}
//==========================================================
int SIPM_device::DMA_Reset (void)
{
    return	WR_IO(3000,0x200020, 0x2, 2);
}
//==========================================================
int SIPM_device::Xilinx_Reset (void)
{
    int res=0;
    res+=WR_IO(3000,0x200024, 0x1, 2);
    res+=WR_IO(3000,0x200024, 0x0, 2);
    return res;
}
//==========================================================
int SIPM_device::GeneratorClock (void)
{
    return	WR_IO(3000,0x200026, 0x0, 2);
}
//==========================================================
int SIPM_device::OpticClock (void)
{
    return	WR_IO(3000,0x200026, 0x1, 2);
}
//==========================================================
int SIPM_device::CheckADC (void)
{
    return	WR_IO(3000,0x200027, 0x0, 2);
}
//==========================================================
int SIPM_device:: Frequence(int freq)
{
//  0 - 100 MHz
//  1 - 50 MHz
//  2 - 33 MHz
//  3 - 25 MHz
//  4 - 20 MHz
    return	WR_IO(3000,0x200028, freq, 2);
}
//==========================================================
int SIPM_device:: ProgrammTrigger(void)
{
    return	WR_IO(3000,0x200034, 0x1, 2);  //  вызывается для каждого события отделбно (вызов триггера = событие)
}
//==========================================================
int SIPM_device:: Test2(int data)
{
    return	WR_IO(3000,0x200040, data, 2);
}
//==========================================================
int SIPM_device:: ApparatTimer(int data)
{
    return	WR_IO(3000,0x200046, data, 2);
}
//==========================================================
int SIPM_device::TriggerEnable(void)
{
    return	WR_IO(3000, 0x200200, 0x1, 2);
}
//==========================================================
int SIPM_device::TriggerDisable(void)
{
    return	WR_IO(3000, 0x200200, 0x0, 2);
}
//==========================================================
int SIPM_device::DeadTimeTrigger(int data)
{
    return	WR_IO(3000, 0x200202, data, 2);
}
//==========================================================
int SIPM_device::DeadTimeRequest(int data)
{
    return	WR_IO(3000, 0x200204, data, 2);
}
//==========================================================
int SIPM_device::RequestDelay(int data)
{
    return	WR_IO(3000, 0x200206, data, 2);
}
//==========================================================
int SIPM_device::GetCurrentTime (word *h, word *m, word *s, word *mls, word *mks, word *dns)
{
    int res=0,data=0;

    res = Time_Store();

    res += RD_IO ( 3001, 0x200010, &data, 2);

    *dns = ( data & 0x7f )*10;
    *mks = ( data & 0xff80 ) >> 7;

    res += RD_IO ( 3001, 0x200012, &data, 2 );

    *mks |= (data & 1) << 9;
    *mls = ( data & 0x7fe ) >> 1;
    *s   = ( data & 0xf800 ) >> 11;

    res += RD_IO ( 3001, 0x200014, &data, 2 );

    *s |= (data & 1) << 5;
    *m  = ( data & 0x7e ) >> 1;
    *h = ( data & 0xf80 ) >> 7;

    res += RD_IO ( 3001, 0x200016, &data, 2 );

//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", *h,*m,*s,*mls,*mks,*dns);
    return res;
}
//==========================================================
int SIPM_device::Get_Counter_Trigger(int *data1,int *data2)
{
//	res=RD_IO(3001, 0x200028,data1,2);
//	res=RD_IO(3001, 0x20002A,data2,2);
    int res=0,dat=0;
    res+=RD_IO ( 3001, 0x200028, &dat, 2);
//	    printf("Serial ID = 0x%X\n",(unsigned int)dat);
    *data1 = (int)dat;
    res+=RD_IO ( 3001, 0x20002A, &dat, 2);
//	    printf("Serial ID = 0x%X\n",(unsigned int)dat);
    *data2 = (int)dat;
    return res;
}
//==========================================================
int SIPM_device::Get_Counter_Request(int *data1,int *data2)
{
//	res=RD_IO(3001, 0x20002c,data1,2);
//	res=RD_IO(3001, 0x20002e,data2,2);
    int res=0,dat=0;
    res+=RD_IO ( 3001, 0x20002C, &dat, 2);
//	    printf("Serial ID = 0x%X\n",(unsigned int)dat);
    *data1 = (int)dat;
    res+=RD_IO ( 3001, 0x20002E, &dat, 2);
//	    printf("Serial ID = 0x%X\n",(unsigned int)dat);
    *data2 = (int)dat;
    return res;
}
//===========================================================================
//int SIPM_device::Check_Frequence ( int *freq )
int SIPM_device::Check_Frequence ( void )
{
//int dat;
    int res=0,data=0;
    res=RD_IO ( 3001, 0x2000010, &data, 2);
//	data = (int)dat;
    if ( (data==0x270E) || (data==0x270F) ||
            (data==0x2710) || (data==0x2711) || (data==0x2712) )
    {
        printf("Num=%3i   Global Frequence is O'K  (0x%X)\n",Number,data);
    }
    else
    {
        printf("!!!   Num=%3i   WRONG Global Frequence  (0x%X)\n",Number,data);
    }
//	*freq = data;
    return res;
}
//===========================================================================
int SIPM_device::Check_Serial_ID ( int *data )
{
    int res=0,dat=0;
    res=RD_IO ( 3001, 0x2000FF0, &dat, 2);
    printf("Serial ID = 0x%X\n",(unsigned int)dat);
    *data = (int)dat;
    return res;
}
//===========================================================================
int SIPM_device::Check_Board_ID ( int *data )
{
    int res=0,dat=0;
    res=RD_IO ( 3001, 0x2000FF2, &dat, 2);
    printf("Board ID = 0x%X\n",(unsigned int)dat);
    *data = (int)dat;
    return res;
}
//===========================================================================
int SIPM_device::Check_Programm_ID ( int *data )
{
    int res=0,dat=0;
    res=RD_IO ( 3001, 0x2000FFA, &dat, 2);
    printf("Programm ID = 0x%X\n",(unsigned int)dat);
    *data = (int)dat;
    return res;
}
//===========================================================================
int SIPM_device::Check_HW_ID ( int *data )
{
    int res=0,dat=0;
    res=RD_IO ( 3001, 0x2000FFC, &dat, 2);
    printf("HardWare ID = 0x%X\n",(unsigned int)dat);
    *data = (int)dat;
    return res;
}
//===========================================================================
//===========================================================================
//int DMA_TCP_Blocked ( void )
int SIPM_device::StopData ( void )
{
    return (WR_IO ( 3002, 0x11, 1, 2));
}
//===========================================================================
//int DMA_Buffer_Clear (void )
int SIPM_device::ClearData ( void )
{
    return (WR_IO ( 3002, 0x12, 1, 2));
}
//===========================================================================
//int MC_Start ( void )
int SIPM_device::StartData ( void )
{
    return (WR_IO ( 3002, 0x10, 1, 2));
}
//===========================================================================
int SIPM_device::MC_Events_Number ( int data )
{
    return (WR_IO ( 3002, 0x21, data, 2));
}
//===========================================================================
int SIPM_device::Get_Proshivka ( void )
{
//(WR_IO ( 3002, 0x21, data, 2));
    return 1;
}
//===========================================================================
int SIPM_device::Set_Proshivka_BASE ( void )
{
//(WR_IO ( 3002, 0x21, data, 2));
    return 1;
}
//===========================================================================
int SIPM_device::Set_Proshivka_WORK ( void )
{
//(WR_IO ( 3002, 0x21, data, 2));
    return 1;
}
//===========================================================================
int SIPM_device::Init(void)
{

    printf("Initialization of SIPM %i\n",Number);

    ChipNumber( 0x2810);
    ChipData( 0x8100);
    usleep(10000);
    ChipNumber( 0x4610);
    ChipData( 0x8201);
    usleep(10000);
    ChipNumber( 0x4210);
    ChipData( 0x8060);
    usleep(10000);


    Frequence( 0x0);  //   0 = 100 MHz


//	for(int ich=0;ich<32;ich+=2) {
    for(int ich=0; ich<32; ich++)
    {
        if (ich<12) RequestEnable(ich);
        else RequestDisable(ich);
        //RequestEnable(ich);

	PolarnostPlus(ich,32);

        DataSmooth(ich,Smooth[ich]);
        DataOffset(ich,Offset[ich]);
        DataLength(ich,Length[ich]);
        StopDelay(ich,512);
        LowThreshold(ich,TH_Low[ich]);
        HighThreshold(ich,TH_High[ich]);

        printf("ch=%i  TH_Low=%i   TH_High=%i  Offset=%i  Length=%i  Smooth=%i\n",
               ich,TH_Low[ich],TH_High[ich],Offset[ich],Length[ich],Smooth[ich]);

    }

    // !!!! Maska
    //Maska(0xfff,0x0);
    Maska(0xffff, 0xffff);

    /// !!! \todo исправить: вместо 1 поставить значение Coinsidence из файла
//    Coinsidence(0x1);
    Coinsidence(Coin);

//    GetCionsidence();
    /// \todo поставить значение CoinsidenceWindow из файла
//    CoinsidenceWindow(25);
    CoinsidenceWindow(WinCoin);

    RequestDelay(0x0);
    DMA_Enable();
//    TriggerEnable();
    TriggerDisable();

int data=0;
    Check_Serial_ID ( &data );
    Check_Board_ID ( &data );
    Check_HW_ID (&data );
    Check_Programm_ID ( &data );

    return 1;
}
//============================================================================
//===========================================================================
int SIPM_device::Time_Amplitude_Analyzer(int time) 
{
return WR_IO(3000, 0x80026, time*1000, 2); // time  <- in mlsec
}
//===========================================================================
int SIPM_device::Start_Amplitude_Analyzer(void) 
{
int kk=0;

    for(int ch=0;ch<32;ch++) {
	kk = WR_IO(3000, 0x80000+(ch<<12)+0x18, 1, 2);
    }
/*
	kk +=WR_IO(3000, 0x90018, 1, 2); // 
	kk +=WR_IO(3000, 0x9001A, 1, 2); // 
	kk +=WR_IO(3000, 0x98018, 1, 2); // 
	kk +=WR_IO(3000, 0x9801A, 1, 2); // 

	kk +=WR_IO(3000, 0xB0018, 1, 2); // 
	kk +=WR_IO(3000, 0xB001A, 1, 2); // 
	kk +=WR_IO(3000, 0xB8018, 1, 2); // 
	kk +=WR_IO(3000, 0xB801A, 1, 2); // 

	kk +=WR_IO(3000, 0xD0018, 1, 2); // 
	kk +=WR_IO(3000, 0xD001A, 1, 2); // 
	kk +=WR_IO(3000, 0xD8018, 1, 2); // 
	kk +=WR_IO(3000, 0xD801A, 1, 2); // 
*/
return kk;
}
//===========================================================================
int SIPM_device::Read_Amplitude_Analyzer_old(int kprint, int kfile, int por) 
{
int kk=0;
unsigned long Addr=0L;

	for(int ich=0;ich<32;ich++) {
	    for(int ib=0;ib<1024;ib++) {
		HIST[ich][ib]=0;
	    }
	}

	for( int ich=7;ich<8;ich++) {
qq(ich);

//		kk = RD_IO( 3001, 0x100000+(ich<<12)+(0xFFF*ich), HIST[ich], 0x800);

	    for(int ib=0;ib<1024;ib++) {

		Addr = 0x100000+(ich<<12)+(ib*2);

//printf("ich=%i  ib=%i   Addr = %lX\n",ich,ib,Addr);

//		kk = RD_IO( 3001, 0x100000+(ich<<12)+(0xFFF*ich+ib*2), &HIST[ich][ib], 2);
		kk = RD_IO( 3001, Addr, &HIST[ich][ib], 2);
if ( HIST[ich][ib]>0 )printf("ch=%i  ib=%i   HIST=%i\n",ich,ib,HIST[ich][ib]);

//press_any_key();


	    }

press_any_key();
	}
/*
	kk +=RD_IO ( 3009, 0x80000, HIST[0], 0x800 );
	kk +=RD_IO ( 3009, 0x82000, HIST[1], 0x800 );
	kk +=RD_IO ( 3009, 0x88000, HIST[2], 0x800 );
	kk +=RD_IO ( 3009, 0x8A000, HIST[3], 0x800 );

	kk +=RD_IO ( 3009, 0xA0000, HIST[4], 0x800 );
	kk +=RD_IO ( 3009, 0xA2000, HIST[5], 0x800 );
	kk +=RD_IO ( 3009, 0xA8000, HIST[6], 0x800 );
	kk +=RD_IO ( 3009, 0xAA000, HIST[7], 0x800 );

	kk +=RD_IO ( 3009, 0xC0000, HIST[8], 0x800 );
	kk +=RD_IO ( 3009, 0xC2000, HIST[9], 0x800 );
	kk +=RD_IO ( 3009, 0xC8000, HIST[10], 0x800 );
	kk +=RD_IO ( 3009, 0xCA000, HIST[11], 0x800 );
*/

if (kfile==1) {
//	OpenNewFile(pathfile,por,0x1030+kfile);
	fhist = fopen("hist.dat","wt");
	if (kfile==2) { 
	    ftime(&Now);
	    t=*localtime(&Now.time);
	    fprintf(fhist,"\n   Start Monitoring at:   %02d:%02d:%02d,%03d\n",
		t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);

	    fprintf(fhist,"HV:  ");
	    for(int ich=0;ich<12;ich++) {
		fprintf(fhist,"%7i",HVcode[ich]);
	    }
	    fprintf(fhist,"\n");
	}
	for(int ib=0;ib<1024;ib++) {
	    fprintf(fhist,"%4i ",ib);
	    for(int ich=0;ich<12;ich++) {
		fprintf(fhist,"%7i",HIST[ich][ib]);
	    }
	    fprintf(fhist,"\n");
	}
	fprintf(fhist,"\n");
	fclose(fhist);
}

if (kprint) {
int kkk=0;
	printf("Amplitude Analyzer:\n");

	printf(" bin     ch1    ch2    ch3    ch4    ch5    ch6    ch7    ch8    ch9   ch10   ch11   ch12\n");
	for(int ib=0;ib<1024;ib++) {
	    kkk=0;
	    for(int ich=0;ich<12;ich++) {
		if (HIST[ich][ib]>0) kkk++;
	    }
	    if (kkk==0) continue;
	    printf("%4i ",ib);
	    for(int ich=0;ich<12;ich++) {
		printf("%7i",HIST[ich][ib]);
	    }
	    printf("\n");
	}
}

return kk;
}
//===========================================================================
int SIPM_device::Read_Amplitude_Analyzer(int ch, int kprint, int kfile, int por) 
{
int kk=0;
unsigned long Addr=0L;
int ch_start=0,ch_fin=0;

    if (ch==0) {
	ch_start = 0;
	ch_fin = 32;
    }
    else {
	ch_start = ch;
	ch_fin = ch+1;
    }

	for(int ich=ch_start;ich<ch_fin;ich++) {
	    for(int ib=0;ib<1024;ib++) {
		HIST[ich][ib]=0;
	    }
	}

//	for( int ich=7;ich<8;ich++) {
	for( int ich=ch_start;ich<ch_fin;ich++) {
qq(ich);

//	    for(int ib=0;ib<1024;ib++) {
//		Addr = 0x100000+(ich<<12)+(ib*2);
//		kk = RD_IO( 3001, Addr, &HIST[ich][ib], 2);
////if ( HIST[ich][ib]>0 )printf("ch=%i  ib=%i   HIST=%i\n",ich,ib,HIST[ich][ib]);
//	    }

		Addr = 0x100000+(ich<<12);
		kk = RD_DIM( 3009, Addr, HIST[ich], 1024);
//press_any_key();
	}
/*
	kk +=RD_IO ( 3009, 0x80000, HIST[0], 0x800 );
	kk +=RD_IO ( 3009, 0x82000, HIST[1], 0x800 );
	kk +=RD_IO ( 3009, 0x88000, HIST[2], 0x800 );
	kk +=RD_IO ( 3009, 0x8A000, HIST[3], 0x800 );

	kk +=RD_IO ( 3009, 0xA0000, HIST[4], 0x800 );
	kk +=RD_IO ( 3009, 0xA2000, HIST[5], 0x800 );
	kk +=RD_IO ( 3009, 0xA8000, HIST[6], 0x800 );
	kk +=RD_IO ( 3009, 0xAA000, HIST[7], 0x800 );

	kk +=RD_IO ( 3009, 0xC0000, HIST[8], 0x800 );
	kk +=RD_IO ( 3009, 0xC2000, HIST[9], 0x800 );
	kk +=RD_IO ( 3009, 0xC8000, HIST[10], 0x800 );
	kk +=RD_IO ( 3009, 0xCA000, HIST[11], 0x800 );
*/

if (kfile==1) {
//	OpenNewFile(pathfile,por,0x1030+kfile);
	fhist = fopen("hist.dat","wt");
	if (kfile==2) { 
	    ftime(&Now);
	    t=*localtime(&Now.time);
	    fprintf(fhist,"\n   Start Monitoring at:   %02d:%02d:%02d,%03d\n",
		t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);

	    fprintf(fhist,"HV:  ");
	    for(int ich=0;ich<32;ich++) {
		fprintf(fhist,"%7i",HVcode[ich]);
	    }
	    fprintf(fhist,"\n");
	}
	for(int ib=0;ib<1024;ib++) {
	    fprintf(fhist,"%4i ",ib);
	    for(int ich=0;ich<32;ich++) {
		fprintf(fhist,"%7i",HIST[ich][ib]);
	    }
	    fprintf(fhist,"\n");
	}
	fprintf(fhist,"\n");
	fclose(fhist);
}

if (kprint) {
int kkk=0;
	printf("Amplitude Analyzer:\n");

	printf(" bin     ch1    ch2    ch3    ch4    ch5    ch6    ch7    ch8    ch9   ch10   ch11   ch12\n");
	for(int ib=0;ib<1024;ib++) {
	    kkk=0;
	    for(int ich=0;ich<32;ich++) {
		if (HIST[ich][ib]>0) kkk++;
	    }
	    if (kkk==0) continue;
	    printf("%4i ",ib);
	    for(int ich=0;ich<32;ich++) {
		printf("%7i",HIST[ich][ib]);
	    }
	    printf("\n");
	}
}

return kk;
}
//===========================================================================
/*
int SIPM_device::Read_Amplitude_Analyzer_by_bytes(void) 
{
int kk=0;
unsigned int data;


	for(int ich=0;ich<12;ich++) {
	    for(int ib=0;ib<1024;ib++) {
		HIST[ich][ib]=0;
	    }
	}



qq(80);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0x80000+0x0+ib );
	    HIST[0][ib/2] = data;
	}
qq(81);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0x80000+0x800+ib );
	    HIST[1][ib/2] = data;
	}
qq(82);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0x88000+0x1000+ib );
	    HIST[2][ib/2] = data;
	}
qq(83);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0x88000+0x1800+ib );
	    HIST[3][ib/2] = data;
	}
//	for(int ib=0;ib<100;ib++) {
//	    printf("80000::  %4i %7i\n",ib,HIST[0][ib]);
//	}


qq(84);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xA0000+ib );
	    HIST[4][ib/2] = data;
	}
qq(85);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xA0800+ib );
	    HIST[5][ib/2] = data;
	}
qq(86);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xA8000+ib );
	    HIST[6][ib/2] = data;
	}
qq(87);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xA8800+ib );
	    HIST[7][ib/2] = data;
	}

qq(88);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xC0000+ib );
	    HIST[8][ib/2] = data;
	}
qq(89);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xC0800+ib );
	    HIST[9][ib/2] = data;
	}
qq(90);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xC8000+ib );
	    HIST[10][ib/2] = data;
	}
qq(91);
	for(int ib=0;ib<0x800;ib+=2) {
	    data=RD_IO ( 3001, 0xC8800+ib );
	    HIST[11][ib/2] = data;
	}
qq(100);
	OpenNewFile(pathfile,0,0x1031);
	for(int ib=0;ib<1024;ib++) {
	    fprintf(fhist,"%4i ",ib);
	    if (ib<100) printf("%4i ",ib);
	    for(int ich=0;ich<12;ich++) {
		fprintf(fhist,"%7i",HIST[ich][ib]);
		if (ib<100) printf("%7i",HIST[ich][ib]);
	    }
	    fprintf(fhist,"\n");
	    if (ib<100) printf("\n");
	}
	fclose(fhist);

return kk;
}
*/
//===========================================================================
int SIPM_device::CountRate(int ch, int time,int kprint, int kfile,int por) 
{
int data=0;
float corate;
int stat=0;
int ch_start=0,ch_fin=0;
//int kktime=0;

	printf("\n		Please, wait %i sec:\n\n",time);
qq(100);
	Time_Amplitude_Analyzer(time);
qq(200);
	Start_Amplitude_Analyzer();
qq(300);
/*
	if (time>=5) {
	    while {
		sleep(5);
		kktime+=5;
	    } (kktime<time);
	else
	    sleep(time+1);
	}
*/
	    sleep(time+2);

if (ch==0) {
    ch_start = 0;
    ch_fin = 32;
}
else {
    ch_start = ch;
    ch_fin = ch+1;
}
    

	for(int ich=ch_start;ich<ch_fin;ich++) {
	    stat=0;
printf("ch=%i   stat=%i(0x%X)\n",ich,stat,stat);
	    RD_IO( 3001, 0x80000+(ich<<12), &stat, 2);
printf("ch=%i   stat=%i(0x%X)\n",ich,stat,stat);
	    }



	Read_Amplitude_Analyzer(ch,kprint,kfile,por);
qq(400);
	for(int ich=ch_start; ich<ch_fin;ich++) {
	    data=0;
	    for(int ib=0;ib<1024;ib++) {
		data+=HIST[ich][ib];
	    }
//	    CoRate[ich]=data/time;
	    corate=(float)data/(float)time;
printf("ich=%i   CoRate=%.2f\n",ich+1,corate);
	}




return 1;
}
//==========================================================
