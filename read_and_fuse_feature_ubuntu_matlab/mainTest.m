%
%main program for reading and fusing c3d feature
clear;
%%setting path of c3d feature
rgbvalidationC3dPath = '../python/feature/test/rgb';
depthvalidationC3dPath = '../python/feature/test/depth';

%%setting path

depthvalidationFeaturePath = 'iso_validation_only_hand_depth_map_2stream.mat';
rgbvalidationFeaturePath = 'iso_validation_only_hand_face_rgb_2stream.mat';

savevalidationfile = 'fusion_test.mat';


ntrain = 30441; %the number of training sample
nvalid = 9113; %the number of segmented validation sample.

readC3dFeature(rgbvalidationC3dPath, rgbvalidationFeaturePath, nvalid);
readC3dFeature(depthvalidationC3dPath, depthvalidationFeaturePath, nvalid);

feature_fusion(depthvalidationFeaturePath, rgbvalidationFeaturePath, savevalidationfile, 0)
