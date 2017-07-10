function TrainData = ConvertConVedioToIsoGesture()
%%author:zhipeng.liu 2016-7-7
%%
%%-----------------------setting-------------------------
if ~exist('this_dir')
    this_dir=pwd;
end 
data_path   = 'G:\ChaLearn2017\conG\ConGD_phase_1\train';    % Path to the train data.
code_dir    = [this_dir '\mfunc\basic']; % Path to the code.
   
% Add the path to the function library
warning off; 
% addpath(genpath(code_dir)); 
% addpath(code_dir); 
warning on;
%%-----------------------load configure information-------------------------------
%% load original train label information,directory is \input\TrainVedioLabel.mat
OriTrainLabelInPath = 'input\TrainVedioLabel.mat';%%or input\TrainVedioLabelValidVedioLabelnew.mat
OriTrainVedioLabel = load(OriTrainLabelInPath);
OriTrainVedioLabel = OriTrainVedioLabel.TrainVedioLabel;
%%-----------------------end----------------------------------------------
NumOriVedio = size(OriTrainVedioLabel, 2);
CountLabelVedio = zeros(300,1);%%max label:249
%%wrong train number = 9571;
cur = 1;
for i = cur : NumOriVedio
     fprintf('-----------%d/%d-----------\n',i, NumOriVedio);
     OneInstance = OriTrainVedioLabel(1, i);
     IDDevel = OneInstance.IDDevel;
     IDVedio = OneInstance.IDVedio;
     NumGesture = OneInstance.NumGesture;
     ConGesture = OneInstance.ConGesture;%%temporal segmentation content of vedio
     strIDDevel = sprintf('%03d', IDDevel);
     strIDVedio = sprintf('%05d', IDVedio);
     VedioPath = [data_path '\' strIDDevel '\' strIDVedio '.M.avi'];
     DepthVedioPath = [data_path '\' strIDDevel '\' strIDVedio '.K.avi'];
     [M0, fps] = read_movie(VedioPath); 
     [K0, fps] = read_movie(DepthVedioPath);
     if(size(M0, 2) ~= ConGesture(2, size(ConGesture, 2)))
         sprintf('id-%d is wrong', i)
         continue;
     end
     for j = 1 : NumGesture
         fprintf('%d/%d\n', j, NumGesture);
         label = ConGesture(3, j);%label of gesture
         StrLabel = sprintf('%03d', label);  
         Dir = ['convertContinousToIsoGestrueTrain\' StrLabel];%%or convertContinousToIsoGestrueValid

         mkdir(Dir);
%          if ~exist(Dir, 'dir')
%              mkdir(Dir);
%          end
         CountLabelVedio(label, 1) = CountLabelVedio(label, 1) + 1;
         StrCount = sprintf('%04d', CountLabelVedio(label, 1));
         Kvediopath = [Dir '\' StrCount '.K.avi'];
         Mvediopath = [Dir '\' StrCount '.M.avi'];
         tmpcount = 1;
         fstart = ConGesture(1, j);
         fend = ConGesture(2, j);
%          Mframe = zeros(fend - fstart + 1, 240, 320, 3);
%          Kframe = zeros(fend - fstart + 1, 240, 320, 3);
         Mframe = [];
         Kframe = [];
         for t = fstart : fend
             Mframe(tmpcount).data = M0(1, t).cdata;
             Kframe(tmpcount).data = K0(1, t).cdata;
             tmpcount = tmpcount + 1;
         end
         frames2Video(Mvediopath, Mframe);
         frames2Video(Kvediopath, Kframe);
    %      VedioSize 
%      for j = 1 : 
     end
end
end

function frames2Video(vedioPath,allframe)
myObj = VideoWriter(vedioPath);%³õÊ¼»¯Ò»¸öaviÎÄ¼þ
myObj.FrameRate = 10;
if exist(vedioPath)
    return 
end
open(myObj);
fsize = size(allframe, 2);
frame = zeros(240, 320, 3);
for i=1:fsize%the number of frame
    frame = uint8(allframe(i).data);
    writeVideo(myObj,frame);
end 
close(myObj);
end

