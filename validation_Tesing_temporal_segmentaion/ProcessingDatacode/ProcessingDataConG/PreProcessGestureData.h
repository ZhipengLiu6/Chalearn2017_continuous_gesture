#pragma once
#include <afxwin.h>
#include "connexe.h"
#include "VIPLFaceDetector.h"
#include <cv.h>
#include <highgui.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <direct.h>
#include <fstream>
#include <algorithm>   
using namespace cv;

class PreProcessGestureData
{
private:

	vector<IplImage*> vColor;//vColor 存储RGB video
	vector<IplImage*> vDepth;//vDepth 存储Depth vedio
	vector<IplImage*> vfDepth;
	int facenum;
	VIPLFaceInfo FDResult[1024];
	VIPLFaceDetector faceDetector;

	CString MyColorPath;

	int eps;//小于eps的灰度值，看做背景，设为5
	char buffer[256];
private:
	void GetFacePosiFromHist(IplImage *image, int &x, int &y);//当人脸检测不到时候，统计直方图前景分割后的头部最高点 y - 30
	void GetImageGrayMean(IplImage *image, int &grayMean);
	void GetHistAndPartition(IplImage *image, int &peak1, int &peak2, int &Mypartition, int &grayMean);
	void CvtGray(cv::Mat &src, cv::Mat &src_gray);
	void CvtVIPLData(cv::Mat &src, VIPLImageData &srcdata);
	IplImage* ImgDiff(IplImage* img1, IplImage* img2);
	void getConnexeCenterBox(IplImage* image, int& nMaxRect, int* theCent, int* theBox, int nThreshold);
	void backgroundRemove();
	void Rectify(IplImage* img1, IplImage* img2);
	void GetMaxConnectedDomain(IplImage *src, IplImage *dst);
	void onevideoRectify(IplImage* bgrFrameL);

public:
	bool IsoOutheadDetectionVIPLSDK(IplImage* colorImage, CString filename, int countframe, int videoid);
	void getISoFacePositionHist(CString filename, int countframe, int videoid);
	PreProcessGestureData(void);
	~PreProcessGestureData(void);
	void getFacePositionHist(CString filename);
	void ReleaseVector();
	bool headDetectionVIPLSDK(IplImage* colorImage);
	bool OutheadDetectionVIPLSDK(IplImage* colorImage, CString filename, int countframe);
	bool readVideo(CString colorPath, CString depthPath);
	bool getForegroundRect(CString colorPath, CString depthPath, string outputPath);	
	void forgroundSave(string outputPath);
	bool GetFileFacePosition();
	bool cvtRGBToFrontRGB(CString colorPath, CString fdepthPath, CString outputfRGBPath);
public:
	CString faceDataPath;
	int faceX;//表示（faceX, faceY）脸部中心值，如果未检测到人脸，则全为（0，0）
	int faceY;
};

