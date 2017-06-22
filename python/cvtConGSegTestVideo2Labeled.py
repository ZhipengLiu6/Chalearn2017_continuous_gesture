"""
function:The code can transfer origianl segmented continous video or hand detection to labeled file
"""
"""
author:zhipeng.liu
data:2017-6-22
"""
import os
import subprocess
from GetSubfileName import *
import pdb

def cvtConGSegTestVideo2Labeled(segVideoFile = None, labeledFile = None, isVideo = 2):
    if not os.path.exists(labeledFile):
        strcmd = "mkdir -p " + labeledFile
        subprocess.call(strcmd, shell = True)
    labelnameSet = GetSubfileName(segVideoFile)
    for labelname in labelnameSet:
        print(labelname)
        oneLabelFile = segVideoFile + "/" + labelname
        oneSaveLabeledFile = labeledFile + "/" + labelname
        if not os.path.exists(oneSaveLabeledFile):
            os.mkdir(oneSaveLabeledFile)
        oriVideoSet = GetSubfileName(oneLabelFile)
        for oneBigVideo in oriVideoSet:
            oneBigVideoPath = oneLabelFile + "/" + oneBigVideo
            smallVideoSet = GetSubfileName(oneBigVideoPath)
            for oneSamllVideo in smallVideoSet:
                oneSmallVideoPath = oneBigVideoPath + "/" + oneSamllVideo
                videoid = (int(oneBigVideo) ) * 100+ int(oneSamllVideo[: 3])
                if isVideo == 2:
                    newVideoName = "%012d.txt" % (videoid)
                elif isVideo == 1:
                    newVideoName = "%012d.avi" % (videoid)
                dstPath = oneSaveLabeledFile + "/" + newVideoName
                strcp = "mv " + oneSmallVideoPath + " " + dstPath
                subprocess.call(strcp, shell = True)

if __name__ == '__main__':
    segVideoFile = "/media/zhipengliu/zhipeng_NHCI/ChaLearn2017/conG/2017/seg_valid_handDetection_2stream"
    labeledFile = "/home/zhipengliu/ChaLearn2017/ConG/validation/video/labeld_handDetectionSegValid"
    cvtConGSegTestVideo2Labeled(segVideoFile = segVideoFile, labeledFile = labeledFile, isVideo = 2)