%
%main program for reading and fusing c3d feature
clear;
%%setting path
rgbtrainC3dPath = '/home/zhipengliu/ChaLearn2017/ConG/feature/c3d_only_face_hand/train/only_face_hand_rgb_2streams';
rgbvalidationC3dPath = '/home/zhipengliu/ChaLearn2017/ConG/feature/c3d_only_face_hand/validation/only_face_hand_rgb_2streams';

depthtrainC3dPath = '/home/zhipengliu/ChaLearn2017/ConG/feature/c3d_only_hand/train/only_hand_depth_map_2stream';
depthvalidationC3dPath = '/home/zhipengliu/ChaLearn2017/ConG/feature/c3d_only_hand/validation/only_va_hand_depth_map_2streams';

depthtrainFeaturePath = '/home/zhipengliu/ChaLearn2017/IsoGesture/feature/process/only_face_hand/con_train_only_hand_depth_map_2stream.mat';
rgbtrainFeaturePath = '/home/zhipengliu/ChaLearn2017/IsoGesture/feature/process/only_face_hand/con_train_only_hand_face_rgb_2stream.mat';

depthvalidationFeaturePath = '/home/zhipengliu/ChaLearn2017/IsoGesture/feature/process/only_face_hand/con_validation_only_hand_depth_map_2stream.mat';
rgbvalidationFeaturePath = '/home/zhipengliu/ChaLearn2017/IsoGesture/feature/process/only_face_hand/con_validation_only_hand_face_rgb_2stream.mat';

savetrainfile = '/home/zhipengliu/ChaLearn2017/IsoGesture/feature/process/fusion/con_training_fusion_depth_map_only_hand_rgb_only_hand_face_2stream.mat';
savevalidationfile = '/home/zhipengliu/ChaLearn2017/IsoGesture/feature/process/fusion/con_validation_fusion_depth_map_only_hand_rgb_only_hand_face_2stream.mat';


ntrain = 30441; %the number of valid training sample
nvalid = 9113; %the number of valid validation sample(it also can be set 6271, the numer of testing sample)

readC3dFeature(rgbtrainC3dPath,  rgbtrainFeaturePath, ntrain, 1);
readC3dFeature(rgbvalidationC3dPath, rgbvalidationFeaturePath, nvalid, 2);
readC3dFeature(depthtrainC3dPath, depthtrainFeaturePath, ntrain, 1);
readC3dFeature(depthvalidationC3dPath, depthvalidationFeaturePath, nvalid, 2);

feature_fusion(depthtrainFeaturePath, rgbtrainFeaturePath, depthvalidationFeaturePath, rgbvalidationFeaturePath, savetrainfile, savevalidationfile)

