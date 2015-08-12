#include <malloc.h>
#include <string.h>
#include "imageproc.h"


Texture CreateTextureFromRawData(const unsigned char *data, int height, int width, int bytePerPixel, int rIdx, int gIdx, int bIdx)
{
	Texture t;
	t.heigth = height;
	t.width = width;
	t.pixels = (Pixel*)malloc(height * width * sizeof(Pixel));
	int j = 0;
	for (int i = 0; i < height * width * bytePerPixel; i += bytePerPixel)
	{

		t.pixels[j].a = 1.f;
		t.pixels[j].r = ((float)*(data + i  + rIdx)) / 255.f;
		t.pixels[j].g = ((float)*(data + i  + gIdx)) / 255.f;
		t.pixels[j].b = ((float)*(data + i  + bIdx)) / 255.f;
		j++;
	}
	return t;
}
void DeleteTexture(Texture t)
{
	free(t.pixels);
}
void TextureToRawData(const Texture texture,unsigned char *data, int height, int width, int bytePerPixel, int rIdx, int gIdx, int bIdx)
{
	int j = 0;
	for (int i = 0; i < height * width * bytePerPixel; i += bytePerPixel)
	{
		*(data + i + rIdx) = texture.pixels[j].r * 255.f;
		*(data + i + gIdx) = texture.pixels[j].g * 255.f;
		*(data + i + bIdx) = texture.pixels[j].b * 255.f;
		*(data + i + 3) = 0xFF;		
		if (*(data + i) < 200)
			int foo = 1;
		j++;
	}
}
void PixelShader(const Texture input, Texture *output, Shader shader, void *shaderOptions, int sampleSize)
{
	output->heigth = input.heigth;
	output->width = input.width;
	output->pixels = (Pixel*)malloc(input.heigth * input.width * sizeof(Pixel));
	Pixel *buffer = (Pixel*)malloc(sampleSize * sampleSize * sizeof(Pixel));

	for (int h = sampleSize ; h < input.heigth - sampleSize ; h++)
	{
		for (int w = sampleSize ; w < input.width - sampleSize ; w++)
		{
			for (int s = 0; s < sampleSize; s++)
			{
				memcpy(
					buffer + s * sampleSize, 
					input.pixels + (h - sampleSize + s) * input.width + (w - sampleSize + s),
					sampleSize * sizeof(Pixel));
			}
			output->pixels[h * input.width + w] = shader(buffer, sampleSize, shaderOptions);
		}
	}
	free(buffer);
}


Pixel KernelShader(const Pixel* sample, int sampleSize, void *kernel)
{
	Pixel ret;
	ret.a = 1.f;
	ret.b = 0.f;
	ret.g = 0.f;
	ret.r = 0.f;
	for (int i = 0; i < sampleSize * sampleSize; i++)
	{
		ret.r += sample[i].r * ((float*)kernel)[i];
		ret.g += sample[i].g * ((float*)kernel)[i];
		ret.b += sample[i].b * ((float*)kernel)[i];
	}
	
	return ret;
}