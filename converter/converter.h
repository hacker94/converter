#include <stdint.h>
#include <Windows.h>
#include <stdexcept>

namespace Converter {
	class Converter {
	public :
						Converter() {}
						~Converter() {}
		void			Convert(char *bmpname, double zoom);
	
	private :
		void			Openbitmap(char *bmpname);
		void			WriteASCII(FILE *output, RGBTRIPLE *pixels, double zoom);
		RGBTRIPLE		MixColor(int y, int x, int zoomy, int zoomx);
	
		RGBTRIPLE		*pixels;
		int				w, h;
	};
	
	void Converter::Convert(char *bmpname, double zoom) {
		Openbitmap(bmpname);
		FILE *output;
		if (fopen_s(&output, "out.txt", "w") != 0) {
			throw std::exception("Cannot Create OutputFile!");
		}
		WriteASCII(output, pixels, zoom);
		fclose(output);
		return;
	}
	
	void Converter::Openbitmap(char *bmpname) {
		FILE *bmp;
		if (fopen_s(&bmp, bmpname, "rb") != 0) {
			throw std::exception("Cannot Open Bmp!");
		}
	
		BITMAPFILEHEADER bf;
		BITMAPINFOHEADER bi;
		fread(&bf, sizeof(bf), 1, bmp);
		fread(&bi, sizeof(bi), 1, bmp);
		if (bi.biBitCount != 24) {
			throw std::exception("That's not a 24bit bmp!");
		}
		if (bi.biCompression != BI_RGB) {
			throw std::exception("The bmp has been expressed!");
		}
		w = bi.biWidth;
		h = bi.biHeight;
	
		pixels = new(RGBTRIPLE[w * h]);
		int rowsize = (w * bi.biBitCount + 31) / 32 * 4; //bytes in one row
		uint8_t *row = new(uint8_t[rowsize]);
		for (int i = 0; i < h; i++) {
			fread(row, sizeof(uint8_t), rowsize, bmp);
			for (int j = 0; j < w; j++) {
				pixels[(h - 1 - i) * w + j].rgbtBlue = row[j * 3];
				pixels[(h - 1 - i) * w + j].rgbtGreen = row[j * 3 + 1];
				pixels[(h - 1 - i) * w + j].rgbtRed = row[j * 3 + 2];
			}
		}
	
		fclose(bmp);
		return;
	}
	
	void Converter::WriteASCII(FILE* output, RGBTRIPLE *pixels, double zoom) {
		char *dic = "M80V1;:*-. ";
		double x = 0, y = 0;
		for (int i = 0; i < h; y += zoom, i = (int)y) {
			for (int j = 0; j < w; x += zoom, j = (int)x) {
				RGBTRIPLE color = MixColor(i, j, (int)(y + zoom) - (int)y, (int)(x + zoom) - (int)x);
				int brightness = (color.rgbtBlue + color.rgbtGreen + color.rgbtRed) / 3;
				fprintf(output, "%c", dic[brightness * (strlen(dic) - 1) / 0xFF]);
			}
			x = 0;
			fprintf(output, "\n");
		}
	}
	
	RGBTRIPLE Converter::MixColor(int y, int x, int zoomy, int zoomx) {
		RGBTRIPLE color;
		int r = 0, g = 0, b = 0, cnt = 0;
		for (int i = y; i < y + zoomy; i++) {
			if (i >= h) break;
			for (int j = x; j < x + zoomx; j++) {
				if (j >= w) break;
				b += pixels[i * w + j].rgbtBlue;
				g += pixels[i * w + j].rgbtGreen;
				r += pixels[i * w + j].rgbtRed;
				cnt++;
			}
		}
		color.rgbtBlue = b / cnt;
		color.rgbtGreen = g / cnt;
		color.rgbtRed = r / cnt;
		return color;
	}
}