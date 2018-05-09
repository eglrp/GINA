
function create_HTML(varargin)
%% create_HTML creates automatically an html-file in the current folder.  
%              This html allows to display a certian track on google-maps
%
%  Example for full parameters:
%  create_HTML(name_file, latlng1, latlng2, ... , name_course1, name_course2, ...)
%
%   Input arguments                         Type             Remark
%       - name_file                         [String]        html-file will get this as filename
%       - latlng1, latlng2, ...             [double2,n]     n samples; degrees      
%                                                           1:n,1 = latitude-values
%                                                           1:n,2 = longitudinal-values
%       - name_course1, name_course2, ...   [String]        the routes on google-maps
%                                                           will get this as lable
%   Maximum number routes is 8!
%
%   The html-file can be opened with the browser (Firefox or Internet-Explorer)   
   
    kinematic = false;
    if ~isempty(strfind(varargin{1}.mode,'kinematic'))
        kinematic = true;
    end
    
    
    col(1)=15632445; %[EE 88 3D]
    col(2)=255;      %[00 00 FF]
    col(3)=65280;    %[00 FF 00]
    col(4)=16711680; %[FF 00 00]
    col(5)=65535;    %[00 FF FF]
    col(6)=16711935; %[FF 00 FF]
    col(7)=16776960; %[FF FF 00]
    col(8)=0;        %[00 00 00]

    fid = fopen(sprintf('%s.html',varargin{1}.name),'w+');

    fprintf(fid,'<!DOCTYPE html>\n');
    fprintf(fid,'<html>\n');
    fprintf(fid,'<head>\n');
    fprintf(fid,'<meta name="viewport" content="initial-scale=1.0, user-scalable=no">\n');
    fprintf(fid,'<meta charset="utf-8">\n');
    fprintf(fid,'<title>Simple Polylines</title>\n');
    fprintf(fid,'<style>\n');
    fprintf(fid,'html, body, #map-canvas {\n');
    fprintf(fid,'height: 100%%;\n');
    fprintf(fid,'margin: 0px;\n');
    fprintf(fid,'padding: 0px\n');
    fprintf(fid,'}\n');
    fprintf(fid,'</style>\n');
    fprintf(fid,'<script src="https://maps.googleapis.com/maps/api/js?v=3.exp"></script>\n');
    % fprintf(fid,'<script type="text/javascript" src="googleMapResults\laufe.js"> </script>\n');
    % fprintf(fid,'<script type="text/javascript" src="googleMapResults\laufz.js"> </script>\n');
    fprintf(fid,'<script>\n');
    fprintf(fid,'function initialize() {\n');
    fprintf(fid,'var mapOptions = {\n');
    fprintf(fid,'zoom: 15,    // was 3\n');
    %fprintf(fid,'center: new google.maps.LatLng(%10.8f, %10.8f),\n',varargin{2}(1,1),varargin{2}(1,2));
    try
        fprintf(fid,'center: new google.maps.LatLng(%10.8f, %10.8f),\n',varargin{2}(1,1),varargin{2}(1,2));
    catch
        ll=1;
        while isempty (varargin{ll})
            ll=ll+1;
        end
        fprintf(fid,'center: new google.maps.LatLng(%10.8f, %10.8f),\n',varargin{ll}(1,1),varargin{ll}(1,2));
    end
    fprintf(fid,'mapTypeId: google.maps.MapTypeId.SATELLITE  // was .TERRAIN\n');
    fprintf(fid,'};\n');
    fprintf(fid,'var map = new google.maps.Map(document.getElementById(''map-canvas''),\n');
    fprintf(fid,'mapOptions);\n');
    fprintf(fid,'var testcases = new Array();\n');
        
    
    % Print Marker for kinematic scenarios
    if kinematic
        fprintf(fid, 'var iconBase = ''http://labs.google.com/ridefinder/images/'';\n');
        fprintf(fid, 'var ora = iconBase + ''mm_20_red.png'';\n');
        fprintf(fid, 'var blu = iconBase + ''mm_20_blue.png'';\n');
        fprintf(fid, 'var grn = iconBase + ''mm_20_green.png'';\n');
        fprintf(fid, 'var red = iconBase + ''mm_20_orange.png'';\n');

        for c=1:(nargin-1)/2
            for l=1:(length(varargin{c+1}))-1
                fprintf(fid,'var myLatLng = new google.maps.LatLng(%10.8f, %10.8f);\n',varargin{c+1}(l,1),varargin{c+1}(l,2));  
                fprintf(fid,'var marker = new google.maps.Marker({\n');
                fprintf(fid,'position: myLatLng,\n');
                if c==1
                    fprintf(fid, 'icon: red,\n');
                elseif c==2
                    fprintf(fid, 'icon: blu,\n');
                elseif c==3
                    fprintf(fid, 'icon: grn,\n');
                else
                    fprintf(fid, 'icon: ora,\n');
                end
                fprintf(fid,'});\n');
                fprintf(fid,'marker.setMap(map);');     

            end
        end   
    end
       

