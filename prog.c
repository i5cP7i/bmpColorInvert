#include <stdio.h>
#include <stdlib.h>

#ifndef ifBMP
#define ifBMP	"C:\\Users\\11703088\\OneDrive - PXL\\Elektronica-ICT\\C_programmeren_2\\PE2\\BMP\\input.bmp"
#endif

#ifndef ofBMP
#define ofBMP	"C:\\Users\\11703088\\OneDrive - PXL\\Elektronica-ICT\\C_programmeren_2\\PE2\\BMP\\output.bmp"	
#endif

int main()
{
	FILE *ifp = fopen(ifBMP, "rb");
	return 0;
}
