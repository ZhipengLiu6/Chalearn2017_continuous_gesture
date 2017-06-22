import re
import sys,os
import scipy.io as sio
import matplotlib.pyplot as plt
import numpy as np
from numpy import *
def readFaceFileyin(filepath = None):
	if os.path.exists(filepath) == False:
		print filepath + " dose not exists!"
		return 0, 0, 0
	filestream = open(filepath)
	content = np.loadtxt(filestream)
	idFrame = content[:, 0]
	allfaceX = (content[:, 2] + content[:, 4]) / 2
	allfaceY = (content[:, 3] + content[:, 5]) / 2
	print "content.shape", content.shape
	return idFrame, allfaceY, allfaceY
def readTEFaceFileliu(filepath = None):
	if os.path.exists(filepath) == False:
		print filepath + " dose not exists!"
		return 0, 0, 0
	filestream = open(filepath)
	content = np.loadtxt(filestream)
	idFrame = content[:, 0]
	allfaceX = content[:, 2]
	allfaceY = content[:, 3]
	print "content.shape", content.shape
	return idFrame, allfaceY, allfaceY	
def readHOG(filepath = None):
	if(os.path.exists(filepath) == False):
				print filepath + " does not exists!"
				return 0,0,0,0,0
	filestream = open(filepath, 'r')
	title = filestream.readline()
	if not title:
		print "empty file!"
		return  0,0,0,0,0
	r = re.compile('[ \t\n\r:]+')
	titleSplit = r.split(title)
	label = int(titleSplit[1])
	numframe = int(titleSplit[0])
	content = np.loadtxt(filestream)
	listshape = list(content.shape)
	#print "listshape:",listshape
	if len(listshape) == 1:
		print "one line!"
		return 0,0,0,0,0
	handframe = content[:, 0]
	handposition = content[:, 1 : 5]
	handhog = content[:, 5:]
	return numframe, label, handframe, handposition, handhog
def v3_readHOG(filepath = None):
	if(os.path.exists(filepath) == False):
				print filepath + " does not exists!"
				return 0,0,0,0,0,0	
	filestream = open(filepath, 'r')
	title = filestream.readline()
	if not title:
		print "empty file!"
		return  0,0,0,0,0,0
	r = re.compile('[ \t\n\r:]+')
	titleSplit = r.split(title)
	label = int(titleSplit[1])
	numframe = int(titleSplit[0])
	content = np.loadtxt(filestream)
	listshape = list(content.shape)
	#print "listshape:",listshape
	if len(listshape) == 1:
		print "one line!"
		return 0,0,0,0,0,0
	handframe = content[:, 0]
	handposition = content[:, 1 : 5]
	detect_flag = content[:, 5]
	handhog = content[:, 6:]
	return numframe, label, handframe, handposition, detect_flag, handhog
##it can index the corresponding hand position acording to the number of frame
def readHandPosition(filepath = None):
	#filepath = '/media/zhipengliu/backupNHCI/zhipengliu/dataset/convertContinousToIsoGestrueTrain_includehand/001/Label_0001.M.txt'
	if(os.path.exists(filepath) == False):
				print filepath + " does not exists!"
				return None			
	filestream = open(filepath)
	r = re.compile('[ \t\n\r:]+')
	nd = {-1 : -1}
	handpostion = []
	count = 0;
	while 1:
		lines = filestream.readlines(10000)
		if not lines:
			break;
		for oneline in lines:
			onelinespilt = r.split(oneline)
			arr = np.zeros(8)
			nframe = int(onelinespilt[0])
			nd[nframe] = count
			count = count + 1
			if len(onelinespilt) == 6:
				for i in range(1, 5):
					arr[i] = int(onelinespilt[i])
				handposition.append(arr)
			else:
				for i in range(1, 9):
					arr[i] = int(onelinespilt[i])
				handposition.append(arr)
	return nd, handpostion

def readFaceFileliu(filepath):
	if(os.path.exists(filepath) == False):
				print filepath + " does not exists!"
				return None		
	filestream = open(filepath)
	r = re.compile('[ \t\n\r:]+')
	oneline = filestream.readline()
	faceX = 0
	faceY = 0
	if not oneline:
		return faceX, faceY
	else:
		onelinespilt = r.split(oneline)
		faceX = int(onelinespilt[0])
		faceY = int(onelinespilt[1])
		return faceX, faceY




"""
filepath = '/home/zhipengliu/dataset/Continue/HOG_001_001.txt'
numframe, label, handframe, handposition, handhog = readHOG(filepath)
print numframe, label
print handframe.shape
print handposition.shape
print handhog.shape
"""
if __name__ == '__main__':

	filepath = '/media/zhipengliu/zhipeng/research/competetion/IsoGesture/version2/Iso_TrainValidOnly HOGVersion2/Train/HOG_01426.txt'
	numframe, label, handframe, handposition, handhog = readHOG(filepath)
	print numframe