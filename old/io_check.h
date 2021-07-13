#ifndef __IO_CHECK__
#define __IO_CHECK__


#define Fbase	1
#define Fmaroc	2
#define Fcurr	4
#define Frs45	8
#define Fint32	16
#define Fonly	32
#define Fiic	64



#include "semaphore.h"
//#define IO_DEBUG
//#include "iact-rs485.c"

//====================================================================
class IO_device {

public:


char IPname[16];
int Port;
int sd_control;
int sd_data;
int sd_current;
int IsConnected;
int active;
int Number;
int Type;
char TypeName[200];

FILE *fdat;
FILE *fcur;
FILE *fped;
FILE *frate;
FILE *fcuco;
char filename[80];


	IO_device()
	{
//	    strcpy(IPname,"192.168.1.1");
//	    Port=3000;
	    sd_control=-1;
	    sd_data=-1;
	    sd_current=-1;
	    IsConnected=-1;
	    active=-1;
	    Number=-1;
	    Type=-1;
	    strcpy(TypeName,"");
	};
	~IO_device(){};

//	bool CreateSockets(void);
	bool Connect(int *nsid, char *fAddress, int fPort);
	bool UnConnect (void);
	bool Reset_Sockets ( void);

	int CloseFile(void);
	int OpenNewFile(char *path, int por, int flag );

	int IO_check(int flag, int sidi, unsigned long Address, byte *Data, int NumBytes, int nsid, int Number, char Type[]);
	int RDbuf(int nsid, byte *buf, int NumBytes, int flag, unsigned long Addr, int Number, char Type[]);

	int WR_BSM(int Addr, byte *Data, int NumBytes);
	int RD_BSM(int Addr, byte *Data, int NumBytes);

	int WR_IO(int sidi, unsigned long Address, unsigned long Data, int NumBytes);
	int WR_IO_only(int sidi, unsigned long Address, unsigned long Data, int NumBytes);
	int RD_IO(int sidi, unsigned long Address, int *Data, int NumBytes);
//	int RD_CC(int sidi, unsigned long Address);

	int WR_IIC(int IICaddr, int comm, int Data, int NumBytes, unsigned long ADDR);
//	int WR_IIC(int Chan, int comm, int Addr, int NumBytes, byte *Data);
	int RD_IIC(int Chan, int comm, int Addr, int NumBytes, byte *Data);

