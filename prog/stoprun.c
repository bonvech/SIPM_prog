//====================================================================
/// \file stoprun.c
/// \brief функции для окончания набора данных
//====================================================================

//====================================================================
int h_stop=0,m_stop=0,s_stop=0;
struct timeb Stop;
int ArrayTrigger=1;
//====================================================================
//#define MAX_MOTOR_HEAT_TIME  15
//int MotorHeatTime=3;
//====================================================================

//====================================================================
void ReadStopTime (int flag)
{
FILE *fff;
char ch[10];
char stt[80];
    if ( (fff = fopen("STOPRUN.CNF","r")) == NULL ) {
	printf("\n	ERROR::  NOT  STOPRUN.CNF   !!!!!!!\n\n");
	abort();
    }
    fscanf(fff,"%2d%1s%2d%1s%2d%s",&h_stop,&ch[0],&m_stop,&ch[0],&s_stop,stt);
//printf("stt=%s\n",(char*)stt);
//    fscanf(fff,"%i%s",&MotorHeatTime,stt);
//printf("Motor Heat= %i sec, stt=%s\n",MotorHeatTime,(char*)stt);
//    fscanf(fff,"%i%s",&ArrayTrigger,stt);
//printf("Array Trigger = %i,  stt=%s\n",ArrayTrigger,(char*)stt);
    fclose(fff);

/*
    if ( MotorHeatTime>MAX_MOTOR_HEAT_TIME ) {
	printf("ERROR::   MOTORs will been peregreved\n");
	printf("   Please, decrease time of motors heat ( < %i min )\n",
		MAX_MOTOR_HEAT_TIME);
	printf("           in   STOPRUN.CNF  - file\n");
	abort();
    }
    if ( MotorHeatTime>MAX_MOTOR_HEAT_TIME ) MotorHeatTime=MAX_MOTOR_HEAT_TIME;
//    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
//    printf("     MOTORs will be headed at %i min\n",MotorHeatTime);
//    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
*/
    
//    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    printf("\n     RUN will be stoped at    %02d:%02d:%02d\n\n",
		h_stop,m_stop,s_stop);
//    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");


    if (flag==1) {
    ffstat = fopen(statfile,"a");
//    fprintf(ffstat,"\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
//    fprintf(ffstat,"     MOTORs will be headed at %i min\n",MotorHeatTime);
//    fprintf(ffstat,"\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
//    fprintf(ffstat,"\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    fprintf(ffstat,"\n   RUN will be stoped at       %02d:%02d:%02d,000\n",
		h_stop,m_stop,s_stop);
//    fprintf(ffstat,"\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    fclose(ffstat);
    }
}
//====================================================================
//#define EVENTS_PER_END_MEASURE 1000000
//====================================================================
//int GetStopRun  ( timeb Timer)
int GetStopRun  ( int key )
{
//int hs=0,ms=0,ss=0;
//unsigned long Global_Events=0L;
//byte Claster;
int loctime,stoptime;
int dtime=0;
ftime(&Stop);
//ctime(&Stop.time);
t=*localtime(&Stop.time);

//printf("GetStopRun::  "); printf(ctime(&Stop.time));

//   hs=t.tm_hour;
// ms=t.tm_min;
//   ss=t.tm_sec;


   loctime = (int)t.tm_hour*60 + t.tm_min;

    ReadStopTime(0);

   stoptime = (int)h_stop*60 + m_stop;


//printf("%02d %02d %02d\n",hs,ms,ss);

   
//    if ( (hs>=19) && (h_stop<=7) ) return 0;
//    for(Claster=1;Claster<NCLASTREADY;Claster++) {
//	Claster = ClasterReady[ii];

//	if ( !CER_CLUST(Claster) ) continue;



//    if ( (hs >= h_stop) && (ms>=m_stop) ) return 1;

    dtime = stoptime-loctime;
if (key==1) {
    if (dtime<100) printf("     Until the finish of RUN there were %i min\n\n",dtime);
    else printf("     Until the finish of RUN there were %i min (%.1f h)\n\n",dtime, float(dtime)/60.);
}

    if ( loctime >= stoptime ) return 1;

return 0;
}
//====================================================================
