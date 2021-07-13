//==========================================================
int SIPM_device::RequestDisable(int ch,int nreq) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x0, 0x4, 2); // Request Disable
}
//==========================================================
int SIPM_device::RequestEnable(int ch,int nreq) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x0, 0x0, 2); // Request Enable
}
//==========================================================
int SIPM_device::PolarnostPlus(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x0, data, 2);
}
//==========================================================
int SIPM_device::StopDelay(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0xE, data, 2);
}
//==========================================================
int SIPM_device::LowThreshold(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x10, data+2048, 2);
}
//==========================================================
int SIPM_device::HighThreshold(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x20, data+2048, 2);
}
//==========================================================
int SIPM_device::ResetAndStartAnalizator(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x18, data, 2);
}
//==========================================================
int SIPM_device::HistogrammTime(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x26, data, 2);
}
//==========================================================
int SIPM_device::DataSmooth(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x28, data, 2);
}
//==========================================================
int SIPM_device::DataOffset(int ch,int nreq,int data) 
{
return	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x40, data+8, 2);
}
//==========================================================
int SIPM_device::DataLength(int ch,int nreq,int data) 
{
    if (data>2048) {
	printf("Too Huge Data Length value (>2048): %i\n",data);
	abort();
    }

	WR_IO(3000, 0x80000+nreq+(ch<<12)+0x44, data, 2);
//press_any_key();
return 1;
}
//==========================================================
int SIPM_device::StatusRequest(int ch,int nreq,int *stat) 
{
int res=0,dat=0;
	res=RD_IO(3001, 0x80000+nreq+(ch<<12)+0x0, &dat, 2);
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
	    (data==0x2710) || (data==0x2711) || (data==0x2712) ) {
	    printf("Num=%3i   Global Frequence is O'K  (0x%X)\n",Number,data);
	}
	else {
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
	for(int ich=0;ich<32;ich++) {


	    if (ich<12) RequestEnable(ich,0);
	    else RequestDisable(ich,0);

		DataSmooth(ich,0,Smooth[ich]);
		DataOffset(ich,0,Offset[ich]);
		DataLength(ich,0,Length[ich]);
		StopDelay(ich,0,512);
		LowThreshold(ich,0,TH_Low[ich]);
		HighThreshold(ich,0,TH_High[ich]);

printf("ch=%i  TH_Low=%i   TH_High=%i  Offset=%i  Length=%i  Smooth=%i\n",
ich,TH_Low[ich],TH_High[ich],Offset[ich],Length[ich],Smooth[ich]);

	    }




		Maska(0xfff,0x0);

		Coinsidence(0x1);

		CoinsidenceWindow(25);

		RequestDelay(0x0);
		DMA_Enable();
		TriggerEnable();
//		TriggerDisable();
return 1;
}
//============================================================================