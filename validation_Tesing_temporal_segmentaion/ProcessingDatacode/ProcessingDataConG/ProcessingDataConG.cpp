// ProcessingDataConG.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "stdafx.h"
#include "PreProcessGestureData.h"
#include <fstream>
#include <string>
#include <direct.h>
#include <iostream>
#include <io.h>
#include "afx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <stdio.h>
using namespace std;
bool FCvtVideoToImage(CString vFileNmae, vector<IplImage *>& vectorImage);
void getFiles(string path, vector<string>& files );
void GetMaxConnectedDomain(IplImage *src, IplImage *dst);
bool ExtractHogFeature(CString colorPath, CString LabelPath,int label, CString savePath);
bool ExtractHogFeature_flag(CString colorPath, CString LabelPath,int label, CString savePath);
int getCurrentDir(string path);
void getCurrentDir(string path, vector<string>& files);
void getDefinedFiles(string path, vector<string>& results);

//string filePath = "C:\\Work\\Data\\Chalearn\\IsoGD_phase_2\\IsoGD_phase_2\\test";
//CString Trainlist = "C:\\Work\\Data\\Chalearn\\IsoGD_phase_2\\IsoGD_phase_2\\test_list.txt";
//CString LabelHandFile = "C:\\Work\\Data\\Chalearn\\NeatedDetectionLabel\\IsoGD\\test";

CString ValidPath = "E:\\zhipengliu\\dataset\\Continuous Gesture\\ConGD_phase_1\\valid";
CString HandLabelPath = "C:\\zhipeng\\ICT_NHCI_ContinuousGestureCode\\ConAllcodes\\ProcessingDatacode\\cong\\valid";

#define getVideoLength
//#define ConGSegmentation
//#define CvtConGDepthSegmentation
//#define CvtConGHandDetctionSegmentaion

void cvtRGBHandLabel2Depth(CString handLabelFilePath, CString LabelSavePath);

CvMat *mx1;
CvMat *my1;
CvMat *mx2;
CvMat *my2;
void fgenerateMap();
PreProcessGestureData cvtConToIsoPreProcessFace;
CString faceTxtFilename = "D:\\face.txt";

struct TwoDimension
{
	int x;
	int y;
};
struct HandBox
{
	TwoDimension A;
	TwoDimension B;
};

struct CandidateArea
{
	int tlx;
	int tly;
	int brx;
	int bry;
	int flag;
};

struct Foreground
{
	string frameID;
	int tlx;
	int tly;
	int brx;
	int bry;
};

double dis(CandidateArea rect1, CandidateArea rect2)
{
	return abs(rect1.tlx-rect2.tlx)+abs(rect1.tly-rect2.tly)+abs(rect1.brx-rect2.brx)+abs(rect1.bry-rect2.bry);
}

bool isForeground(Foreground fore, CandidateArea rect)
{
	int centerx = (rect.tlx+rect.brx)/2;
	int centery = (rect.tly+rect.bry)/2;
	if(centerx>=fore.tlx && centerx<=fore.brx && centery>=fore.tly && centery<=fore.bry)
	{
		return true;
	}
	return false;
}

double centerDis(CandidateArea rect1, CandidateArea rect2)
{
	int center1x = (rect1.tlx+rect1.brx)/2;
	int center1y = (rect1.tly+rect1.bry)/2;
	int center2x = (rect2.tlx+rect2.brx)/2;
	int center2y = (rect2.tly+rect2.bry)/2;
	return abs(center1x-center2x)+abs(center1y-center2y);
}

struct IsoVideoInfo
{
	int start;
	int end;
};
struct frameHandPosition
{
	int nframe;
	bool isDetectOneHand;//Detecting one hand is true ,else is false
	TwoDimension handA1;
	TwoDimension handA2;
	TwoDimension handB1;
	TwoDimension handB2;
};
double getTwoDimDis(TwoDimension A, TwoDimension B)
{
	return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y) * 1.0;
}
double CvtConvideoToIsovideo(CString handLabelFilePath, CString rgbFilePath, CString IsoVideoSaveName, vector<IsoVideoInfo> & cvtIsovideoInfo, CString SegInfoDevelpath);
double CvtConvDepthVideo2IsoVideo(CString depthVideoFile, CString videoSegResult,CString saveVideoFile);
double CvtConvHandLabel2Iso(CString dstLabelFile, CString videoSegResult,CString saveIsoHandLabelFile);
double CvtConvideoToIsovideo_flag(CString handLabelFilePath, CString rgbFilePath, CString IsoVideoSaveName, vector<IsoVideoInfo>& cvtIsovideoInfo, CString SegInfoDevelpath);
double fvisualHandDetection(CString handLabelFilePath, CString rgbFilePath, CString IsoVideoSaveName);
void RGB2DepthRectify(TwoDimension src, TwoDimension & dst);
int fGetVideoLength(CString rgbFilePath){
	cout<<"videoPath:"<<rgbFilePath<<endl;
	CvCapture *capture = cvCreateFileCapture(rgbFilePath);
  	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
	
	cvReleaseCapture(&capture);
	return numFrames;
}

int getTxtFileNum(vector<string> files)
{
	int vsize = files.size();
	int txtnum = 0;
	CString txtname = "txt";
	for(int i = 0; i < vsize; i++)
	{
		CString filename = files[i].c_str();
		CString key = filename.Right(3);
		if (key == txtname)
		{
			txtnum ++;
		}
	}
	return txtnum;
}
bool IsCorrespondingFile(CString videofile, CString labelfile)
{
	CString CSvideoid = videofile.Right(10).Left(4);
	CString CSlabelid = labelfile.Right(10).Left(4);
	if(CSvideoid == CSlabelid)
		return true;
	else
		return false;
}

void swapInt(int &a, int &b)
{
	int tmp;
	tmp = a;
	a = b;
	b = tmp;
}

int min(int a, int b)
{
	if(a<=b) return a;
	return b;
}

double gethandPosition(frameHandPosition hand, int frame);

