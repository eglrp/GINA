function h_data = read_header(fileName)
%h_data is a structure of the header data. 
%It is declared default values as followe: 
h_data.startTime = 0;
h_data.endTime = 0;
h_data.positonFormat = 'defaultValue';
h_data.startPositon = [0,0,0];
h_data.startVelocity = [0,0,0];
h_data.startAttitude =[0,0,0];
h_data.epochTime = 0;

pinaFile = fopen(fileName);

keyword_start = 'START OF HEADER';
keyword_end = 'END OF HEADER';

keywords = {'START TIME','END TIME','POSITION FORMAT',...
             'START POSITION','START VELOCITY',...
             'START ATTITUDE','EPOCH TIME'};
         
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


















