//====================================================================
/*
bool Connect_MEGA(void)
{

	MEGA.IsConnected=false;
	MEGA.CreateSocket();
	MEGA.Connect(MEGA.sd_control,MEGA.IPname,MEGA.Port);

	return true;
}
//====================================================================
bool UnConnect_MEGA(void)
{
	MEGA.UnConnect();
return true;
}
//====================================================================
int Control_Firmware() 
{
int kkk=0;

    kkk = Get_Proshivka();

    if ( (kkk!=0) && (kkk!=1) )  abort();

    if (kkk==1) return 1;

    if (kkk==0) {

	printf("   PLEASE, wait 30 sec.  Firmware WORK download\n");

	Set_Proshivka_WORK();

	shutdown(IACT_CC.sd_control, SHUT_RD|SHUT_WR );
	close(IACT_CC.sd_control);
	IACT_CC.sd_control = -1;
	sleep(30);

	if(IACT_CC.sd_control < 0)
	{
		IACT_CC.sd_control = socket(AF_INET, SOCK_STREAM, 0);
		if(IACT_CC.sd_control < -1) 
		{
			printf("Unable to create CONTROL socket.\n");
			return false;
		}
		// setting nonblocking option for socket
		fcntl(IACT_CC.sd_control, F_SETFL, O_NONBLOCK);
		printf("New CONTROL socket=%i\n",IACT_CC.sd_control);
	}

	IACT_CC.IsConnected=1;
	IACT_CC.Connect(IACT_CC.sd_control,IACT_CC.IPname,3000);

sleep(1);
	kkk = Get_Proshivka();

	if (kkk==1) return 1;
	if (kkk==0) {
	    printf("   !!! ALARM !!!   Firmware NOT Download\n");
	    press_any_key();
	    return -1;
	}
	if ( (kkk!=0) && (kkk!=1) )  abort();
    }

return 1;
}
//====================================================================
int Control_Frequence() 
{
int kkk=0;

    kkk = Check_Frequence();

    if (kkk==1) return 1;

    Host_Frequence(1);

    kkk = Check_Frequence();

    if (kkk==0) press_any_key();

return 1;
}
*/
//====================================================================
bool Connect_SIPM(void)
{
//int ii;
//void*(*fAlarm)(void*);


//    for(int h=1;h<3;h++) {
//	if (IACT_CC.active==0) continue;
//	printf("h=%i  name=%s  sd_control=%i,  sd_data=%i\n",h,IACT_CC.IPname,IACT_CC.sd_control,IACT_CC.sd_data);
//    }

//qq(100);

//    for(int h=1;h<3;h++) {

//	if (IACT_CC.active==0) continue;



	SIPM[0].Connect(&SIPM[0].sd_control,SIPM[0].IPname,3000);


//	Control_Frequence();

//qq(2);

//	Control_Firmware();

//	DMA_Enable();

//	Trigger_Disable();

//	Control_Frequence();

	SIPM[0].Connect(&SIPM[0].sd_data,SIPM[0].IPname,3007);
//	IACT_CC.Connect(IACT_CC.sd_current,IACT_CC.IPname,3005);
//	IACT_CC.IsConnected=true;
//	IACT_CC.Number = 1;
    



///ffstat = fopen(statfile,"at");
//fprintf(ffstat,"\nIACT_CC  Connect::   IP:%s/3000   sd_control=%i\n",IACT_CC.IPname,IACT_CC.sd_control);
//fprintf(ffstat,"IACT_CC  Connect::   IP:%s/3007   sd_data=%i\n",IACT_CC.IPname,IACT_CC.sd_data);
//fprintf(ffstat,"IACT_CC  Connect::   IP:%s/3005   sd_currentl=%i\n",IACT_CC.IPname,IACT_CC.sd_current);
//fclose(ffstat);


	return true;
}
//====================================================================