	int WR_RS(int AA, int CC, int RR, uint Addr, byte *Data);
	int RD_RS(int AA, int CC, int RR, uint Addr, byte *Data);

//	int WR_CUR(int sidi, unsigned long Address,  byte *Data, int NumBytes);
//	int RD_Status_CUR(int sidi, unsigned long Address,  byte *Data, int NumBytes);


//====================================================================

private:


byte buf[20000];
byte data[4000];
byte datar[4000];

pthread_t fRecvThread;
//pthread_t fAlarmThread;

static void* pRecv(void *parten);
static void* pSave(void *parten);
static void* pCurrent(void *parten);
//static void* pAlarmCUR(void *parten);

};
//====================================================================
//====================================================================
int IO_device::IO_check(int flag, int sidi, unsigned long Address, byte *Data, int NumBytes, int nsid, int Number, char Type[])
{
//	sem_wait(&sem_3000);

word rr=0x0000;
int sz=0;
int res=-1;
int DataSz=0;
int hd=0,data_sz=0,Stat=0;
//int nsid = sd_control;
int mode=0;  // WR or RD
//int NumWords=0;

	if (active==-1) {
//	    sem_post(&sem_3000);
	    return -1;
	}

	if (IsConnected==-1) {
//	    sem_post(&sem_3000);
	    return -1;
	}

	if ((nsid&1)==1) mode=1;  // RD    //  проверить
	else mode=0;              // WR


	if (flag==Fbase) {
	    DataSz = 12 + NumBytes;
	}

	if (flag==Frs45) DataSz = 9;

	if (flag==Fiic) DataSz = NumBytes+8;


//	if (flag==Fonly) DataSz = 12 + NumBytes;
//	if (flag==Fint32) DataSz = 12 + 2;

	data[0] = (sidi&0xFF00)>>8;
	data[1] = sidi&0xFF;
	data[2] = data[0];
	data[3] = data[1];
	data[4] = (DataSz&0xFF00)>>8;
	data[5] = (DataSz&0xFF);
	data[6] = (rr&0xFF00)>>8;
	data[7] = (rr&0xFF);


	if (flag==Fbase) {
	    memcpy(&data[8],&Address,4);
	    if (NumBytes>0) memcpy(&data[12],Data,NumBytes);
	}
	if (flag==Fonly) {
	    memcpy(&data[8],&Address,4);
	    if (NumBytes>0) memcpy(&data[12],Data,NumBytes);
	}

	if (flag==Frs45) {
	    data[9] = (byte)NumBytes;
	}

	if (flag==Fiic) {
	    memcpy(&data[8],&Address,NumBytes);
//	    data[9] = (byte)NumBytes;
	}




/*
	if (flag!=Frs45) {
	    memcpy(&data[8],&Address,4);
	    if (NumBytes>0) memcpy(&data[12],Data,NumBytes);
	}
	if (flag==Fint32) {
	    NumWord = NumBytes/2;
	    if (NumBytes>0) memcpy(&data[12],&NumWords,2);
	}
*/


#ifdef IO_DEBUG
	printf("================================\n");
	if (mode==1) printf("<IO_device::RD>  Send to %s %d (nsid=%i, Address=0x%lX) ...\n",Type,Number,nsid,Address);
	if (mode==0) printf("<IO_device::WR>  Send to %s %d (nsid=%i, Address=0x%lX) ...\n",Type,Number,nsid,Address);

	for(int j = 0; j < DataSz; j+=2) {
		printf(" %02X %02X ", data[j],data[j+1]);
		if ((j+1)%8 == 0) printf("\n");
	}
	printf("\n================================\n");
#endif

	sz = send(nsid, (char *)data, DataSz, 0);  //sending command message

	if ( (sz != DataSz) || (sz<0) ) { //  error sending command message
	    if (mode==1) printf("<IO_device::RD> - not sending data at %s %d (nsid=%i, Address=0x%lX) ...\n",
			Type,Number,nsid,Address);
	    if (mode==0) printf("<IO_device::WR> - not sending data at %s %d (nsid=%i, Address=0x%lX) ...\n",
			Type,Number,nsid,Address);
	    IsConnected=-1;
//	    sem_post(&sem_3000);
	    abort();
	}

	if (flag==Fonly) {
//	    sem_post(&sem_3000);
	    return 0;
	}


	for(int j = 0; j < 20000; j++) {
	    buf[j]=0xFF;
	}

	res = RDbuf( nsid, buf, 8, 0, Address, Number, Type );
	if (res<0) {
	    if (mode==1) printf("!!!   ERROR <IO_device::RD> - Not answer from %s %d (nsid=%i, Address=0x%lX) ...header\n",
			Type,Number,nsid,Address);
	    if (mode==0) printf("!!!   ERROR <IO_device::WR> - Not answer from %s %d (nsid=%i, Address=0x%lX) ...header\n",
			Type,Number,nsid,Address);
	    IsConnected=-1;
//	    sem_post(&sem_3000);
//	    abort();
	    return res;
	}
#ifdef IO_DEBUG
	printf("\n================================\n");
	    if (mode==1) printf("<IO_device::RD> - Answer from %s %d (nsid=%i, Address=0x%lX) ...header\n",
			Type,Number,nsid,Address);
	    if (mode==0) printf("<IO_device::WR> - Answer from %s %d (nsid=%i, Address=0x%lX) ...header\n",
			Type,Number,nsid,Address);

	    printf("Header:  ");
	    for(int j = 0; j < res; j+=2) {
		printf(" %02X %02X ",buf[j],buf[j+1] );
//		if ((j+1)%8 == 0) printf("\n");
	    }
	    printf("\n");
#endif

	if ( (flag == Frs45) && (res==8) ) {
	
	    hd = buf[0]*256+buf[1];
	    data_sz = buf[4]*256+buf[5];


	    if (hd != sidi){
//	    if (FLAG_PRINT)
		if (mode==1) printf("!!!   ERROR <IO_device::RD> - from %s %d (nsid=%i, Address=0x%lX)   Wrong SIDI=%i\n",
			Type,Number,nsid,Address,hd);
		if (mode==0) printf("!!!   ERROR <IO_device::WR> - from %s %d (nsid=%i, Address=0x%lX)   Wrong SIDI=%i\n",
			Type,Number,nsid,Address,hd);
		IsConnected=-1;
//		sem_post(&sem_3000);
		return -1;
	    }

	    if (data_sz < 8){
//		    if (FLAG_PRINT)
		if (mode==1) printf("!!!   ERROR <IO_device::RD> - from %s %d (nsid=%i, Address=0x%lX)   Wrong Data Size=%i\n",
			Type,Number,nsid,Address,data_sz);
		if (mode==0) printf("!!!   ERROR <IO_device::WR> - from %s %d (nsid=%i, Address=0x%lX)   Wrong Dtat Size=%i\n",
			Type,Number,nsid,Address,data_sz);
		IsConnected=-1;
//		sem_post(&sem_3000);
		return -3;
	    }
	    if (data_sz==8) {
//		sem_post(&sem_3000);
		return 0;
	    }
	}



	hd = buf[0]*256+buf[1];
	data_sz = buf[4]*256+buf[5];


	res = RDbuf( nsid, buf+8, data_sz-8, 0, Address, Number, Type );
	if (res<0) {
	    if (mode==1) printf("!!!   ERROR <IO_device::RD> - Not answer from %s %d (nsid=%i, Address=0x%lX) ...status\n",
			Type,Number,nsid,Address);
	    if (mode==0) printf("!!!   ERROR <IO_device::WR> - Not answer from %s %d (nsid=%i, Address=0x%lX) ...status\n",
			Type,Number,nsid,Address);
	    IsConnected=-1;
//	    sem_post(&sem_3000);
//	    abort();
	    return res;
	}


	if (flag == Frs45) {
		memcpy( Data, buf+8, res );
	}
	else {
	    memcpy(&Stat,&buf[8],2);
	    if (res>2) {
		memcpy( Data, &buf[10], res-2 );
	    }
	}



#ifdef IO_DEBUG

	if (flag == Frs45) {

	    if (mode==1) printf("<IO_device::RD> - Answer from %s %d (nsid=%i, Address=0x%lX) ...data\n",
			Type,Number,nsid,Address);
	    if (mode==0) printf("<IO_device::WR> - Answer from %s %d (nsid=%i, Address=0x%lX) ...data\n",
			Type,Number,nsid,Address);
	    printf("Data:    ");
	    for(int j = 8; j < res+8; j+=2) {
		printf(" %02X %02X ",Data[j],Data[j+1] );
		if ((j+1)%8 == 0) printf("\n");
	    }
	    printf("\n==================================\n");
	}
	
	else {

	    if (mode==1) printf("<IO_device::RD> - Answer from %s %d (nsid=%i, Address=0x%lX) ...status\n",
			Type,Number,nsid,Address);
	    if (mode==0) printf("<IO_device::WR> - Answer from %s %d (nsid=%i, Address=0x%lX) ...status\n",
			Type,Number,nsid,Address);
	    for(int j = 8; j < 10; j+=2) {
		printf("Status:   %02X  %02X\n",buf[j],buf[j+1] );
	    }


	    if (mode==1) printf("<IO_device::RD> - Answer from %s %d (nsid=%i, Address=0x%lX) ...data\n",
			Type,Number,nsid,Address);
	    if (mode==0) printf("<IO_device::WR> - Answer from %s %d (nsid=%i, Address=0x%lX) ...data\n",
			Type,Number,nsid,Address);
	    printf("Data:    ");
	    for(int j = 10; j < res+10; j+=2) {
		printf(" %02X %02X ",Data[j],Data[j+1] );
		if ((j+1)%8 == 0) printf("\n");
	    }
	    printf("\n==================================\n");
	}
#endif


	if (flag==Frs45) {
//	    sem_post(&sem_3000);
	    return res;  // было в IACT
//	    return 0;  // должно быть,  проверить
	}

//  ERRORS::

	if (hd != sidi){
//	    if (FLAG_PRINT)
	    if (mode==1) printf("!!!   ERROR <IO_device::RD> - from %s %d (nsid=%i, Address=0x%lX)   Wrong SIDI=%i\n",
			Type,Number,nsid,Address,hd);
	    if (mode==0) printf("!!!   ERROR <IO_device::WR> - from %s %d (nsid=%i, Address=0x%lX)   Wrong SIDI=%i\n",
			Type,Number,nsid,Address,hd);
	    IsConnected=-1;
//	    sem_post(&sem_3000);
	    return -1;
	}

	if (Stat != 0){
//	    if (FLAG_PRINT)
			printf("!!!   ERROR <io-IACT::Write> - from %s#%02d  Recive Status=0x%X\n",Type,Number,Stat);
	    if (mode==1) printf("!!!   ERROR <IO_device::RD> - from %s %d (nsid=%i, Address=0x%lX)   Wrong Status=%i\n",
			Type,Number,nsid,Address,Stat);
	    if (mode==0) printf("!!!   ERROR <IO_device::WR> - from %s %d (nsid=%i, Address=0x%lX)   Wrong Status=%i\n",
			Type,Number,nsid,Address,Stat);
//	    IsConnected=-1;
//	    sem_post(&sem_3000);
	    return -2;
	}

	if (data_sz < 10){
//		    if (FLAG_PRINT)
	    if (mode==1) printf("!!!   ERROR <IO_device::RD> - from %s %d (nsid=%i, Address=0x%lX)   Wrong Data Size=%i\n",
			Type,Number,nsid,Address,data_sz);
	    if (mode==0) printf("!!!   ERROR <IO_device::WR> - from %s %d (nsid=%i, Address=0x%lX)   Wrong Dtat Size=%i\n",
			Type,Number,nsid,Address,data_sz);
	    IsConnected=-1;
//	    sem_post(&sem_3000);
	    return -3;
	}

//	sem_post(&sem_3000);
return 0;
}
//====================================================================



