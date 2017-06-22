# -*- coding: cp936 -*-
#funtion:read ChaLearn dataset
#author@zhipeng.liu
#date:2016.7.29
#input:Txtpath
#output:list:data and label
import sys,os
import scipy.io as sio
import matplotlib.pyplot as plt
from numpy import *
from GetSubfileName import *
from scipy.linalg.misc import norm
import scipy.io as sio
from readHandHog import *
import h5py
import re
from math import *
def IsInlist(onelist, key):
	for element in onelist:
		if element == key:
			return 1
	return 0
def Generate_submission_file(Isovideolistpath, validresulth5filename, seginfopath, SubFile, validtxtpath, GroundTruethFile):
	r = re.compile('[ \t\n\r:,/]+')
	h5stream = h5py.File(validresulth5filename, "r")
	result = h5stream['result_class'][ : ]	##array
	h5stream.close()

	isolisttream = open(Isovideolistpath, 'r')
	isovideoalllines = isolisttream.readlines()
	isolisttream.close()
	isonumlines = len(isovideoalllines)
	isolistarray = np.zeros((isonumlines))
	isolistfileid = []
	oneContinuousVideoLabel = []
	dic = {-1: -1}	#fileid to lines
	for i in range(isonumlines):
		tmp = isovideoalllines[i]
		videoname = int(tmp[0:5])
		isolistfileid.append(videoname)
		isolistarray[i] = int(tmp[10:])
		dic[videoname] = i
		oneContinuousVideoLabel.append([])
		#print videoname, isolistarray[i]

	countsample = 0
	for i in range(isonumlines):
		numContinuouscvtIso = int(isolistarray[i])
		for j in range(numContinuouscvtIso):
			tmp = result[countsample] + 1
			countsample = countsample + 1
			#print 'label = ', tmp
			oneContinuousVideoLabel[i].append(tmp)
			
	
	valid_liststream = open(validtxtpath, 'r')
	validvideoLabel = []
	validvideoDevel = []
	validlabestr = []
	validallines = valid_liststream.readlines()
	valid_liststream.close()
	validnumline = len(validallines)
	label = -1
	listvideoid = []
	for i in range(validnumline):
		oneline = validallines[i]
		linespilt = r.split(oneline)
		validvideoDevel.append(int(linespilt[0]))
		validvideoLabel.append([])

		videoid = int(linespilt[1])
		listvideoid.append(videoid)
		oneisosefinfo = seginfopath + '/' + linespilt[0] + '/' + linespilt[1] + ".txt"
		oneseginfo_stream = open(oneisosefinfo, 'r')
		alllines = oneseginfo_stream.readlines()
		oneseginfo_stream.close()
		numline = len(alllines)	
		startbefor = 0;
		endbefor = 0;
		strtmp = ""
		if not dic.has_key(videoid):
			print "No file!"
			label = 1
			validvideoLabel[i].append(label)
			tmp = " 1,10:1"
			validlabestr.append(tmp)
			strtmp = " 1,10:1"
		else:
			#print dic
			lineid = dic[videoid]
			numlabel = len(oneContinuousVideoLabel[lineid])
			startlist = []
			endlist = []
			for j in range(numlabel):
				validvideoLabel[i].append(oneContinuousVideoLabel[lineid][j])
				start = 0
				end = 1
				if j < numline:
					seginfostr = alllines[j]
					seginforsplit = r.split(seginfostr)

					start = int(seginforsplit[0])
					end = int(seginforsplit[1])
					startlist.append(start)
					endlist.append(end)
			if numlabel == 1:
				label = validvideoLabel[i][0]
				start = startlist[0]
				end =  endlist[0]	
				strtmp = strtmp + " %d,%d:%d" % (start, end, label)			
			else:
				for j in range(numlabel):
					label = validvideoLabel[i][j]
					if j == 0 :
						start = startlist[j]
						end = int((endlist[0] + startlist[1]) / 2) - 1	
					elif j == numlabel - 1:
						start = int((endlist[j - 1] + startlist[j]) / 2)
						end  = endlist[j]
					else:
						start = int((endlist[j - 1] + startlist[j]) / 2)
						end = int((endlist[j] + startlist[j + 1]) / 2) - 1
					strtmp = strtmp + " %d,%d:%d" % (start, end, label)
		#print 'str1,:' , strtmp
		validlabestr.append(strtmp)
	##generate sub file
	subfilestream = open(SubFile, 'w')
	for i in range(validnumline):
		strtmp = "%03d/%05d" % (validvideoDevel[i], listvideoid[i])  + validlabestr[i] + "\n"
		#print 'str2:', strtmp
		subfilestream.write(strtmp)
	subfilestream.close()
	##compute acc
	groundstream = open(GroundTruethFile, 'r')
	alllines = groundstream.readlines()
	numline = len(alllines)
	groundTruthlable = []
	for i in range(numline):
		groundTruthlable.append([])
		oneline = alllines[i]
		onelinesplit = r.split(oneline)
		numsplit = len(onelinesplit)
		for j in range(4, numsplit, 3):
			oneIsolabel = int(onelinesplit[j])
			print "label=", oneIsolabel
			groundTruthlable[i].append(oneIsolabel)
	acc = 0
	for i in range(numline):
		numiso = len(groundTruthlable[i])
		count = 0
		for j in range(numiso):
			isotruelabel = groundTruthlable[i][j]
			if IsInlist(validvideoLabel[i], isotruelabel):
				count = count + 1
		print float(count) / numiso, count, numiso
		acc = acc + float(count) / numiso
	acc = acc / numline
	return acc
