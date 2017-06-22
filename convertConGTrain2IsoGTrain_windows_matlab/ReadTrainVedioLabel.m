function TrainVedioLabel = ReadTrainVedioLabel(TrainFileName)
%The function read the input/train.txt original vedio label,Then convert it
%to a label structure including IDDevel/IDVedio, Fbegin, Fend:IDGesture,and
%NumGesture
%save as a file
TrainFileName = 'input\train.txt';
OriTrainlabel = importdata(TrainFileName);


TrainSize = size(OriTrainlabel, 1);
TrainVedioLabel = [];
for i = 1 : TrainSize
    OneLabelContent = OriTrainlabel{i, 1};
    OneLabelSize = size(OneLabelContent, 2);
    FirstId = 3;
    SecondId = 9;
    ThirdID = zeros(3, 100);%one continuous gesture contain the max number of getsure
    IDDevel = str2num(OneLabelContent(1 : FirstId));
    IDVedio = str2num(OneLabelContent(FirstId + 2 : SecondId));
    
    count = 1;
    for j = SecondId + 2 : OneLabelSize
        tmp = OneLabelContent(1, j);
        tmp2 = ':';
        tmp3 = ' ';
        if(OneLabelContent(1, j) == ',')
            ThirdID(1, count) = j - 1;
            continue;
        end
        if(OneLabelContent(1, j) == tmp2)
            ThirdID(2, count) = j - 1;
            continue;
        end
        if(OneLabelContent(1, j) == tmp3)
            ThirdID(3, count) = j - 1;
            count = count + 1;
            continue;
        end
    end
    ThirdID(3, count) = OneLabelSize;
    ConGesture = zeros(3, count);
    for j = 1 : count
        tmp = OneLabelContent(SecondId + 2 : ThirdID(1, j));
        if(j == 1)
            ConGesture(1, j) = str2num(OneLabelContent(SecondId + 2 : ThirdID(1, j)));
            ConGesture(2, j) = str2num(OneLabelContent(ThirdID(1, j) + 2 : ThirdID(2, j)));
            ConGesture(3, j) = str2num(OneLabelContent(ThirdID(2, j) + 2 : ThirdID(3, j)));        
        else 
            ConGesture(1, j) = str2num(OneLabelContent(ThirdID(3, j - 1) + 2 : ThirdID(1, j)));
            ConGesture(2, j) = str2num(OneLabelContent(ThirdID(1, j) + 2 : ThirdID(2, j)));
            ConGesture(3, j) = str2num(OneLabelContent(ThirdID(2, j) + 2 : ThirdID(3, j)));   
        end
    end
    NumGesture = count;  
    TrainVedioLabel(i).IDDevel = IDDevel;
    TrainVedioLabel(i).IDVedio = IDVedio;
    TrainVedioLabel(i).NumGesture = NumGesture;
    TrainVedioLabel(i).ConGesture = ConGesture;
end
save('TrainVedioLabelversion2.mat', 'TrainVedioLabel');