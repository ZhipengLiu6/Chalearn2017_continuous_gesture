# Chalearn2017_continuous_gesture
This an implementaion of our method about ChaLearn LAP Large-scale Continuous Gesture Recognition Challenge (Round 2) @ICCV 2017

## Notes: This code was tested on Windows10 OS with VS2012 and Ubuntu 14.04 OS with Python 2.7, caffe-C3D, sklearn, faster-rcnn.
Please double check the paths in code before your run it.
The process of training:

##Step1: Use the code in convertConGTrain2IsoGTrain_windows_matlab to convert continuous gesture to isolated gesture in order to train conveniently

##Step2: Get face and hand detetion about preprocessed continuous training dataset.
Face and hand postion can be detected by using the code in the detection file. The detailed introduction can be seen in Detection-Pipeline-Cons.pdf. 
	you can get detection result, such as ConsRGBTrainFaceDetect.txt and ConsTrainHandDetection.

##Step3: Preprocess the training dataset and get special images. RGB images only have face and hand. Depth images only have hand. The length of C3D input videos are all 32 frames
by sampling.(All functions have detailed comment in con_process_data.py)

###Step3.1: Convert RGB and depth video to images by running the funtion of cvtVideo2Img and set corresponding paths.
###Step3.2: Get the images containing only hands in RGB and Depth by running the funtion of blackNonHand and set corresponding paths.
###Step3.3: Depth and RGB frames unification and get the images containing only hands and face in RGB by running the funtion of frameUnification and blackNonHandFace.
###Step3.4: Get the input and output image list for finetuning the C3D, which is pre-trained on Sports-1M, by the function of set_image_list and set_c3d_output_list.

##Step4: Finetune the C3D with only-hand-face RGB images and only-hand Depth image respectively.

###Step4.1: Installation. For more information about the installation and usage, please refer ro http://vlg.cs.dartmouth.edu/c3d/

###Step4.2: Get mean-value file. 

###Step4.3: Finetune C3D.  c3d_finetuning_len_32_only_hand_depth_map_2stream and c3d_finetuning_len_32_only_hand_face_rgb_2streams can be placed in the directory of example. Set corresponding paths(must be careful). (The finetuned c3d model can be downloaded from https://pan.baidu.com/s/1kUU7oRP)

###Step4.4: Feature extraction. C3d_feature_extraction can be placed in the directory of example. Set corresponding paths(must be careful). (The extracted c3d feature can be downloaded from https://pan.baidu.com/s/1hsOhTFu)

###Step4.5: Read extracted c3d feature using the code of read_and_fuse_feature_ubuntu_matlab. (the final feature can also be downloaded from https://pan.baidu.com/s/1o7TLg3g)

##Step5: Train linear-SVM model using the code of svm_c3d.py.(The svm_model file has trained svm model)

The process of testing:

##Step1: Get face and hand detection about validation or testing dataset.

##Step2: Use the code in validation_Tesing_temporal_segmentaion to get segmented video, hand detection and video length.

##Step3: Use the code in cvtConGSegTestVideo2Labeled.py to get labeled file format about validation or testing data.

##Step4: Preprocess the validation or testing dataset dataset like training step 3.

##Step5: Feature extraction and read it like training step 4.

##Step6: Test the validation or testing dataset in svm_c3d.py and get prediction file.
