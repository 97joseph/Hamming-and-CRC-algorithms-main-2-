#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bool int
#include <time.h>



//ASCII to Binary convertion mode
int *bin(int x, int *n)
{
        int c,k;
        for (c = 7; c>=0; c--)
        {
                k = x>>c;
                if (k&1)
                        n[7-c] = 1;
                else
                        n[7-c] = 0;
        }
        return n;
}

//Odd bit parity 
bool oddParity (int n)
{
        bool parity = 1;
        while (n)
        {
                parity = !parity;
                n = n & (n-1);
        }
        return parity;
}


//Application Layer
char *applicationLayer(char *filename, char *ch)
{
	//reading data from input files
	FILE *fPointer;
	fPointer = fopen(filename,"r");
	int i = 0;
	char singleChar;
	while ((singleChar = fgetc(fPointer)) != EOF)
	{
		ch[i] = singleChar;
		i = i+1;
	}
	ch[i] = 0;
	fclose(fPointer);
	return ch;
}

//Data Link Layer
int *dataLinkLayer(char *a, int n, int len, int *frame){
	int syn = 22; //syn character
	int index = 0;
	//framing
	for (int i = 0; i<(n*11); i=i+11){
		frame[i] = syn; //first syn character insertion in the frame
		frame[i+1] = syn; //second syn character insertion in the frame
		//control character insertion in the frame
		if ((len-(((i+11)/11)*8))>=0)
			frame[i+2] = 8;
		else if ((len-(((i+11)/11)*8))<0)
			frame[i+2] = len%8;
		//data insertion in the frame
		for (int j = i+3; j<i+11; j++){
			if (index < len){
				frame[j] = a[index];
				index++;
			}
		}	
	}
	return frame;
}