class predictionLabel(object):
	"""one iso prediction label in continou label, include label, begin of frames, end of frames"""
	def __init__(self, label, begin, end):
		super(predictionLabel, self).__init__()
		self.label = label
		self.begin = begin
		self.end = end
		
def computer_conACC(GroundTruethFile, Subfile):
	##compute acc
	r = re.compile('[ \t\n\r:,/]+')
	groundstream = open(GroundTruethFile, 'r')
	alllines = groundstream.readlines()
	numline = len(alllines)
	groundTruthlable = []
	groundvideoid = []
	gdic = {-1 : -1} #videoid to lineid
	# predictionLabel labelTemplate(0, 0, 0)
	
	for i in range(numline):
		groundTruthlable.append([])
		oneline = alllines[i]
		onelinesplit = r.split(oneline)
		numsplit = len(onelinesplit)
		videoid = int(onelinesplit[1])
		groundvideoid.append(videoid)
		gdic[videoid] = i
		for j in range(4, numsplit, 3):
			oneIsolabel = int(onelinesplit[j])
			ishave = False
			for previousLabel in groundTruthlable[i]:
				if oneIsolabel == previousLabel:
					ishave = True
					break
			if not ishave:
				groundTruthlable[i].append(oneIsolabel)

	substream = open(Subfile, 'r')
	suballlines = substream.readlines()
	sublabel = []
	subdic = {-1 : -1}
	subnumline = len(suballlines)
	for i in range(subnumline):
		sublabel.append([])
		oneline = suballlines[i]
		onelinesplit = r.split(oneline)
		numsplit = len(onelinesplit)
		videoid = int(onelinesplit[1])
		#print "videoid", videoid
		subdic[videoid] = i
		for j in range(4, numsplit, 3):
			oneIsolabel = int(onelinesplit[j])
			ishave = False
			for previousLabel in sublabel[i]:
				if oneIsolabel == previousLabel:
					ishave = True
					break
			if not ishave:
				sublabel[i].append(oneIsolabel)
	acc = 0
	for i in range(numline):
		numgiso = len(groundTruthlable[i])
		numpiso = len(sublabel[i])
		videoid = groundvideoid[i]
		if subdic.has_key(videoid):
			sublineid = subdic[videoid]
			subvideolabel = sublabel[sublineid]
			count = 0
			for j in range(numgiso):
				isotruelabel = groundTruthlable[i][j]
				if IsInlist(subvideolabel, isotruelabel):
					count = count + 1
			#print float(count) / numiso, count, numiso

			acc = acc + float(count) / (numgiso)
			print("line :", i)
			print("numgiso: %d, numpsio: %d, count: %d, len: %d, one acc: %f" % (numgiso, numpiso, count,  (numgiso), float(count) / (numgiso)))
	print "acc", acc, numline
	acc = acc / numline	
	return acc

if __name__ == '__main__':
	GroundTruethFile = '/home/zhipengliu/ChaLearn2017/ConG/submission/valid_groundtruth.txt'
	mysubfile = '/home/zhipengliu/ChaLearn2017/ConG/submission/valid_prediction_v1.txt'
	print computer_conACC(GroundTruethFile, mysubfile)



	