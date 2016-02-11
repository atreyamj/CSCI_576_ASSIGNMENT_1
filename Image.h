//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Atreya Madhav Joshi
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
	int m_nChromaY, m_nChromaH, m_nChromaV;
	int m_nWidthCbCr, m_nHeightCbCr;
	unsigned char*	YChannelData;
	unsigned char*	UChannelData;
	unsigned char*	VChannelData;

	unsigned char*	YChannelDataOut;
	unsigned char*	UChannelDataOut;
	unsigned char*	VChannelDataOut;
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
	void	setYChannel( unsigned char *Channel) 
	{ 
		YChannelData = Channel;

	}
	void	setUChannel( unsigned char *Channel)
	{
		UChannelData = Channel;
	

	}

	void	setVChannel( unsigned char *Channel)
	{
		VChannelData = Channel;
		

	}
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImageData() { return Data; };
	char*	getImagePath() { return ImagePath; }
	unsigned char*	getYChannel(const char *Channel)
	{
		return YChannelData;
	}
	unsigned char*	getUChannel(const char *Channel)
	{
		return UChannelData;
	}

	unsigned char*	getVChannel(const char *Channel)
	{
		return VChannelData;
	}
	

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify(MyImage Src);

	bool	toYUV(MyImage Src)
	{
		return true;

	}
	void doRGBtoYUVFromImage(MyImage srcImage)
	{
		unsigned char* YChannel = new unsigned char[srcImage.getWidth()*srcImage.getWidth() * 3];
		unsigned char* UChannel = new unsigned char[srcImage.getWidth()*srcImage.getWidth() * 3];
		unsigned char* VChannel = new unsigned char[srcImage.getWidth()*srcImage.getWidth() * 3];
		unsigned char* YUVChannels[3];
		m_nChromaH = m_nChromaV = 1;
		m_nWidthCbCr = srcImage.getWidth() * (m_nChromaH / 4.0);
		m_nHeightCbCr = srcImage.getHeight() * (m_nChromaV / 4.0);
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
				YChannel[nYoutPos] = (unsigned char)(RGBTOYUV[0][0] * nR + RGBTOYUV[0][1] * nG + RGBTOYUV[0][2] * nB); // B

				UChannel[nYoutPos] = (unsigned char)(RGBTOYUV[1][0] * nR+ RGBTOYUV[1][1] * nG + RGBTOYUV[1][2] * nB + 128);

				VChannel[nYoutPos] = (unsigned char)(RGBTOYUV[2][0] * nR+ RGBTOYUV[2][1] * nG+ RGBTOYUV[2][2] * nB + 128);


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
		setYChannel(YChannel);
		setUChannel(UChannel);
		setVChannel(VChannel);
		YUVChannels[0] = YChannel;
		YUVChannels[1] = UChannel;
		YUVChannels[2] = VChannel;
		//k = 2;
		for (int i = 0; i < Height*Width * 3; i++)
		{
			this->Data[i++] = YUVChannels[0][i];
			this->Data[i++] = YUVChannels[1][i];
			this->Data[i] = YUVChannels[2][i];
		}
		

	}

	unsigned char* GetCbData(int nWidth_o, int nHeight_o)
	{
		int nInPos = 0;
		int nOutPos = 0;
		int nFactor = 4 / m_nChromaH;
		int nFactorV = 4 / m_nChromaV;

	
		nWidth_o = m_nWidthCbCr;
		nHeight_o = m_nHeightCbCr;
		UChannelDataOut = new unsigned char[m_nWidthCbCr*m_nHeightCbCr * 3];
		

		for (int nCol = 0; nCol < this->getHeight(); nCol++)
		{
			if (0 != (nCol % nFactorV))
			{
				continue;
			}
			for (int nWid = 0; nWid < (this->getWidth() * 3); )
			{
				int nCbValue = 0;
				for (int nCount = 0; nCount < (nFactor); nCount++)
				{
					nCbValue += UChannelData[(nCol * this->getWidth() * 3) + nWid];
					nWid += 3;
				}

				for (int nCount = 0; nCount < 3; nCount++)
				{
					UChannelDataOut[nOutPos + nCount] = nCbValue / nFactor;
				}
				nOutPos += 3;
			}
		}

		nOutPos = 0;
		if (2 == nFactorV)
		{
			nOutPos = 0;
			for (int nCol = 0; nCol < (this->getHeight() - 1); nCol += 2)
			{
				for (int nWid = 0; nWid < (this->getWidth() * 3); )
				{
					int nCbValue = 0;
					nCbValue += YChannelData[(nCol * this->getWidth() * 3) + nWid];
					nCbValue += YChannelData[((nCol + 1) * this->getWidth() * 3) + nWid];

					for (int nCount = 0; nCount < 3; nCount++)
					{
						UChannelDataOut[nOutPos + nCount] = nCbValue / 2;
					}
					nWid += (nFactor * 3);
					nOutPos += 3;
				}
			}
		}

		return UChannelDataOut;
	}

	unsigned char* GetCrData(int nWidth_o, int nHeight_o)
	{
		int nInPos = 0;
		int nOutPos = 0;
		int nFactor = 4 / m_nChromaH;
		int nFactorV = 4 / m_nChromaV;

		
		nWidth_o = m_nWidthCbCr;
		nHeight_o = m_nHeightCbCr;
		this->VChannelDataOut = new unsigned char[m_nWidthCbCr*m_nHeightCbCr * 3];
		//ZeroMemory(m_pbyCrDataOut, m_nWidthCbCr*m_nHeightCbCr * 3);

		for (int nCol = 0; nCol < this->getHeight(); nCol++)
		{
			if (0 != (nCol % nFactorV))
			{
				continue;
			}
			for (int nWid = 0; nWid < (this->getWidth() * 3); )
			{
				int nCbValue = 0;
				for (int nCount = 0; nCount < (nFactor); nCount++)
				{
					nCbValue += this->VChannelData[(nCol * this->getWidth()* 3) + nWid];
					nWid += 3;
				}

				for (int nCount = 0; nCount < 3; nCount++)
				{
					this->VChannelDataOut[nOutPos + nCount] = nCbValue / nFactor;
				}
				nOutPos += 3;
			}
		}

		nOutPos = 0;
		if (2 == nFactorV)
		{
			nOutPos = 0;
			for (int nCol = 0; nCol < (this->getHeight() - 1); nCol += 2)
			{
				for (int nWid = 0; nWid < (this->getWidth() * 3); )
				{
					int nCbValue = 0;
					nCbValue += VChannelData[(nCol * getWidth() * 3) + nWid];
					nCbValue += VChannelData[((nCol + 1) * getWidth() * 3) + nWid];

					for (int nCount = 0; nCount < 3; nCount++)
					{
						VChannelDataOut[nOutPos + nCount] = nCbValue / 2;
					}
					nWid += (nFactor * 3);
					nOutPos += 3;
				}
			}
		}

		return VChannelDataOut;
	}


	unsigned char* GetYData(int nWidth_o, int nHeight_o)
	{
		int nInPos = 0;
		int nOutPos = 0;
		int nFactor = 4 / m_nChromaH;
		int nFactorV = 4 / m_nChromaV;


		nWidth_o = m_nWidthCbCr;
		nHeight_o = m_nHeightCbCr;
		this->YChannelDataOut = new unsigned char[this->getHeight()*this->getWidth()* 3];
		//ZeroMemory(m_pbyCrDataOut, m_nWidthCbCr*m_nHeightCbCr * 3);

		for (int nCol = 0; nCol < this->getHeight(); nCol++)
		{
			if (0 != (nCol % nFactorV))
			{
				continue;
			}
			for (int nWid = 0; nWid < (this->getWidth() * 3); )
			{
				int nCbValue = 0;
				for (int nCount = 0; nCount < (nFactor); nCount++)
				{
					nCbValue += this->YChannelData[(nCol * this->getWidth() * 3) + nWid];
					nWid += 3;
				}

				for (int nCount = 0; nCount < 3; nCount++)
				{
					this->YChannelDataOut[nOutPos + nCount] = nCbValue / nFactor;
				}
				nOutPos += 3;
			}
		}

		nOutPos = 0;
		if (2 == nFactorV)
		{
			nOutPos = 0;
			for (int nCol = 0; nCol < (this->getHeight() - 1); nCol += 2)
			{
				for (int nWid = 0; nWid < (this->getWidth() * 3); )
				{
					int nCbValue = 0;
					nCbValue += YChannelData[(nCol * getWidth() * 3) + nWid];
					nCbValue += YChannelData[((nCol + 1) * getWidth() * 3) + nWid];

					for (int nCount = 0; nCount < 3; nCount++)
					{
						YChannelDataOut[nOutPos + nCount] = nCbValue / 2;
					}
					nWid += (nFactor * 3);
					nOutPos += 3;
				}
			}
		}

		return YChannelDataOut;
	}
	
	void doYUVtoRGBFromImage()
	{
		//int nWidth = 0;
		//int nHeight = 0;
		m_nChromaH = 4, m_nChromaV = ;
		int nFactorW = 4 / m_nChromaH;
		int nFactorV = 4 / m_nChromaV;

		int m_nWidth = getWidth();
		 int m_nHeight = getHeight();
		// GetYData(m_nWidth, m_nHeight);
		GetCbData(m_nWidth, m_nHeight);
		GetCrData(m_nWidth, m_nHeight);
		//m_nWidth = m_nWidthCbCr;
		//m_nHeight = m_nHeightCbCr;

		unsigned char* RGBData = new unsigned char[m_nWidth *m_nHeight * 3];
		//ZeroMemory(RGBData, m_nWidth*m_nHeight * 3);
		double r =0.0, g = 0.0,b = 0.0;
		int nYoutPos = 0;
		int nCrHeigth = 0;
		for (int nCol = 0; nCol < m_nHeight; nCol++)
		{

			int nCrWidth = 0;
			for (int nWid = 0; nWid < (m_nWidth * 3); nWid += 3)
			{

				int nY = YChannelData[(nCol * m_nWidth * 3) + nWid];
				int nCb = UChannelDataOut[(nCrHeigth * m_nWidthCbCr * 3) + nCrWidth] - 128;
				int nCr = VChannelDataOut[(nCrHeigth * m_nWidthCbCr * 3) + nCrWidth] - 128;

				if (0 == (nWid % nFactorW))
				{
					nCrWidth += 3;
				}
				//nCrWidth += 3;
				r = (YUVTORGB[0][0] * nY + YUVTORGB[0][1] * nCb + YUVTORGB[0][2] * nCr);
				g= (YUVTORGB[1][0] * nY + YUVTORGB[1][1] * nCb + YUVTORGB[1][2] * nCr);
				b = (YUVTORGB[2][0] * nY + YUVTORGB[2][1] * nCb + YUVTORGB[2][2] * nCr);
				// ITU-R version formul
				
				if (r < 0)
				{
					r = r*-1.0;
				}
				if (g < 0)
				{
					g = g*-1.0;
				}
				if (b < 0)
				{
					b = b*-1.0;
				}
				RGBData[nYoutPos + 2] = (unsigned char)r; // R


				RGBData[nYoutPos + 1] = (unsigned char)g; // G

				RGBData[nYoutPos] = (unsigned char)b; //B

				nYoutPos += 3;
			}
			//nCrHeigth++;
			if (0 == (nCol % nFactorV))
			{
				nCrHeigth++;
			}

		}
		for (int i = 0; i < m_nWidth*m_nHeight * 3; i++)
		{
			this->Data[i] = RGBData[i];
		}
	}
	
	
};

#endif //IMAGE_DISPLAY
