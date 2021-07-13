//====================================================================
//
//====================================================================
/*
int GetClasterNumber (int id_host, int link);
int  GetTime(int Claster,int NumEvent,byte buf[8]);
int  GetTime2(int Claster,int NumEvent,byte buf[8],byte bb1,byte bb2);
int SaveBadFile_GRANDE(int id_host,int clast,byte bbt[8]);
int SaveBadFile_T133(int id_host,int clast,byte bbt[8]);
int SaveErrFile(int id_host, int flag, int clast, byte bb0, byte bb1);
*/
int he=0,me=0,se=0,mlse=0,mkse=0,dnse=0;


int  GetTime(int Claster,int NumEvent,byte buf[8],int flag);
long long GetDtime (int h, int m, int s, int mls, int mks, int dns);

//====================================================================
void* READ_socket_SIPM(void *number ) 
{
unsigned char buff[80000];
//short int dd0=0,dd1=0,dd2=0,dd3=0;
int sz=0,szd=0;
int id_host=0;
int sidi=0;
//int kkk=0;
//int gate=-1;
int Claster=0;
FILE *fff;
//FILE *fsub;
int NumBytes=0;
unsigned long NumEvents;
//struct timeb NowErr;
int SumBytes=0;
int res=0;
int kkk=0;
//struct tm te;


fff = fopen("proba.dat","wb");
fclose(fff);




    id_host = *(int*)number;
//    printf("\nREAD_socket::  nsid=%i   num=%i\n",SIPM[0].sd_data,(*(int*)number));
    printf("\nREAD_socket::  nsid=%i   num=%i\n",SIPM[0].sd_data,(*(int*)number));

    while (1) {


//qq(200);
	if ( SIPM[0].Stop_socket==1 ) usleep(1000);
	if ( SIPM[0].Stop_socket==1 ) continue;


//qq(300);
//	sem_wait(&sem_Q);

//	sz = recv(SIPM[0].sd_data, buff, 24, MSG_WAITALL );
	sz = recv(SIPM[0].sd_data, buff, 24, 0 );


//if (sz>0) printf("sz=%i\n",sz);




	if ( (sz>0) && (sz<24) ) {

	    SumBytes=sz;
	    res=0;
	    kkk=0;

	    while (SumBytes!=24) {
		res = recv(SIPM[0].sd_data, buff+SumBytes, 24-SumBytes, 0 );
		if (res>0) {
		    SumBytes += res;
		    sz += res;
		}
		if (SumBytes!=24) {
		    kkk++;
		    if (kkk>1000) {
			printf("ERROR::<READ_socket>  SIPM=%i   Not full Header  sz=%i from 1 sec\n",id_host,sz);
			abort();
		    }
		    usleep(1000);
		}
	    }
	}

//if (sz!=-1) 
//	printf("RECV===============================================>  sz=%i\n",sz);

	if (sz>0)
	{

	    sidi = buff[1]*256+buff[0];
//	    printf("*********  sidi=%i\n",sidi);
	    NumBytes = buff[3]*256+buff[2];
//	    printf("*********  NumBytes=%i\n",NumBytes);



	    if (sz!=24) {
		printf("ERROR::<READ_socket>  SIPM=%i   Not full Header  sz=%i\n",id_host,sz);
/*
		ftime(&NowErr);
		fsub = fopen(errfile,"a");
		fprintf(fsub,"\n%s",(char*)ctime(&NowErr.time));
		fprintf(fsub,"ERROR::<READ_socket>  SIPM=%i   Not full Header  sz=%i\n\n",id_host,sz);
		fwrite(buff,sz,1,fsub);
		fclose(fsub);
*/
		abort();
	    }


		SIPM[0].NumEvents++;

		NumEvents = (unsigned long)buff[7]*256L*256L*256L
		+ (unsigned long)buff[6]*256L*256L
		+ (unsigned long)buff[5]*256L
		+ (unsigned long)buff[4];

		SIPM[0].NumEvents = NumEvents;




//if ( !(SIPM[0].NumEvents%5000)  ||  (SIPM[0].NumEvents==1) ) {

//ftime(&NowErr);
//te = *localtime(&NowErr.time);
//printf("%s",(char*) ctime(&NowErr.time));
//printf("Comp time::  %02d:%02d:%02d,%03d\n",te.tm_hour,te.tm_min,te.tm_sec,NowErr.millitm);


//CC_Time_Store_1();
//CC_Get_Current_Time(0,&dd0,&dd1,&dd2,&dd3);

//	    GetTime(Claster,0,buff+12,1);
//}
//else {


//}

	    sidi = buff[1]*256+buff[0];
//	    printf("*********  sidi=%i\n",sidi);
	    NumBytes = buff[3]*256+buff[2];
//	    NumBytes = 2052;;
//	    printf("*********  NumBytes=%i\n",NumBytes);

if ( !(SIPM[0].NumEvents%100)  ||  (SIPM[0].NumEvents==1) ) {

	    GetTime(Claster,0,buff+12,0);
	    printf("*********  sidi=%i    NumBytes=%i\n",sidi,NumBytes);
}


//	    if (NumBytes>4000) {

/*
	    if (NumBytes!=132) {
		printf("ERROR::<READ_socket>  SIPM=%i   BSM=%02d  Not match  NumBytes = %i\n",id_host,Claster,NumBytes);

		ftime(&NowErr);
		fsub = fopen(errfile,"a");
		fprintf(fsub,"\n%s",(char*)ctime(&NowErr.time));
		fprintf(fsub,"ERROR::<READ_socket>  SIPM=%i   BSM=%02d  Not match  NumBytes = %i\n",id_host,Claster,NumBytes);
//		szd = recv(SIPM[0].sd_data, buff+24, NumBytes, 0 );
		fwrite(buff,sz,1,fsub);
		fclose(fsub);
//		abort();
	    }
*/
//	    szd = recv(SIPM[0].sd_data, buff+24, NumBytes, MSG_WAITALL );
	    szd = recv(SIPM[0].sd_data, buff+24, NumBytes, 0 );

	    if ( (szd>0) && (szd<NumBytes) ) {

		SumBytes=szd;
		res=0;
		kkk=0;

		while (SumBytes!=NumBytes) {
		    res = recv(SIPM[0].sd_data, buff+24+SumBytes, NumBytes-SumBytes, 0 );
		    if (res>0) {
			SumBytes += res;
			szd += res;
		    }
		    if (SumBytes!=24) {
			kkk++;
			if (kkk>1000) {
			    printf("ERROR::<READ_socket>  SIPM=%i   Not full Data  sz=%i from 1 sec\n",id_host,sz);
			    abort();
			}
			usleep(1000);
		    }
		}
	    }

	    if (szd>0)
	    {

//		    printf("szd=%i\n",szd);

		if (szd!=NumBytes) {
		    printf("ERROR::<READ_socket>  SIPM=%i   Not full DATA  szd=%i\n",id_host,szd);

/*
		    ftime(&NowErr);
		    fsub = fopen(errfile,"a");
		    fprintf(fsub,"\n%s",(char*)ctime(&NowErr.time));
		    fprintf(fsub,"ERROR::<READ_socket>  SIPM=%i   Not full DATA  szd=%i\n",id_host,szd);
		    fwrite(buff,szd+24,1,fsub);
		    fclose(fsub);
*/
//		    press_any_key();
//		    abort();
		}

//		if (szd!=132) {
//		    printf("ERROR::<READ_socket>  SIPM=%i   Not match DATA size::   szd=%i\n",id_host,szd);
/*
		    ftime(&NowErr);
		    fsub = fopen(errfile,"a");
		    fprintf(fsub,"\n%s",(char*)ctime(&NowErr.time));
		    fprintf(fsub,"ERROR::<READ_socket>  SIPM=%i   Not match  DATA size::  szd=%i\n",id_host,szd);
		    fwrite(buff,szd+24,1,fsub);
		    fclose(fsub);
*/
//		    press_any_key();
//		    abort();
//		}
//qq(100);
//fff = fopen("proba.dat","ab");
//		fwrite(buff, NumBytes+24, 1, fff);
//fclose(fff);
//qq(200);

		fwrite(buff, NumBytes+24, 1, SIPM[0].fdat);

	    }
	    else
	    {
		usleep(1000);
	    }


/*
//		for (int ib=0;ib<156;ib+=2) {
//			printf("recv=%i::  data=%i (0x%x 0x%x)\n",ib,(buff[ib]+buff[ib+1]*256),buff[ib],buff[ib+1]);
//		}	
		for (int ib=24;ib<152;ib+=2) {
			printf("%7d",(buff[ib]+buff[ib+1]*256))/4.;
			if( (ib+10)%16==0 ) printf("\n");
		}	
	printf("acc_thread (3007)\n");
*/

	}  // sz>0
	else
	{
		usleep(1000);
	}


//	sem_post(&sem_Q);


    } // while
fclose(fff);
}
//====================================================================
//====================================================================
long long GetDtime (int h, int m, int s, int mls, int mks, int dns)
{
long long times=0;

//    if (type==1) times = (long long)(Delay);
//    if (type==2) times = (long long)(Delay*5);

	times += (long long)dns;
	times += (long long)mks*1000;
	times += (long long)mls*1000*1000;
	times += (long long)s*1000*1000*1000;
	times += (long long)m*1000*1000*1000*60;
	times += (long long)h*1000*1000*1000*60*60;

return times;
}
//====================================================================
//====================================================================
int  GetTime(int Claster,int NumEvent,byte buf[8],int flag)
{
int data[4];
int h=0,m=0,s=0,mls=0,mks=0,dns=0;

    for(int ii=0; ii<8; ii+=2) {
	data[ii/2]=((buf[ii+1]&0xff)<<8)+(buf[ii]&0xff);
//	printf("buf[%d] = 0x%x  buf[%d] = 0x%x    data[%d] = 0x%x\n",
//	printf("buf[%d] = 0x%x  buf[%d] = 0x%x    data[%d] = 0x%x\n",
//	    (ii+1) , buf[ii+1], ii, buf[ii], ii/2, data[ii/2] );
    }

	    dns = ( data[0] & 0x7f ) * 10;
	    mks = ( data[0] & 0xff80 ) >> 7;
	    mks |= (data[1] & 1) << 9;
	    mls = ( data[1] & 0x7fe ) >> 1;
	    s   = ( data[1] & 0xf800 ) >> 11;
	    s |= (data[2] & 1) << 5;
	    m  = ( data[2] & 0x7e ) >> 1;
	    h = ( data[2] & 0xf80 ) >> 7;


//if (BSM[Claster].dtime1==0) BSM[Claster].dtime1 =  GetDtime (h,m,s,mls,mks,dns);

//BSM[Claster].dtime2 =  GetDtime (h,m,s,mls,mks,dns);


struct timeb NowT;
struct tm tt;
	    ftime(&NowT);
	    tt = *localtime(&NowT.time);

	printf ("SIPM=%02d   time: %02d:%02d:%02d,%03d.%03d.%03d\n", 
	    SIPM[0].Number, h,m,s,mls,mks,dns);


/*
if (flag) {

	printf ("BSM %02d   time: %02d:%02d:%02d,%03d.%03d.%03d", 
	    BSM[Claster].Number, h,m,s,mls,mks,dns);
//	printf("   %11ld\n",BSM[Claster].NumEvents);
	printf("   %11ld   ",BSM[Claster].NumEvents);
//	printf ("   %02d:%02d:%02d,%03d   %ld\n", 
//	    tt.tm_hour,tt.tm_min,tt.tm_sec,NowT.millitm,SIPM[0].NumEvents);
	printf("%s",(char*)ctime(&NowT.time));

//	printf("    dtime1=%Ld   dtime2=%Ld\n",BSM[Claster].dtime1,BSM[Claster].dtime2);
}
*/


/*
	he=h;
	me=m;
	se=s;
	mlse=mls;
	mkse=mks;
	dnse=dns;


//if (SIPM[0].mode==0) {  //RUN

	if ( SIPM[0].time_coin==0 )SIPM[0].time_coin = BSM[Claster].dtime2;
	SIPM[0].time_now = BSM[Claster].dtime2;

	if ( (fabs)(SIPM[0].time_now-SIPM[0].time_coin) > 100) {

	    fprintf(ffcoin,"%02d%11ld%17Ld   %02d:%02d:%02d,%03d.%03d.%03d\n",
		BSM[Claster].Number,SIPM[0].NumEvents,SIPM[0].time_now,h,m,s,mls,mks,dns);
	    SIPM[0].time_coin = SIPM[0].time_now;
	}
*/
//}



//FILE *fft;
//    fft = fopen("time.txt","at");
//	fprintf (fft,"Claster=%02d   time: %02d:%02d:%02d,%03d.%03d.%03d   Event=%d\n", 
//	    ClusterT[Claster].Number, h,m,s,mls,mks,dns,NumEvent);
//    fclose(fft);

return 1;
}
//====================================================================