int main()
{
#ifdef getVideoLength
	CString videoLenFile = "..\\output\\videoLength.txt";
	ofstream of;
	of.open(videoLenFile, ios::app);
	vector<string> videoPath;
	int ndevel = getCurrentDir(ValidPath.GetBuffer(0)) ;
	for(int i = 1; i <= ndevel; i++)
	{
		CString CSDevel;
		CSDevel.Format("%03d", i);
		CString ValidDevelPath = ValidPath + "\\" + CSDevel;

		string subFilePath = ValidDevelPath.GetBuffer(0);
		getFiles(subFilePath, videoPath);
		int pathlen = videoPath.size();
		int labelnum = getTxtFileNum(videoPath);

		int videoNum = (videoPath.size() - labelnum)/ 2;

		int labelbegin = videoNum * 2;
		int numrgbvideo = videoNum;
		int rgbbegin = 1 + 0 * 2;
		for(int j = 0; j < numrgbvideo; j++)
		{
			CString rgbFilePath = videoPath[rgbbegin].c_str();
			rgbbegin += 2;

			int len = fGetVideoLength(rgbFilePath);
			int videoid = atoi(rgbFilePath.Right(11).Left(5));
			CString tmp;
			tmp.Format("%05d %03d\n", videoid, len);
			of<<tmp;
		}
		int vsize = videoPath.size();
		for(int j = 0; j < vsize; j ++)
		{
			videoPath.pop_back();
		}
	}
	of.close();
#endif
#ifdef ConGSegmentation
	CString IsoVideoSaveName = "..\\output\\cvtConGTestToIso";
	CString SegInfopath = "..\\output\\ConGTestSegInfo";
	_mkdir(IsoVideoSaveName);
	_mkdir(SegInfopath);

	vector<string> videoPath;
	int ndevel = getCurrentDir(ValidPath.GetBuffer(0)) ;
	for(int i = 1; i <= ndevel; i++)
	{
		CString CSDevel;
		CSDevel.Format("%03d", i);
		CString ValidDevelPath = ValidPath + "\\" + CSDevel;
		CString IsoVideoDevelPath = IsoVideoSaveName + "\\" + CSDevel;
		CString SegInfoDevelpath = SegInfopath + "\\" + CSDevel;
		CString HandLabelDevel = HandLabelPath + "\\" + CSDevel;
		ifstream fin(IsoVideoDevelPath);
		_mkdir(IsoVideoDevelPath);

		_mkdir(SegInfoDevelpath);
		string subFilePath = ValidDevelPath.GetBuffer(0);
		getFiles(subFilePath, videoPath);
		int pathlen = videoPath.size();
		int labelnum = getTxtFileNum(videoPath);

		int videoNum = (videoPath.size() - labelnum)/ 2;

		int labelbegin = videoNum * 2;
		int numrgbvideo = videoNum;
		int rgbbegin = 1 + 0 * 2;
		for(int j = 0; j < numrgbvideo; j++)
		{

			vector<IsoVideoInfo> cvtIsovideoInfo;
			CString rgbFilePath = videoPath[rgbbegin].c_str();
			
			CString handLabelFilePath = HandLabelDevel + "\\Label_" + rgbFilePath.Right(11).Left(7) + ".txt";
			//for depth
			rgbbegin += 2;
			if(!IsCorrespondingFile(rgbFilePath, handLabelFilePath))
			{
				CString tmp = rgbFilePath;
				//of<<tmp.Right(14)<<endl;
				continue;
			}
			double a = CvtConvideoToIsovideo(handLabelFilePath, rgbFilePath, IsoVideoDevelPath, cvtIsovideoInfo, SegInfoDevelpath);
			int tmpsize = cvtIsovideoInfo.size();
			for(int k = 0; k < tmpsize; k++)
			{
				cvtIsovideoInfo.pop_back();
			}
		}
		int vsize = videoPath.size();
		for(int j = 0; j < vsize; j ++)
		{
			videoPath.pop_back();
		}
	}
#endif
#ifdef CvtConGHandDetctionSegmentaion
	CString IsoDepthVideoSaveName = "C:\\zhipeng\\ICT_NHCI_ContinuousGestureCode\\ConAllcodes\\ProcessingDatacode\\seg_valid_handDetection_2stream";
	CString SegInfopath = "C:\\zhipeng\\ICT_NHCI_ContinuousGestureCode\\2017\\seg_valid_rgb_2stream\\ConGTestSegInfo";
	CString oriDepthVideo = "C:\\zhipeng\\ICT_NHCI_ContinuousGestureCode\\cong\\valid";

	_mkdir(IsoDepthVideoSaveName);

	vector<string> videoPath;
	int ndevel = getCurrentDir(oriDepthVideo.GetBuffer(0)) ;
	for(int i = 1; i <= ndevel; i++)
	{
		CString CSDevel;
		CSDevel.Format("%03d", i);
		CString ValidDevelPath = oriDepthVideo + "\\" + CSDevel;
		CString IsoVideoDevelPath = IsoDepthVideoSaveName + "\\" + CSDevel;
		CString SegInfoDevelpath = SegInfopath + "\\" + CSDevel;

		ifstream fin(IsoVideoDevelPath);
		_mkdir(IsoVideoDevelPath);

		string subFilePath = ValidDevelPath.GetBuffer(0);
		getFiles(subFilePath, videoPath);
		int pathlen = videoPath.size();

		int videoNum = videoPath.size();

		int numrgbvideo = videoNum;
		int depthbegin = 0;
		for(int j = 0; j < numrgbvideo; j++)
		{
			vector<IsoVideoInfo> cvtIsovideoInfo;
			CString depthFilePath = videoPath[depthbegin].c_str();
			
			CString desSegFilePath = SegInfoDevelpath + "\\" + depthFilePath.Right(11).Left(5) + ".txt";

			CString dessavePath = IsoVideoDevelPath + "\\" + depthFilePath.Right(11).Left(5);
			ifstream fin(dessavePath);
			_mkdir(dessavePath);
			//for depth
			depthbegin += 1;

			double a = CvtConvHandLabel2Iso(depthFilePath, desSegFilePath, dessavePath);

		}
		int vsize = videoPath.size();
		for(int j = 0; j < vsize; j ++)
		{
			videoPath.pop_back();
		}
	}
#endif // CvtConGHandDetctionSegmentaion

#ifdef CvtConGDepthSegmentation
	CString IsoDepthVideoSaveName = "C:\\zhipeng\\ICT_NHCI_ContinuousGestureCode\\ConAllcodes\\ProcessingDatacode\\seg_valid_depth_2stream";
	CString SegInfopath = "C:\\zhipeng\\ICT_NHCI_ContinuousGestureCode\\2017\\seg_valid_rgb_2stream\\ConGTestSegInfo";
	CString oriDepthVideo = "F:\\ChaLearn2017\\conG\\Cons\\ConGD_Phase_1_aligned\\valid";

	_mkdir(IsoDepthVideoSaveName);

	vector<string> videoPath;
	int ndevel = getCurrentDir(oriDepthVideo.GetBuffer(0)) ;
	for(int i = 1; i <= ndevel; i++)
	{
		CString CSDevel;
		CSDevel.Format("%03d", i);
		CString ValidDevelPath = oriDepthVideo + "\\" + CSDevel;
		CString IsoVideoDevelPath = IsoDepthVideoSaveName + "\\" + CSDevel;
		CString SegInfoDevelpath = SegInfopath + "\\" + CSDevel;

		ifstream fin(IsoVideoDevelPath);
		_mkdir(IsoVideoDevelPath);

		string subFilePath = ValidDevelPath.GetBuffer(0);
		getFiles(subFilePath, videoPath);
		int pathlen = videoPath.size();

		int videoNum = videoPath.size();

		int numrgbvideo = videoNum;
		int depthbegin = 0;
		for(int j = 0; j < numrgbvideo; j++)
		{
			vector<IsoVideoInfo> cvtIsovideoInfo;
			CString depthFilePath = videoPath[depthbegin].c_str();
			
			CString desSegFilePath = SegInfoDevelpath + "\\" + depthFilePath.Right(11).Left(5) + ".txt";

			CString dessavePath = IsoVideoDevelPath + "\\" + depthFilePath.Right(11).Left(5);
			ifstream fin(dessavePath);
			_mkdir(dessavePath);
			//for depth
			depthbegin += 1;

			double a = CvtConvDepthVideo2IsoVideo(depthFilePath, desSegFilePath, dessavePath);

		}
		int vsize = videoPath.size();
		for(int j = 0; j < vsize; j ++)
		{
			videoPath.pop_back();
		}
	}
#endif


	return 0;
}
double gethandPosition(frameHandPosition hand, int frame)
{
	if(hand.isDetectOneHand)
		return (hand.handA2.y + hand.handA1.y) / 2.0;
	else                            //返回两只手中位置较高的位置
	{
		double handAcenter = (hand.handA1.y + hand.handA2.y) / 2.0;
		double handBcenter = (hand.handB1.y + hand.handB2.y) / 2.0;
		if(frame != 0)				//actioning
			if(handAcenter < handBcenter)
				return handAcenter;
			else
				return handBcenter;
		else
		{
		//直接取两个手中心的平均高度,防止开始手部区域故意抬高
			return (handAcenter + handBcenter) / 2;
		}

	}

}
double fvisualHandDetection(CString handLabelFilePath, CString rgbFilePath, CString IsoVideoSaveName)
{
	CString videoname = rgbFilePath.Right(11);
	IsoVideoSaveName = IsoVideoSaveName + "\\" + "VH" + videoname;

	std::cout<<videoname<<endl;
	fstream filestream;

	map <int, frameHandPosition> maphand;
	
	frameHandPosition handposition;
	vector<IplImage*> vColor;//vColor 存储RGB video
	char buffer[256];

	filestream.open(handLabelFilePath, ios::in);
	if(!filestream)
	{
		std::cout<<handLabelFilePath<<" does not exists"<<endl;
		return -1;
	}
	//read handLabelFilePath
	while(filestream.getline(buffer, 256))
	{
		int bufferlen = strlen(buffer);
		if(bufferlen <= 20)//detect one hand
		{
			handposition.isDetectOneHand = true;
			sscanf(buffer, "%04d %03d %03d %03d %03d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y);
			TwoDimension tmp;
			RGB2DepthRectify(handposition.handA1, handposition.handA1);
			RGB2DepthRectify(handposition.handA2, handposition.handA2);
			handposition.handB1.x = 0;
			handposition.handB1.y = 0;
			handposition.handB2.x = 0;
			handposition.handB2.y = 0;
		}
		else//detect two hands
		{
			handposition.isDetectOneHand = false;
			sscanf(buffer, "%04d %03d %03d %03d %03d %03d %03d %03d %03d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y,
				&handposition.handB1.x, &handposition.handB1.y, &handposition.handB2.x, &handposition.handB2.y);
			RGB2DepthRectify(handposition.handA1, handposition.handA1);
			RGB2DepthRectify(handposition.handA2, handposition.handA2);
			RGB2DepthRectify(handposition.handB1, handposition.handB1);
			RGB2DepthRectify(handposition.handB2, handposition.handB2);
		}
		maphand[handposition.nframe] = handposition;

	}
	// read video
	CvCapture *capture = cvCreateFileCapture(rgbFilePath);
  	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
	if(numFrames < 1)
		return -1;
	for(int i = 0; i < numFrames; i++)
	{
		IplImage* tempFrame = cvQueryFrame(capture);
		vColor.push_back(cvCloneImage(tempFrame));
	}
	cvReleaseCapture(&capture);

	CvVideoWriter *writerhand = 0;
	writerhand = cvCreateVideoWriter(IsoVideoSaveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);
	for(int i = 0; i < numFrames; i++)
	{
		
		if (maphand.find(i) != maphand.end())
		{
			handposition = maphand.find(i)->second;
			TwoDimension A, B;
			A = handposition.handA1;
			B = handposition.handA2;
			cvRectangle(vColor[i], cvPoint(A.x,A.y), cvPoint(B.x, B.y), Scalar(0, 255, 255), 1, 1, 0);  
			if(handposition.isDetectOneHand == false)
			{
				A = handposition.handB1;
				B = handposition.handB2;
				cvRectangle(vColor[i], cvPoint(A.x,A.y), cvPoint(B.x, B.y), Scalar(0,255,0), 1, 1, 0);  
			}
		}
		cvWriteFrame(writerhand,vColor[i]);
	}
	cvReleaseVideoWriter(&writerhand);
	//delete maphand
	map<int, frameHandPosition>::iterator it;
	for(it = maphand.begin(); it != maphand.end();)
	{
		maphand.erase(it++);
	}
	//release video vector memory
	for(int i = numFrames - 1; i >= 0; i--)
	{
		cvReleaseImage(&vColor[i]);
		vColor.pop_back();
	}
	return 0;
}

double CvtConvideoToIsovideo(CString handLabelFilePath, CString rgbFilePath, CString IsoVideoSaveName, vector<IsoVideoInfo>& cvtIsovideoInfo, CString SegInfoDevelpath)
{
	CString videoname = rgbFilePath.Right(11).Left(5);
	IsoVideoSaveName = IsoVideoSaveName + "\\" + videoname;

	_mkdir(IsoVideoSaveName);

	CString IsoinfoTxt = SegInfoDevelpath + "\\" + videoname + ".txt";
	ofstream of;
	of.open(IsoinfoTxt, ios::app);

	std::cout<<videoname<<endl;
	fstream filestream;

	map <int, frameHandPosition> maphand;
	
	frameHandPosition handposition;
	vector<IplImage*> vColor;//vColor 存储RGB video
	char buffer[256];

	filestream.open(handLabelFilePath, ios::in);
	if(!filestream)
	{
		std::cout<<handLabelFilePath<<" does not exists"<<endl;
		return -1;
	}
	//read handLabelFilePath
	while(filestream.getline(buffer, 256))
	{
		int bufferlen = strlen(buffer);
		if(bufferlen <= 20)//detect one hand
		{
			//handposition.isDetectOneHand = true;
			sscanf(buffer, "%04d %03d %03d %03d %03d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y);
			handposition.handB1.x = 0;
			handposition.handB1.y = 0;
			handposition.handB2.x = 0;
			handposition.handB2.y = 0;
		}
		else//detect two hands
		{
			//handposition.isDetectOneHand = false;
			sscanf(buffer, "%04d %03d %03d %03d %03d %03d %03d %03d %03d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y,
				&handposition.handB1.x, &handposition.handB1.y, &handposition.handB2.x, &handposition.handB2.y);
		}
		//for new 2017
		if(handposition.handB1.x == 0)
			handposition.isDetectOneHand = true;
		else
			handposition.isDetectOneHand = false;
		maphand[handposition.nframe] = handposition;

	}
	// read video
	CvCapture *capture = cvCreateFileCapture(rgbFilePath);
  	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
	if(numFrames < 1)
		return -1;
	for(int i = 0; i < numFrames; i++)
	{
		IplImage* tempFrame = cvQueryFrame(capture);
		vColor.push_back(cvCloneImage(tempFrame));
	}
	cvReleaseCapture(&capture);

	int gestureStart = 0;//the frame indicates the one gesture begins
	int gestureEnd = 0;//the frame indicates the one gesture ends
	double level = 0;//the level can judge if the gesture begins 
	//IplImage* tempImg = cvCreateImage(cvSize(vColor[0]->width,vColor[0]->height),vColor[0]->depth,vColor[0]->nChannels);
	double threshold = 10;
	int videocount = 0;
	CvVideoWriter *writer = 0;

	CvVideoWriter *writerhand = 0;
	CString CStmp;
	//CStmp.Format("\\%03d.avi", videocount++);
	//CString saveName = IsoVideoSaveName + CStmp; 
	//writerhand = cvCreateVideoWriter(saveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);

	double maxnum = 999999;
	double maxdis = 100 * 100;
	bool isFirstEndFrame = false;
	bool isFirstActionFrame = false;
	double thresholdFaceToLevel = 2;
	double faceX, faceY; 
	bool isFaceDetect = false;
	for(int i = 0; i < numFrames; i++)
	{		
		if (!isFaceDetect)
		{
			cvtConToIsoPreProcessFace.OutheadDetectionVIPLSDK(vColor[i], faceTxtFilename, i);
			faceY = cvtConToIsoPreProcessFace.faceY;
			faceX = cvtConToIsoPreProcessFace.faceX;
			if(faceX !=0&& faceY !=0)//detect the face
			{
				isFaceDetect = true;
			}
		}
		if (maphand.find(i) != maphand.end())
		{			
			handposition = maphand.find(i)->second;
			level = gethandPosition(handposition, 0) - threshold;
			double facehigh = 240 - faceY;
			double levelhigh = 240 - level;
			if(facehigh / levelhigh < thresholdFaceToLevel)//This means the level is so closed to the face, the level is wrong.An image's height is 240
				continue;
			else
				break;
		}
		else                                                       //the level is assigned 240 - 5 if the first frame does not detect the hand
		{
			level = 240 - threshold / 2;
			double facehigh = 240 - faceY;
			double levelhigh = 240 - level;
			if(facehigh / levelhigh < thresholdFaceToLevel)//This means the level is so closed to the face, the level is wrong.An image's height is 240
				continue;
			else
				break;
		}

	}



	//test
	//level = 240;

	
	isFirstActionFrame = true;//开始默认是动作结束状态
	isFirstEndFrame = false;
	IsoVideoInfo oneIsoVideoInfo;
	int threshFrame = 5;
	double removedis = 0;
	bool IsFirstDetectHand = true;
	TwoDimension lefthandcenter;
	TwoDimension righthandcenter;
	for(int i = 0; i < numFrames; i++)
	{
		
		//cvLine(vColor[i], cvPoint(1, level), cvPoint(310, level), CV_RGB(255,0,0),1);

		double lowest = 0;
		if (maphand.find(i) == maphand.end())
		{
			lowest = maxnum;
		}
		else
		{
			handposition = maphand.find(i)->second;
			if(IsFirstDetectHand)
			{
				IsFirstDetectHand = false;
				TwoDimension A, B;
				A.x = (handposition.handA1.x + handposition.handA2.x) / 2;
				A.y = (handposition.handA1.y + handposition.handA2.y) / 2;
				B.x = (handposition.handB1.x + handposition.handB2.x) / 2;
				B.y = (handposition.handB1.y + handposition.handB2.y) / 2;
				if(handposition.isDetectOneHand)
				{
					if(A.x < faceX)//A is right hand
					{
						righthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						righthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						lefthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						lefthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
						swapInt(maphand.find(i)->second.handB1.x, maphand.find(i)->second.handA1.x);
						swapInt(maphand.find(i)->second.handB1.y, maphand.find(i)->second.handA1.y);
						swapInt(maphand.find(i)->second.handB2.x, maphand.find(i)->second.handA2.x);
						swapInt(maphand.find(i)->second.handB2.y, maphand.find(i)->second.handA2.y);
					}
					else//A is left hand
					{
						lefthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						lefthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						righthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						righthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
					}
				}
				else 
				{
					if(A.x < B.x)//A is right and B is left hand
					{
						righthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						righthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						lefthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						lefthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
						swapInt(maphand.find(i)->second.handB1.x, maphand.find(i)->second.handA1.x);
						swapInt(maphand.find(i)->second.handB1.y, maphand.find(i)->second.handA1.y);
						swapInt(maphand.find(i)->second.handB2.x, maphand.find(i)->second.handA2.x);
						swapInt(maphand.find(i)->second.handB2.y, maphand.find(i)->second.handA2.y);
					}
					else
					{
						lefthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						lefthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						righthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						righthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
					}
				}
			}
			else
			{
				TwoDimension A, B;
				A.x = (handposition.handA1.x + handposition.handA2.x) / 2;
				A.y = (handposition.handA1.y + handposition.handA2.y) / 2;
				B.x = (handposition.handB1.x + handposition.handB2.x) / 2;
				B.y = (handposition.handB1.y + handposition.handB2.y) / 2;
				double disLeftToA = getTwoDimDis(lefthandcenter, A);//A is left and B is right hand always
				double disLeftToB = getTwoDimDis(lefthandcenter, B);
				if(disLeftToA > disLeftToB)
				{
					lefthandcenter.x = B.x;
					lefthandcenter.y = B.y;
					righthandcenter.x = A.x;
					righthandcenter.y = A.y;
					//exchange the lef and right hand
					swapInt(maphand.find(i)->second.handB1.x, maphand.find(i)->second.handA1.x);
					swapInt(maphand.find(i)->second.handB1.y, maphand.find(i)->second.handA1.y);
					swapInt(maphand.find(i)->second.handB2.x, maphand.find(i)->second.handA2.x);
					swapInt(maphand.find(i)->second.handB2.y, maphand.find(i)->second.handA2.y);
				}
				else
				{
					lefthandcenter.x = A.x;
					lefthandcenter.y = A.y;
					righthandcenter.x = B.x;
					righthandcenter.y = B.y;
				}
			}
			//TwoDimension A, B;
			//A = handposition.handA1;
			//B = handposition.handA2;
			////cvRectangle(vColor[i], cvPoint(A.x,A.y), cvPoint(B.x, B.y), Scalar(0, 255, 255), 1, 1, 0);  
			//if(handposition.isDetectOneHand == false)
			//{
			//	A = handposition.handB1;
			//	B = handposition.handB2;
			//	cvRectangle(vColor[i], cvPoint(A.x,A.y), cvPoint(B.x, B.y), Scalar(0,255,0), 1, 1, 0);  
			//}
			//smoothing process，解决只检测到一只手，且位于level之下的情况，检测出错，smoothing插值
			if (handposition.isDetectOneHand && i != 0)
			{
				double high = (handposition.handA1.y + handposition.handA2.y) / 2.0;
				if(high > level)
				{
					double thresholdDis = 60 * 60;
					
					frameHandPosition front;
					frameHandPosition next;
					TwoDimension fA, nB;
					int fj,nj;
					for(int j = i - 1; j >= 0;j --)
					{
						if(maphand.find(j) != maphand.end())
						{
							front = maphand.find(j)->second;
							if(!front.isDetectOneHand)
							{
								fA.x = (front.handB1.x + front.handB2.x) / 2;
								fA.y = (front.handB1.y + front.handB2.y) / 2;
								fj = j;
								break;
							}
						}
					}
					
					for(int j = i + 1; j < numFrames; j++)
					{
						if(maphand.find(j) != maphand.end())
						{
							next = maphand.find(j)->second;
							if(!next.isDetectOneHand)
							{
								nB.x = (next.handB1.x + next.handB2.x) / 2;
								nB.y = (next.handB1.y + next.handB2.y) / 2;
								nj = j;
								break;
							}
						}
					}
					if(fj != -1&&nj != numFrames)//不是最前面和最后面的
					{
						double twoFrameHandDistance = (fA.x - nB.x) * (fA.x - nB.x) + (fA.y - nB.y) * (fA.y - nB.y);

						if (twoFrameHandDistance < thresholdDis)
						{
							handposition.isDetectOneHand = false;
							handposition.handB1.x = (front.handB1.x + next.handB1.x) / 2;
							handposition.handB1.y = (front.handB1.y + next.handB1.y) / 2; 
							handposition.handB2.y = (front.handB2.y + next.handB2.y) / 2; 
							handposition.handB2.x = (front.handB2.x + next.handB2.x) / 2;
							maphand.find(i)->second.isDetectOneHand = false;
							maphand.find(i)->second.handB1.x = (front.handB1.x + next.handB1.x) / 2;
							maphand.find(i)->second.handB1.y = (front.handB1.y + next.handB1.y) / 2; 
							maphand.find(i)->second.handB2.y = (front.handB2.y + next.handB2.y) / 2; 
							maphand.find(i)->second.handB2.x = (front.handB2.x + next.handB2.x) / 2;
						}
					}
				}
			}
			if(!handposition.isDetectOneHand && i != 0)//矫正错误检测，利用插值拟合
			{
				double tmphighest = gethandPosition(handposition, i);
				if(tmphighest > level)
				{
					int fj, nj;
					frameHandPosition front;
					frameHandPosition next;
					
					for(fj = i - 1; fj >= 0;fj --)
					{
						if(maphand.find(fj) != maphand.end())
						{		
							if(!maphand.find(fj)->second.isDetectOneHand)
							{
								front = maphand.find(fj)->second;
								break;
							}	
						}
					}
					for(nj = i + 1; nj < numFrames;nj ++)
					{
						if(maphand.find(nj) != maphand.end())
						{		
							if(!maphand.find(nj)->second.isDetectOneHand)
							{
								next = maphand.find(nj)->second;
								break;
							}								
						}
					}
					if(fj != -1 && nj != numFrames)
					{
						TwoDimension c11, c12, c21, c22, c31, c32;
						c11.x = (front.handA1.x + front.handA2.x) / 2;
						c11.y = (front.handA1.y + front.handA2.y) / 2;
						c12.x = (front.handB1.x + front.handB2.x) / 2;
						c12.y = (front.handB1.y + front.handB2.y) / 2;

						c21.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						c21.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						c22.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						c22.y = (handposition.handB1.y + handposition.handB2.y) / 2;

						c31.x = (next.handA1.x + next.handA2.x) / 2;
						c31.y = (next.handA1.y + next.handA2.y) / 2;
						c32.x = (next.handB1.x + next.handB2.x) / 2;
						c32.y = (next.handB1.y + next.handB2.y) / 2;


						TwoDimension ave1, ave2;
						ave1.x = (c11.x + c31.x) / 2;
						ave1.y = (c11.y + c31.y) / 2;
						ave2.x = (c12.x + c32.x) / 2;
						ave2.y = (c12.y + c32.y) / 2;
						double twohandMoveDis = getTwoDimDis(ave1, c21) + getTwoDimDis(ave2, c22);
						if(twohandMoveDis > maxdis)
						{
							handposition.isDetectOneHand = false;
							handposition.handB1.x = (front.handB1.x + next.handB1.x) / 2;
							handposition.handB1.y = (front.handB1.y + next.handB1.y) / 2; 
							handposition.handB2.y = (front.handB2.y + next.handB2.y) / 2; 
							handposition.handB2.x = (front.handB2.x + next.handB2.x) / 2;
						}
					}
				}
			}
			lowest = gethandPosition(handposition, i);
		}
		if (lowest < level)
		{
			if(isFirstActionFrame == true)
			{
				std::cout<<"begin frame="<<i<<endl;
				gestureStart = i;
				oneIsoVideoInfo.start = gestureStart;
				
				isFirstActionFrame = false;					//之后就不是第一次动作开始帧了
				isFirstEndFrame = true;
				//CStmp.Format("%03d.avi", videocount++);
				//CString saveName = IsoVideoSaveName + "\\" + CStmp; 
				//writer = cvCreateVideoWriter(saveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);
				//cvWriteFrame(writer, vColor[i]);
			}
			else
			{
				//cvWriteFrame(writer, vColor[i]);
			}
		}
		else
		{
			if(isFirstEndFrame == false)
			{
				continue;
			}
			else                                             //FirstEndFrame
			{
				std::cout<<"end frame="<<i<<endl;
				gestureEnd = i;
				oneIsoVideoInfo.end = gestureEnd;
				if(gestureEnd - gestureStart > threshFrame)
				{
					of<<gestureStart<<"	"<<gestureEnd<<endl;
					cvtIsovideoInfo.push_back(oneIsoVideoInfo);
				}
				isFirstActionFrame = true;
				isFirstEndFrame = false;					//之后就不是第一次动作结束帧了

			}
		}

	}
	if(isFirstActionFrame == false)							//if the action don't stop until ending
	{
		cvReleaseVideoWriter(&writer);
		gestureEnd = numFrames - 1;
		oneIsoVideoInfo.end = gestureEnd;
		if(gestureEnd - gestureStart > threshFrame)
		{
			of<<gestureStart<<"	"<<gestureEnd<<endl;
			cvtIsovideoInfo.push_back(oneIsoVideoInfo);
		}
	}
	int numIsoVideo = cvtIsovideoInfo.size();
	int countIsoVideo = 1;
	for(int i = 0; i < numIsoVideo; i++)
	{
		int videostart = cvtIsovideoInfo[i].start;
		int videoend = cvtIsovideoInfo[i].end;
		CStmp.Format("%03d.avi", countIsoVideo++);
		CString saveName = IsoVideoSaveName + "\\" + CStmp; 
		writer = cvCreateVideoWriter(saveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);
		for(int j = videostart; j < videoend; j++)
		{
			cvWriteFrame(writer, vColor[j]);
		}
		cvReleaseVideoWriter(&writer);
	}

	//close file
	of.close();
	//delete maphand
	map<int, frameHandPosition>::iterator it;
	for(it = maphand.begin(); it != maphand.end();)
	{
		maphand.erase(it++);
	}
	//release video vector memory
	for(int i = numFrames - 1; i >= 0; i--)
	{
		cvReleaseImage(&vColor[i]);
		vColor.pop_back();
	}
	return 0;
}
double CvtConvideoToIsovideo_flag(CString handLabelFilePath, CString rgbFilePath, CString IsoVideoSaveName, vector<IsoVideoInfo>& cvtIsovideoInfo, CString SegInfoDevelpath)
{
	CString videoname = rgbFilePath.Right(11).Left(5);
	IsoVideoSaveName = IsoVideoSaveName + "\\" + videoname;

	_mkdir(IsoVideoSaveName);

	CString IsoinfoTxt = SegInfoDevelpath + "\\" + videoname + ".txt";
	ofstream of;
	of.open(IsoinfoTxt, ios::app);

	std::cout<<videoname<<endl;
	fstream filestream;

	map <int, frameHandPosition> maphand;
	
	frameHandPosition handposition;
	vector<IplImage*> vColor;//vColor 存储RGB video
	char buffer[256];

	filestream.open(handLabelFilePath, ios::in);
	if(!filestream)
	{
		std::cout<<handLabelFilePath<<" does not exists"<<endl;
		return -1;
	}
	//read handLabelFilePath
	while(filestream.getline(buffer, 256))
	{
		int bufferlen = strlen(buffer);
		{
			int flag1,flag2;
			sscanf(buffer, "%04d %03d %03d %03d %03d %d %03d %03d %03d %03d %d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y,&flag1,
				&handposition.handB1.x, &handposition.handB1.y, &handposition.handB2.x, &handposition.handB2.y, &flag2);
		}		
		if(handposition.handB1.x == 0)
			handposition.isDetectOneHand = true;
		else
			handposition.isDetectOneHand = false;
		maphand[handposition.nframe] = handposition;
	}
	// read video
	CvCapture *capture = cvCreateFileCapture(rgbFilePath);
  	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
	if(numFrames < 1)
		return -1;
	for(int i = 0; i < numFrames; i++)
	{
		IplImage* tempFrame = cvQueryFrame(capture);
		vColor.push_back(cvCloneImage(tempFrame));
	}
	cvReleaseCapture(&capture);

	int gestureStart = 0;//the frame indicates the one gesture begins
	int gestureEnd = 0;//the frame indicates the one gesture ends
	double level = 0;//the level can judge if the gesture begins 
	//IplImage* tempImg = cvCreateImage(cvSize(vColor[0]->width,vColor[0]->height),vColor[0]->depth,vColor[0]->nChannels);
	double threshold = 10;
	int videocount = 0;
	CvVideoWriter *writer = 0;

	CvVideoWriter *writerhand = 0;
	CString CStmp;
	//CStmp.Format("\\%03d.avi", videocount++);
	//CString saveName = IsoVideoSaveName + CStmp; 
	//writerhand = cvCreateVideoWriter(saveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);

	double maxnum = 999999;
	double maxdis = 100 * 100;
	bool isFirstEndFrame = false;
	bool isFirstActionFrame = false;
	double thresholdFaceToLevel = 2;
	double faceX, faceY; 
	bool isFaceDetect = false;
	for(int i = 0; i < numFrames; i++)
	{		
		if (!isFaceDetect)
		{
			cvtConToIsoPreProcessFace.OutheadDetectionVIPLSDK(vColor[i], faceTxtFilename, i);
			faceY = cvtConToIsoPreProcessFace.faceY;
			faceX = cvtConToIsoPreProcessFace.faceX;
			if(faceX !=0&& faceY !=0)//detect the face
			{
				isFaceDetect = true;
			}
		}
		if (maphand.find(i) != maphand.end())
		{			
			handposition = maphand.find(i)->second;
			level = gethandPosition(handposition, 0) - threshold;
			double facehigh = 240 - faceY;
			double levelhigh = 240 - level;
			if(facehigh / levelhigh < thresholdFaceToLevel)//This means the level is so closed to the face, the level is wrong.An image's height is 240
				continue;
			else
				break;
		}
		else                                                       //the level is assigned 240 - 5 if the first frame does not detect the hand
		{
			level = 240 - threshold / 2;
			double facehigh = 240 - faceY;
			double levelhigh = 240 - level;
			if(facehigh / levelhigh < thresholdFaceToLevel)//This means the level is so closed to the face, the level is wrong.An image's height is 240
				continue;
			else
				break;
		}

	}



	//test
	//level = 240;

	
	isFirstActionFrame = true;//开始默认是动作结束状态
	isFirstEndFrame = false;
	IsoVideoInfo oneIsoVideoInfo;
	int threshFrame = 5;
	double removedis = 0;
	bool IsFirstDetectHand = true;
	TwoDimension lefthandcenter;
	TwoDimension righthandcenter;
	for(int i = 0; i < numFrames; i++)
	{
		
		//cvLine(vColor[i], cvPoint(1, level), cvPoint(310, level), CV_RGB(255,0,0),1);

		double lowest = 0;
		if (maphand.find(i) == maphand.end())
		{
			lowest = maxnum;
		}
		else
		{
			handposition = maphand.find(i)->second;
			if(IsFirstDetectHand)
			{
				IsFirstDetectHand = false;
				TwoDimension A, B;
				A.x = (handposition.handA1.x + handposition.handA2.x) / 2;
				A.y = (handposition.handA1.y + handposition.handA2.y) / 2;
				B.x = (handposition.handB1.x + handposition.handB2.x) / 2;
				B.y = (handposition.handB1.y + handposition.handB2.y) / 2;
				if(handposition.isDetectOneHand)
				{
					if(A.x < faceX)//A is right hand
					{
						righthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						righthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						lefthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						lefthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
						swapInt(maphand.find(i)->second.handB1.x, maphand.find(i)->second.handA1.x);
						swapInt(maphand.find(i)->second.handB1.y, maphand.find(i)->second.handA1.y);
						swapInt(maphand.find(i)->second.handB2.x, maphand.find(i)->second.handA2.x);
						swapInt(maphand.find(i)->second.handB2.y, maphand.find(i)->second.handA2.y);
					}
					else//A is left hand
					{
						lefthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						lefthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						righthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						righthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
					}
				}
				else 
				{
					if(A.x < B.x)//A is right and B is left hand
					{
						righthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						righthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						lefthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						lefthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
						swapInt(maphand.find(i)->second.handB1.x, maphand.find(i)->second.handA1.x);
						swapInt(maphand.find(i)->second.handB1.y, maphand.find(i)->second.handA1.y);
						swapInt(maphand.find(i)->second.handB2.x, maphand.find(i)->second.handA2.x);
						swapInt(maphand.find(i)->second.handB2.y, maphand.find(i)->second.handA2.y);
					}
					else
					{
						lefthandcenter.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						lefthandcenter.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						righthandcenter.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						righthandcenter.y = (handposition.handB1.y + handposition.handB2.y) / 2;
					}
				}
			}
			else
			{
				TwoDimension A, B;
				A.x = (handposition.handA1.x + handposition.handA2.x) / 2;
				A.y = (handposition.handA1.y + handposition.handA2.y) / 2;
				B.x = (handposition.handB1.x + handposition.handB2.x) / 2;
				B.y = (handposition.handB1.y + handposition.handB2.y) / 2;
				double disLeftToA = getTwoDimDis(lefthandcenter, A);//A is left and B is right hand always
				double disLeftToB = getTwoDimDis(lefthandcenter, B);
				if(disLeftToA > disLeftToB)
				{
					lefthandcenter.x = B.x;
					lefthandcenter.y = B.y;
					righthandcenter.x = A.x;
					righthandcenter.y = A.y;
					//exchange the lef and right hand
					swapInt(maphand.find(i)->second.handB1.x, maphand.find(i)->second.handA1.x);
					swapInt(maphand.find(i)->second.handB1.y, maphand.find(i)->second.handA1.y);
					swapInt(maphand.find(i)->second.handB2.x, maphand.find(i)->second.handA2.x);
					swapInt(maphand.find(i)->second.handB2.y, maphand.find(i)->second.handA2.y);
				}
				else
				{
					lefthandcenter.x = A.x;
					lefthandcenter.y = A.y;
					righthandcenter.x = B.x;
					righthandcenter.y = B.y;
				}
			}
			//TwoDimension A, B;
			//A = handposition.handA1;
			//B = handposition.handA2;
			////cvRectangle(vColor[i], cvPoint(A.x,A.y), cvPoint(B.x, B.y), Scalar(0, 255, 255), 1, 1, 0);  
			//if(handposition.isDetectOneHand == false)
			//{
			//	A = handposition.handB1;
			//	B = handposition.handB2;
			//	cvRectangle(vColor[i], cvPoint(A.x,A.y), cvPoint(B.x, B.y), Scalar(0,255,0), 1, 1, 0);  
			//}
			//smoothing process，解决只检测到一只手，且位于level之下的情况，检测出错，smoothing插值
			if (handposition.isDetectOneHand && i != 0)
			{
				double high = (handposition.handA1.y + handposition.handA2.y) / 2.0;
				if(high > level)
				{
					double thresholdDis = 60 * 60;
					
					frameHandPosition front;
					frameHandPosition next;
					TwoDimension fA, nB;
					int fj,nj;
					for(int j = i - 1; j >= 0;j --)
					{
						if(maphand.find(j) != maphand.end())
						{
							front = maphand.find(j)->second;
							if(!front.isDetectOneHand)
							{
								fA.x = (front.handB1.x + front.handB2.x) / 2;
								fA.y = (front.handB1.y + front.handB2.y) / 2;
								fj = j;
								break;
							}
						}
					}
					
					for(int j = i + 1; j < numFrames; j++)
					{
						if(maphand.find(j) != maphand.end())
						{
							next = maphand.find(j)->second;
							if(!next.isDetectOneHand)
							{
								nB.x = (next.handB1.x + next.handB2.x) / 2;
								nB.y = (next.handB1.y + next.handB2.y) / 2;
								nj = j;
								break;
							}
						}
					}
					if(fj != -1&&nj != numFrames)//不是最前面和最后面的
					{
						double twoFrameHandDistance = (fA.x - nB.x) * (fA.x - nB.x) + (fA.y - nB.y) * (fA.y - nB.y);

						if (twoFrameHandDistance < thresholdDis)
						{
							handposition.isDetectOneHand = false;
							handposition.handB1.x = (front.handB1.x + next.handB1.x) / 2;
							handposition.handB1.y = (front.handB1.y + next.handB1.y) / 2; 
							handposition.handB2.y = (front.handB2.y + next.handB2.y) / 2; 
							handposition.handB2.x = (front.handB2.x + next.handB2.x) / 2;
							maphand.find(i)->second.isDetectOneHand = false;
							maphand.find(i)->second.handB1.x = (front.handB1.x + next.handB1.x) / 2;
							maphand.find(i)->second.handB1.y = (front.handB1.y + next.handB1.y) / 2; 
							maphand.find(i)->second.handB2.y = (front.handB2.y + next.handB2.y) / 2; 
							maphand.find(i)->second.handB2.x = (front.handB2.x + next.handB2.x) / 2;
						}
					}
				}
			}
			if(!handposition.isDetectOneHand && i != 0)//矫正错误检测，利用插值拟合
			{
				double tmphighest = gethandPosition(handposition, i);
				if(tmphighest > level)
				{
					int fj, nj;
					frameHandPosition front;
					frameHandPosition next;
					
					for(fj = i - 1; fj >= 0;fj --)
					{
						if(maphand.find(fj) != maphand.end())
						{		
							if(!maphand.find(fj)->second.isDetectOneHand)
							{
								front = maphand.find(fj)->second;
								break;
							}	
						}
					}
					for(nj = i + 1; nj < numFrames;nj ++)
					{
						if(maphand.find(nj) != maphand.end())
						{		
							if(!maphand.find(nj)->second.isDetectOneHand)
							{
								next = maphand.find(nj)->second;
								break;
							}								
						}
					}
					if(fj != -1 && nj != numFrames)
					{
						TwoDimension c11, c12, c21, c22, c31, c32;
						c11.x = (front.handA1.x + front.handA2.x) / 2;
						c11.y = (front.handA1.y + front.handA2.y) / 2;
						c12.x = (front.handB1.x + front.handB2.x) / 2;
						c12.y = (front.handB1.y + front.handB2.y) / 2;

						c21.x = (handposition.handA1.x + handposition.handA2.x) / 2;
						c21.y = (handposition.handA1.y + handposition.handA2.y) / 2;
						c22.x = (handposition.handB1.x + handposition.handB2.x) / 2;
						c22.y = (handposition.handB1.y + handposition.handB2.y) / 2;

						c31.x = (next.handA1.x + next.handA2.x) / 2;
						c31.y = (next.handA1.y + next.handA2.y) / 2;
						c32.x = (next.handB1.x + next.handB2.x) / 2;
						c32.y = (next.handB1.y + next.handB2.y) / 2;


						TwoDimension ave1, ave2;
						ave1.x = (c11.x + c31.x) / 2;
						ave1.y = (c11.y + c31.y) / 2;
						ave2.x = (c12.x + c32.x) / 2;
						ave2.y = (c12.y + c32.y) / 2;
						double twohandMoveDis = getTwoDimDis(ave1, c21) + getTwoDimDis(ave2, c22);
						if(twohandMoveDis > maxdis)
						{
							handposition.isDetectOneHand = false;
							handposition.handB1.x = (front.handB1.x + next.handB1.x) / 2;
							handposition.handB1.y = (front.handB1.y + next.handB1.y) / 2; 
							handposition.handB2.y = (front.handB2.y + next.handB2.y) / 2; 
							handposition.handB2.x = (front.handB2.x + next.handB2.x) / 2;
						}
					}
				}
			}
			lowest = gethandPosition(handposition, i);
		}
		if (lowest < level)
		{
			if(isFirstActionFrame == true)
			{
				std::cout<<"begin frame="<<i<<endl;
				gestureStart = i;
				oneIsoVideoInfo.start = gestureStart;
				
				isFirstActionFrame = false;					//之后就不是第一次动作开始帧了
				isFirstEndFrame = true;
				//CStmp.Format("%03d.avi", videocount++);
				//CString saveName = IsoVideoSaveName + "\\" + CStmp; 
				//writer = cvCreateVideoWriter(saveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);
				//cvWriteFrame(writer, vColor[i]);
			}
			else
			{
				//cvWriteFrame(writer, vColor[i]);
			}
		}
		else
		{
			if(isFirstEndFrame == false)
			{
				continue;
			}
			else                                             //FirstEndFrame
			{
				std::cout<<"end frame="<<i<<endl;
				gestureEnd = i;
				oneIsoVideoInfo.end = gestureEnd;
				if(gestureEnd - gestureStart > threshFrame)
				{
					of<<gestureStart<<"	"<<gestureEnd<<endl;
					cvtIsovideoInfo.push_back(oneIsoVideoInfo);
				}
				isFirstActionFrame = true;
				isFirstEndFrame = false;					//之后就不是第一次动作结束帧了

			}
		}

	}
	if(isFirstActionFrame == false)							//if the action don't stop until ending
	{
		cvReleaseVideoWriter(&writer);
		gestureEnd = numFrames - 1;
		oneIsoVideoInfo.end = gestureEnd;
		if(gestureEnd - gestureStart > threshFrame)
		{
			of<<gestureStart<<"	"<<gestureEnd<<endl;
			cvtIsovideoInfo.push_back(oneIsoVideoInfo);
		}
	}
	int numIsoVideo = cvtIsovideoInfo.size();
	int countIsoVideo = 1;
	for(int i = 0; i < numIsoVideo; i++)
	{
		int videostart = cvtIsovideoInfo[i].start;
		int videoend = cvtIsovideoInfo[i].end;
		CStmp.Format("%03d.avi", countIsoVideo++);
		CString saveName = IsoVideoSaveName + "\\" + CStmp; 
		writer = cvCreateVideoWriter(saveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);
		for(int j = videostart; j < videoend; j++)
		{
			cvWriteFrame(writer, vColor[j]);
		}
		cvReleaseVideoWriter(&writer);
	}

	//close file
	of.close();
	//delete maphand
	map<int, frameHandPosition>::iterator it;
	for(it = maphand.begin(); it != maphand.end();)
	{
		maphand.erase(it++);
	}
	//release video vector memory
	for(int i = numFrames - 1; i >= 0; i--)
	{
		cvReleaseImage(&vColor[i]);
		vColor.pop_back();
	}
	return 0;
}
double CvtConvDepthVideo2IsoVideo(CString depthVideoFile, CString videoSegResult, CString saveVideoFile){
	vector<IplImage*> vColor;//vColor 存储depth video
	char buffer[256];
	// read video
	cout<<"dst:"<<depthVideoFile<<endl;
	CvCapture *capture = cvCreateFileCapture(depthVideoFile);
  	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
	if(numFrames < 1)
		return -1;
	for(int i = 0; i < numFrames; i++)
	{
		IplImage* tempFrame = cvQueryFrame(capture);
		vColor.push_back(cvCloneImage(tempFrame));
	}
	cvReleaseCapture(&capture);


	//read segmentation result
	fstream filestream;

	filestream.open(videoSegResult, ios::in);
	if(!filestream)
	{
		std::cout<<videoSegResult<<" does not exists"<<endl;
		return -1;
	}
	vector<IsoVideoInfo> vseginfor;
	IsoVideoInfo example;
	while(filestream.getline(buffer, 256))
	{
		int start, end;
		sscanf(buffer, "%d %d\n", &start, &end);
		example.start = start;
		example.end = end;
		vseginfor.push_back(example);
	}
	int numIsoVideo = vseginfor.size();
	CString CStmp;
	CvVideoWriter *writer = 0;
	for(int i = 1; i <= numIsoVideo; i++){
		int videostart = vseginfor[i - 1].start;
		int videoend = vseginfor[i - 1].end;
		CStmp.Format("%03d.avi", i);
		CString saveName = saveVideoFile + "\\" + CStmp; 
		writer = cvCreateVideoWriter(saveName, CV_FOURCC('M','P','4','2'), 10, cvSize(vColor[0]->width, vColor[0]->height), 1);
		for(int j = videostart; j < videoend; j++)
		{
			cvWriteFrame(writer, vColor[j]);
		}
		cvReleaseVideoWriter(&writer);
	}
	//release video vector memory
	for(int i = numFrames - 1; i >= 0; i--)
	{
		cvReleaseImage(&vColor[i]);
		vColor.pop_back();
	}
	return 0;
}
double CvtConvHandLabel2Iso(CString dstLabelFile, CString videoSegResult,CString saveIsoHandLabelFile){
	fstream filestream;
	map <int, frameHandPosition> maphand;
	frameHandPosition handposition;
	char buffer[256];
	cout<<dstLabelFile<<endl;
	filestream.open(dstLabelFile, ios::in);
	if(!filestream)
	{
		std::cout<<dstLabelFile<<" does not exists"<<endl;
		return -1;
	}
	//read handLabelFilePath
	while(filestream.getline(buffer, 256))
	{
		int bufferlen = strlen(buffer);
		{
			sscanf(buffer, "%04d %03d %03d %03d %03d %03d %03d %03d %03d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y,
				&handposition.handB1.x, &handposition.handB1.y, &handposition.handB2.x, &handposition.handB2.y);
		}		
		if(handposition.handB1.x == 0)
			handposition.isDetectOneHand = true;
		else
			handposition.isDetectOneHand = false;
		maphand[handposition.nframe] = handposition;
	}
	filestream.close();

	//read segmentation result

	filestream.open(videoSegResult, ios::in);
	if(!filestream)
	{
		std::cout<<videoSegResult<<" does not exists"<<endl;
		return -1;
	}
	vector<IsoVideoInfo> vseginfor;
	IsoVideoInfo example;
	while(filestream.getline(buffer, 256))
	{
		int start, end;
		sscanf(buffer, "%d %d\n", &start, &end);
		example.start = start;
		example.end = end;
		vseginfor.push_back(example);
	}
	filestream.close();
	int numIsoVideo = vseginfor.size();
	CString CStmp;
	ofstream of;
	
	for(int i = 1; i <= numIsoVideo; i++){
		int videostart = vseginfor[i - 1].start;
		int videoend = vseginfor[i - 1].end;
		CStmp.Format("%03d.txt", i);
		CString saveName = saveIsoHandLabelFile + "\\" + CStmp; 
		of.open(saveName, ios::app);
		for(int j = videostart; j < videoend; j++)
		{
			if(maphand.find(j) != maphand.end()){
				CString strWrite;
				strWrite.Format("%04d %03d %03d %03d %03d %03d %03d %03d %03d\n", j - videostart, maphand[j].handA1.x, maphand[j].handA1.y, maphand[j].handA2.x, maphand[j].handA2.y, 
					maphand[j].handB1.x, maphand[j].handB1.y, maphand[j].handB2.x, maphand[j].handB2.y);
				of<<strWrite;
			}
		}
		of.close();
	}

	return 0;
}

void getFiles( string path, vector<string>& files )  
{  
    //文件句柄  
    long   hFile   =   0;  
    //文件信息  
    struct _finddata_t fileinfo;  
    string p;  
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
    {  
        do  
        {  
            //如果是目录,迭代之  
            //如果不是,加入列表  
            if((fileinfo.attrib &  _A_SUBDIR))  
            {  
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
					 getFiles( p.assign(path).append("\\").append(fileinfo.name), files );  
            }  
            else  
            {  
                files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
            }  
        }while(_findnext(hFile, &fileinfo)  == 0);  
        _findclose(hFile);  
    }  
}  

/*
input:src the original image(RGB or grey),dst is MaxConnectedDomain image
function:The function can get a image max connected domain
*/
void GetMaxConnectedDomain(IplImage *src, IplImage *dst)
{
    //IplImage *src = cvLoadImage("wind.png", CV_LOAD_IMAGE_COLOR);  
    cvNamedWindow("原始图像");  
    cvShowImage("原始图像", src);  
	cvWaitKey(100);
    //IplImage*    
	if(src->nChannels > 1)
		cvCvtColor(src, dst, CV_BGR2GRAY);  
    cvNamedWindow("灰度图像");  
    cvShowImage("灰度图像", dst);  
	cvWaitKey(100);
    cvThreshold(dst, dst, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);//OTSU二值化     
    IplConvKernel *element = cvCreateStructuringElementEx(5, 5, 0, 0, CV_SHAPE_ELLIPSE);  
    cvMorphologyEx(dst, dst, NULL, element, CV_MOP_OPEN);//开运算，去除比结构元素小的点     
    cvReleaseStructuringElement(&element);  
    cvNamedWindow("二值图像");  
    cvShowImage("二值图像", dst);  
	cvWaitKey(100);
    int w,h;  
    CvSize sz = cvGetSize(dst);  
  
    int color = 254; // 不对0计数,不可能为255,所以254     
    for (w = 0; w < sz.width; w++)    
    {    
        for (h = 0; h < sz.height; h++)    
        {    
            if (color > 0)    
            {    
                if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == 255)    
                {    
                    // 把连通域标记上颜色     
                    cvFloodFill(dst, cvPoint(w, h), CV_RGB(color, color, color));  
                    color--;  
                }    
            }    
        }    
    }    
    cvNamedWindow("标记颜色后的图像");  
    cvShowImage("标记颜色后的图像", dst);  
	cvWaitKey(100);
    int colorsum[255] = {0};  
    for (w=0; w<sz.width; w++)    
    {    
        for (h=0; h<sz.height; h++)    
        {    
            if (CV_IMAGE_ELEM(dst, unsigned char, h, w) > 0)    
            {    
                colorsum[CV_IMAGE_ELEM(dst, unsigned char, h, w)]++;//统计每种颜色的数量     
            }    
        }    
    }    
    std::vector<int> v1(colorsum, colorsum+255);//用数组初始化vector     
    //求出最多数量的染色，注意max_element的使用方法     
    int maxcolorsum = max_element(v1.begin(), v1.end()) - v1.begin();  
    printf("%d\n",maxcolorsum);  
  
    for (w=0; w<sz.width; w++)    
    {    
        for (h=0; h<sz.height; h++)    
        {    
            if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == maxcolorsum)    
            {    
                CV_IMAGE_ELEM(dst, unsigned char, h, w) = 255;  
            }    
            else    
            {    
                CV_IMAGE_ELEM(dst, unsigned char, h, w) = 0;  
            }    
        }    
    }    
    cvNamedWindow("最大连通域图");  
    cvShowImage("最大连通域图", dst);  
	cvWaitKey(100);  
    cvDestroyAllWindows();  
    return ;
}

