/// \file ss.c
/// \brief Функция Init

int Init(void)
{

	ChipNumber( 0x2810);
	ChipData( 0x8100);
            usleep(10000);
	ChipNumber( 0x4610);
	ChipData( 0x8201);
            usleep(10000);
	ChipNumber( 0x4210);
	ChipData( 0x8000);
            usleep(10000);
	Frequence( 0x1);

	    for(int ich=0;ich<12;ich+=2) {
		DataSmooth(ich,0,Smooth[ich]);
	    }

	    for(int ich=0;ich<12;ich+=2) {
		DataOffset(ich,0,Offset[ich]);
	    }
	    for(int ich=1;ich<12;ich+=2) {
		DataOffset(ich,2,Offset[ich]);
	    }

	    for(int ich=0;ich<12;ich+=2) {
		DataLength(ich,0,Length[ich]);
	    }
	    for(int ich=1;ich<12;ich+=2) {
		DataLength(ich,2,Length[ich]);
	    }


	    for(int ich=0;ich<12;ich+=2) {
		LowThreshold(ich,0,TH_Low[ich]);
	    }
	    for(int ich=1;ich<12;ich+=2) {
		LowThreshold(ich,4,TH_Low[ich]);
	    }
	    for(int ich=0;ich<12;ich+=2) {
		HighThreshold(ich,0,TH_High[ich]);
	    }
	    for(int ich=1;ich<12;ich+=2) {
		HighThreshold(ich,4,TH_High[ich]);
	    }


		Maska(0x80,0x0);

		Coinsidence(0x1);

		CoinsidenceWindow(25);

		RequestDelay(0x0);
		DMA_Enable();
		TriggerEnable();
return 1;
}