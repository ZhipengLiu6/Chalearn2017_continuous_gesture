%
%main program for reading and fusing c3d feature
clear;
%%setting path of c3d feature
rgbtrainC3dPath = '../python/feature/train/rgb';
depthtrainC3dPath = '../python/feature/train/depth';

%%setting path
depthtrainFeaturePath = 'iso_train_only_hand_depth_map_2stream.mat';
rgbtrainFeaturePath = 'iso_train_only_hand_face_rgb_2stream.mat';


savetrainfile = 'fusion_train.mat';


ntrain = 30441; %the number of training sample
nvalid = 9113; %the number of segmented validation sample

readC3dFeature(rgbtrainC3dPath,  rgbtrainFeaturePath, ntrain);
readC3dFeature(depthtrainC3dPath, depthtrainFeaturePath, ntrain);
%%fuse training rgb and depth feature
feature_fusion(depthtrainFeaturePath, rgbtrainFeaturePath, savetrainfile, 1)
