#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Binary to ASCII conversion mode
int binToDec(int *n){
	int dec_value = 0;
	int num;
	int base = 1;
	for (int i = 7; i>0; i--)
	{
		num = n[i] * base;
		dec_value = dec_value + num;
		base = base * 2;
	}
	return dec_value;
}

//Physical Layer
int *physicalLayer(char *filename, int *frame, int *nValue){
	//reading binary file
	FILE *fPointer;
	fPointer = fopen(filename,"rb");
	int bitArray[1200];
	fread(bitArray, sizeof(bitArray),1,fPointer);
	fclose(fPointer);
	
	int choice;	
	printf("Choice of error detection used for transmission:\n1. For CRC detection, enter 1\n2.For Hamming code detection, enter 2\n");
	scanf("%d",&choice);

	//CRC
	if (choice == 1){

		//counting frames for CRC
		int n = 0;
		int synArr[8] = {0,0,0,1,0,1,1,0};
		for (int a = 0; a<1200; a= a+120)
		{	
			if ((bitArray[a] == synArr[0]) & (bitArray[a+1] == synArr[1]) & (bitArray[a+2] == synArr[2]) & 	(bitArray[a+3] == synArr[3]) & (bitArray[a+4] == synArr[4]) & (bitArray[a+5] == synArr[5]) & (bitArray[a+6] == synArr[6]) & (bitArray[a+7] == synArr[7]))
					{n++;}
			
		}
		//printf("%d ",len);
		int len = n*120;
		*nValue = n;
		//printf("%d ",n);
	 	
		//CRC check
		int generator[33] = {1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,1,1,1};
		int gsize = 33;
		int dataword[64+33-1];
		int dsize = 64;
		int zeros[gsize];
		int remainder[gsize];
		for (int b = 0; b<gsize; b++)
		{
			zeros[b] = 0;
		}	
		
		
		for (int i = 0; i<len; i=i+120)
		{	int l = 0;
			for (int j = i; j< (i+dsize+gsize-1); j++)
			{
				dataword[l] = bitArray[j+24];
				//printf("%d",dataword[l]);
				l++;
			}
			//printf("\n");

			//check for zero remainder
			for (int a = 0; a<gsize; a++)
			{
				remainder[a] = dataword[a];	
				//printf("%d",remainder[a]);
			}
			//printf("\n");

			int d = gsize;

			while (d<dsize+gsize)
			{				
				if (remainder[0] == generator[0])
				{						
					for (int c = 0; c<gsize; c++)
					{	if (remainder[c] == generator[c]) 
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

			//printf("\n");
			//printf("Check Final ");
			int code[gsize-1];
			int k = 0;
			for (int b=0; b<gsize-1;b++)
			{
				code[b] = remainder[b];
				//printf("%d",code[b]);
				if (code[b] != 0)
					k++;
			}
			//printf("\n");
			if (k!=0)
				printf("Error detected in CRC detection for frame %d \n",i/120+1);
			else 
				printf("No Error detected in CRC detection for frame %d \n",i/120+1);
		}

		//converting dataword to decimal
		int binNum[8];
		int k = 0; 
		int j;
		for (int i = 0; i<len; i = i+120){
			j = i;
			while ((j>=i) & (j<(i+24+dsize))){
				for (int b = 0; b<8; b++){
					binNum[b] = bitArray[j];
					//printf("%d",binNum[b]);
					j++;
				}					
				frame[k] = binToDec(binNum);   
				//printf("\n%d",frame[k]);			
				k++;
				//printf("\n");		
				
			}    	
		}
		return frame;
	}

	//Hamming code
	else if (choice == 2){
		//Counting Frames for hamming
		int n = 0;
		int synArr[8] = {0,0,0,1,0,1,1,0};
		for (int a = 0; a<1200; a= a+96)
		{	
			if ((bitArray[a] == synArr[0]) & (bitArray[a+1] == synArr[1]) & (bitArray[a+2] == synArr[2]) & 	(bitArray[a+3] == synArr[3]) & (bitArray[a+4] == synArr[4]) & (bitArray[a+5] == synArr[5]) & (bitArray[a+6] == synArr[6]) & (bitArray[a+7] == synArr[7]))
					{n++;}
			
		}	
		
		int len = n*96;
		*nValue = n;
		//printf("%d ",n);
	 	//printf("%d ",len);

		//hamming check
		int dsize = 64;
		int psize = 7;
		int data[64];
		int parityIndex[7] = {1,2,4,8,16,32,64};
		int encoded[dsize+psize+1];
		int errorParity[7];
		int k = 0;

		for (int i = 0; i<len; i=i+96)
		{	int l = 1;
			
			for (int j = i; j< (i+dsize+psize); j++)
			{
				encoded[l] = bitArray[j+25];
				//printf("%d",encoded[l]);
				l++;
			}
			/*
			printf("Encoded:\n");	
			for (int a = 1; a<psize+dsize+1; a++)
			{
				printf("%d",encoded[a]);
			}
			printf("\n");
			*/
		
			//check parity bit
			int a;
			int d = 0;
			int c = 0;
			int p = 6;
			int decimalParity = 0;
			for (int b = 0; b<psize; b++)
			{	int sum = 0;
				//printf("In %d:\n",parityIndex[b]);
				for (int h = parityIndex[b]; h<psize+dsize+1; h=h+parityIndex[b]+1)	{
					for (a = 0; a < parityIndex[b]; a++){	
						if (h+a>psize+dsize)	
							break;					
						//printf("%d ",h+a);							
						//printf("Encode Index %d value: %d\n", h+a,encoded[h+a]);
						sum = sum + encoded[h+a];
					}
					h = h+a-1;	
				}
				
				if (sum%2 == 1)
					errorParity[p] = 1;
				else 
					errorParity[p] = 0;
					
				p--;
				
					
			}
			
			int sum = bitArray[i+24];
			int parityBit;
			for (int b = 1; b<dsize+psize+1; b++)
			{
				sum = sum + encoded[b];
			}
			if (sum%2==1)
				parityBit = 1;
			else 
				parityBit = 0;
			
			int dec_value = 0;
			int num;
			int base = 1;
			for (int i = 6; i>0; i--)
			{
				num = errorParity[i] * base;
				decimalParity = decimalParity + num;
				base = base * 2;
			}
			
			
			if (parityBit == 0 & decimalParity == 0)
				printf("No error detected in hamming code detection for frame %d\n",(i/95)+1);
			else if (parityBit == 1 & decimalParity != 0)
				{printf("Single error detected in hamming code detection for frame %d\n",(i/95)+1);
				printf("Checking for the error bit...\n");
				printf("Error bit at %d bit postion of the frame\n",decimalParity);
				printf("Fixing the error bit...\n");
				if (encoded[decimalParity]==1)
					encoded[decimalParity] = 0;
				else 
					encoded[decimalParity] = 1;
				printf("Error Bit corrected\n");}
			else if (parityBit == 0 & decimalParity != 0)
				printf("Double error detected which cannot be fixed for frame %d\n",(i/95)+1);	
		
				

			//Decoded data
			//printf("Decoded:\n");
			int decoded[24+dsize];
			int temp[dsize];
			l = 0;
			for (int j  = i; j< i+24; j++)
			{	
				decoded[l] = bitArray[j];
				l++;
			}
			int q = 0;
			for (int j = 1; j<72; j++)
			{
				if ((j==1) || (j==2) || (j==4) || (j==8) || (j==16) || (j==32) || (j==64))
					continue;
				else
				{	
					temp[q] = encoded[j];
					//printf("%d",temp[q]);
					q++;	
				}		
			}
			
			for (int j  = 0; j<dsize; j++)
			{
			 	decoded[l] = temp[63-j];	
				//printf("%d",decoded[l]);
				l++;
			}
				


			//converting dataword to decimal
			int binNum[8];
			for (int e = 0; e<88; e = e+8){
				for (int b = 0; b<8; b++){
					binNum[b] = decoded[e+b];
				}					
				frame[k] = binToDec(binNum);   
				//printf("\n%d",frame[k]);			
				k++;
				//printf("\n");		
			}   
			 	
		}
		return frame;
	}
	
}	

//Data Link Layer
int *dataLinkLayer(int *frame, int *dataArray,int frameNo, int *kValue){
	//deframing
	int n,j,k=0;
	int max = frameNo*11;
	//printf("%d\n",len);
	for (int i = 0; i<max; i++){
		if ((frame[i] == 22) & (frame[i+1] == 22)){
			n = frame[i+2];
			//printf("%d\n",n);
			for (j = i+3; j<i+3+n; j++){
				dataArray[k] = frame[j];
				//printf("%c",dataArray[k]);
				k++;
			}
			//printf("\n");
		}
	}
	*kValue = k;
	return dataArray;
}

//Application Layer
void applicationLayer(char *filename, int *data, int k){
	//writing the data in output file
	
	FILE *fPointer;
	fPointer = fopen(filename,"w");
	int i = 0;
	while (i<k){	
		fprintf(fPointer, "%c", data[i]);
		i++;
	}
	fclose(fPointer);
}

int main()
{
	//Physical layer
	char *filename1 = "BinaryFile.binf";
	int frameArray[100];
	int *frame;
	int n;
	frame = physicalLayer(filename1,frameArray, &n);

	
	//Data Link Layer
	int dataArray[100];
	int *data;
	int k;
	data = dataLinkLayer(frame,dataArray,n,&k);
	
	
	//Application Layer
	char *filename2 = "output.outf";
	applicationLayer(filename2, data, k);
	
	return 0;
}
