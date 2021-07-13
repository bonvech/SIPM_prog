#ifndef __READ_CNF__
#define __READ_CNF__
//====================================================================
int ReadHostInfo(void);
int ReadStationInfo(int key);
void SaveConfigHost(void);
//void SaveConfigStation(void);
//====================================================================

/// @param key управляет версией конфиг файла для чтения
/// 0 - стандартный SIPM_01.CNF
int ReadStationInfo(int key)
{
    char fname[200];
    char st[200];
    FILE *hf;
    const int max_length=80;

    char tmpline[max_length];
    char line[max_length];
    int  i=0;
    int lng =0;
    int pos=0;
    int flag=0;
    char type[200];
    int num=0;
    //char IPsock[16];
    //int IDsock= 0;
    //int ClNum[4] = {0,0,0,0};
    //IPsock[0]=NULL;
    //IPsock[0]=0;
    //int cr=0;
    int data=0;
    int det=0,thl=0,thh=0,off=0,len=0,smo=0,act=0;
    float power=0;
    //int thm=0;
    int m[12];
    int matrix=0;


    int full_list=0;

    ffstat = fopen(statfile,"at");
    fprintf(ffstat,"\nConfiguration of SIPM:\n");


    for(int ii=0;ii<NMUONREADY;ii++)  {

	if (SIPM[ii].active==-1) continue;
	num = SIPM[ii].Number;

	if (key==0) {
	    sprintf(st,"SIPM_%02d.CNF",num);
	    strcpy(fname,st);
	}
//	if (key==1) {
//	    sprintf(st,"TAIMU_%02d.CNF.monitor",num);
//	    strcpy(fname,st);
//	}

	hf = fopen(fname,"r");

	if (!hf) {
	    printf("\n!!!   Configuration file %s not found   !!!\n\n",fname);
	    fprintf(ffstat,"\n!!!   Configuration file %s not found   !!!\n\n",fname);
	    abort();
	}

	printf("\nConfiguration of SIPM:   (%s)\n",fname);

	while(fgets(&tmpline[0],max_length,hf))   //check if the line starts with #, in this case it is comment line
	{
	    flag =0;
	    for (i=0;i<=(lng = strlen(tmpline)); i++)
		if ((tmpline[i] == '#')&&(!flag)) { pos =i; flag =1;}  //printf("String length %d\n", lng);

	    if (flag) {
		strncpy(line,tmpline,pos);
		flag = 0;   //line[pos]= NULL;
		line[pos]=0;   //printf("Comment position %d\n", pos);
	    } 
	    else strcpy(line,tmpline);

//read info from line
//printf ("Read line, comments excluded: %s\n", line);

	    sscanf(&line[0],"%s",&type[0]);
//	    printf("type={%s}\n",type);

	    if ( strcmp(type,"SIPM_01:")==0 ) {
		sscanf(&line[10],"%s",&st[0]);
		memcpy(SIPM[ii].IPname,st,16);
		strcpy(type,"#");
	    }

	    if ( strcmp(type,"Coinsidence:")==0 ) {
		sscanf(&line[14],"%d",&data);
		SIPM[ii].Coin = data;
		strcpy(type,"#");
	    }

	    if ( strcmp(type,"Window")==0 ) {
		sscanf(&line[33],"%d",&data);
		SIPM[ii].WinCoin = data;
		strcpy(type,"#");
	    }

	    if ( strcmp(type,"Thresholds:")==0 ) {
		fgets(&tmpline[0],max_length,hf);
		fgets(&tmpline[0],max_length,hf);
		for(int jj=0;jj<12;jj++) {
//		    fscanf(hf,"%i%i%i%i%i%i%i",&det,&thl,&thh,&off,&len,&smo,&thm);
		    fscanf(hf,"%i%i%i%i%i%i%f%i",&det,&thl,&thh,&off,&len,&smo,&power,&act);
		    SIPM[ii].TH_Low[jj] = thl;
		    SIPM[ii].TH_High[jj] = thh;
		    SIPM[ii].Offset[jj] = off;
		    SIPM[ii].Length[jj] = len;
		    SIPM[ii].Smooth[jj] = smo;
		    SIPM[ii].Power[jj] = power;
		    SIPM[ii].ActiveCh[jj] = act;
//		    SIPM[ii].TH_Mon[jj] = thm;
		}
		strcpy(type,"#");
	    }

	    if ( strcmp(type,"Matrix_of_Coinsidence:")==0 ) {
		fgets(&tmpline[0],max_length,hf);
		fgets(&tmpline[0],max_length,hf);
		for(int jj=0;jj<12;jj++) {
		    fscanf(hf,"%i%i%i%i%i%i%i%i%i%i%i%i%i",&det,
			&m[0],&m[1],&m[2],&m[3],&m[4],&m[5],&m[6],&m[7],&m[8],&m[9],&m[10],&m[11]);
//		    printf("%2i   %i %i %i %i %i %i %i %i %i %i %i %i\n",det,
//			m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11]);
		    matrix = 0;
		    for(int ij=0;ij<12;ij++) {
			if (m[ij]==0) continue;
			matrix += pow(2,ij);
//printf("jj=%i   mij=%i   matrix=%i\n",jj,m[ij],matrix);
		    }
		    SIPM[ii].Matrix[jj] = matrix;
		}
		strcpy(type,"#");
	    }  // matrix

/*
	    if ( strcmp(type,"Matrix_for_Monitoring:")==0 ) {
		fgets(&tmpline[0],max_length,hf);
		fgets(&tmpline[0],max_length,hf);
		for(int jj=0;jj<12;jj++) {
		    fscanf(hf,"%i%i%i%i%i%i%i%i%i%i%i%i%i",&det,
			&m[0],&m[1],&m[2],&m[3],&m[4],&m[5],&m[6],&m[7],&m[8],&m[9],&m[10],&m[11]);
//		    printf("%2i   %i %i %i %i %i %i %i %i %i %i %i %i\n",det,
//			m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11]);
		    matrix = 0;
		    for(int ij=0;ij<12;ij++) {
			if (m[ij]==0) continue;
			matrix += pow(2,ij);
//printf("jj=%i   mij=%i   matrix=%i\n",jj,m[ij],matrix);
		    }
		    SIPM[ii].Matrix_Mon[jj] = matrix;
		}
		strcpy(type,"#");
	    }  // matrix
*/


	}  //while

	fclose(hf);
	full_list++;

	printf("\n'%s'#%02d   IP:'%s'\n  Coin=%i  WinCoin=%i\n",
	    SIPM[ii].TypeName,SIPM[ii].Number,SIPM[ii].IPname,SIPM[ii].Coin,SIPM[ii].WinCoin);
	fprintf(ffstat,"\n%s#%02d   IP:%s\n  Coin=%i  WinCoin=%i\n",
	    SIPM[ii].TypeName,SIPM[ii].Number,SIPM[ii].IPname,SIPM[ii].Coin,SIPM[ii].WinCoin);

//	printf("      TH_L   TH_H   Offset   Lengt   Smooth   TH_M   Matrix   Matrix_Mon\n");
//	fprintf(ffstat,"      TH_L   TH_H   Offset   Lengt   Smooth   TH_M   Matrix   Matrix_Mon\n");
	printf("      TH_L   TH_H   Offset   Lengt   Smooth   Power   Active   Matrix\n");
	fprintf(ffstat,"      TH_L   TH_H   Offset   Lengt   Smooth   Power   Active   Matrix\n");
	for(int jj=0;jj<12;jj++) {
//		printf("%3i%7i%7i%9i%8i%8i%8i    0x%X    0x%X\n",jj,
		printf("%3i%7i%7i%9i%8i%8i%8.2f%8i    0x%X\n",jj,
		    SIPM[ii].TH_Low[jj], SIPM[ii].TH_High[jj],
		    SIPM[ii].Offset[jj], SIPM[ii].Length[jj],
		    SIPM[ii].Smooth[jj], SIPM[ii].Power[jj], 
		    SIPM[ii].ActiveCh[jj], SIPM[ii].Matrix[jj]);
//		    SIPM[ii].Smooth[jj], SIPM[ii].TH_Mon[jj],
//		    SIPM[ii].Matrix[jj],SIPM[ii].Matrix_Mon[jj]);
		fprintf(ffstat,"%3i%7i%7i%9i%8i%8i%8.2f%8i    0x%X\n",jj,
//		fprintf(ffstat,"%3i%7i%7i%9i%8i%8i%8i    0x%X    0x%X\n",jj,
		    SIPM[ii].TH_Low[jj], SIPM[ii].TH_High[jj],
		    SIPM[ii].Offset[jj], SIPM[ii].Length[jj],
		    SIPM[ii].Smooth[jj], SIPM[ii].Power[jj], 
		    SIPM[ii].ActiveCh[jj], SIPM[ii].Matrix[jj]);
//		    SIPM[ii].Smooth[jj], SIPM[ii].TH_Mon[jj],
//		    SIPM[ii].Matrix[jj],SIPM[ii].Matrix_Mon[jj]);
	}

//	press_any_key();
    }  //for
    fclose(ffstat);

//full_list--;
    if (full_list!=NMUONREADY) {
	    printf("\n!!!   Not all Configuration files were found   !!!\n\n");
	    abort();
    }
return 1;
}
//====================================================================
#endif
