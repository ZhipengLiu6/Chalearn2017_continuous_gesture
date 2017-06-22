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


void getCurrentDir(string path, vector<string>& files);
void getDefinedFiles(string path, vector<string>& results);

void string_replace(string&s1,const string&s2,const string&s3)
{
	string::size_type pos=0;
	string::size_type a=s2.size();
	string::size_type b=s3.size();
	while((pos=s1.find(s2,pos))!=string::npos)
	{
		s1.replace(pos,a,s3);
		pos+=b;
	}
}

int main(int argc, char* argv)
{
	PreProcessGestureData PreProcessFace;
	string root_path = ".\\sample\\ConGD_Phase_1";   // modify the root path
	string outputPath = ".\\sample\\FacePosition";
	_mkdir(outputPath.c_str());
	
	vector<string> datasets;
	getCurrentDir(root_path, datasets);
	for (int i=0; i<datasets.size(); i++)
	{
		cout<<"Creating folder "<<datasets[i]<<" in FacePosition ..."<<endl;
		_mkdir((outputPath+"\\"+datasets[i]).c_str());
		vector<string> subfolders;
		getCurrentDir(root_path+"\\"+datasets[i], subfolders);
		for (int j=0; j<subfolders.size(); j++)
		{
			cout<<"Creating folder "<<subfolders[j]<<" in FacePosition/"<<datasets[i]<<" ..."<<endl;
			_mkdir((outputPath+"\\"+datasets[i]+"\\"+subfolders[j]).c_str());
			vector<string> videos;
			getDefinedFiles(root_path+"\\"+datasets[i]+"\\"+subfolders[j]+"\\*M*.avi", videos);
			for (int k = 0; k < videos.size(); k++)
			{
				string rgb_video_path = root_path+"\\"+datasets[i]+"\\"+subfolders[j]+"\\"+videos[k];
				string depth_video_name = videos[k];
				string head_out_name = videos[k].substr(0, 4)+".txt";
				string_replace(depth_video_name, "M", "K");
				string depth_video_path =  root_path+"\\"+datasets[i]+"\\"+subfolders[j]+"\\"+depth_video_name;
				string head_out_path = outputPath+"\\"+datasets[i]+"\\"+subfolders[j]+"\\"+head_out_name;
				fstream filestream;
				filestream.open(head_out_path, ios::in);
				if(filestream)
				{
					filestream.close();
					continue;
				}
				cout<<"RGB="<<datasets[i]+"\\"+subfolders[j]+"\\"+videos[k]<<endl;
				CvCapture *capture = cvCreateFileCapture(rgb_video_path.c_str());
				int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT); 
				if(numFrames < 1)
				{
					cout<<"video file's path is wrong and can be read"<<endl;
					continue;
				}		
				for(int i = 0; i < numFrames; i++)
				{
					IplImage* tempFrame = cvQueryFrame(capture);
					int countframe = i + 1;

					if(PreProcessFace.OutheadDetectionVIPLSDK(tempFrame, head_out_path.c_str(), countframe))
						break;
					if(i == numFrames - 1)
					{
						PreProcessFace.readVideo(rgb_video_path.c_str(), depth_video_path.c_str());
						PreProcessFace.getFacePositionHist(head_out_path.c_str());
						PreProcessFace.ReleaseVector();
					}

				}
				filestream.close();
				PreProcessFace.ReleaseVector();
				cvReleaseCapture(&capture);
			}
		}
	}
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