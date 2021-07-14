/**
 * \file main.cpp
 * Программа для работы с кластерами SiPM для установки TAIGA в Тункинской долине.
 * \brief Программа для Тунки:
 * - из аргументов программы получает имя хоста и порт
 * - по имени хоста связывает соответствующий ip-адрес с сокетом
 * -- \todo нужно сделать:
 * -- подключает клиентов и выводит на экран ip клиента
 * -- следит за отключением клиентов
 * -- подключать клиентов только с описанными в конфигурационном файле ip
 * 
 * **/
#include <sys/types.h>
#include <signal.h>
#include <sys/io.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <time.h>
#include <math.h>
#include <sys/timeb.h>


//net libs
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>

#include <pthread.h>

struct timeb Now;
struct tm t;

//#include <semaphore.h>

#define NOT_MEGA

/*

#include <sys/types.h>
#include <signal.h>
#include <sys/io.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//#define TEST_MODE
//#include <curses.h>
#include <time.h>
#include <math.h>
#include <sys/timeb.h>
*/

#define NPIN     4  // Number of pins in plate
#define NCLAST   100
#define NPLAT    4
#define NCHAN    2
#define NDET     NPLAT*NCHAN

int COINS[NCLAST];
int Thresh[NCLAST*NDET];
int HV[NCLAST*NDET];
int LED[NCLAST*NDET];
char DEFECT[NCLAST][NPLAT][NCHAN];
//unsigned long nnn=0L;

#define  EventsPerFile  100
#define  EventsPerEndMeasure 1000000
#define  TimePerHistogramm 100  // in sec

//#define NOT_MEGA


#include "bitslib.h"
#include "tools.h"


/*
void nrerror( char error_text[] ) {

	fprintf( stderr, "%s\n",error_text );
	fprintf( stderr, "... now exiting to system ...\n" );
	exit( 1 );
}
*/

FILE *ferr, *ftm, *ffsum;


#include "create_dir.c"
//#include "common.h"

//#include "io.cpp"
//#include "io-lib.cpp"
//#include "io_dev.cpp"

//#include "drs.cpp"
//#include "drs-lib.cpp"
//#include "madc.cpp"
//#include "madc-lib.cpp"


#include "sipm.cpp"
SIPM_device SIPM[100];
#include "sipm-lib.cpp"
#include "sipm_iic-lib.cpp"
#include "sipm-init.c"

//#include "mega.cpp"
//#include "mega-lib.cpp"

//#define NCLAST 20

//DEV_device MADC[NCLAST];
//DEV_device MEGA;

//byte NCLASTREADY=2;

byte NCLASTREADY=1;
byte NMUONREADY=1;
//byte ClasterReady[NCLAST];

//DRS_device DRS[100];
//MEGA_device MEGA;

//#include "drs-init.c"
//#include "drs-pshost.c"

//#include "madc-init.cpp"
//#include "settime.c"




//#include "globals_clT.h"
//#include "vme.h"
//#include "vme.cpp"
//#include "vme-lib.c"
//#include "flash_adc.c"
//#include "fadc_vme.c"

word h,m,s,mls,mks,dns;
word h0,m0,s0,mls0;
long tclast0,tclast1,tcomp0,tcomp1;


//#include "master.c"

long dat1,dat2;
int mass[2000];
FILE *fmass;




//char *path="c:\\bc\\2007\\data\\";
//char *path="~/t133/vme-inet/data/";
//char *path="";
char filename[NCLAST][120],txtname[NCLAST][120],histname[NCLAST][120];
char filetime[120];
char summname[120];


//struct date d;
//struct time t;







//#include "mwcd_lib.c"
//#include "menu_MWCD.c"

#include "stoprun.c"
#include "connect.c"
#include "acc_thread.c"
#include "accusition.c"
//#include "thresholds.c"

//#include "savedata.c"
//#include "mainblck.c"
//#include "oscill.c"
#include "config_read.c"
//#include "readconf.c"
//#include "tools.c"
//#include "controller.c"
//#include "status.c"
//#include "array.c"