//====================================================================
/*
bool IO_device::CreateSockets()
{
	if (active==0) return -1;

	if(sd_control < 0)
	{
		sd_control = socket(AF_INET, SOCK_STREAM, 0);
		if(sd_control < -1) 
		{
			printf("Unable to create CONTROL socket.\n");
			return false;
		}
		// setting nonblocking option for socket
		fcntl(sd_control, F_SETFL, O_NONBLOCK);
	}
//	return true;

	if(sd_data < 0)
	{
		sd_data = socket(AF_INET, SOCK_STREAM, 0);
		if(sd_data < -1) 
		{
			printf("Unable to create DATA socket.\n");
			return false;
		}
		// setting nonblocking option for socket
		fcntl(sd_data, F_SETFL, O_NONBLOCK);
	}
	if(sd_current < 0)
	{
		sd_current = socket(AF_INET, SOCK_STREAM, 0);
		if(sd_current < -1) 
		{
			printf("Unable to create CURRENT socket.\n");
			return false;
		}
		// setting nonblocking option for socket
		fcntl(sd_current, F_SETFL, O_NONBLOCK);
	}
	IsConnected = 1;
	return true;
}
*/
//====================================================================
bool IO_device::Connect(int *nsid, char *fAddress, int fPort)
{
int fConnectionRepeatCurr = 0;
int fConnectionRepeat=50;
int sd=*nsid;

	if (active==-1) return -1;


printf("sd=%i\n",sd);
	if(sd > 0) 
	{
		printf("Error in programm::  This socket is present and not closed:   nsid=%d.\n",sd );
	    abort();
	    return false;
	}
 
	if(sd < 0) 
	{
		sd = socket(AF_INET, SOCK_STREAM, 0);
//		if(sd < -1) 
		if(sd < 0) 
		{
			printf("Unable to create CONTROL socket.\n");
			abort();
			return false;
		}
		// setting nonblocking option for socket
		fcntl(sd, F_SETFL, O_NONBLOCK);
	}

	IsConnected = -1;

	in_addr raw_addr;
	sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(fPort);

	if(inet_aton(fAddress, &raw_addr) == -1)
	{
		printf("Unable to convert server address. %s\n",fAddress);
		return false;
	}
	addr.sin_addr = raw_addr;

qq(1111);

	while(connect(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		printf("Try connect to: %s %d  (IP::%s:%i)   nsid=%i   Try %d/%d.\n", 
			TypeName,Number,fAddress,fPort,sd, fConnectionRepeatCurr + 1, fConnectionRepeat );

		if(fConnectionRepeat >= 0 && fConnectionRepeatCurr + 1 >= fConnectionRepeat)
		{
			printf("        Unable to connect to %s %d  (%s:: %i)\n",
				TypeName,Number,fAddress,fPort);
			printf("           %s %d is not active\n",TypeName,Number);

//ffstat = fopen(statfile,"at");
//fprintf(ffstat,"\n        Unable to connect to IACT_CC %d  (%s:: %i)\n",
//				Number,fAddress,fPort);
//fprintf(ffstat,"           IACT_CC %d is not active\n\n",Number);
//fclose(ffstat);
			return false;
		}
		fConnectionRepeatCurr++;
		sleep(1);

qq(2222);
	};

	IsConnected = 1;
	*nsid = sd;

//press_any_key();
return true;
}
//====================================================================
bool IO_device::UnConnect ( void) 
{
	if (active==0) return -1;

//    pthread_cancel(fRecvThread);
//    pthread_cancel(fSaveThread);
    shutdown(sd_control, SHUT_RD|SHUT_WR);
    close(sd_control);
    shutdown(sd_data, SHUT_RD|SHUT_WR);
    close(sd_data);
    shutdown(sd_current, SHUT_RD|SHUT_WR);
    close(sd_current);
	    sd_control=-1;
	    sd_data=-1;
	    sd_current=-1;

//ffstat = fopen(statfile,"at");
//fprintf(ffstat,"\n        UnConnect IACT_CC  sd_control => %i\n",sd_control);
//fprintf(ffstat,"        UnConnect IACT_CC  sd_data => %i\n",sd_data);
//fprintf(ffstat,"        UnConnect IACT_CC  sd_current => %i\n",sd_current);
//fclose(ffstat); 

return true;
}
//====================================================================
bool IO_device::Reset_Sockets ( void) 
{
	if (active==0) return -1;

    pthread_cancel(fRecvThread);
    pthread_kill(fRecvThread,0);
//    pthread_cancel(fSaveThread);
    shutdown(sd_control, SHUT_RD|SHUT_WR);
    close(sd_control);
    shutdown(sd_data, SHUT_RD|SHUT_WR);
    close(sd_data);
    shutdown(sd_current, SHUT_RD|SHUT_WR);
    close(sd_current);
	    sd_control=-1;
	    sd_data=-1;
	    sd_current=-1;
//ffstat = fopen(statfile,"at");
//fprintf(ffstat,"\n        UnConnect IACT_CC  sd_control => %i\n",sd_control);
//fprintf(ffstat,"        UnConnect IACT_CC  sd_data => %i\n",sd_data);
//fprintf(ffstat,"        UnConnect IACT_CC  sd_current => %i\n",sd_current);
//fclose(ffstat); 

return true;
}
//====================================================================
//====================================================================
//====================================================================
//====================================================================
int IO_device::RDbuf(int nsid, byte *buf, int NumBytes, int flag, unsigned long Addr, int Number, char Type[])
{
int SumBytes=0;
int res;
int kkk=0;
int TimeOut=1000;  // 10 mksec

	if (active==0) return -1;

	for(int j=0;j<NumBytes;j++) {
	    buf[j]=0;
	}

	while(SumBytes!=NumBytes) {

	    res = recv( nsid, buf+SumBytes, NumBytes-SumBytes, flag );

	    if (res>0) {

		SumBytes += res;

#ifdef IO_DEBUG
		printf("RDbuf::\n");
		for(int j = 0; j < NumBytes; j++) {
			printf(" 0x%X",buf[j]);
			if (j%2) printf("  ");
		}
		printf("\n");
#endif
	    }

	    if (SumBytes==NumBytes) return SumBytes;

	    kkk++;
	    if ( !(kkk%1000) ) {
//		if (FLAG_PRINT)
		    printf("ERROR <io-cc.c::RDbuf> Not recivind data at %s %i (sd=%i)  TimeOut=%ld s res=%i  Addr=0x%lx\n",
			Type,Number,nsid,(unsigned long)kkk*TimeOut/1000,res,Addr);
	    }
	    if ( kkk>30000 ) return -20;  //  30 sec

	    usleep(TimeOut);

	}  //  while

return SumBytes;
}
//====================================================================
//====================================================================
//====================================================================
/*
int IO_device::WR_BSM(int Addr, byte *Data, int NumBytes) 
{
int res=0;
int sidi=3064;
word addr=0x0001;
word comm=0;
unsigned long Address=0L;
byte buf[200];
int kk=0;

	if (active==0) return -1;

	NumBytes+=1;

	comm = ( ( gateCC & 0x1F ) | ( ( NumBytes & 0x7FF ) << 5 ) );

	data[0] = (comm&0xFF);
	data[1] = (comm&0xFF00)>>8;

	data[2] = (addr&0xFF);
	data[3] = (addr&0xFF00)>>8;

	memcpy(&Address,data,4);

	buf[0]=(byte)(Addr);
	memcpy(buf+1,Data,NumBytes-1);

CYC:
	if (kk>3) {
	    printf("ERROR!!!  May be not connect \n");
	    return res;
	}
//	res=WR(sidi, Address, buf, NumBytes, Number, (char*)"BSM");
	res=IO_check(Fbase,sidi, Address, buf, NumBytes, sd_control, Number, (char*)"BSM");

	if (res<0) {
	    printf("BSM %02d   comm=0x%x\n",Number,Data[0]);
	    kk++;
	    goto CYC;
	}

return res;
}
*/
//====================================================================
/*
int IO_device::RD_BSM(int Addr, byte *Data, int NumBytes) 
{
int res=0;
int sidi=3065;
word addr=0x0001;
word comm=0;
unsigned long Address=0L;
byte buf[200];
int kk=0;

	if (active==0) return -1;

	comm = ( ( gateCC & 0x1F ) | ( ( 2 & 0x7FF ) << 5 ) );

	data[0] = (comm&0xFF);
	data[1] = (comm&0xFF00)>>8;

	data[2] = (addr&0xFF);
	data[3] = (addr&0xFF00)>>8;

	memcpy(&Address,data,4);

	buf[0]=(byte)(Addr)+0x80;
	buf[1]=(byte)NumBytes;

	for(int ii=0;ii<NumBytes;ii++) 
	{
 	    Data[ii]=0xFF;
//	printf("buf=0x%x  ",buf[ii]);
	}

CYC:
	if (kk>3) {
	    printf("ERROR!!!  May be not connect \n");
	    return res;
	}
//	res=RD(sidi, Address, buf, 2, Number, (char*)"BSM");
	res=IO_check(Fbase,sidi, Address, buf, 2, sd_control, Number, (char*)"BSM");

	if (res<0) {
	    printf("BSM %02d   comm=0x%x  res=%i\n",Number,Addr,res);
	    kk++;
printf("kk=%i\n",kk);
	    goto CYC;
	}

	for(int ii=0;ii<res;ii++) 
	{
 	    Data[ii]=buf[ii];
//	printf("buf=0x%x  ",buf[ii]);
	}
//	printf("\n");
//	}

return res;
}
*/
//====================================================================
//====================================================================
//====================================================================
/*
int IO_device::WR_RS(int AA, int CC, int RR, uint Addr, byte *Data)
{
int res=0;
int sidi=3004;
uint CS=0;
char buf[30];
int ii=2;
int DataSz=0;
unsigned long Address=0L;
int kk=0;

	if (active==0) return -1;

	for(int i=0;i<30;i++) {
	    buf[i]=0;
	}

	if (CC==0) {
	    buf[0] = 26;
	    DataSz=23;
	}
	if (CC==2) {
	    buf[0] = 18;
	    DataSz=15;
	}

	buf[1] = '#';  //'#'

	ByteToHex(AA,&buf[ii],&CS);
	ii+=2;

	ByteToHex(CC,&buf[ii],&CS);
	ii+=2;

	ByteToHex(RR,&buf[ii],&CS);
	ii+=2;

	ByteToHex( (Addr&0XFF), &buf[ii],&CS);
	ii+=2;

	ByteToHex( ((Addr&0XFF00)>>8), &buf[ii],&CS);
	ii+=2;

	ByteToHex( ((Addr&0XFF0000)>>16), &buf[ii],&CS);
	ii+=2;

	ByteToHex( ((Addr&0XFF000000)>>24), &buf[ii],&CS);
	ii+=2;

	if (CC==0) {

	    ByteToHex(Data[0],&buf[ii],&CS);
	    ii+=2;

	    ByteToHex(Data[1],&buf[ii],&CS);
	    ii+=2;

	    ByteToHex(Data[2],&buf[ii],&CS);
	    ii+=2;

	    ByteToHex(Data[3],&buf[ii],&CS);
	    ii+=2;
	}

	ByteToHex( (CS&0xFF),&buf[ii],&CS);
	ii+=2;

	buf[ii]=13;

	memcpy(&Address,buf,4);

CYC:
	if (kk>1) {
	    printf("ERROR!!!  May be not connect \n");
	    return res;
	}
//	res=WR(sidi, Address, (byte*)(buf+4), DataSz, Number, (char*)"RS");
	res=IO_check(Fbase,sidi, Address, (byte*)buf+4, DataSz, sd_control, Number, (char*)"RS");



	if (res<0) {
	    kk++;
	    goto CYC;
	}

return res;
}
*/
//====================================================================
/*
int IO_device::RD_RS(int AA, int CC, int RR, uint Addr, byte *Data)
{
int res=0;
unsigned long Address=0L;
int ww=0;
byte datars[200];
int kk=0;

	if (active==0) return -1;

CYC:
	if (kk>1) {
	    printf("ERROR!!!  May be not connect \n");
	    return res;
	}
	res=WR_RS(AA, CC, RR, Addr, Data);

	if (res<0) {
	    kk++;
	    goto CYC;
	}

usleep(20000);

	ww=0;
//CYC:
	res=0;
//	res=RD_DIM(3005,Address,datars+ww,100,Number,(char*)"RS");
	res=IO_check(Frs45,3005, Address, datars+ww, 100, sd_control, Number, (char*)"RS");




	if ( (res>0) && (datars[res-1] != 0x0D) ) {
	    ww+=res;
//	    goto CYC;

//ИСПРАВИТЬ!!!
	}

//	for (int ii=0;ii<res;ii++) {
//	    printf("RS(3005)::  ii=%i   datars=0x%x\n",ii,datars[ii]);
//	}

	for (int ii=0;ii<res-5;ii+=2) {
	    HexToByte(&Data[ii/2],&datars[ii+2]);
//	    printf("RS(3005)::  ii=%i   datars=0x%x\n",ii,Data[ii/2]);
	}

return res;
}
*/
//====================================================================
int IO_device::WR_IIC(int IICaddr, int comm, int Data, int NumBytes, unsigned long ADDR)
{
int res=0;
int sidi=3010;
char buf[30];
int DataSz=NumBytes;
unsigned long Address=0L;
int kk=0;
unsigned long add1=0,add2=0l,add3=0;
unsigned long addAddress=0L;

	if (active==0) return -1;

	for(int i=0;i<30;i++) {
	    buf[i]=0;
	}

if (NumBytes==3) {

	Address = (IICaddr<<16)+(comm<<10)+Data;
	add1=Address>>16;
//	add2=(Address-(add1<<16))>>8;
	add2=(Address&0xFFFF)>>8;
	add3=Address&0xFF;
//((IICaddr*256) + comm);
//	Address = ( ((IICaddr*256) + comm) << 7) + Data;
	Address = ADDR;
printf("Addres = 0x%lX   Add = 0x%lX  0x%lX    0x%lX \n",Address, add1,add2,add3);
//	Address = ADDR;
	addAddress = (add3<<16) + (add2<<8) + add1;
	Address = addAddress;
printf("addAddres = 0x%lX   ADDR =0x%lX\n",addAddress,ADDR);
press_any_key();
}

if (NumBytes==4) {
Address = 0L;
//Address = ( ((Data) << 16) + (comm<<8) + IICaddr) &0xFFFFFFFF;
Address = ( ( ( ((Data&0xFF)<<8) + (Data>>8) ) << 16) + (comm<<8) + IICaddr) &0xFFFFFFFF;
//Address = ( ( ( ((Data&0xFF)<<8) + (Data>>8) ) << 8) + (comm<<24) + IICaddr) &0xFFFFFFFF;
//Address = ( (Data  << 8) + (comm<<24) + IICaddr) &0xFFFFFFFF;
printf("Addres = 0x%lX  datasz=%i\n",Address,DataSz);
press_any_key();
}


CYC:
	if (kk>1) {
	    printf("ERROR!!!  May be not connect \n");
	    return res;
	}
//	res=WR(sidi, Address, (byte*)(buf+4), DataSz, Number, (char*)"RS");
	res=IO_check(Fiic,sidi, Address, (byte*)buf, DataSz, sd_control, Number, (char*)"SIPM_IIC");

	if (res<0) {
	    kk++;
	    goto CYC;
	}

return res;
}

//====================================================================
int IO_device::RD_IIC(int Chan, int comm, int Addr, int NumBytes, byte *Data)
{
int res=0;
//unsigned long Address=0L;
int ww=0;
byte datars[200];
int kk=0;

	if (active==0) return -1;

CYC:
	if (kk>1) {
	    printf("ERROR!!!  May be not connect \n");
	    return res;
	}
//	res=WR_I2C(Chan, comm, Addr, NumBytes, Data);

	if (res<0) {
	    kk++;
	    goto CYC;
	}

usleep(20000);

	ww=0;
//CYC:
	res=0;
//	res=RD_DIM(3005,Address,datars+ww,100,Number,(char*)"RS");
//	res=IO_check(Frs45,3005, Address, datars+ww, 100, sd_control, Number, (char*)"RS");




	if ( (res>0) && (datars[res-1] != 0x0D) ) {
	    ww+=res;
//	    goto CYC;

//ИСПРАВИТЬ!!!
	}

//	for (int ii=0;ii<res;ii++) {
//	    printf("RS(3005)::  ii=%i   datars=0x%x\n",ii,datars[ii]);
//	}

	for (int ii=0;ii<res-5;ii+=2) {
//	    HexToByte(&Data[ii/2],&datars[ii+2]);
//	    printf("RS(3005)::  ii=%i   datars=0x%x\n",ii,Data[ii/2]);
	}

return res;
}
//====================================================================
//====================================================================
int IO_device::WR_IO_only(int sidi, unsigned long Address,  unsigned long Data, int NumBytes)
{
//byte buf[4];
int res=0;
//int kk=0;

	if (active==0) return -1;

	if ( NumBytes==2 ) {
	    buf[0]=(unsigned char)(Data)&0xff;
	    buf[1]=(unsigned char)((Data>>8)&0xff);
	}

	if ( NumBytes==4 ) {
	    buf[0]=(unsigned char)(Data)&0xff;
	    buf[1]=(unsigned char)((Data>>8)&0xff);
	    buf[2]=(unsigned char)((Data>>16)&0xffff);
	    buf[3]=(unsigned char)((Data>>24)&0xffffff);
	}

//CYC:
//	if (kk>1) {
//	    printf("ERROR!!!  May be not connect \n");
//	    return res;
//	}
//	res=WR_only(sidi, Address, buf, NumBytes, Number, (char*)"IACT_CC");
	res=IO_check(Fbase,sidi, Address, buf, NumBytes, sd_control, Number, (char*)"BSM");
//	if (res<0) {
//	    kk++;
//	    goto CYC;
//	}

return res;
}
//====================================================================
int IO_device::WR_IO(int sidi, unsigned long Address,  unsigned long Data, int NumBytes)
{
//byte buf[4];
int res=0;
int kk=0;

	if (active==0) return -1;

	if ( NumBytes==2 ) {
	    buf[0]=(unsigned char)(Data)&0xff;
	    buf[1]=(unsigned char)((Data>>8)&0xff);
	}

	if ( NumBytes==4 ) {
	    buf[0]=(unsigned char)(Data)&0xff;
	    buf[1]=(unsigned char)((Data>>8)&0xff);
	    buf[2]=(unsigned char)((Data>>16)&0xffff);
	    buf[3]=(unsigned char)((Data>>24)&0xffffff);
	}

CYC:
	if (kk>1) {
	    printf("ERROR!!!  May be not connect \n");
	    return res;
	}
//	res=WR(sidi, Address, buf, NumBytes, Number, (char*)"IACT_CC");
	res=IO_check(Fbase,sidi, Address, buf, NumBytes, sd_control, Number, (char*)TypeName);
	if (res<0) {
	    kk++;
	    goto CYC;
	}

return res;
}
//====================================================================
//int IO_device::RD_IO(int sidi, unsigned long Address)
int IO_device::RD_IO(int sidi, unsigned long Address, int *Data, int NumBytes)
{
//byte buf[2];
int res=0;
int kk=0;
int dat=0L;
//int data=0;

	if (active==0) return -1;

//	buf[0] = (Data&0xFF);
//	buf[1] = (Data&0xFF00)>>8;
	buf[0] = 0;
	buf[1] = 0;

CYC:
	if (kk>1) {
	    printf("ERROR!!!  May be not connect \n");
	    return -1;
	}
//	res=RD(sidi, Address, buf, NumBytes, Number, (char*)"IACT_CC");
//	res=RD(sidi, Address, buf, 0, Number, (char*)"IACT_CC");
	res=IO_check(Fbase,sidi, Address, buf, 0, sd_control, Number, (char*)TypeName);
	if (res<0) {
	    kk++;
//printf("kk=%i\n",kk);
	    dat =-1;
	    *Data = dat;
	    goto CYC;
	}

	if (res<0) {  dat=-1; *Data=dat; }
	else {
	    dat=((buf[1]&0xff)<<8)+(buf[0]&0xff);
	    *Data = dat;
	}


//printf("res=%i  buf[0]=%x  buf[1]=%x   dat=%x\n",res,buf[0],buf[1],dat);
return res;
}
//====================================================================
/*
int IO_device::WR_CUR(int sidi, unsigned long Address,  byte *Data, int NumBytes)
{
//	sem_wait(&sem);

byte data[0x1000];
word rr=0x0000;
int sz=0;
int res=-1;
int DataSz=0;
int hd=0,data_sz=0,Stat=0;
int nsid = sd_current;
unsigned int NumPack=0;

	if (active==0) {
//	    sem_post(&sem);
	    return -1;
	}


	DataSz = 12 + NumBytes;

	NumPackCUR++;
	if (NumPackCUR>65500) NumPackCUR=1;

	data[0] = (NumPackCUR&0xFF00)>>8;
	data[1] = NumPackCUR&0xFF;
	data[2] = (sidi&0xFF00)>>8;
	data[3] = sidi&0xFF;
	data[4] = (DataSz&0xFF00)>>8;
	data[5] = (DataSz&0xFF);
	data[6] = (rr&0xFF00)>>8;
	data[7] = (rr&0xFF);

	memcpy(&data[8],&Address,4);

	if (NumBytes>0) memcpy(&data[12],Data,NumBytes);

	for(int j = 0; j<4000; j++) {
	    datar[j]=0xFF;
	}

#ifdef IO_DEBUG
	printf("================================\n");
	printf("<io-IACT::Write CUR>  Send to CC (sd=%i) Addr=0x%lX  DataSz=%i\n",nsid,Address,DataSz);
	for(int j = 0; j < DataSz; j+=2) {
//		printf("%i   0x%X  0x%X\n", j-12, data[j],data[j+1]);
		if (!(j%12)) printf("\n");
		printf("0x%X 0x%X   ", data[j],data[j+1]);
	}
	printf("\n");
#endif

	if ( (sz = send(nsid, (char *)data, DataSz, 0) ) < 0 )  //sending command message
	{
	    printf("!!!   ERROR <io-IACT::Write CUR> - not sending data at CC (sd=%i) Addr=0x%lX\n",nsid,Address);
	}


	do {
	    NumPack = datar[2]*256 + datar[3];
	    usleep(1000);

	} while( NumPack!=NumPackCUR);

	res = datar[4]*256 + datar[5];

#ifdef IO_DEBUG
	printf("*********res=%i\n",res);
	    printf("$$$<io-IACT::Write CUR>  Answer from CC=%i, Address=0x%lX) ...header\n",Number,Address);
	    for(int j = 0; j < res; j+=2) {
		if (!(j%12)) printf("\n");
//		printf("j=%i  0x%X 0x%X   ", j, datar[j],datar[j+1]);
		printf("0x%X 0x%X   ", datar[j],datar[j+1]);
//		printf(" 0x%X",buf[j] );
//		if (j%2) printf("  ");
	    }
	    printf("\n");
#endif


	if( res>0 ) { 

	    hd = datar[0]*256+datar[1];
	    data_sz = datar[4]*256+datar[5];
	    memcpy(&Stat,&datar[8],2);

		if (hd != sidi){
//		    if (FLAG_PRINT)
			printf("!!!   ERROR <io-IACT::Write CUR> - from CC%02d  Recive SI,DI=%i   hd=%i \n",Number,sidi,hd);
//		    sem_post(&sem);
//			return -1;
		}

		if (Stat != 0){
//		    if (FLAG_PRINT)
			printf("!!!   ERROR <io-IACT::Write CUR> - from CC%02d  Recive Status=0x%X\n",Number,Stat);
//		    sem_post(&sem);
//			return -1;
		}

		if (data_sz < 10){
//		    if (FLAG_PRINT)
			printf("!!!   ERROR <io-IACT::Write CUR> - from CC%02d  Recive data_sz=%i\n",Number,data_sz);
//		    sem_post(&sem);
//			return -1;
		}
#ifdef IO_DEBUG
		if (data_sz==10) {
		    printf("==================================\n");
		    sem_post(&sem);
		    return 0;
		}
#endif
		res=0;
		data_sz -= 10;
//#ifdef IO_DEBUG
//		printf("   data_sz=%i\n",data_sz);
//#endif

		memcpy( Data, datar+10, data_sz );

//		    sem_post(&sem);

		return data_sz;

	}  //  if answer from socket OK

	//answer from controller lost
//	if (FLAG_PRINT)
	    printf("!!!   ERROR <io-IACT::Read> - Not answer from CC%02d (NumCC=%i, Address=0x%lX) socket=%i\n",Number,NumCC,Address,sd_control);

//sem_post(&sem);

return res;
}
*/
//====================================================================
//====================================================================
void* IO_device::pRecv(void* parent)
{
IO_device *client = (IO_device *) parent;
unsigned char buff[2000];
long sz = 0L;
FILE *fff;


printf("===============  THREAD RECV data started  =============\n");
printf("socket=%i\n",client->sd_data);

	while(1)
	{
	    sz = recv(client->sd_data, buff, 156, 0);

	    if(sz == 156)
	    {
	printf("\nresv (3007)::  BSM#%i  res=%ld\n",client->Number, sz);
//		for (int ib=0;ib<156;ib+=2) {
		for (int ib=24;ib<152;ib+=2) {
			printf("%7d",(buff[ib]+buff[ib+1]*256));
			if( (ib+10)%16==0 ) printf("\n");
		}	
	printf("resv (3007)::\n");

		fff = fopen("data.dat","ab");
		fwrite(buff,sz,1,fff);
		fclose(fff);
	    }
	    else 
	    {
//	printf("recv  sleep 1 mls\n");
	    	usleep(1000);
	    }

	}  // while

return NULL;
}
//====================================================================
/*
void* IO_device::pCurrent(void* parent)
{
IO_device *client = (IO_device *) parent;
unsigned char buff[4000];
long sz = 0L,res=0;
int sidi=0;
int NumBytes=0;
//FILE *fff;
char Type[]="CC";
int NumCUR=0;


//printf("===============  THREAD CURRENT started  =============\n");
//printf("socket=%i\n",client->sd_current);

	while(1)
	{
	    sz = recv(client->sd_current, buff, 8, 0);

//		printf("\nresv (3005)::  CC#%i  sz=%ld   nsid=%i\n",client->Number, sz, client->sd_current);


	    if(sz==8)
	    {

//		printf("\nresv (3005)::  CC#%i  sz=%ld   nsid=%i\n",client->Number, sz, client->sd_current);


//		printf("\nresv (3005)::  CC#%i  sz=%ld\n",client->Number, sz);
//		for (int ib=0;ib<sz;ib+=2) {
//		for (int ib=0;ib<sz;ib++) {
//			printf("%7d",(buff[ib]+buff[ib+1]*256));
//			printf("%i  0x%X \n",ib,buff[ib]);
//		    if (!(ib%12)) printf("\n");
//			printf("0x%X 0x%X   ",buff[ib],buff[ib+1]);
//			if( (ib%2)!=0 ) printf(" ");
//		}
//		printf("\n");


		    sidi = buff[0]*256+buff[1];
//printf("sidi=%i\n",sidi);


		NumBytes=buff[4]*256+buff[5];
//printf("NumBytes=%i\n",NumBytes);

		res = client->RDbuf( client->sd_current, buff+8, NumBytes-8, 0, 0, client->Number, Type );

		if (res>0) {

//		printf("\nresv (3005)::  CC#%i  szd=%ld   nsid=%i\n",client->Number, res, client->sd_current);


//		for (int ib=0;ib<res+sz;ib+=2) {
//			printf("%7d",(buff[ib]+buff[ib+1]*256));
//			printf("%i  0x%X \n",ib,buff[ib]);
//		    if (!(ib%12)) printf("\n");
//			printf("0x%X 0x%X   ",buff[ib],buff[ib+1]);
//			if( (ib%2)!=0 ) printf(" ");
//		}
//		printf("\n");



//		    sidi = buff[0]*256+buff[1];
//printf("sidi=%i\n",sidi);


		    if ( (sidi==3067) || (sidi==3066) ) {

			for(int ii=0;ii<sz+res;ii++) (client->datar[ii])=buff[ii];
		    }
//		    else {
//			fff = fopen("current.txt","wb");
//			fwrite(buff,sz,1,fff);
//			fclose(fff);
//		    }

		    if (sidi==3069) {
NumCUR++;
if(NumCUR>65000) NumCUR=1;
			printf("\n   !!! <pCurrent>::   Current Overload:   BSM#%02d   !!!\n",client->bsmOverCUR);
			for(int ii=0;ii<96;ii++) {
			    (client->StatusCUR[ii])=buff[ii+10];
			}
			for(int ii=0;ii<10;ii++) (client->datacur[ii])=buff[ii];
			for(int ii=10+96;ii<sz+res;ii++) (client->datacur[ii-96])=buff[ii];






			(client->Alarm)=1;
		    }
		    if (sidi==3070) {
			printf("!!! <pCurrent>::   NOT Answer from MAROC Current:\n");
			(client->numErrH)=buff[13]*256 + buff[12];
			(client->dataErrH)=buff[11]*256*256*256 +
						buff[10]*256*256 +
						buff[9]*256 + buff[8];
		    }
		    if (sidi==3071) {
			printf("!!! <pCurrent>::    NOT full FIFO of MAROC Current:\n");
			(client->numErrF)=buff[13]*256 + buff[12];
			(client->dataErrF)=buff[11]*256*256*256 +
						buff[10]*256*256 +
						buff[9]*256 + buff[8];
		    }
		    if (sidi==3072) {
			printf("!!! <pCurrent>::    Current:\n");
			for(int ii=0;ii<sz+res;ii++) (client->datacur[ii])=buff[ii];

*/
/*
		for (int ib=0;ib<res+sz;ib+=2) {
//			printf("%7d",(buff[ib]+buff[ib+1]*256));
//			printf("%i  0x%X \n",ib,buff[ib]);
//		    if (!(ib%12)) printf("\n");
			printf("%02X %02X   ",buff[ib],buff[ib+1]);
//			if( (ib%2)!=0 ) printf(" ");
		}
		printf("\n");
*/

/*
			(client->Current)=1;

		    }
		}
		else 
		{
		    printf("!!!   ERROR <recv 3005> - Not answer from  socket=%i\n",client->sd_current);
		}
	    }
	    else 
	    {
	    	usleep(10000);
	    }

	}  // while

return NULL;
}
*/
//====================================================================
/*
void* IO_device::pAlarm(void* parent)
{
IO_device *client = (IO_device *) parent;
//unsigned char buff[2000];
//long sz = 0L,res=0;
//int sidi=0;
//int NumBytes=0;
//FILE *fff;
//char Type[]="CC";
int over[24];


printf("===============  THREAD CURRENT started  =============\n");
printf("socket=%i\n",client->sd_current);

	while(1)
	{
	    usleep(10000);
	    if ( client->Alarm)<=0) continue;

//		memset(IACT_CC.StatusCUR,0,24*32);
//		memcpy(IACT_CC.StatusCUR,IACT_CC.datacur+10,24*32);

	    for(int pls=0;pls<24;pls++) {
		over[pls]=0;
		for(int jj=0;jj<32;jj++) {
		    printf("pls=%i  jj=%i  stat=%i\n",pls,jj,client->StatusCUR[pls*32+jj]);
		    if ( client->StatusCUR[pls*32+jj]>0 ) over[pls]++;
		}
	    }
	    (client->Alarm)=-1;

printf("OVERLOAD::\n");
	    GetOverloadCUR(num);


	    

//	    ii = IACT_CC.linkCC[pls];
//	    if (BSM[ii].active==0) continue;

//	    p=0;
//	    k=1;

//		if ( (jj>1) && (jj<30) ) p |= k;
//		else                     p &= ~k;

//if ((ii==16) && ( (jj==17)||(jj==19)||(jj==21)||(jj==23)||(jj==25)||(jj==27)||(jj==29) ) ) p &= ~k;
//if ((ii==19) && ( (jj==17)||(jj==19)||(jj==21)||(jj==23)||(jj==25)||(jj==27)||(jj==29) ) ) p &= ~k;

//if ((ii==17) && ( (jj==3)||(jj==5)||(jj==7)||(jj==9)||(jj==11)||(jj==13)||(jj==15) ) ) p &= ~k;
//if ((ii==18) && ( (jj==3)||(jj==5)||(jj==7)||(jj==9)||(jj==11)||(jj==13)||(jj==15) ) ) p &= ~k;

//		printf("pls=%i   jj=%i    kk=%ld p=0x%lx   maska=0x%X\n",pls,jj,k,p,IACT_CC.maskaCUR[pls]);
//		k=k<<1;
//	    }

//	    IACT_CC.StatusCUR[pls][jj] = p;
//	    printf("gate=%i   maska=0x%X\n",pls+1,IACT_CC.maskaCUR[pls]);

//	}


	}

return 1;
}
//===========================================================================


	}  // while

return NULL;
}
*/
//====================================================================
int IO_device::OpenNewFile(char *path, int por, int flag )
{
char st[10];
unsigned long mmm=0L;

	if (active==0) return -1;

	strcpy(filename,path);
//printf("open::pathname=%s  filename=%s\n",path,filename);

	if (flag==1) {
	    sprintf(st,"/BSM%02d/",Number);
	    strcat(filename,st);
	    mkdir(filename,0x41FF);
//printf("mkdir::pathname=%s  filename=%s\n",path,filename);
	}

	if ( (flag==2) || (flag==5) ) {
	    strcat(filename,"/PED/");
	    mkdir(filename,0x41FF);
//printf("mkdir::pathname=%s  filename=%s\n",path,filename);
	}

	if (flag==3) {
	    strcat(filename,"/CORATE/");
	    mkdir(filename,0x41FF);
//printf("mkdir::pathname=%s  filename=%s\n",path,filename);
	}

	if (flag==4) {
	    strcat(filename,"/CURRENT/");
	    mkdir(filename,0x41FF);
//printf("mkdir::pathname=%s  filename=%s\n",path,filename);
	}

	if (flag==6) {
	    strcat(filename,"/CUR&COR/");
	    mkdir(filename,0x41FF);
//printf("mkdir::pathname=%s  filename=%s\n",path,filename);
	}

	if (flag==200) {
	    strcat(filename,"/SIPM/");
	    mkdir(filename,0x41FF);
//printf("mkdir::pathname=%s  filename=%s\n",path,filename);
	}

	mmm = nnn*1000L;
	mmm += (unsigned long)Number;
//	printf("nnn=%ld   mmm=%ld   por=%i  pathfile={%s}\n",nnn,mmm,por,path);

	if ( mmm<=9999999L ) sprintf(st,"0%7ld",mmm);
	else                 sprintf(st,"%8ld",mmm);
	strcat(filename,st);

	if (flag==1) {
	    sprintf(st,".%03d",por);
	    strcat(filename,st);
	}

	if (flag==200) {
	    sprintf(st,".%03d",por);
	    strcat(filename,st);
	}

	if (flag==2) {
	    strcat(filename,".ped");
	}

	if (flag==5) {
	    strcat(filename,".ped_txt");
	}

	if (flag==3) {
	    strcat(filename,".corate");
	}

	if (flag==4) {
	    strcat(filename,".current");
	}

	if (flag==6) {
	    strcat(filename,".cur&cor");
	}

	if (flag==1) fdat = fopen ( filename,"wb");
	if (flag==200) fdat = fopen ( filename,"wb");
	if (flag==2) fdat = fopen ( filename,"wb");
	if (flag==3) frate = fopen ( filename,"at");
	if (flag==4) fcur = fopen ( filename,"at");
//	if (flag==6) fcuco = fopen ( filename,"at");
	if (flag==5) fped = fopen ( filename,"wt");

	printf("<OpenFile>(BSM#%02d)::OPEN fname=%s\n",Number,filename);

return 1;
}
//====================================================================
int IO_device::CloseFile(void)
{
int kk=0;

	if (active==0) return -1;

	if(fdat!=NULL) {
	    kk=fclose(fdat);
	    printf("CLOSE fdat=%s  (BSM#%02d)\n",filename,Number);
	}
return kk;
}
//====================================================================
//====================================================================
//====================================================================
#endif

