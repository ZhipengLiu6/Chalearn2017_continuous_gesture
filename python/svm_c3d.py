#!/usr/bin/env python
# -*- coding: utf8 -*-
import scipy.io as scio
import os, sys
import pdb
import numpy as np
import time
# from svmutil import *
# from svm import *
from sklearn import svm, datasets
from sklearn.externals import joblib
import matplotlib.pyplot as plt
import h5py
import pickle
import pprint
import re
import argparse
import subprocess
from generate_ConG_final_sub import Generate_final_submission_file
"""
function: test the svm model, generate and save the accuracy
@x: feature, numpy, shape: (nsamples, nfeature)
@y: label, numpy, shape: (nsample): groundtruth
@modelPath: the saved linear model path
@saveResultPath: the pickle path, which is used to save the accuracy, prediction and  groundtruth result
@para: the trained parameter
"""
def test_has_gr(x = None, y = None, modelPath = None, saveResultPath = None, para = None):
    yg = y.squeeze()
    lin_svc = joblib.load(modelPath)
    predict = lin_svc.predict(x)
    cnt_correct = 0
    cnt_all = yg.shape[0]
    for i in range(cnt_all):
        if yg[i] == predict[i]:
            # print("i = %d, ground = %d, predict = %d" % (i, yg[i], predict[i]))
            cnt_correct = cnt_correct + 1

    acc = float(cnt_correct) / cnt_all
    
    print('accuracy: ', acc)
    # pdb.set_trace()
    result = {1 : acc,
                    2 : predict,
                    3 : yg,
                    4 : para,
    }
    # pdb.set_trace()
    saveStream = open(saveResultPath, 'wb')
    pickle.dump(result, saveStream)
    saveStream.close()
    return predict
"""
function: test the svm model, generate prediction result
@x: feature, numpy, shape: (nsamples, nfeature)
@modelPath: the saved linear model path
@saveResultPath: the pickle path, which is used to save the prediction.
@para: the trained parameter
"""
def test_no_gr(x = None, modelPath = None, saveResultPath = None, para = None):
    lin_svc = joblib.load(modelPath)
    predict = lin_svc.predict(x)
    result = {2 : predict,
                    4 : para,
    }
    # pdb.set_trace()
    saveStream = open(saveResultPath, 'wb')
    pickle.dump(result, saveStream)
    saveStream.close()
    return predict
"""
function: train and save the linear svm model
@x: feature, numpy, shape: (nsamples, nfeature)
@y: label, numpy, shape: (nsample)
@para: svm hyper-parameter
@return: none
"""
def train(x = None, y = None, saveModelPath = 'iso_training_plinear_svm_model_max_iter_2w.m', para = None):
    # pdb.set_trace()
    y = y.squeeze()
    ## set hyper-parameter
    print("begin training")
    C = para['C']
    dual = para['dual']
    class_weight = para['class_weight']
    max_iter = para['max_iter']

    lin_svc = svm.LinearSVC(C = C, dual = dual, class_weight = class_weight).fit(x, y)
    # pdb.set_trace()
    joblib.dump(lin_svc, saveModelPath)
    print("training done!")
"""
function: generate submission prediction file as the format of https://competitions.codalab.org/competitions/16491#learn_the_details-evaluation
@videoid: the id of predicted video: (1, nsample)
@predict: prediction result
@mysubfile: the file of submission
@GivenVideolistfile: the official given test video list file
@isTest: 1 : generate validation submission file; 2 : generate testing submission file 
"""
def checkPath(path = None):
    if not os.path.exists(path):
        strcmd = "mkdir -p " + path
        subprocess.call(strcmd, shell = True)   
