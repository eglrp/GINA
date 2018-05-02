%The fuction is required a string and a eywords. 
%Keywords can be : vectro of strings, string,cahr.
%Return the values of after the keywords.
%It is named by data.
function [info,data_double]= find_keywords(line,keywords)

info = {'Not a keyword'};
data_double = NaN;

buff_data = NaN;

line = cut_comment(line);
mat_line = str2mat(line);
k=0; % It is make sense if there is more keywords in one line.
for word_index=1:max(size(keywords))
    start_index = regexp(line,keywords{word_index});
    if min(start_index) > 0
        i = min(start_index) + max(size(keywords{word_index})) + 1; %+1 for sapce! 
        j = 1;
        k =k+1;
        while  i < max(size(mat_line))
            buff_data(k,j)= mat_line(i);
            info(k,1)= keywords(word_index);
            i=i+1;
            j=j+1;
        end          
    end
end
info = cell2mat(info);
mat_data = str2mat(buff_data);
cell_data = strsplit(mat_data,' ');
for i=1:max(size(cell_data))
    data_double(i) =  str2double(cell_data{i});
end

if max(size(buff_data)) > 1
    disp('A data mérete:')
    size(data_double)
end

end