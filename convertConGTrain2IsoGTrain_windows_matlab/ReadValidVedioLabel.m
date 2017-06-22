function ValidVedioLabel = ReadTrainVedioLabel(validFileName)
%The function read the input/train.txt original vedio label,Then convert it
%to a label structure including IDDevel/IDVedio, Fbegin, Fend:IDGesture,and
%NumGesture
%save as a file
validFileName = 'input\label_valid.txt';
OriValidlabel = importdata(validFileName);


validSize = size(OriValidlabel, 1);
ValidVedioLabel = [];
for i = 1 : validSize
    i
    OneLabelContent = OriValidlabel{i, 1};
    OneLabelSize = size(OneLabelContent, 2);
    ThirdID = zeros(3, 100);%one continuous gesture contain the max number of getsure    
    blankPosi = zeros(1, 100);
    numblank = 0;
    for j = 1 : OneLabelSize
        tmp3 = ' ';
        if( OneLabelContent(1, j) == tmp3)
            numblank = numblank + 1;
            blankPosi(numblank) = j;
        end
    end
    mystr = OneLabelContent(1 : blankPosi(1) - 1);
    num = sscanf(mystr, '%d/%d');
    IDDevel = num(1);
    IDVedio = num(2);
    for j = 1 : numblank - 1
        mystr = OneLabelContent(blankPosi(j) + 1 : blankPosi(j + 1) - 1);
        num = sscanf(mystr, '%d,%d:%d');
        ThirdID(1, j) = num(1);
        ThirdID(2, j) = num(2);
        ThirdID(3, j) = num(3);        
    end

    mystr = OneLabelContent(blankPosi(numblank) + 1 : OneLabelSize);
    num = sscanf(mystr, '%d,%d:%d');
    ThirdID(1, numblank) = num(1);
    ThirdID(2, numblank) = num(2);
    ThirdID(3, numblank) = num(3);
    ConGesture = zeros(3, numblank);
    ConGesture = ThirdID(:, 1 : numblank);
    NumGesture = numblank;  
    ValidVedioLabelnew(i).IDDevel = IDDevel;
    ValidVedioLabelnew(i).IDVedio = IDVedio;
    ValidVedioLabelnew(i).NumGesture = NumGesture;
    ValidVedioLabelnew(i).ConGesture = ConGesture;
end
save('ValidVedioLabelnew.mat', 'ValidVedioLabelnew');