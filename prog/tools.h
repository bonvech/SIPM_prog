#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <sys/types.h>


typedef unsigned char    	byte;
//typedef u_int16_t        	word;
typedef short unsigned int		word; 
//typedef int        	word;
typedef unsigned int     	longint;

typedef unsigned int     	uint;


#define Print 1
#define NoPrint 0
#define PrintErr -1

//====================================================================
void qq(int i) 
{
    printf("QQQQQQQQQQQQQ  --  step #%i\n",i);
}
//====================================================================
void press_any_key(void) 
{
int key=0;
    printf("Press 1 and ENTER\n");
    scanf("%i",&key);
}
//====================================================================
void NB( char text[] ) 
{
    printf("%s\n",text );
}
//====================================================================
void NowIs(FILE *fff){
struct timeb NowC;
struct tm tc;
	
	ftime(&NowC);
	tc = *localtime(&NowC.time);
	fprintf(fff,"%02d:%02d:%02d  ",tc.tm_hour,tc.tm_min,tc.tm_sec);

}
//====================================================================
/*
void SaveNowTime(int flag_file, int flag_print){
struct timeb NowC;
struct tm tc;
FILE *fff;
	
	ftime(&NowC);
	tc = *localtime(&NowC.time);
//	if (flag_file!=0) {
	
	fprintf(fff,"%02d:%02d:%02d.%03d  ",tc.tm_hour,tc.tm_min,tc.tm_sec,Now.millitc);

}
*/
//====================================================================
void ERROR ( char  error_text[] )
{
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("!!!                                                                  !!!\n");
    printf("!!!          %s          !!!\n",error_text);
    printf("!!!                                                                  !!!\n");
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}

#endif