bool FCvtVideoToImage(CString vFileNmae, vector<IplImage *>& vectorImage)
{
	CvCapture *vcapture = cvCreateFileCapture(vFileNmae);
	int nframe  = (int)cvGetCaptureProperty(vcapture, CV_CAP_PROP_FRAME_COUNT);
	if(nframe < 1)
	{
		std::cout<<"Reading Video Fail"<<endl;
		return false;
	}
	for(int i = 0; i < nframe; i++)
	{
		IplImage * tmpFrame = cvQueryFrame(vcapture);
		vectorImage.push_back(cvCloneImage(tmpFrame));//此处申请内存
	}
	cvReleaseCapture(&vcapture);
	return true;
}
bool ExtractHogFeature_flag(CString colorPath, CString LabelPath,int label, CString savePath)
{
	vector<IplImage*> vColor;
	fstream labelFile;
	labelFile.open(LabelPath,ios::in);
	if(!labelFile)
		return false;

	CvCapture *capture=cvCreateFileCapture(colorPath);
	
	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
	if(numFrames < 1)
		return false;
	//cout<<"numFrames:"<<numFrames<<endl;
	for(int i=0;i<numFrames;i++)
	{
		IplImage* tempFrame=cvQueryFrame(capture);
		//IplImage* tempFrameNew=cvCreateImage(cvSize(tempFrame->width,tempFrame->height),tempFrame->depth,tempFrame->nChannels);
		//cvSetImageROI(tempFrame,cvRect(25,20,tempFrame->width-45,tempFrame->height-40));
		//cvResize(tempFrame,tempFrameNew);
		//cvResetImageROI(tempFrame);
		//cvReleaseImage(&tempFrame);
		vColor.push_back(cvCloneImage(tempFrame));
	}

	cvReleaseCapture(&capture);

	fstream hogFile;
	hogFile.open(savePath,ios::out|ios::trunc);

	hogFile<<numFrames<<"\t"<<label<<endl;

	string temp;
	while(getline(labelFile,temp))
	{
		std::stringstream coord(temp);
		int num;
		coord>>num;
		while(!coord.eof())
		{
			int x1,y1,x2,y2, flag;
			coord>>x1>>y1>>x2>>y2>>flag;
			if(x1 >= x2 || y1 >= y2)
				continue;
			cvSetImageROI(vColor[num],cvRect(x1,y1,x2-x1+1,y2-y1+1));
			IplImage* handImage;
			handImage=cvCreateImage(cvSize(64,64),8,3);
			cvResize(vColor[num],handImage);
			cvResetImageROI(vColor[num]);
			IplImage* grayImage;
			grayImage=cvCreateImage(cvSize(64,64),8,1);
			cvCvtColor(handImage,grayImage,CV_RGB2GRAY);

			HOGDescriptor *hog=new HOGDescriptor(cvSize(64,64),cvSize(32,32),cvSize(16,16),cvSize(16,16),9); 

			Mat handMat(grayImage);

			vector<float> descriptors;

			hog->compute(handMat, descriptors,Size(0,0), Size(0,0));

			hogFile<<num<<"\t"<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<"\t"<<flag<<"\t";
			double total = 0;
			for(int i=0;i<descriptors.size();i++)
				hogFile<<descriptors[i]<<"\t";
			hogFile<<endl;

			delete hog;
			cvReleaseImage(&handImage);
			cvReleaseImage(&grayImage);
			descriptors.clear();

			//Mat src2(vColor[num]);
			//cv::rectangle(src2, cv::Rect(x1,y1,x2-x1+1,y2-y1+1), cv::Scalar(255, 0, 0), 3);
		}
	}

	labelFile.close();
	hogFile.close();
	int vsize = vColor.size();
	for(int i = vsize - 1; i >= 0; i--)
	{
		cvReleaseImage(&vColor[i]);
		vColor.pop_back();
	}
	vColor.clear();
	return true;
}
bool ExtractHogFeature(CString colorPath, CString LabelPath,int label, CString savePath)
{
	vector<IplImage*> vColor;
	fstream labelFile;
	labelFile.open(LabelPath,ios::in);
	if(!labelFile)
		return false;

	CvCapture *capture=cvCreateFileCapture(colorPath);
	
	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
	if(numFrames < 1)
		return false;
	//cout<<"numFrames:"<<numFrames<<endl;
	for(int i=0;i<numFrames;i++)
	{
		IplImage* tempFrame=cvQueryFrame(capture);
		//IplImage* tempFrameNew=cvCreateImage(cvSize(tempFrame->width,tempFrame->height),tempFrame->depth,tempFrame->nChannels);
		//cvSetImageROI(tempFrame,cvRect(25,20,tempFrame->width-45,tempFrame->height-40));
		//cvResize(tempFrame,tempFrameNew);
		//cvResetImageROI(tempFrame);
		//cvReleaseImage(&tempFrame);
		vColor.push_back(cvCloneImage(tempFrame));
	}

	cvReleaseCapture(&capture);

	fstream hogFile;
	hogFile.open(savePath,ios::out|ios::trunc);

	hogFile<<numFrames<<"\t"<<label<<endl;

	string temp;
	while(getline(labelFile,temp))
	{
		std::stringstream coord(temp);
		int num;
		coord>>num;
		while(!coord.eof())
		{
			int x1,y1,x2,y2,flag;
			coord>>x1>>y1>>x2>>y2>>flag;
			if(x1 >= x2 || y1 >= y2 || (x1+x2+y1+y2)==0 || flag==1)
				continue;
			cvSetImageROI(vColor[num],cvRect(x1,y1,x2-x1+1,y2-y1+1));
			IplImage* handImage;
			handImage=cvCreateImage(cvSize(64,64),8,3);
			cvResize(vColor[num],handImage);
			cvResetImageROI(vColor[num]);
			IplImage* grayImage;
			grayImage=cvCreateImage(cvSize(64,64),8,1);
			cvCvtColor(handImage,grayImage,CV_RGB2GRAY);

			HOGDescriptor *hog=new HOGDescriptor(cvSize(64,64),cvSize(32,32),cvSize(16,16),cvSize(16,16),9); 

			Mat handMat(grayImage);

			vector<float> descriptors;

			hog->compute(handMat, descriptors,Size(0,0), Size(0,0));

			hogFile<<num<<"\t"<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<"\t";
			double total = 0;
			for(int i=0;i<descriptors.size();i++)
				hogFile<<descriptors[i]<<"\t";
			hogFile<<endl;

			delete hog;
			cvReleaseImage(&handImage);
			cvReleaseImage(&grayImage);
			descriptors.clear();

			//Mat src2(vColor[num]);
			//cv::rectangle(src2, cv::Rect(x1,y1,x2-x1+1,y2-y1+1), cv::Scalar(255, 0, 0), 3);
		}
	}

	labelFile.close();
	hogFile.close();
	int vsize = vColor.size();
	for(int i = vsize - 1; i >= 0; i--)
	{
		cvReleaseImage(&vColor[i]);
		vColor.pop_back();
	}
	vColor.clear();
	return true;
}
//coordination map from RGB to depth video 

