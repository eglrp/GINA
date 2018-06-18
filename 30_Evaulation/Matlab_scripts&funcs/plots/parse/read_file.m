
% Képes beolvasni egy trajectoria fájlt. Ezt ki kell tudni plotolni.
% LLH egy plot de külön subploton a koordináták. ECEF-fel ugyanez. 
% Függelten attól, hogy a trajectória fájlban, hogy volt megadva, az ECEF és
% LLH kel. 
% Szóval tudni kell átváltani is.
% 
% Külön ploton ábrázolni a beállási szöget. roll pitch yaw, 3 subploton
% 2 D-s ploton ábrázolnal a lat longot. Matlabos 2D-s plot.
% 2D es ploton ábrázoljuk a NED-et. Ehhez az elsõ kezdõpontot válasszuk
% ûreferinciának 
% és mindig ahhoz képest számoljuk ki a Cne-t és az elmozdulásokat.
% 
% Ugyanez a plot csak 3D-ben.
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


function = data = read_data
data_switch = 0; 
key_word = 'START OF DATA';
ginaFile = fopen('imu.gina');
row = 1;
while ~feof(ginaFile)
    %disp(line);
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
disp(data)
fclose(ginaFile);




















