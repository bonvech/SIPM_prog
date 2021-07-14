//====================================================================
/// @file accusition.c
/// @brief Функция набора данных, следит за набором данных.
//====================================================================
//#define EventsPerEndMeasure 100
//#define EventsPerFile 10
//#define TimeForStop 60
//#define TimePerControlAndHistogramm  100
//#include "maska.c"
//====================================================================

int TimeForStop = 30;  ///< sec интервал опроса файла stopRun
int TimePerFile = 120; ///< время набора данных в один файл

//pthread_t thread[200];
//int sig=0;

void* (*func)(void *);
//void* (*func2)(void *);
//void* (*func3)(void *);
//void* (*func4)(void *);

//void* (*fSaveT133)(void *);
//void* (*fReadSock_VME)(void *);
//void* (*fReadSock_DRS)(void *);
//void* (*fReadSock_MADC)(void *);
//void* (*fSaveGrande)(void *);

//====================================================================

//#include "acc_task_DRS.c"
//#include "acc_task_VME.c"
//#include "acc_thread.c"

/// Считает статистику после набора одной порции данных
int Statistica (int por,int dtime);

//====================================================================
//int Accusition ( unsigned long nnn )
/// Функция набора данных, следит за набором данных.
/// @param time
/// @param chan
int Accusition ( int time, int chan )
{
    //unsigned long event=0;
    time_t this_moment, last_moment;
    //int ih=0;
    //intjj=0,nj=0;
    int StopRUN=0;
    //int flag_STOP=0;
    //int ii;
    //int kk;
    //int maska=0;
    int id=0;
    int portion=1;
    //word hht=0,mmt=0,sst=0,mlsst=0,mksst=0,dnsst=0;

    //byte buff[1000];

    //FILE *fsub;
    //fsub=fopen("aaaaaaa.dat","w");
    //fclose(fsub);
    int data=0;

    for(int ih=0; ih<NMUONREADY; ih++)
    {
        SIPM[ih].Check_Frequence();
        SIPM[ih].Check_Board_ID(&data);
        SIPM[ih].Check_HW_ID(&data);
        //SIPM[ih].Check_SOFT_ID();
    }


    //press_any_key();

    //	for(int ih=0;ih<NMUONREADY;ih++) SIPM[ih].DMA_TCP_Blocked(); // stop data
    //	usleep(500000);
    //	sleep(2);

    //	for(int ih=0;ih<NMUONREADY;ih++)  SIPM[ih].DMA_Buffer_Clear(); // clear data
    //	sleep(2);

    //	for(int ih=0; ih<NMUONREADY; ih++) SIPM[ih].MC_Start();
    //	sleep(1);

    //	for (int ih=0; ih<NMUONREADY; ih++) SIPM[ih].Reset_Counter_Events();

    //press_any_key();

    if (time==0)
        portion = 1;
    else
        portion = chan;

    for(int clast=0; clast<NMUONREADY; clast++)
    {
        if (SIPM[clast].Number==0) continue;
        if (time==0)
            SIPM[clast].OpenNewFile(pathfile, portion, 200);
        else
            SIPM[clast].OpenNewFile(pathfile, portion, 200);
    }
    portion++;

    /// Функция чтения данных для параллельного потока
    //fReadSock_SIPM = READ_socket_SIPM;
    func = READ_socket_SIPM;

    /// создать всем кластерам свой поток
    for(int ih=0; ih<NMUONREADY; ih++)
    {
        if (SIPM[ih].Number == 0) continue;
        id = ih;
        //pthread_create ( &thread[SIPM[ih].id], 0, fReadSock_SIPM, &SIPM[ih].id );
        //pthread_create ( &SIPM[ih].fRecvThread, 0, fReadSock_SIPM, &SIPM[ih].id );
        //pthread_create ( &SIPM[ih].fRecvThread, 0, func, &SIPM[ih].Number );
        //pthread_create ( &SIPM[ih].fRecvThread, 0, func, &SIPM[ih].id );
        /// \todo вместо 0 передать ih
        pthread_create(&SIPM[ih].fRecvThread, 0, func, &id);
    }

    //Give_me_Time(MEGA.LinkMask);

    OpenRunFile(1);  //  create_dir.c

    /*
    /// Работа с Мегахостом
    for(int ir=0;ir<10;ir++) {

    	ftime(&Now);
    	this_moment=Now.time;
    	last_moment=Now.time;
    	printf("Computer time:   "); printf("%s",(char*)ctime(&Now.time));
    	for(int ih=0; ih<NMUONREADY; ih++) {
    		SIPM[ih].GetCurrentTime(&hht,&mmt,&sst,&mlsst,&mksst,&dnsst);
    		printf("Station # %02d ::   Current Time::  %02d:%02d:%02d,%03d.%03d.%03d\n",
    			SIPM[h].Number,hht,mmt,sst,mlsst,mksst,dnsst);
    	}
    	sleep(1);

    }
    //press_any_key();
    */

    /// Сбросить счетчики событий кластеро
    for(int ih=0; ih<NMUONREADY; ih++)
    {
        SIPM[ih].Reset_Counter_Events();
        SIPM[ih].NumEventRealy = 0; ///< Счетчик событий
        //SIPM[ih].MC_Events_Number(1);
        //SIPM[ih].MC_Start();
    }

    for(int ih=0; ih<NMUONREADY; ih++)
    {
        SIPM[ih].Stop_socket = 0; // разрешить прием данных
    }

//	for(int ih=0; ih<NMUONREADY; ih++) SIPM[ih].MC_Start();

    for(int ih=0; ih<NMUONREADY; ih++)
    {
//		    SIPM[ih].TriggerEnable_int();
        SIPM[ih].TriggerEnable(); /// Разрешили триггер
        SIPM[ih].StartData();     /// Разрешили отправлять данные
    }


    StopRUN = 0;
    ftime(&Now); /// Текущее комп время
    this_moment=Now.time;
    last_moment=Now.time;
    printf("Start accusition data at:   ");
    printf("%s",(char*)ctime(&Now.time));

    if (time>0) TimeForStop=time;

    /// Бесконечный цикл чтения данных
    while (!StopRUN)
    {
        sleep(TimeForStop);
        //sleep(60);

        NB((char*)"TIME!!!");
        StopRUN = GetStopRun(1);  //stoprun.c

        //------------------  STOP RUN START  --------------------------------
        //if (time > 0) StopRUN = 1; // для калибровки
        /// \todo это выполняется после окончания цикла - убрать за цикл
        if (StopRUN)
        {
            printf("Stop RUN\n");
            qq(111111); // вывод на экран

            for(int ih=0; ih<NMUONREADY; ih++)
            {
                //SIPM[ih].TriggerDisable_int();
                SIPM[ih].TriggerDisable(); /// остановить триггер
                SIPM[ih].StopData();       /// запрет передавать данные
            }

            ftime(&Now);
            this_moment=Now.time;
            printf("Trigger Disable at:   ");
            printf("%s",(char*)ctime(&Now.time));

            /*
            		for(int ih=0;ih<NHOSTS;ih++)  {
            		    if(HOST[ih].packet==VME_packet)  StopDataAccusition_VME(ih);
            		    if(HOST[ih].packet==DRS_packet)  {
            			StopData(ih);
            			HOST[ih].Stop_socket=1;
            		    }
            		}
            */
            printf("       WAIT 10 sec\n");
            sleep(10);  /// wait data reading

            qq(22222);

            for(int ih=0; ih<NMUONREADY; ih++)
            {
                SIPM[ih].Stop_socket = 1; ///< остановить сокет
            }

            qq(33333);

            //------------------------
            /*
            printf("     !!!  Double !!!\n");


            for(int ih=0; ih<NMUONREADY; ih++) {
                SIPM[ih].Stop_socket=0;
            }
            for(int ih=0; ih<NMUONREADY; ih++) {
                SIPM[ih].TriggerEnable(); //???
            }

            printf("       WAIT 10 sec\n");
            sleep(10);

            for(int ih=0; ih<NMUONREADY; ih++) {
                SIPM[ih].TriggerDisable(); //???
            }
            sleep(30);
            for(int ih=0; ih<NMUONREADY; ih++) {
                SIPM[ih].Stop_socket=1;
            }
            */
            //--------------------------------

            ftime(&Now);
            this_moment=Now.time;
            printf("Stop Socket at:   ");
            printf("%s",(char*)ctime(&Now.time));

            sleep(1);

            /// Закрыть и убить потоки
            for(int ih=0; ih<NMUONREADY; ih++)
            {
                //if ( SIPM[ih].id>0 ) {
                printf("\n   !!!    kill  thread   id=%i\n\n",SIPM[ih].id);

                //			pthread_cancel ( thread[SIPM[ih].id] );
                //			pthread_kill ( thread[SIPM[ih].id], sig );
                //			pthread_kill ( thread[SIPM[ih].id], 0 );
                //		    pthread_cancel ( thread[HOST[ih].id+NHOSTS] );

                pthread_cancel ( SIPM[ih].fRecvThread );
                //pthread_kill ( thread[SIPM[ih].id], sig );
                pthread_kill ( SIPM[ih].fRecvThread, 0 );
                //pthread_cancel ( thread[HOST[ih].id+NHOSTS] );
                //    }
            }
            sleep(1);
            qq(2);

            /*
            		for(int ih=0; ih<NHOSTS; ih++) {
            		    if ( HOST[ih].id>0 )
            //		    	pthread_cancel ( thread[HOST[ih].id] );
            			pthread_cancel ( thread[HOST[ih].id+NHOSTS] );
            			pthread_kill ( thread[HOST[ih].id+NHOSTS], sig );
            		}
            */

            Statistica (portion,(int)(this_moment-last_moment));


            /// Закрыть все файлы данных
            for (int clast=0; clast<NMUONREADY; clast++)
            {
                if (SIPM[clast].Number==0) continue;
                SIPM[clast].CloseFile();
                //if (ClusterT[clast].fdat!=NULL)
                //    fclose(ClusterT[clast].fdat);
            }
            qq(10);
        }  //  StopRUN=1;

        if ( StopRUN ) continue;


        //------------------  STOP RUN FINISH  -------------------------------
        ftime(&Now);
        this_moment=Now.time;

        //if(this_moment>=last_moment+TimePerControlAndHistogramm) {
        if(this_moment >= last_moment+TimePerFile)
        {
            printf("Time Per Control And Histogramm And New Files\n");

            printf("Stop data\n");
            for(int ih=0; ih<NMUONREADY; ih++)
            {
                SIPM[ih].Stop_socket=1; // остановить прием данных
            }
            //qq(100);
            //printf("		usleep(100000) \n");

            usleep(100000);
            //		sleep(1);
            //qq(101);
            printf("Close and Open new file\n");

            /// Считать статистику
            Statistica (portion,(int)(this_moment-last_moment));

            /// Закрыть старый и открыть новый файл
            for (int clast=0; clast<NMUONREADY; clast++)
            {
                if (SIPM[clast].Number==0) continue;
                SIPM[clast].CloseFile();

                SIPM[clast].OpenNewFile(pathfile,portion,200);
            }
            portion++;

            //qq(200);
            //printf("Read Time\n");
            //		AfterTime(this_moment-last_moment);  //time_after.c
            last_moment = this_moment;

            printf("Start New data\n");
            for(int ih = 0; ih<NMUONREADY; ih++)
            {
                SIPM[ih].Stop_socket=0; // Начать прием данных
            }  //for
            //qq(300);
            printf("New Cycle\n");
        }  // this_moment>last_moment
    };  // while

//------------------  main thread  -----------------------------------
//------------------  main thread  -----------------------------------

    //if (fferr!=NULL) fclose(fferr);

    ftime(&Now);   // Get Time
    printf("Stop at - ");
    printf("%s",(char*)ctime(&Now.time));

//    exit(1);

    return StopRUN;
}
//====================================================================
//====================================================================
int Statistica (int por,int dtime)
{
    int freq=0;
//unsigned long NumEv[NCLAST];
    int hh,mm,ss;
//int th=0;
    float CoRate=0;
    int kk_all=0;
    int kk_rej=0;
    word hht=0,mmt=0,sst=0,mlsst=0,mksst=0,dnsst=0;

    ffstat = fopen(statfile,"a");
    fftime = fopen(timefile,"a");
    printf("\n");

    ftime(&Now);
    t=*localtime(&Now.time);
    hh=t.tm_hour;
    mm=t.tm_min;
    ss=t.tm_sec;


    for(int h=0; h<NMUONREADY; h++)
    {
        freq = SIPM[h].Check_Frequence();
        fprintf(ffstat,"%s # %3d ::   Frequence=0x%X\n",SIPM[h].TypeName,SIPM[h].Number,freq);
    }
    fprintf(ffstat,"\n");

    printf("\n");


    printf("\nPortion=%i          Computer Time:  %02d:%02d:%02d,%03d\n",por,hh,mm,ss,Now.millitm);
    fprintf(ffstat,"\nPortion=%i          Computer Time:  %02d:%02d:%02d,%03d\n",por,hh,mm,ss,Now.millitm);

    for(int h=0; h<NMUONREADY; h++)
    {

//		if ( DRS[h].Type == HOST ) continue;
//		if ( DRS[h].Type == HOST_NEW ) continue;

        hht=0;
        mmt=0;
        sst=0;
        mlsst=0;
        mksst=0;
        dnsst=0;

        SIPM[h].GetCurrentTime(&hht,&mmt,&sst,&mlsst,&mksst,&dnsst);

        printf("Station # %02d ::   Current Time::  %02d:%02d:%02d,%03d.%03d.%03d\n",
               SIPM[h].Number,hht,mmt,sst,mlsst,mksst,dnsst);
        fprintf(ffstat,"Station # %02d ::   Current Time::  %02d:%02d:%02d,%03d.%03d.%03d\n",
                SIPM[h].Number,hht,mmt,sst,mlsst,mksst,dnsst);
    }
    fprintf(ffstat,"\n");


    fprintf(fftime,"%02d:%02d:%02d   ",hh,mm,ss);

    kk_all=0;
    kk_rej=0;

    for(int h=0; h<NMUONREADY; h++)
    {

//		if ( DRS[h].Type == HOST ) continue;
//		if ( DRS[h].Type == HOST_NEW ) continue;

//		CoRate = (float)SIPM[h].NumEventRealy/(float)TimePerFile;
        CoRate = (float)SIPM[h].NumEventRealy/(float)dtime;



//		NumEv[h] = SIPM[h].GetNumEvents();
//		DRS[h].NumBytesFile=0;
//		DRS[h].NumEvents += NumEv[h];

//		th = DRS[h].Read_Any();

//		printf("Station # %02d ::   Count Rate = %.2f Hz    Threshold = %i",
//		    SIPM[h].Number, (float)NumEv[h]/(float)TimePerFile,th);
//		fprintf(ffstat,"Station # %02d ::   Count Rate = %.2f Hz    Threshold = %i",
//		    SIPM[h].Number, (float)NumEv[h]/(float)TimePerFile,th);

        printf("Station # %02d ::   Count Rate = %.2f Hz   dtime = %i sec",
               SIPM[h].Number, CoRate,dtime);
        fprintf(ffstat,"Station # %02d ::   Count Rate = %.2f Hz",
                SIPM[h].Number, CoRate);

        if ( SIPM[h].IsConnected<=0 )
        {
            fprintf(ffstat,"   Station does not work\n");
            printf("   Station does not work\n");
            kk_rej++;
        }
        else
        {
            fprintf(ffstat,"\n");
            printf("\n");
        }

        SIPM[h].NumEventRealy=0;
        kk_all++;

//		fprintf(fftime,"%7.2f",(float)(NumEv[h])/(float)TimePerFile);


//		DRS[h].Get_Error_Plus();
//		DRS[h].Get_Error_Minus();

    }
    fprintf(fftime,"\n");
    fclose(fftime);


    fprintf(ffstat,"\n");


    fclose(ffstat);
    return kk_all;
}
//====================================================================