void fgenerateMap()
{
    double R1[3][3],R2[3][3],P1[3][4],P2[3][4];
	int width = 320;
	int height = 240;
    CvSize imageSize={width, height};
	//师姐计算出来的对齐参数
	double M1[3][3]={597.27, 0, 322.67, 0, 597.04, 232.64, 0, 0, 1};
    double M2[3][3]={529.45, 0, 319.35, 0, 530.51, 234.79, 0,0, 1};
        
    /*double D1[5]={3.71403933e-01, -1.29047451e+01, 0, 0, 1.56843994e+02};
    double D2[5]={4.39538926e-01, -1.70257187e+01, 0, 0, 2.23054718e+02};*/

	double D1[5]={0, 0, 0, 0, 0};
	double D2[5]={0, 0, 0, 0, 0};
    //double D2[5]={0.1884,-0.4865,0,-0.003,0.0007};

    double R[3][3]={0.9999,-0.0106,    0.0049,  0.0106,   0.9999,   0.0034, -0.0049,   -0.0034 ,  0.9999};
    double T[3]={25.0479, 0.2849, -2.0667};
    CvMat CvM1=cvMat(3,3,CV_64F,M1);
    CvMat CvM2=cvMat(3,3,CV_64F,M2);
    CvMat _D1=cvMat(1,5,CV_64F,D1);
    CvMat _D2=cvMat(1,5,CV_64F,D2);
    CvMat _R=cvMat(3,3,CV_64F,R);
    CvMat _T=cvMat(3,1,CV_64F,T);
    CvMat _R1=cvMat(3,3,CV_64F,R1);
    CvMat _R2=cvMat(3,3,CV_64F,R2);
    CvMat _P1=cvMat(3,4,CV_64F,P1);
    CvMat _P2=cvMat(3,4,CV_64F,P2);

    //imageSize=cvGetSize(bgrFrameL);
    cvStereoRectify(&CvM1,&CvM2,&_D1,&_D2,imageSize,&_R,&_T,&_R1,&_R2,&_P1,&_P2,0,0);

    mx1=cvCreateMat(imageSize.height,imageSize.width,CV_32F);
    my1=cvCreateMat(imageSize.height,imageSize.width,CV_32F);
    mx2=cvCreateMat(imageSize.height,imageSize.width,CV_32F);
    my2=cvCreateMat(imageSize.height,imageSize.width,CV_32F);
    cvInitUndistortRectifyMap(&CvM1,&_D1,&_R1,&_P1,mx1,my1);
    cvInitUndistortRectifyMap(&CvM2,&_D2,&_R2,&_P2,mx2,my2);
}

