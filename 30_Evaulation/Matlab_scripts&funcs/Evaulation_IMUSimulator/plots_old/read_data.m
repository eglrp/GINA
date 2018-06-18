


% 
% 
% Google mapet csinálni az llh-ból. (errõl van matlabos script a neten, csak 
% túrni kell)
% 

% 3D ploton ábrázolni a trajecktóriát.
% 
% Olvassuk be az IMU fájlt.
% Ha van a trajectória fájlhoz megfelelõ IMU fájl, akkor a 3D-s ploton
% ábrázoljük
% ûaz erõket is az ismert pontokban.
% 
% 
% Plusz feladat:
% A 3D plotot az erõkkel animáljuk. Idõben látszódjon, ahogy haladunk, és
% ahogy más, más erõk hatnak rá.
%% Az adatok után nem állhat üres sor! 
%%És az adatok sorok között sem lehet üres sor!
function data = read_data(fileName)
format long;
ginaFile = fopen(fileName);
key_word = 'START OF DATA'; % érzékeny a spcaekre !!!! a fileban 
data_switch = 0; 
row = 1;
while ~feof(ginaFile)
    line = fgetl(ginaFile);
    if data_switch == 1
       data_cells = strsplit(line);
       for col = 1:length(data_cells)
            data(row,col) = str2double(cell2mat(data_cells(col)));
       end
       row=row+1;
    end
    if strcmp(line,key_word) == 1
        data_switch = 1;
    end
end
fclose(ginaFile);
end


