%     
%     % Print maker label at every 10th data point
%     for c=1:(nargin-1)/2
%         for l=1:(length(varargin{c+1}))-1
%             fprintf(fid,'var myLatLng = new google.maps.LatLng(%10.8f, %10.8f);\n',varargin{c+1}(l,1),varargin{c+1}(l,2));  
%             %if mod(l,10)==0
%                 fprintf(fid, 'var mapLabel = new MapLabel({\n');
%                 fprintf(fid, 'text: ''%02d:%02d:%02d'',\n',varargin{c+1}(l,3),varargin{c+1}(l,4),varargin{c+1}(l,5));
%                 fprintf(fid, 'position: myLatLng,\n');
%                 fprintf(fid, 'map: map,\n');
%                 fprintf(fid, 'fontSize: 35,\n');
%                 fprintf(fid, 'align: ''right''\n');   
%                 fprintf(fid, '});\n');
%                 fprintf(fid, 'mapLabel.setMap(map);\n');
%                 fprintf(fid, 'mapLabel.set(''position'', myLatlng);');
%             %end
%         end
%     end     
    

    % Collect Lat Lon Tuples for Polyline plot
    for c=1:(nargin-1)/2
        fprintf(fid,sprintf('var coord%i = [',c));
        for l=1:(length(varargin{c+1}))-1
            try
            fprintf(fid,'new google.maps.LatLng(%10.8f, %10.8f),\n',varargin{c+1}(l,1),varargin{c+1}(l,2));      
            end
        end
        %fprintf(fid,'new google.maps.LatLng(%10.8f, %10.8f)\n',varargin{c+1}(l,1),varargin{c+1}(l,2));
        fprintf(fid,'];\n');
    end

    % Print Marker 
    for c=1:(nargin-1)/2
        fprintf(fid,'var Path = new google.maps.Polyline({\n');
        fprintf(fid,'path: coord%i,\n',c);
        fprintf(fid,'geodesic: true,\n');
        fprintf(fid,'strokeColor: ''#%.6x'',\n',col(c));
        fprintf(fid,'strokeOpacity: 0.8,\n');
        if c == (nargin-1)/2
            fprintf(fid,'strokeWeight: 2\n');
        else
            fprintf(fid,'strokeWeight: 2\n');
        end
        fprintf(fid,'});\n');
        fprintf(fid,'Path.setMap(map);\n');
    end

    fprintf(fid,'}\n');

     fprintf(fid,'function plotPath(pathNo) {\n');
        fprintf(fid,'if( pathNo >= testcases.length ) {\n');
            fprintf(fid,'alert("no enough pathes selcted (only:" + testcases.length + " and required ID:" + pathNo + ")");\n');
            fprintf(fid,'return\n');
        fprintf(fid,'}\n');

        fprintf(fid,'for (i=0; i< printedOverlays.length; i++) { \n');
            fprintf(fid,'printedOverlays[i].setMap(null); \n');
        fprintf(fid,'}\n');
        fprintf(fid,'printedOverlays.length = 0;\n');

        fprintf(fid,'for(var i=2; i<testcases[pathNo].length; i++) {\n');
            fprintf(fid,'testcases[pathNo][i].setMap(map);\n');
            fprintf(fid,'printedOverlays.push(testcases[pathNo][i]);\n');
        fprintf(fid,'}\n');

        fprintf(fid,'for(var i=1; i<25; i++) {	\n');
            fprintf(fid,'var POS = new google.maps.LatLng(testt[(i-1)*3+0], testt[(i-1)*3+1]);\n');
            fprintf(fid,'var marker = new google.maps.Marker({ position: POS, map: map, title: timePoints_120615_Stadt_Rueck_01[(i-1)*3+2] });\n');
        fprintf(fid,'}\n');

        fprintf(fid,'// center map accordingly\n');
        fprintf(fid,'var mapOptions = { zoom: 17, center: testcases[pathNo][1], mapTypeId: google.maps.MapTypeId.SATELLITE };    // mapTypeId: google.maps.MapTypeId.TERRAIN\n');
        fprintf(fid,'map.setOptions(mapOptions);\n');
      fprintf(fid,'}\n');

    fprintf(fid,'google.maps.event.addDomListener(window, ''load'', initialize);\n');

        fprintf(fid,'</script>\n');

      fprintf(fid,'</head>\n');
      fprintf(fid,'<body>\n');
        fprintf(fid,'<form>\n');
            fprintf(fid,'</script>\n');
            for c=1:(nargin-1)/2
              fprintf(fid,'<font color=#%.6x> &nbsp;&nbsp;%s</font> &nbsp;&nbsp;\n',col(c),varargin{(nargin-1)/2+c+1});  
            end 
            fprintf(fid,'</form>\n');
        fprintf(fid,'<div id="map-canvas" style="width:100%%; height:90%%"></div>\n');
      fprintf(fid,'</body>\n');
    fprintf(fid,'</html>\n');

    fclose(fid);            
end
