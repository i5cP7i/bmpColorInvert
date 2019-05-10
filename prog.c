#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>

typedef unsigned char 	byte; //FF
typedef unsigned short 	WORD; //FF FF
typedef unsigned		DWORD;//FF FF FF FF

typedef struct bmpDataStruct
{
	WORD 	magicNum;
	DWORD 	fileSize;
	WORD  	rsvd1;        
	WORD  	rsvd2;        
	DWORD  	offset;           
	DWORD 	dib;
	int 	width;
	int 	height;
	WORD  	num_planes;     
	WORD  	bppx;   
	DWORD   compType;
	DWORD 	imgSize;
	DWORD   x_res; 
	DWORD   y_res; 
	DWORD  	num_colors;        
	DWORD  	important_colors;
	struct	bmpDataStruct *nextData;
} bmpData;

void fgetData(FILE*, bmpData*, byte*); 
void print_bmpData(bmpData*);

const byte headerSize = 54;
const DWORD arr_size = 1024;


int main()
{
	char ifBMP[arr_size];
	char ofBMP[arr_size];
	bmpData bmp;
	FILE *ifp;
	
	getcwd(ifBMP, sizeof(ifBMP));
	strcat(ifBMP, "\\");
	 
	char *ifp_str = (char*) malloc(arr_size*sizeof(char));
	
	system("cls");
	
	printf("BMP COLOR INVERTER PROGRAM\n\n");
	printf("This program inverts the colors of a BMP image and saves the new image to an output file named output.bmp\n"); 
	puts("Please provide a valid filename.\nThe output file will be saved in the same directory of the provided bmp image file.\n");
	printf("Current directory:\n %s", ifBMP);
	scanf("%s", ifp_str);
	const char *bmpCharSet = "bmp";
	for (size_t i = 0; ifp_str[i] != '\0'; ++i)
	{
		if (ifp_str[i] == '.')
		{
			if (ifp_str[i+1] != bmpCharSet[0] || ifp_str[i+2] != bmpCharSet[1] || ifp_str[i+3] != bmpCharSet[2])
			{
				puts("Invalid file extension! The file must be a bmp file (extension .bmp).");
				system("pause");
				exit(EXIT_FAILURE);
			}
		}
	}
	strcat(ifBMP, ifp_str);
	strcpy(ofBMP, ifBMP);
	for (size_t i = strlen(ofBMP); ; --i)
	{
		
		if (ofBMP[i-1] == '\\')
		{
			ofBMP[i] = '\0';
			break;
		}
	}
	strcat(ofBMP, "output.bmp");
	ifp = fopen(ifBMP, "rb");
	
	
	
	if (ifp == NULL)
	{
		printf("Could not open file %s\n", ifBMP);
		system("pause");
		exit(EXIT_FAILURE);
	}
	
	
	free(ifp_str);
	
	byte header[headerSize];
	
	printf("\nInput file data:\n");
	fgetData(ifp, &bmp, header);
	print_bmpData(&bmp);
	
	byte *ipxs = (byte*) calloc(bmp.imgSize, sizeof(byte));
	
	fread(ipxs, sizeof(byte), bmp.imgSize, ifp);
	
	//close input file
	fclose(ifp);
	
	//Invert the colors
	for (size_t i = 0; i < (bmp.imgSize); i += 3)
	{
		byte tpxs = ipxs[i];
		ipxs[i] = ipxs[i+2];
		ipxs[i+2] = tpxs;
	}
	
	//Create output file ofp
	FILE *ofp = fopen(ofBMP, "wb"); 
	if (ofp == NULL)
	{
		printf("Error while creating output file %s\n", ofBMP);
		system("pause");
		exit(EXIT_FAILURE);
	}
	printf("\nOutput file generated!\n");
	printf("Output file located at:\n%s\n\n", ofBMP);
	//Write bmp header
	for (size_t i = 0; i < headerSize; ++i)
	{
		putc(header[i], ofp);
	}
	
	byte *opxs = (byte*) calloc(bmp.imgSize, sizeof(byte));
	printf("Now writing inverted pixels to the file");
	for(size_t i = 0; i < bmp.imgSize; ++i)
	{
		opxs[i] = 255 - ipxs[i];
		putc(opxs[i], ofp);
		if (i%(102400) == 0)
		{
			printf(".");
		}
	}

	fwrite(ofp, sizeof(int), bmp.width, ofp);
		
	fwrite(ofp, sizeof(byte), bmp.imgSize, ofp);
	printf("\n\nOutput file data:\n");
	fgetData(ofp, &bmp, header);
	print_bmpData(&bmp);
	/*printf("bmp Magic number = %4x\nbmp width = %u\nbmp height = %u\nbmp size = %u bytes\nbmp image size = %u\n", 
		bmp.magicNum, bmp.width, bmp.height, bmp.fileSize, bmp.imgSize);
	*/
	
	fclose(ofp);
	free(ipxs);
	free(opxs);
	system("pause");
	return 0;
}

/*
 *
*/
void fgetData(FILE *ifp_c, bmpData *Data, byte *header)
{
	bmpData *pData = Data;
	fread(header, sizeof(byte), headerSize, ifp_c);
	
	pData->magicNum = *(WORD*)&header[0];				//%hu\n							
	pData->fileSize = *(DWORD*)&header[2];          	//%u\n             
	pData->rsvd1 = *(WORD*)&header[6];					//%hu\n 	                               
	pData->rsvd2 = *(WORD*)&header[8]; 					//%hu\n                                               
	pData->offset = *(DWORD*)&header[10];				//%u\n                                                 
	pData->dib = *(DWORD*)&header[14];					//%u\n                           
	pData->width = *(int*)&header[18];					//%d\n                           
	pData->height = *(int*)&header[22];					//%d\n                          
	pData->num_planes = *(WORD*)&header[26];			//%hu\n                       
	pData->bppx = *(WORD*)&header[28];					//%hu\n                                                   
	pData->compType = *(DWORD*)&header[30];				//%u\n                                               
	pData->imgSize = *(DWORD*)&header[34];				//%u\n                       
	pData->x_res = *(DWORD*)&header[38];				//%u\n                         
	pData->y_res = *(DWORD*)&header[42];				//%u\n                       
	pData->num_colors = *(DWORD*)&header[46]; 			//%u\n                                            
	pData->important_colors = *(DWORD*)&header[50];		//%u\n                                  
}

/*
 *
*/
void print_bmpData(bmpData *Data)
{
	bmpData *pData = Data;
	printf("Magic Number: %4x\n", pData->magicNum);
	printf("Filesize: %u\n", pData->fileSize);
	printf("Reserved 1: %hu\n", pData->rsvd1);
	printf("Reserved 2: %hu\n", pData->rsvd2);
	printf("Offset: %u\n", pData->offset);
	printf("DIB: %u\n", pData->dib);
	printf("width: %d\n", pData->width);
	printf("height: %d\n", pData->height);
	printf("Number of planes: %hu\n", pData->num_planes);
	printf("Bits per pixel: %hu\n", pData->bppx);
	printf("Compression type: %u\n", pData->compType);
	printf("Image size: %u\n", pData->imgSize);
	printf("x resolution: %u\n", pData->x_res);
	printf("y resolution: %u\n", pData->y_res);
	printf("Number of colors: %u\n", pData->num_colors);
	printf("Number of important colors: %u\n", pData->important_colors);
}                                
