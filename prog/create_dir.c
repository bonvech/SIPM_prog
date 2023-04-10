//====================================================================
#include <sys/stat.h>
#include <sys/timeb.h>
//====================================================================

FILE *ffstat, *fftime, *fferr, *ffdat, *ffmnt,*ffcal, *fhist;
char pathfile[80];
char pathrun[80];
char statfile[80];
char timefile[80];
char monitor[80];
char calibrate[80];
char errname[80];
char overfile[80];
char datafile[80];
unsigned long nnn=0L;
//int portion=0;
int key_sync=0;

//====================================================================

int OpenRunFile (int key);

//====================================================================
unsigned long Create_Dir_Files(void) 
{
FILE *fsub;
char st[20];
int res=0;
char datadir[200];
int year=0;
//unsigned long ttt=0L;
int dir_add=1;


    if ( ( fsub = fopen ( "PATHFILES.CNF","r") ) == NULL  ) {
	printf("\n	ERROR::   NOT  PATHFILES.CNF   !!!!!!!\n\n");
	abort();
    }

//    fscanf(fsub,"%s",datadir);
    fscanf(fsub,"%s",pathfile);
    fclose(fsub);


    ftime(&Now);                   //  for DOS and LINUX
    t = *localtime(&Now.time);

//    t.tm_year=213;
//    t.tm_mday = 21;

    t.tm_mon+=1; 
    year = (int) fmod(t.tm_year,100);
    t.tm_year+=1900;

    nnn  =  (unsigned long)t.tm_mday*10000L + 
	    (unsigned long)t.tm_mon*100L + 
	    (unsigned long)year;
    printf ("Now is : day=%i,  month=%i,  year=%i,  nnn=%ld\n",
	    t.tm_mday,t.tm_mon,t.tm_year,nnn);
    printf ("Time is : %02d:%02d:%02d.%03d\n",
	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);

    do {

//	strcpy(pathfile,datadir);
	strcpy(datadir,pathfile);
	sprintf(st,"%06ld",nnn);
	strcat(datadir,st);

	res = mkdir(datadir,0x41FF);  //  create new directory

	if ( res==0 ) printf("NEW DIRECTORY FOR DATA IS CREATED :  %s\n",datadir);
	if ( res==0 ) continue;

	strcpy(pathrun,datadir);
	res = OpenRunFile(0);
	if ( res==0 ) continue;

	sprintf(st,".%02d",dir_add);
	strcat(datadir,st);
	res = mkdir(datadir,0x41FF);  //  create new directory
	dir_add++;
	if ( res!=0 )   printf("OLD DIRECTORY FOR DATA IS CREATED :  %s\n",datadir);
	if ( res==0 )   printf("NEW DIRECTORY FOR DATA IS CREATED :  %s\n",datadir);

	strcpy(pathrun,datadir);
	res = OpenRunFile(0);
	if ( res==0 ) continue;

//	if ( res==0 ) continue;
//	res = OpenRunFile(0);
//	if ( res==0 ) continue;

    } while (res!=0);

    printf("\n");

    strcat(datadir,"//");

	strcpy(pathfile,datadir);
	strcpy(pathrun,pathfile);

/*
    sprintf(st,"//%06ld",nnn);
    strcat(pathfile,st);
    ttt = t.tm_hour*100+t.tm_min;
    sprintf(st,"-%04ld//",ttt);
    strcat(pathfile,st);
    res = mkdir(pathfile,0x41FF);  //  create new directory
    if ( res==0 )   printf("NEW DIRECTORY FOR DATA IS CREATED :  %s\n",pathfile);
    if ( res==-1 )  printf("DIRECTORY FOR DATA WAS CREATED USIALLY :  %s\n",pathfile);
*/
//    strcpy(datadir,pathfile);
//    strcat(datadir,"/txt/");
//    res = mkdir(datadir,0x41FF);  //  create new directory

//    strcpy(datadir,pathfile);

//    strcpy(datadir,pathfile);

//    strcat(datadir,"/hist/");
//    res = mkdir(datadir,0x41FF);  //  create new directory
//    if ( res==0 )   printf("  CREATED directory  %s\n",datadir);

    strcpy(statfile,pathfile);
    strcat(statfile,"/sipm_status.txt");
    ffstat = fopen(statfile,"a");
    fprintf (ffstat,"\n================================================================================\n");
    fprintf (ffstat,"\n   Program Start:");
    fprintf (ffstat,"  %02d-%02d-%02d  %02d:%02d:%02d.%03d\n",
	    t.tm_mday,t.tm_mon,t.tm_year,
	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);
    fclose(ffstat);
printf("  CREATED  statfile=%s\n",statfile);


    strcpy(errname,pathfile);
    strcat(errname,"/sipm_errors.txt");
//    fferr = fopen(errname,"a");
//    fprintf (fferr,"\n================================================================================\n");
//    fprintf (fferr,"\n Program Start:");
//    fprintf (fferr,"  %02d-%02d-%02d  %02d:%02d:%02d.%03d LT\n",
//	    t.tm_mday,t.tm_mon,t.tm_year,
//	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);
//    fclose(fferr);
printf("  CREATED  errfile =%s\n",errname);

    strcpy(overfile,pathfile);
    strcat(overfile,"/sipm_overcur.txt");
//    fferr = fopen(errfile,"a");
//    fprintf (fferr,"\n================================================================================\n");
//    fprintf (fferr,"\n Program Start:");
//    fprintf (fferr,"  %02d-%02d-%02d  %02d:%02d:%02d.%03d LT\n",
//	    t.tm_mday,t.tm_mon,t.tm_year,
//	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);
//    fclose(fferr);
printf("  CREATED  overcur =%s\n",overfile);


    strcpy(timefile,pathfile);
    strcat(timefile,"/sipm_corate.txt");
printf("  CREATED  count rate file =%s\n",timefile);


/*
    fftime = fopen(timefile,"rt");
    if (fftime==NULL) {
	fftime = fopen(timefile,"at");
	fprintf(fftime,"             ");
        for(int num=1;num<NCLAST;num++) {
	    fprintf(fftime,"  BSM%02d",num);
	}
	fprintf(fftime,"\n");
	fclose(fftime);
    }
*/



    strcpy(monitor,pathfile);
    strcat(monitor,"/sipm_current.txt");
printf("  CREATED  current monitor =%s\n",monitor);

//    strcpy(calibrate,pathfile);
//    strcat(calibrate,"/calibrat-sci.txt");
//printf("  CREATED  calibrate =%s\n",calibrate);



/*
    fftime = fopen(timefile,"a");
    fprintf (fftime,"\n================================================================================\n");
    fprintf (fftime,"\n Program Start:");
    fprintf (fftime,"  %02d-%02d-%02d  %02d:%02d:%02d.%03d LT\n",
	    t.tm_mday,t.tm_mon,t.tm_year,
	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);
    fclose(fftime);
*/


    year = (int) fmod(t.tm_year,10);
    nnn  =  (unsigned long)t.tm_mday*1000L + 
	    (unsigned long)t.tm_mon*10L + 
	    (unsigned long)year;
//    printf ("Now is : day=%i,  month=%i,  year=%i,  nnn=%ld   path=%s\n\n",
//	    t.tm_mday,t.tm_mon,year,nnn,pathfile);



//    strcat(pathfile,"/");

//    mkdir("./CNF.tmp/",0x41FF);  //  create new directory

return nnn;
}
//====================================================================
int OpenRunFile (int key) 
{
char filename[80];
FILE *fff;

    strcpy(filename,pathrun);
    strcat(filename,"/startrun.txt");

printf("  start file =%s\n",filename);


    if (key==1) {

	ftime(&Now);

	fff = fopen(filename,"at");
	fprintf(fff,"START RUN  ->  ");
	fprintf(fff,"%s",(char*)ctime(&Now.time));
	if (key_sync==0) fprintf(fff,"Not Synchro\n");
	if (key_sync==1) fprintf(fff,"Synchro by GPS\n");
	if (key_sync==2) fprintf(fff,"Synchro by Computer\n");
//	if ((key_sync!=1)||(key_sync!=2)) fprintf(fff,"\nIACT_CC  hasn't been synchronized\n");

	fclose(fff);
    }
    if (key==0) {
	if ( (fff = fopen(filename,"r"))==NULL) 
	    return 0;
	else {
	    fclose(fff);
	    return -1;
	}
    }
return 0;
}
//====================================================================