//Physical Layer
void physicalLayer(int n, int *frame, char*filename){

	int choice;

	printf("Choice of error detection:\n1. For CRC detection, enter 1\n2. For Hamming code detection, enter 2\n");
	scanf("%d",&choice);
			

	//converting frames to binary and adding odd bit parity
	int binData[n*88];
	int l = 0;	
	for (int i=0; i<(n*11); i=i+11)
	{	for (int j=i; j<i+11; j++){
			int binArray[8];
			int *binNum;
			int x;
			x = frame[j];
			binNum = bin (x,binArray); //converting frame to binary
			binNum[0] = oddParity(x); //adding oddParity to the frame
			
			for(int a = 0; a<8; a++)
			{
				binData[l] = binNum[a];
				l++;	
			}
		}
	}
	
	//accessing each frame

	int frameTransmit[1200];
	int f = 0;
	for (int i=0; i<(n*88); i=i+88)
	{

		int binFrame[88];
		int l = 0;
		
		for (int j=i; j<i+88; j++){
		
			binFrame[l] = binData[j];
			//printf("%d",binFrame[l]);
			l++;	
		}
		//printf("\n");
		
		// CRC
		if (choice == 1){
			int generator[33] = {1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,1,1,1};
			int gsize = 33;
			int dataword[64+33-1];
			int dsize = 64;
			int zeros[gsize];
			int remainder[gsize];

			//reading binary data in dataword
			for(int b = 0; b<64; b++)
			{
				dataword[b] = binFrame[b+24];
			} 
			

			//adding 32 0's
			
			for (int b = 0; b<gsize-1;b++)
			{
				dataword[64+b] = 0;
			}
			for (int b = 0; b<gsize; b++)
			{
				zeros[b] = 0;
			}

			//Remainder calculation

			for (int a = 0; a<gsize; a++)
			{
				remainder[a] = dataword[a];	
			}
			int d = gsize;
			while (d<dsize+gsize)
			{	
				if (remainder[0] == generator[0])
				{						
					for (int c = 0; c<gsize; c++)
					{
						if (remainder[c] == generator[c]) 
							remainder[c] = 0;
						else
							remainder[c] = 1;
					}
				}	
				else 
				{	
					for (int c = 0; c<gsize; c++)
					{
						if (remainder[c] == zeros[c]) 
							remainder[c] = 0;
						else
							remainder[c] = 1;
					}
				}	
				for (int c = 0; c<gsize-1; c++)
				{
					remainder[c] = remainder[c+1];
				}
				remainder[gsize-1] = dataword[d];
				d++;
			}
			int code[gsize-1];
			for (int i=0; i<gsize-1;i++)
			{
				code[i] = remainder[i];
				//printf("%d",code[i]);
			
			}
			//printf("\n");
			for (int a = 0; a<gsize-1; a++)
			{	
				dataword[dsize+a] = code[a];
			}
			
			//checking zero remainder
					
			for (int a = 0; a<gsize; a++)
			{
				remainder[a] = dataword[a];	
				//printf("%d",remainder[a]);
			}
			//printf("\n");
			d = gsize;
			while (d<dsize+gsize)
			{				
				if (remainder[0] == generator[0])
				{						
					for (int c = 0; c<gsize; c++)
					{
						if (remainder[c] == generator[c]) 
							remainder[c] = 0;
						else
							remainder[c] = 1;
					}
				}	
				else 
				{	
					for (int c = 0; c<gsize; c++)
					{
						if (remainder[c] == zeros[c]) 
							remainder[c] = 0;
						else
							remainder[c] = 1;
					}
				}
				for (int c = 0; c<gsize-1; c++)
				{
					remainder[c] = remainder[c+1];
				}
				remainder[gsize-1] = dataword[d];
				d++;
			}
			
			//printf("Check Final ");
			code[gsize-1];
			for (int b=0; b<gsize-1;b++)
			{
				code[b] = remainder[b];
				//printf("%d",code[b]);
			}
			
			//printf("\n");		
			
			//reframing after CRC code addition		
			
			int frameCRC[88+gsize-1];
			for (int b = 0; b<88;b++)
			{
				frameCRC[b] = binFrame[b];
			}
			for (int b = 0; b<gsize-1;b++)
			{
				frameCRC[b+88] = dataword[b+64];
			}

			//Putting the frames together for transmission		

			for(int b = 0; b<24+dsize+gsize-1; b++)
			{
				frameTransmit[f] = frameCRC[b];			
				f++;
			}
			
		}
		// Hamming code detection
		else if (choice == 2){
			int dsize = 64;
			int psize = 7;
			int data[64];
			int parity[7] = {0,0,0,0,0,0,0};
			int parityIndex[7] = {1,2,4,8,16,32,64};
			int encoded[dsize+psize+1];
			int parityBit;
			//printf("Data:\n");
			for (int b = 0; b<dsize; b++)
			{
				data[b] = binFrame[b+24];
				//printf("%d",data[b]);
			}

			//printf("\n");
			//inserting data in encoded array
			int d = dsize-1;
		 	int p = 0;
			for (int a = 1; a<psize+dsize+1; a++)
			{	
				//printf("a:%d,p:%d,d:%d",a,p,d);
				if ((a == 1) | (a == 2) | (a == 4) | (a == 8) | (a == 16) | (a == 32) | (a == 64)){
					encoded[a] = parity[p];
					p++;
				}
				else {
					encoded[a] = data[d];	
					//printf("%d",encoded[a]);
					d--;
				}	 
			}
			//printf("\n");
			// introducing parity bit
			int a;
			for (int b = 0; b<psize; b++)
			{	int sum = 0;
				//printf("\nIn %d:\n",parityIndex[b]);
				for (int c = parityIndex[b]; c<psize+dsize+1; c=c+parityIndex[b]+1)	{
					for (a = 0; a < parityIndex[b]; a++){	
						//printf("%d ",c+a);					
						if (c+a>psize+dsize)	
							break;		
						//printf("Encode Index %d value: %d\n", c+a,encoded[c+a]);
						sum = sum + encoded[c+a];
					}
					c = c+a-1;	
				}
				if (sum%2 == 1)
					encoded[parityIndex[b]] = 1;
				else 
					encoded[parityIndex[b]] = 0;

				//printf("Parity Encoded value: %d\n",encoded[parityIndex[b]]);
			}
			/*
			printf("Encoded:\n");	
			for (int a = 1; a<psize+dsize+1; a++)
			{
				printf("%d",encoded[a]);
			}
			printf("\n");
			*/
			int sum = 0;
			for (int b = 1; b<psize+dsize+1; b++)
			{
				sum = sum + encoded[b];
			}			
			if (sum%2 == 1)
				parityBit = 1;
			else 
				parityBit = 0;
			
			//reframing after hamming encoding
			
			int frameHamming[24+psize+dsize+1];
			for (int b = 0; b<24;b++)
			{
				frameHamming[b] = binFrame[b];
			}
			frameHamming[24] = parityBit;
			for (int b = 1; b<psize+dsize+1;b++)
			{
				frameHamming[b+24] = encoded[b];
			}
			
			//Putting the frames together for transmission		
			for(int b = 0; b<24+dsize+psize+1; b++)
			{
				frameTransmit[f] = frameHamming[b];	
				//printf("%d",frameTransmit[f]);		
				f++;
			}
			//printf("\n");
  		}
		
	}
	//Error creation
	printf("Do you wish to introduce error in the transmitted message? Enter 1 for yes\n");
	int e,m;
	scanf("%d",&e);

	if (e == 1)
	{ 	printf("How many bits of error do you wish to introduce?\n");
		scanf("%d",&m);
	}
	
	if (e == 1){		
		if (choice == 1){			//error creation for CRC
			int temp[n*64];
			int l = 0;		
			for (int i=0; i<(n*120); i=i+120)
			{			
				for (int j=i+24; j<i+88; j++){
				
					temp[l] = frameTransmit[j];
					//printf("%d",temp[l]);
					l++;	
				}
			}
			int randomIndex;
			srand (time(NULL));
		 	int frameNum, byteNum;
			double bitPosition;
			for (int i = 0; i<m; i++){
				randomIndex = rand()%(n*64);
				frameNum = randomIndex/64+1;
				byteNum = (randomIndex/8)-((frameNum-1)*8)+1;
				bitPosition = randomIndex-((frameNum-1)*64)-((byteNum-1)*8);
				if (temp[randomIndex] == 1)
					temp[randomIndex] = 0;
				else
					temp[randomIndex] = 1;
				printf("Index: %d Frame Number: %d Byte Number: %d Bit Position: %d \n",randomIndex,frameNum,byteNum,(int)bitPosition);
			}
			l = 0;
			for (int i=0; i<(n*120); i=i+120)
			{			
				for (int j=i+24; j<i+88; j++){
				
					frameTransmit[j] = temp[l];
					//printf("%d",temp[l]);
					l++;	
				}
				//printf("\n");
			}
		}
		else if (choice == 2){					//error creation in hamming code
			int temp[n*64];
			int l = 0;		
			for (int i=0; i<(n*96); i=i+96)
			{			
				for (int j=i+25; j<i+96; j++){
					temp[l] = frameTransmit[j];
					//printf("%d",temp[l]);
					l++;
					
				}
				//printf("\n");
			}
			
			
			int randomIndex;
			srand (time(NULL));
		 	int frameNum, byteNum;
			double bitPosition;
			for (int i = 0; i<m; i++){
				randomIndex = rand()%(n*71);
				frameNum = randomIndex/71+1;
				byteNum = randomIndex/8+1;
				bitPosition = randomIndex-((frameNum-1)*71);
				if (temp[randomIndex] == 1)
					temp[randomIndex] = 0;
				else
					temp[randomIndex] = 1;
				printf("Index: %d Frame Number: %d Byte Number: %d Bit Position: %d \n",randomIndex+1,frameNum,byteNum,(int)bitPosition+1);
			}

			l = 0;
			for (int i=0; i<(n*96); i=i+96)
			{		
				for (int j=i+25; j<i+96; j++){
					frameTransmit[j] = temp[l];
					//printf("%d",frameTransmit[j]);
					l++;
				}
				
				
			}
		/*		
		for (int i = 24; i<95; i++)
		{
			printf("%d",frameTransmit[i]);
		}	
		//printf("\n");*/
		}
	}

	//writing the packed binary frames in the output file
	FILE *fPointer;
		
	fPointer = fopen(filename,"wb");
	fwrite(frameTransmit,sizeof(frameTransmit),1,fPointer);
	fclose(fPointer);		
}


int main()
{
	//Application Layer
	
	char *filename_input = "input.inpf";	
	char charArray[40];
	char *a;
	
	a = applicationLayer(filename_input,charArray);
	

	//Data Link Layer: Framing
	
	int len = strlen(a)-1;//length of data
	int n = len/8 +1; //number of frames necessary	
	int frameArray[n*11];
	int *frame;
	
	
	if (len%8 == 0){
		n = len/8;}
	else {
		n = len/8 + 1;}
	
	frame = dataLinkLayer(a, n,len, frameArray);
	
	
	//Physical Layer: 
	
	char *filename_output = "BinaryFile.binf";
	physicalLayer(n,frame, filename_output);

	
	return 0;
}