void RGB2DepthRectify(TwoDimension src, TwoDimension &dst)
{
	TwoDimension facemiddle;
	int height = 240;
	int width = 320;
	facemiddle.x = cvmGet(mx1, src.y, src.x);
	facemiddle.y = cvmGet(my1, src.y, src.x);
	//cout<<"hello\n";
	//cout<<src.x<<" "<<cvmGet(mx1, src.y, src.x)<<endl;
	int aroundWidth = 30;
	int beginx = max(0, facemiddle.x - aroundWidth);
	int endx = min(width, facemiddle.x + aroundWidth);
	int beginy = max(0, facemiddle.y - aroundWidth);
	int endy = min(height, facemiddle.y + aroundWidth);
	double mindistance = 99999999999;
	//from depth to 
	for(int i = beginy; i < endy; i++)
	{
		for(int j = beginx; j < endx; j++)
		{
			TwoDimension tmp;
			tmp.x = cvmGet(mx2, i, j);
			tmp.y = cvmGet(my2, i, j);
			double tmpdis = getTwoDimDis(tmp, facemiddle);
			if(mindistance > tmpdis)
			{
				mindistance = tmpdis;
				dst.x = j;
				dst.y = i;
			}
		}
	}
}
void Depth2RGBRectify(TwoDimension src, TwoDimension &dst)
{
	TwoDimension facemiddle;
	int height = 240;
	int width = 320;
	facemiddle.x = cvmGet(mx2, src.y, src.x);
	facemiddle.y = cvmGet(my2, src.y, src.x);
	//cout<<"hello\n";
	//cout<<src.x<<" "<<cvmGet(mx1, src.y, src.x)<<endl;
	int aroundWidth = 30;
	int beginx = max(0, facemiddle.x - aroundWidth);
	int endx = min(width, facemiddle.x + aroundWidth);
	int beginy = max(0, facemiddle.y - aroundWidth);
	int endy = min(height, facemiddle.y + aroundWidth);
	double mindistance = 99999999999;
	//from depth to determination
	for(int i = beginy; i < endy; i++)
	{
		for(int j = beginx; j < endx; j++)
		{
			TwoDimension tmp;
			tmp.x = cvmGet(mx1, i, j);
			tmp.y = cvmGet(my1, i, j);
			double tmpdis = getTwoDimDis(tmp, facemiddle);
			if(mindistance > tmpdis)
			{
				mindistance = tmpdis;
				dst.x = j;
				dst.y = i;
			}
		}
	}
}
void cvtRGBHandLabel2Depth(CString handLabelFilePath, CString LabelSavePath)
{

	fstream filestream;
	ofstream writestream;

	map <int, frameHandPosition> maphand;
	frameHandPosition handposition;
	vector<IplImage*> vColor;//vColor 存储RGB video
	char buffer[256];

	filestream.open(handLabelFilePath, ios::in);
	writestream.open(LabelSavePath, ios::out|ios::trunc);//write
	if(!filestream)
	{
		std::cout<<handLabelFilePath<<" does not exists"<<endl;

	}
	//read handLabelFilePath
	while(filestream.getline(buffer, 256))
	{
		int bufferlen = strlen(buffer);
		if(bufferlen <= 20)//detect one hand
		{
			handposition.isDetectOneHand = true;
			sscanf(buffer, "%04d %03d %03d %03d %03d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y);
			TwoDimension tmp;
			RGB2DepthRectify(handposition.handA1, handposition.handA1);
			RGB2DepthRectify(handposition.handA2, handposition.handA2);
			char outcontent[200];
			sprintf(outcontent, "%04d %03d %03d %03d %03d\n", handposition.nframe, handposition.handA1.x, handposition.handA1.y, handposition.handA2.x, handposition.handA2.y);
			//writestream<<handposition.nframe<<" "<<handposition.handA1.x<<" "<<handposition.handA1.y<<" "<<handposition.handA2.x<<" "<<handposition.handA2.y<<endl;
			writestream<<outcontent;
			handposition.handB1.x = 0;
			handposition.handB1.y = 0;
			handposition.handB2.x = 0;
			handposition.handB2.y = 0;
		}
		else//detect two hands
		{
			handposition.isDetectOneHand = false;
			sscanf(buffer, "%04d %03d %03d %03d %03d %03d %03d %03d %03d\n", &handposition.nframe, &handposition.handA1.x, &handposition.handA1.y, &handposition.handA2.x, &handposition.handA2.y,
				&handposition.handB1.x, &handposition.handB1.y, &handposition.handB2.x, &handposition.handB2.y);
			RGB2DepthRectify(handposition.handA1, handposition.handA1);
			RGB2DepthRectify(handposition.handA2, handposition.handA2);
			RGB2DepthRectify(handposition.handB1, handposition.handB1);
			RGB2DepthRectify(handposition.handB2, handposition.handB2);
			char outcontent[200];
			sprintf(outcontent, "%04d %03d %03d %03d %03d %03d %03d %03d %03d\n", handposition.nframe, handposition.handA1.x, handposition.handA1.y, handposition.handA2.x, handposition.handA2.y, handposition.handB1.x, handposition.handB1.y, handposition.handB2.x, handposition.handB2.y);
			writestream<<outcontent;
			//writestream<<handposition.nframe<<" "<<handposition.handA1.x<<" "<<handposition.handA1.y<<" "<<handposition.handA2.x<<" "<<handposition.handA2.y<<" "<<handposition.handB1.x<<" "<<handposition.handB1.y<<" "<<handposition.handB2.x<<" "<<handposition.handB2.y<<endl;
		}
		maphand[handposition.nframe] = handposition;
	}
	filestream.close();
	writestream.close();
}

int getCurrentDir(string path) 
{ 
	vector<string> files;
	//文件句柄 
	long  hFile  =  0; 
	//文件信息 
	struct _finddata_t fileinfo; 
	string p; 
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1) 
	{ 
		do
		{  
			if((fileinfo.attrib & _A_SUBDIR)) 
			{ 
				if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0) 
				{
					files.push_back(fileinfo.name);
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
				}

			}  
		}while(_findnext(hFile, &fileinfo) == 0); 
		_findclose(hFile); 
	} 
	return files.size();
} 

void getDefinedFiles(string path, vector<string>& results)
{
	HANDLE hFile;
	LPCTSTR lpFileName = path.c_str();
	WIN32_FIND_DATA pNextInfo;	//搜索得到的文件信息将储存在pNextInfo中;
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是pNextInfo;
	if(hFile == INVALID_HANDLE_VALUE)
	{
		//搜索失败
		exit(-1);
	}
	else
	{
		do 
		{
			if(pNextInfo.cFileName[0] == '.')//过滤.和..
				continue;
			results.push_back(pNextInfo.cFileName);
		} while(FindNextFile(hFile, &pNextInfo));
	}
}

void getCurrentDir(string path, vector<string>& files) 
{ 
	//文件句柄 
	long  hFile  =  0; 
	//文件信息 
	struct _finddata_t fileinfo; 
	string p; 
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1) 
	{ 
		do
		{  
			if((fileinfo.attrib & _A_SUBDIR)) 
			{ 
				if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0) 
				{
					files.push_back(fileinfo.name);
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
				}

			}  
		}while(_findnext(hFile, &fileinfo) == 0); 
		_findclose(hFile); 
	} 
} 