if __name__ == '__main__':
    ##set data path
    parser = argparse.ArgumentParser()
    parser.add_argument('--trainfilepath', default='../read_and_fuse_feature_ubuntu_matlab/fusion_train.mat', type=str, help = 'the path to fusion c3d training feature file')
    parser.add_argument('--testfilepath', default='../read_and_fuse_feature_ubuntu_matlab/fusion_test.mat', type=str, help = 'the path to fusion c3d testing feature file')
    parser.add_argument('--validSeginforPath', default='data/seg_valid_rgb_2stream/ConGTestSegInfo', type=str, help = 'the path to validation segmentation information file')
    parser.add_argument('--submissionFile', default='submission/valid_prediction.txt', type=str, help = 'the path to submission file')
    parser.add_argument('--GivenVideolistfile', default='data/valid.txt', type=str, help = 'the path to given submission video list file')
    parser.add_argument('--isTrain', default=1, type=int, help = 'whether to run the training svm code(1: run, 0: do not run)')
    parser.add_argument('--isTest', default=1, type=int, help = 'whether to run the testing svm code and generate submission file(1: run, 0: do not run)')
    parser.add_argument('--saveModelPath', default = "../svm_model/iso_training_linear_svm_model_con_depth_map_only_hand_rgb_only_hand_face_2streams_iter_1k_c_Efu1.m", type = str, help = 'the path to saved svm model')
    parser.add_argument('--videoLenFile', default='valid_video_Length.txt', type=str)
    args = parser.parse_args()

    trainfilepath = args.trainfilepath
    testfilepath = args.testfilepath
    validSeginforPath = args.validSeginforPath
    submissionFile = args.submissionFile
    GivenVideolistfile = args.GivenVideolistfile
    videoLenFile = args.videoLenFile
    isTrain = args.isTrain
    isTest = args.isTest
    strpara = "iso_depth_map_only_hand_rgb_only_hand_face_2streams_iter_1k_c_Efu1"
    print("experiment type:", strpara)
    saveModelPath = args.saveModelPath
    saveTestResult = "valid_videoid_predict.h5"

    saveResultPath = 'svm_result'
    checkPath(saveResultPath)
    checkPath("submission")
    saveResultPath = saveResultPath + "/" + strpara + '.pkl'
    ## set hyper-parameter
    para = {}
    para['C'] = 0.1
    para['dual'] = False #dual=False when n_samples > n_features.
    para['class_weight'] = 'balanced'
    para['max_iter'] = 1000
    ##-------------------------------    
    pprint.pprint(para)
    ##load data
    #------------------------------load data---------------------------------
    # trainmat = scio.loadmat(trainfilepath) #for v7.0 mat
    trainmat = h5py.File(trainfilepath)
    train_x = trainmat['trainfeature'][:]
    train_y = trainmat['trainlabel'][:]
    train_x = train_x.transpose(1, 0)
    print("training x shape:", train_x.shape)
    print("training y shape:", train_y.shape)

    timestart = time.time()
    if isTrain == 1:

        # pdb.set_trace()
        train(x = train_x, y = train_y, saveModelPath = saveModelPath, para = para)
    #-----------------------------------------------------------------------------
    if isTest == 1:
        validationmat = h5py.File(testfilepath)
        # validationmat = scio.loadmat(testfilepath)
        validation_x = validationmat['validationfeature'][:]
        validation_y = validationmat['validationlabel'][:]
        videoid = validationmat['validationVideoid'][:]

        validation_x = validation_x.transpose(1, 0)
        print("testing x shape:", validation_x.shape)
        print("testing y shape:", validation_y.shape)
        print("testing the training data:")
        test_has_gr(x = train_x, y = train_y, modelPath = saveModelPath, saveResultPath = saveResultPath, para = para)
        print("testing the testing data:")
        predict = test_no_gr(x = validation_x, modelPath = saveModelPath, saveResultPath = saveResultPath, para = para)
        h5stream = h5py.File(saveTestResult, 'w')
        h5stream['xtestVideoId'] = videoid.squeeze()
        # print(videoid[200: 400])
        h5stream['result_class'] = predict.squeeze()
        h5stream.close()

        Generate_final_submission_file(resulth5filename = saveTestResult, seginfopath = validSeginforPath, myConGsubfile = submissionFile, testvideolistfile = GivenVideolistfile, videoLenFile = videoLenFile)
    timeend = time.time()
    print("using time: ", timeend - timestart)