//byte h_stop=0,m_stop=0,s_stop=0;

//#include "monitor.c"
//#include "monitor_data.c"
#include "menu_MAIN.c"



//char host[]="192.168.1.89";
//int SIZE=0;

//void* (*fReadSock)(void *);
//pthread_t  thread[100];

//====================================================================
int main(int argc, char *argv[])
{

    /*
    int  add=0xC0000083;
    int  addadd=0;
    int trigg[32];

    	add ^= 0xFFFFFFFF;
    	for(int ii=0;ii<32;ii++) {
    //	    addadd = (add>>ii);
    	    addadd = add/(int)pow(2,ii);
    	    trigg[ii] = ( addadd & 0x1);
    	    printf("ii=%i   trigg[ii]=%i    add=0x%X\n",ii,trigg[ii], addadd);
    	}
    press_any_key();
    */
//------------------------------------------------------------
    /*
    	SIPM[0].Number=1;
    	SIPM[0].active=1;
    //	SIPM[0].IsConnected=1;
    	    memcpy(SIPM[0].IPname,"192.168.1.200",16);
    	    SIPM[0].Port =3000;

    	SIPM[0].Connect(&SIPM[0].sd_control,SIPM[0].IPname,SIPM[0].Port);
    printf("sd_control = %i\n", SIPM[0].sd_control);

    int IDnum=0;
    	SIPM[0].Check_HW_ID(&IDnum);
    	printf("HW ID num = 0x%X\n",(unsigned int) IDnum);

    	SIPM[0].Check_Serial_ID(&IDnum);
    	printf("Serial ID num = 0x%X\n",(unsigned int) IDnum);

    	SIPM[0].Check_Board_ID(&IDnum);
    	printf("Board ID num = 0x%X\n",(unsigned int) IDnum);

    	SIPM[0].Test1(1);
    	SIPM[0].GetTest1(&IDnum);

    	SIPM[0].InitMinusPower(1,1);
    	SIPM[0].SetMinusPower(1,1);

    	SIPM_Init();

    	    SIPM[0].Port =3007;

    	SIPM[0].Connect(&SIPM[0].sd_data,SIPM[0].IPname,SIPM[0].Port);
    printf("sd_data = %i\n", SIPM[0].sd_data);

    	fReadSock = READ_socket;
    	pthread_create (&thread[SIPM[0].Number], 0, fReadSock,&SIPM[0].Number);

    	SIPM[0].StopData();
    	usleep(100000);
    	SIPM[0].ClearData();
    	usleep(100000);


    	SIPM[0].Stop_socket = 0;
    	SIPM[0].StartData();

    int step=0;

    while (step<100) {

    sleep(1);
    step++;

    printf("step %i\n",step);

    }
    	SIPM[0].Stop_socket = 1;
    */
//-----------------------


    SIPM[0].Number=1; /// number of SiPM в имени конфиг файл
    SIPM[0].active=1;

    Create_Dir_Files();  // create_dir.c

    ReadStopTime(0);  // stoprun.c


//	ReadConfigFile(Thresh,HV);  // readconf.c
//	ReadHostInfo();  // config_read.c
    ReadStationInfo(0);  // config_read.c

//press_any_key();
//exit(1);

//	Init_PsHOST();  //

    ///  Создать все соединения
    Connect_SIPM();  // connect.c

#ifndef NOT_MEGA
//	Test_Sync_Time();  //  settime.c
#endif

    /// Инициализировать все устройства
    Init_SIPM();  //sipm-init.c


    /// Основная программа
    menu_MAIN();  // main_MENU.c

//	UnConnect();
//	Accusition();
//	printf("\n           PLEASE, WAIT   2 sec\n\n");
//	sleep(2);
//	MainBlock(nnn,Thresh,1);  // mainblck.c
//	UnConnect();  // vme-lib.c
    return 1;
}
//====================================================================

