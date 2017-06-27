# Chalearn2017_continuous_gesture

This is an implementaion of our method about ChaLearn LAP Large-scale Continuous Gesture Recognition Challenge (Round 2) @ICCV 2017

## Notes: 

This code was tested on Windows10 OS with VS2012 and Ubuntu 14.04 OS with Python 2.7, caffe-C3D, sklearn, faster-rcnn.
Please double check the paths in code before your run it.

## Training:

### Step1: Convert continuous gesture to isolated gesture in order to train conveniently
```
(1): set the data_path in convertConGTrain2IsoGTrain_windows_matlab/ConvertConVedioToIsoGesture.m as your own path 
(2): run convertConGTrain2IsoGTrain_windows_matlab/ConvertConVedioToIsoGesture.m
```
Then there will be a new file named convertContinousToIsoGestrueTrain, which contain all converted training dataset.

### Step2: Get face and hand detetion about continuous training dataset.

Face and hand postion can be detected by using the codes in the detection folder. To use these codes, you can refer to Detection/Detection-Pipeline-Cons.pdf. 

### Step3: Preprocess the training dataset and get the input data and setting files needed by Step4(for c3d finetune)

Dependencies: cv2, numpy, ffmpeg.

All input data for step2 are lied in python/data.  The files in python/data should look like as follows:
```
ConGD_phase_1_RGB/convertContinousToIsoGestrueTrain_new/
    --001/
    ...
ConGD_phase_1_aligned_depth/convertContinousToIsoGestrueTrain_new/
    --001/
    ...
ConGD_phase_2_RGB/
ConGD_phase_2_aligned_depth/
ConGfacePosition/
    --RGBConGTestDataFacePosition/
    --RGBConGTrainDataFacePosition/
    --RGBConGValidDataFacePosition/
handDetection/
    --convertContinousToIsoGestrueTrain_new/
seg_valid_depth_2stream/
    --001/
    ...
seg_valid_handDetection_2stream/
    --001/
    ...
seg_valid_rgb_2stream/
    --ConGTestSegInfo/
        --001/
        ...
    cvtConGTestToIso/
        --001/
        ...
test.txt
valid.txt
```
You should place all the 249 converted RGB and aligned depth subfolders(001,002,...) of training set in ConGD_phase_1_RGB/convertContinousToIsoGestrueTrain_new/ and ConGD_phase_1_aligned_depth/convertContinousToIsoGestrueTrain_new/ respectively.  Similarly, place all the 84 segmented RGB, aligned depth  and handDetection subfolders(001,002,...) of validation set in seg_valid_depth_2stream/, seg_valid_rgb_2stream/ and seg_valid_rgb_2stream/cvtConGTestToIso/.  The validation temporal segmentation file are placed in seg_valid_rgb_2stream/ConGTestSegInfo/. (The temporal segmentation will be introduced in Testing) 

In this github, there is only a subfolder(001). We take it as an example to show you how to get the input data and setting files needed by Setp4 quickly.
You can run the following commands to get all the files:
```
sh preprocessTrainRGBData.sh  #(for getting the 'RGB files' needed by finetuning c3d of traning set)
sh preprocessTrainDepthData.sh  #(for getting the 'depth files' needed by finetuning c3d of traning set)
```
After the two commands, the generated input data for c3d will be saved in python/data/train/unifi_only_hand_file/depth and python/data/train/only_hand_face_file/rgb.  The setting files will be saved in python/train_list_c3d. Note, we finetune the c3d with RGB and depth respectively.  For more about the input data and the setting files, you can refer to C3D User Guide(https://docs.google.com/document/d/1-QqZ3JHd76JfimY4QKqOojcEaf5g3JS0lNh-FHTxLag/edit).

he two scripts includes several steps as follows: 

#### Step3.1: Convert RGB and aligned depth video to images.

#### Step3.2: Get the images containing only hands in RGB and Depth.

#### Step3.3: Depth and RGB frames unification.

#### Step3.4: Get the images containing only hands and face in RGB.

#### Step3.5: Get the setting files for finetuning the C3D model, which is pre-trained on Sports-1M.

The pre-trained model can be downloaded from https://drive.google.com/file/d/0Bx-2rTokRAt1Vm9nLWItQTgtSVE/view.

### Tips: 

Final RGB images only have face and hands. Final depth images only have hands. The length of C3D input videos are all 32 frames by sampling. (All functions have detail comments in python/process_data.py)

### Step4: Finetune the C3D with final RGB images and depth images respectively. 

The training process stops after 100000 iterations. It will cost about 60 hours in a single Titan X GPU for finetuning the C3D.

#### Step4.1: Installation.

For more information about the installation and usage, please refer ro http://vlg.cs.dartmouth.edu/c3d/. After Installation of C3D, place C3D-v1.0-key-parts/con_c3d_finetuning_len_32_only_hand_depth_map_2streams, con_c3d_finetuning_len_32_only_hand_face_rgb_2streams, c3d_train_ucf101 and con_validation_only_hand_depth_map_2stream_feature_extraction and con_validation_only_hand_face_rgb_2stream_feature_extraction in C3D-v1.0/example.

#### Step4.2: Compute volume mean from input setting list. 

#### Step4.3: Finetune C3D. 

(The finetuned c3d model can be downloaded from https://pan.baidu.com/s/1kUU7oRP)

### Step5: Extracte c3d 'fc6' feature.

The c3d extracted feature will be saved in python/feature. (The extracted c3d feature can be downloaded from https://pan.baidu.com/s/1mhZoRaw).

### Tips: 

The detail introduction of Step4 and Step5 can be refered to C3D User Guide. You can also refer to our prototxt files inC3D-v1.0-key-parts.

### Step6: Read and fuse extracted RGB and depth feature.

Run the matlab script read_and_fuse_feature_ubuntu_matlab/main.m. The final fused training features lie in the current directory. (the final feature can also be downloaded from https://pan.baidu.com/s/1o7TLg3g)

### Step6: To train linear-SVM model using the code of svm_c3d.py. 

python svm_c3d.py --isTrain 1 --isTest 0

We have already uploaded the trained svm model in svm_model/.

## Testing:

### Step1: Get face and hand detecion about validation or testing dataset.

### Step2: Run the code in validation_Tesing_temporal_segmentaion to get segmented video, hand detection and video length. (described in Step3 of Training)

### Step3: To preprocess the training dataset and get the input data and setting files for extacting c3d fc6 feature: 
```
sh preprocessTestRGBData.sh 
sh preprocessTestDepthData.sh  
```
### Step4: Extracte c3d fc6 feature like training Step5.

### Step5: Read and fuse extracted RGB and depth feature by running read_and_fuse_feature_ubuntu_matlab/mainTest.m

### Step6: To test the validation or testing dataset in svm_c3d.py and get prediction file in python/submission:

python svm_c3d.py --isTrain 0 --isTest 1

## Performance

Accuracy on validation: 0.514451
