//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

class RGBYUV {
public:


};

class YUVRGB {
public:
	double YUVTORGB[3][3] = { 1.000,0.956,0.621,	 /* Y */
		1.000,-0.272,-0.647,		/* U */
		1.000,-1.106,1.703 }; /* v*/
};


// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	char*	Data;					// RGB data of the image
	double RGBTOYUV[3][3] = { 0.299, 0.587, 0.114, /* R */
		0.596,-0.274,-0.322,	/* G */
		0.211,-0.523,0.312 }; /* B*/
	double YUVTORGB[3][3] = { 1.000,0.956,0.621,	 /* Y */
		1.000,-0.272,-0.647,		/* U */
		1.000,-1.106,1.703 }; /* v*/
public:

	// Constructor
	MyImage();

	// Copy Constructor
	MyImage::MyImage(MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth(const int w) { Width = w; };
	void	setHeight(const int h) { Height = h; };
	void	setImageData(const char *img) { Data = (char *)img; };
	void	setImagePath(const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImageData() { return Data; };
	char*	getImagePath() { return ImagePath; }


	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify(MyImage Src);

	bool	toYUV(MyImage Src)
	{
		return true;

	}
	char** doRGBtoYUVFromImage(MyImage srcImage)
	{
		char* YChannel = new char[srcImage.getWidth()*srcImage.getWidth() * 3];
		char* UChannel = new char[srcImage.getWidth()*srcImage.getWidth() * 3];
		char* VChannel = new char[srcImage.getWidth()*srcImage.getWidth() * 3];
		char* YUVChannels[3];

		double * Data = new double[srcImage.getWidth() * srcImage.getHeight() * 3];
		for (int i = 0; i<(srcImage.getWidth()* srcImage.getHeight() * 3); i++)
		{
			Data[i] = (double)srcImage.Data[i];
		}
		int k = 1, j = 0, nYoutPos = 0;
		for (int i = 0; i < srcImage.getHeight(); i++)
		{
			for (int j = 0; j < (srcImage.getWidth() * 3); j += 3)
			{
				int nR = srcImage.Data[(i * srcImage.getWidth() * 3) + j + 2];
				int nG = srcImage.Data[(i * srcImage.getWidth() * 3) + j + 1];
				int nB = srcImage.Data[(i * srcImage.getWidth() * 3) + j];
				YChannel[nYoutPos] = RGBTOYUV[0][0] * nR
					+ RGBTOYUV[0][1] * nG
					+ RGBTOYUV[0][2] * nB; // B

				UChannel[nYoutPos] = RGBTOYUV[1][0] * nR
					+ RGBTOYUV[1][1] * nG
					+ RGBTOYUV[1][2] * nB + 128;

				VChannel[nYoutPos] = RGBTOYUV[2][0] * nR
					+ RGBTOYUV[2][1] * nG
					+ RGBTOYUV[2][2] * nB + 128;


				nYoutPos++;

				YChannel[nYoutPos] = YChannel[nYoutPos - 1];
				UChannel[nYoutPos] = UChannel[nYoutPos - 1];
				VChannel[nYoutPos] = VChannel[nYoutPos - 1];

				nYoutPos++;

				YChannel[nYoutPos] = YChannel[nYoutPos - 1];
				UChannel[nYoutPos] = UChannel[nYoutPos - 1];
				VChannel[nYoutPos] = VChannel[nYoutPos - 1];

				nYoutPos++;

			}
		}

		YUVChannels[0] = YChannel;
		YUVChannels[1] = UChannel;
		YUVChannels[2] = VChannel;
		for (int i = 0; i < Height*Width * 3; i++)
		{
			this->Data[i++] = YUVChannels[0][i];
			this->Data[i++] = YUVChannels[1][i];
			this->Data[i] = YUVChannels[2][i];

		}
		return YUVChannels;

	}

	char* doYUVtoRGBFromImage(MyImage srcImage)
	{
		char * YUVData = new char[srcImage.getWidth() * srcImage.getHeight() * 3];

		double * Data = new double[srcImage.getWidth() * srcImage.getHeight() * 3];
		for (int i = 0; i<(srcImage.getWidth()* srcImage.getHeight() * 3); i++)
		{
			Data[i] = (double)srcImage.Data[i];
		}
		int k = 1, j = 0;
		double val = 0.0;
		for (int i = 0; i < srcImage.getHeight() * srcImage.getWidth() * 3; i++)
		{
			val = ((YUVTORGB[j][0] * Data[i + 2]) + (YUVTORGB[j][1] * Data[i + 1]) + (YUVTORGB[j][2] * Data[i]));
			if (val<0.0)
			{
				val = val*-1.0;
			}
			if (k == 1)
			{

				YUVData[i] = (char)val;
				j++;
				k++;
			}
			else if (k == 2)
			{
				YUVData[i] = (char)val;
				j++;
				k++;
			}
			else if (k == 3)
			{
				YUVData[i] = (char)val;
				j = 0;
				k = 1;
			}

		}
		return YUVData;

	}
};

#endif //IMAGE_DISPLAY
