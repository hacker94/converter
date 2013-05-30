#include <stdio.h>
#include "converter.h"

int main() {
	//readin bmpname
	char *bmpname = new(char[MAX_PATH]);
	printf("FILE name: ");
	scanf("%s", bmpname);

	//readin zoom
	double zoom;
	printf("Zoom ratio(<=1): ");
	while ((scanf("%lf", &zoom) == 0) || (zoom > 1)) {
		printf("Zoom ratio must <=1, please retry: ");
		fflush(stdin);
	}
	zoom = 1 / zoom;

	//convert
	Converter::Converter conv;
	try {
		conv.Convert(bmpname, zoom);
	} catch (std::exception e) {
		printf(e.what());
	}
	printf("Convertion Success.");

	system("pause");
	return 0;
}