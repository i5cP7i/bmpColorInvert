#include <stdio.h>
#include <stdlib.h>

#ifndef ifBMP
#define ifBMP	"C:\\Users\\11703088\\OneDrive - PXL\\Elektronica-ICT\\C_programmeren_2\\PE2\\BMP\\input.bmp"
#endif

#ifndef ofBMP
#define ofBMP	"C:\\Users\\11703088\\OneDrive - PXL\\Elektronica-ICT\\C_programmeren_2\\PE2\\BMP\\output.bmp"	
#endif

typedef unsigned char 	byte;
typedef unsigned short 	WORD;
typedef unsigned		DWORD;

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
} bmpData;

void fgetData(FILE*, bmpData*); 

const byte headerSize = 54;

int main()
{
	bmpData bmp;
	FILE *ifp = fopen(ifBMP, "rb");
	
	if (ifp == NULL)
	{
		printf("Could not open file %s\n", ifBMP);
		system("pause");
		exit(EXIT_FAILURE);
	}
	
	byte header[headerSize];
	
	printf("\nInput file data:\n");
	fgetData(ifp, &bmp);
	printf("bmp Magic number = %4x\nbmp width = %u\nbmp height = %u\nbmp size = %u bytes\nbmp image size = %u\n", 
		bmp.magicNum, bmp.width, bmp.height, bmp.fileSize, bmp.imgSize);
	
	
	DWORD imgSize = bmp.imgSize;

	byte *ipxs = (byte*) calloc(imgSize, sizeof(byte));
	
	fread(ipxs, sizeof(byte), imgSize, ifp);
	
	//close input file
	fclose(ifp);
	
	//Invert the colors
	for (size_t i = 0; i < (imgSize - 3); i += 3)
	{
		byte tmp = ipxs[i];
		ipxs[i] = ipxs[i+2];
		ipxs[i+2] = tmp;
	}
	
	//Create output file ofp
	FILE *ofp = fopen(ofBMP, "wb"); 
	if (ofp == NULL)
	{
		printf("Error while creating output file %s\n", ofBMP);
		system("pause");
		exit(EXIT_FAILURE);
	}
	//Write bmp header
	for (size_t i = 0; i < headerSize; ++i)
	{
		putc(header[i], ofp);
	}
	
	byte *opxs = (byte*) calloc(imgSize, sizeof(byte));
	
	for(size_t i = 0; i < imgSize; ++i)
	{
		opxs[i] = 255 - ipxs[i];
		putc(opxs[i], ofp);
	}
	
	fwrite(ofp, sizeof(byte), imgSize, ofp);
	puts("Output file generated!");
	printf("\nOutput file data:\n");
	fgetData(ofp, &bmp);
	printf("bmp Magic number = %4x\nbmp width = %u\nbmp height = %u\nbmp size = %u bytes\nbmp image size = %u\n", 
		bmp.magicNum, bmp.width, bmp.height, bmp.fileSize, bmp.imgSize);
	fclose(ofp);
	free(ipxs);
	free(opxs);
	
	return 0;
}

void fgetData(FILE *ifp_c, bmpData *Data)
{
	bmpData *pData = Data;
	byte header_c[headerSize];
	fread(header_c, sizeof(byte), headerSize, ifp_c);
	
	pData->magicNum = *(WORD*)&header_c[0];							
	pData->fileSize = *(DWORD*)&header_c[2];                        
	pData->rsvd1 = *(WORD*)&header_c[6];                                
	pData->rsvd2 = *(WORD*)&header_c[8];                                                
	pData->offset = *(DWORD*)&header_c[10];                                                  
	pData->dib = *(DWORD*)&header_c[14];                            
	pData->width = *(int*)&header_c[18];                            
	pData->height = *(int*)&header_c[22];                           
	pData->num_planes = *(WORD*)&header_c[26];                       
	pData->bppx = *(WORD*)&header_c[28];                                                   
	pData->compType = *(DWORD*)&header_c[30];                                                
	pData->imgSize = *(DWORD*)&header_c[34];                        
	pData->x_res = *(DWORD*)&header_c[38];                          
	pData->y_res = *(DWORD*)&header_c[42];                        
	pData->num_colors = *(DWORD*)&header_c[46];                                              
	pData->important_colors = *(DWORD*)&header_c[50];                                   
}
