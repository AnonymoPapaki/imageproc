#ifndef __IMAGEPROC__H
#define __IMAGEPROC__H
struct Pixel
{
	float r, b, g, a;
};
struct Texture
{
	int heigth;
	int width;
	Pixel *pixels;
};
typedef Pixel(*Shader)(const Pixel*, int sampleSize,void*);
Texture CreateTextureFromRawData(const unsigned char *data, int height, int width, int bytePerPixel, int rIdx, int gIdx, int bIdx);
void DeleteTexture(Texture t);
void TextureToRawData(const Texture texture, unsigned char *data, int height, int width, int bytePerPixel, int rIdx, int gIdx, int bIdx);
void PixelShader(const Texture input, Texture *output, Shader shader, void *shaderOptions ,int sampleSize);


Pixel KernelShader(const Pixel* p, int sampleSize, void *kernel);
#endif
