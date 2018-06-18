% Képes beolvasni egy trajectoria fájlt. ok
% Ezt ki kell tudni plotolni.
% LLH egy plot de külön subploton a koordináták. ECEF-fel ugyanez. 

% Függelten attól, hogy a trajectória fájlban, hogy volt megadva, az ECEF és
% LLH kel.
% Szóval tudni kell átváltani is. OK

clc;
clear all;
close all;
fileName = 'trajectory_corner.gina';
t_data = create_data(fileName);
clc;    

enu_plot(t_data); %Ez használja az xyz2enu()-t ami átvált rad-ba...
trajectroy3d_plot(t_data,'llh');
trajectroy3d_plot(t_data,'ecef');
attitude_plot(t_data);
lat_long_plot(t_data);


%ned_plot(t_data);




