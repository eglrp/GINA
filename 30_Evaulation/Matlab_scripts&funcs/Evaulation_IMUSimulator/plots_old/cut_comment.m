function cut_line = cut_comment(line)
expression = '#';
mat_line = str2mat(line);
start_index = regexp(line,expression);
if min(start_index) > 0
    end_of_line =  min(start_index);
    i = 1;
    j = 1;
    while  i < end_of_line
        cut_line(j)= mat_line(i);
        i=i+1;
        j=j+1;
    end
else
    cut_line =line;
end
cut_line = deblank(cut_line); %This remove the unnecessary whitespace 
end