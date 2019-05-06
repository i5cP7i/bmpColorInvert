#include <stdio.h>
#include <stdlib.h>

//input file: read
#ifndef ifBMP
#define ifBMP	"C:\\Users\\11703088\\OneDrive - PXL\\Elektronica-ICT\\C_programmeren_2\\PE2\\BMP\\input.bmp"
#endif

//output file: write
#ifndef ofBMP
#define ofBMP	"C:\\Users\\11703088\\OneDrive - PXL\\Elektronica-ICT\\C_programmeren_2\\PE2\\BMP\\output.bmp"	
#endif

int main()
{
	FILE *ifp = fopen(ifBMP, "rb");
	
	if (ifp == NULL)
	{
		printf("Could not open file %s\n", ifBMP);
		system("pause");
		exit(EXIT_FAILURE);
	}
	
	byte header[headerSize];
	
	fread(header, sizeof(byte), headerSize, ifp);
	unsigned size = *(unsigned*)&header[2];
	unsigned width = *(unsigned*)&header[18];
	unsigned height = *(unsigned*)&header[22];
	unsigned imgSize = *(unsigned*)&header[34];
	printf("bmp width = %u\nbmp height = %u\nbmp size = %u bytes\nbmp image size = %u\n", width, height, size, imgSize);
	
	unsigned bmpSize = imgSize;
	
	//
	byte *ipxs = (byte*) calloc(bmpSize, sizeof(byte));
	
	fread(ipxs, sizeof(byte), bmpSize, ifp);
	
	//close input file
	fclose(ifp);
	return 0;
}
