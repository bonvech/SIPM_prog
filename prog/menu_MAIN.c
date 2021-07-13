//====================//================================================
/*
#include "menu_DET.c"
#include "menu_CLAST.c"
#include "menu_ARRAY.c"
#include "menu_VEER.c"
#include "menu_LEDs.c"
#include "menu_MWCD.c"
*/
//#include "mainblck_T.c"
//#include "menu_CORATE.c"

//====================================================================
//int menu_MAIN(long nnn, int *Thresh, int *HV, int flag) {
//int menu_MAIN(unsigned long nnn)
int menu_MAIN(void)
{
//int Claster;
    int lf=1;
    int key=0;
    int por=0;
    int dtime=2;  //in sec
//int power=0;
//int por_hist=0;
//int jj,h;



//	for(int ih=0; ih<NMUONREADY; ih++) {

//	    MADC[ih].MC_Events_Number(1);
//	    MADC[ih].MC_Start();
//	    MADC[ih].StartData();
//	}




    do
    {
        printf("\n");
        printf("==================================================\n");
        printf("\n");
        printf("                     MAIN MENU:\n");
        printf("\n");
        printf("==================================================\n");
        printf("\n");
        printf("Select command:\n");
        printf("  0  - Change Thresholds in SIPM*.CNF files and read they\n");
        //	printf("  1  - To Syncronase Time of MEGA-Host by GPS time\n");
        //	printf("  2  - To Syncronase Time of MEGA-Host by Computer time\n");
        printf("  3  - Set Minus Power\n");
        printf("  33 - Set Plus Power\n");
        //	printf("  33 - Set ARRAY T133 (Cluster by Cluster)\n");
        printf("\n");
        printf("  4  - START RUN\n");
        printf("\n");
        //	printf("  5  - START RUN whis full data\n");
        //	printf("\n");
        //	printf("  6  - Get Ampl Calibration of DRS\n");
        //	printf("  7  - Get Optical Length of Fibers\n");
/*
        printf("  5  -- Read Amlitude Analyzer (time=%i sec)\n",dtime);
        printf("  6  -- Monitoring for all channels\n");
        printf("  66 -- Monitoring from Data\n");
        printf("  9  -- Count Rate\n");
        printf("  91 -- Time of Count Rate or Amlitude Analyzer (default=%i sec)\n",dtime);
        printf("\n");
        printf("  8  - Check Global Frequence\n");
        printf("\n");
*/
        //	printf("  9  - Count Rate & Amlitude Analyzer\n");
        //	printf("  91  - Time of Amlitude Analyzer (default=20sec)\n");
        //	printf("  99  - Time of Amlitude Analyzer (default=20sec)\n");
        printf("\n");
        printf("  27 - Exit programm\n");
        printf("==================================================\n");
/*        printf("  999 - Get Proshivka Versia\n");
        printf("  190 - Set BASE Proshivka\n");
        printf("  199 - Set Work Proshivka\n");
        printf("==================================================\n");
        printf("\n");
*/
        printf("Enter command:   ");
        scanf("%i",&key);

        switch(key)
        {
        case 199:
            for(int ih=0; ih<NMUONREADY; ih++)
            {
                SIPM[ih].Set_Proshivka_WORK();
            }
            printf("Please, restart programm\n");

            //for(int ih=0;ih<NMUONREADY;ih++) {
            //    SIPM[ih].sd_control=-1;
            //	  SIPM[ih].sd_data=-1;
            //    Connect_SIPM();
            //}
            key=11;
            lf=0;
            break;

        case 190:
            for(int ih=0; ih<NMUONREADY; ih++)
            {
                SIPM[ih].Set_Proshivka_BASE();
            }
            printf("Please, restart programm\n");
//		for(int ih=0;ih<NMUONREADY;ih++) {
//		    SIPM[ih].sd_control=-1;
//		    SIPM[ih].sd_data=-1;
//		    Connect_SIPM();
//		}
            key=11;
            lf=0;
            break;

        case 999:
            for(int ih=0; ih<NMUONREADY; ih++)
            {
                SIPM[ih].Get_Proshivka();
            }
            key=11;
            break;


        /*
        	    case 1:
        		key_sync=Set_GPS_Time();  // settime.c
        		key=11;
        	    break;
        	    case 2:
        		key_sync=Set_Comp_Time();;  // settime.c
        		key=11;
        	    break;
        */

        case 3:
            /// Выставить -24 Вольт
            /// \todo Выставить 4 раза??? на 4 управления по 7 SiPM в каждом
//		for(h=0;h<NMUONREADY;h++) {
            SIPM[0].InitMinusPower(0x2E,0x7,2);
            SIPM[0].SetMinusPower(0x2E,0x1,0x10F);
//		}
            key=11;
            break;

        case 33:
            /// Выставить +5 Вольт
            /// \todo Выставить 4 раза??? на 4 (0C, 1C, 2C, 3C) управления по 7 SiPM в каждом
            //for(h=0;h<NMUONREADY;h++) {
            SIPM[0].InitPlusPower(0x0C,0x40,0x0);
            SIPM[0].InitPlusPower(0x0C,0x70,0x4);

            for(int idet=0; idet<8; idet++)
            {
                //if (idet==7) power=0x7FF0;
                //else power=0xDAC0;
                SIPM[0].SetPlusPower(0x0C,0x30,idet,(int)(SIPM[0].Power[idet]*819)); ///< 819 - coef to obtain 0xDAC0
            }

//		    SIPM[0].SetPlusPower(0x0C,0x30,0x7FF0);
//
//		    SIPM[0].SetPlusPower(0x0C,0x30,0xDAC0);
//		    SIPM[0].SetPlusPower(0x0C,0x31,0xDAC0);
//		    SIPM[0].SetPlusPower(0x0C,0x32,0xDAC0);
//		    SIPM[0].SetPlusPower(0x0C,0x33,0xDAC0);
//		    SIPM[0].SetPlusPower(0x0C,0x34,0xDAC0);
//		    SIPM[0].SetPlusPower(0x0C,0x35,0xDAC0);
//		    SIPM[0].SetPlusPower(0x0C,0x36,0xDAC0);
//
//		    SIPM[0].SetPlusPower(0x0C,0x30,0xFFF0);
//		    SIPM[0].SetPlusPower(0x0C,0x31,0xFFF0);
//		    SIPM[0].SetPlusPower(0x0C,0x32,0xFFF0);
//		    SIPM[0].SetPlusPower(0x0C,0x33,0xFFF0);
//		    SIPM[0].SetPlusPower(0x0C,0x34,0xFFF0);
//		    SIPM[0].SetPlusPower(0x0C,0x35,0xFFF0);
//		    SIPM[0].SetPlusPower(0x0C,0x36,0xFFF0);
//
//		    SIPM[0].SetPlusPower(0x0C,0x30,0xDAC0);
//		    SIPM[0].SetPlusPower(0x0C,0x31,0x0010);
//		    SIPM[0].SetPlusPower(0x0C,0x32,0x0010);
//		    SIPM[0].SetPlusPower(0x0C,0x33,0x0010);
//		    SIPM[0].SetPlusPower(0x0C,0x34,0x0010);
//		    SIPM[0].SetPlusPower(0x0C,0x35,0x0010);
//		    SIPM[0].SetPlusPower(0x0C,0x36,0x0010);
//		}
            key=11;
            break;


//	    case 33:
//		SetArray(nnn,Thresh,HV);  // array.c
//		SetArray(nnn);  // menu_ARRAY.c
//	    break;

        case 4:
            /// if запустился новый набор данных не выходя из программы
            if (por != 0)
            {
                nnn = Create_Dir_Files();
		        //SaveConfig();
                por=0;
            }
//printf("por=%i\n", por);
//		por = Accusition(por+1,0);  // mainblock.c

//		for(int num=0;num<NMUONREADY;num++) {
//		    SIPM[num].Threshold(SIPM[num].TH_Low,SIPM[num].TH_High); // Threshold
//		    SIPM[num].CoinsidenceMatrix(SIPM[num].Matrix); // матрица совпадений
//		}  //  for

            por = Accusition(0,0);  // mainblock.c

            //printf("por=%i\n", por);
            key=11;
            //lf=0;
            break;

        /*
        	    case 5:

        		if (por!=0) {
        		   nnn = Create_Dir_Files();
        //		   SaveConfig();
        		   por=0;
        		}
        //printf("por=%i\n", por);
        //		por = Accusition(por+1,1);  // mainblock.c
        		por = Accusition();  // mainblock.c

        //printf("por=%i\n", por);
        		key=11;
        		lf=0;
        	    break;
        */
//	    case 5:
//		LED_menu();
//		menu_LEDs();  // menu_LEDs.c
//		key=11;
//	    break;
//	    case 6:
//		Calibration();  // calibration.c
//		key=11;
//	    break;

//	    case 7:
//		OpticDelay();  // optic_delay.c
//		key=11;
//	    break;

        case 8:
            /*
            		for (int h=0;h<NCLASTREADY;h++) {
            		    if (DRS[h].Type!=HOST_NEW) continue;
            		    DRS[h].Connect(&DRS[h].sd_control,DRS[h].IPname,DRS[h].Port);
            		    DRS[h].Check_Frequence();
            		    DRS[h].UnConnect(&DRS[h].sd_control);
            		}
            */
            //		for (int h=0;h<NCLASTREADY;h++) {
            //		    if (DRS[h].Type==HOST_NEW) continue;
            //		    DRS[h].Check_Frequence();
            //		}
            for (int h=0; h<NMUONREADY; h++)
            {
                SIPM[h].Check_Frequence();
            }
            key=11;
            break;

        case 0:
            ReadStationInfo(0);
            for (int h=0; h<NMUONREADY; h++)
            {

                for(int ich=0; ich<12; ich+=2)
                {
                    SIPM[h].LowThreshold(ich,0,SIPM[h].TH_Low[ich]);
                    SIPM[h].HighThreshold(ich,0,SIPM[h].TH_High[ich]);
                }
                for(int ich=1; ich<12; ich+=2)
                {
                    SIPM[h].LowThreshold(ich,4,SIPM[h].TH_Low[ich]);
                    SIPM[h].HighThreshold(ich,4,SIPM[h].TH_High[ich]);
                }
            }
            key=11;
            break;

        case 5:
            for (int h=0; h<NMUONREADY; h++)
            {
//		    SIPM[h].Time_Amplitude_Analyzer(dtime);  //  <- in mlsec
            }
            for (int h=0; h<NMUONREADY; h++)
            {
//		    SIPM[h].Start_Amplitude_Analyzer();
            }
            sleep(dtime+1);
            for (int h=0; h<NMUONREADY; h++)
            {
//		    SIPM[h].Read_Amplitude_Analyzer_by_bytes();
//		    SIPM[h].Read_Amplitude_Analyzer(1,1,por_hist);
            }
//		por_hist++;

            key=11;
            break;

//	    case 51:
//		printf("Enter new time for Amplitede Analyzer (int sec)\n");
//		scanf("%i",&dtime);
//		key=11;
//	    break;
        case 6:
//		Monitoring(0);  // monitor.c  //  from analizator
            key=11;
            break;
        case 66:
//		Monitoring(1);  // monitor.c  //  from data
            key=11;
            break;
        case 9:
            for (int h=0; h<NMUONREADY; h++)
            {
//		    SIPM[h].CountRate(dtime,0,0,por_hist);
            }
//		por_hist++;
            key=11;
            break;
        case 91:
            printf("Enter new time for Count Rate (int sec)\n");
            scanf("%i",&dtime);
            key=11;
            break;



        case 27:
            lf=0;
            key=11;
            break;
        default:
//		key=0;
            key=11;
            break;
        }
    }
    while(lf);

    return 1;
}
//====================================================================
//====================================================================

