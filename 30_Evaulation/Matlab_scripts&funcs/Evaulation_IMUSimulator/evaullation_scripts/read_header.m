function h_data = read_header(fileName)
%h_data is a structure of the header data. 
%It is declared default values as followe: 
h_data.startTime = NaN;
h_data.endTime = NaN;
h_data.positionFormat = 'defaultValue';
h_data.startPosition = [NaN,NaN,NaN];
h_data.startVelocity = [NaN,NaN,NaN];
h_data.startAttitude =[NaN,NaN,NaN];
h_data.epochTime = NaN;

pinaFile = fopen(fileName);

keyword_start = 'START OF HEADER';
keyword_end = 'END OF HEADER';

keywords = {'START TIME','END TIME',...
             'START POSITION','START VELOCITY',...
             'START ATTITUDE','EPOCH INTERVAL','TYPE','POSITION FORMAT',...
             'CREATOR','TYPE','TIME SYSTEM','TIME OF CREATION','POSITION FORMAT'};

 header_switch = 0; 

while ~feof(pinaFile) && header_switch ~= -1
    line = fgetl(pinaFile);
    line = cut_comment(line);
    %multi whitspace remove, only one ws between the words
    line = regexprep(line,' +',' ');
    
    if header_switch == 1
      [info,data]= find_keywords(line,keywords);
      h_data = load_h_data(info,data,h_data);
    end
    
    if strcmp(line,keyword_start) == 1
        header_switch = 1;
    end
    
    if strcmp(line,keyword_end) == 1
        header_switch = -1;
    end
end
fclose(pinaFile);
end


















