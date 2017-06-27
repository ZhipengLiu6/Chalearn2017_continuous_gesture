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
import argparse
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
                strcp = "cp " + oneSmallVideoPath + " " + oneSaveLabeledFile
                strmv = "mv " + oneSaveLabeledFile + "/"  + oneSamllVideo  + " " + dstPath
                subprocess.call(strcp, shell = True)
                subprocess.call(strmv, shell = True)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--segVideoFile', default='data/seg_valid_depth_2stream', type=str, help = 'the path to temporal segmentation file')
    parser.add_argument('--labeledFile', default='data/ConGD_phase_2_aligned_depth/temporal_segmentation_valid', type=str, help = 'the path to saved file')
    parser.add_argument('--isVideo', default=1, type=int, help = 'the type of data, 1: video, 2: txt file')
    args = parser.parse_args()
    segVideoFile = args.segVideoFile
    labeledFile = args.labeledFile
    cvtConGSegTestVideo2Labeled(segVideoFile = segVideoFile, labeledFile = labeledFile, isVideo = args.isVideo)