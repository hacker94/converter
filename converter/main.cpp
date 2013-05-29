#include <stdio.h>
#include "converter.h"

int main() {
	char *bmpname = new(char[MAX_PATH]);
	printf("FILE name: ");
	scanf("%s", bmpname);
	double zoom;
	printf("Zoom ratio(>=1): ");
	scanf("%lf", &zoom);
	Converter::Converter conv;
	conv.Convert(bmpname, zoom);
	return 0;
}