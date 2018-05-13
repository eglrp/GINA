h3D = figure;
set(h3D,'Position',[20 75 700 600]);

% Set up axes
hmapaxis = axesm ('globe','Grid', 'on');
set(hmapaxis,'Position',[.1 .5 .8 .4])

view(85,0)
axis off

% Plot data on 3-D globe
meshm(geoidResults,RRR)

% Plot land mass outline
plotm(coast.lat,coast.long,'Color','k')
colormap('jet');

% Plot Title
title({'EGM96 Geoid Heights';['Grid Increment: ' ,num2str(gridDegInc), ' Degrees; Height Units: Meters']})

colorbar;

% 3-D Globe: Geoid Height Using VR Canvas
www3D = vrworld('astGeoidSphere.wrl');
open(www3D)

% Position canvas
geoidcanvas3D = vr.canvas(www3D,'Parent',h3D,...
    'Antialiasing', 'on','NavSpeed','veryslow',...
    'NavMode','Examine','Units', 'normalized',...
    'Position',[.15 .04 .7 .4]);
vrdrawnow;