% 2 D-s ploton ábrázolnal a lat longot. Matlabos 2D-s plot.
function lat_long_plot(t_data,varargin)
    f1 = figure('name','Lattiude Longitude') 
    plot(t_data.lat,t_data.lon)
    line(e(number_of_point),n(number_of_point),t_data.h(number_of_point),...
    'Marker', 'o','Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize', 5);

    grid on;
    title('Hosszúsági és szelességi koordináták')
    xlabel('Szélesség [rad]')
    ylabel('Hosszúság [rad]')
    
     %set the axes of limits 
    r = 0.15;
    lat = t_data.lat;
    lon = t_data.lon;
    h = t_data.h;
    delta_lat = max(lat)-min(lat);
    delta_lat= delta_lat*r;
    
    delta_lon = max(lon)-min(lon);
    delta_lon= delta_lon*r;
    
    delta_h = max(h)-min(h);
    delta_h = delta_h*r;
    
    distance = [delta_lat, delta_lon, delta_h]
    
    %steady state case || total distance < 1mm
    if abs(distance) <= 1*10e-3    
        xlim([min(lat)-10 max(lat)+10]);
        ylim([min(lon)-10 max(lat)+10]);
        zlim([min(h)-10 min(h)+10]);
    end
    
    %total disctance > 1mm 
    if abs(distance) >= 1*10e-3  
        xlim([min(e)-delta_lat max(e)+delta_lat]);
        ylim([min(n)-delta_lon max(n)+delta_lon]);
        zlim([min(u)-delta_h max(u)+delta_h]);
        
        xlim([min(e)-max_delta max(e)+max_delta]);
        ylim([min(n)-max_delta max(n)+max_delta]);
        zlim([min(u)-max_delta max(u)+max_delta]);
    end
  
    
    
    
    
    
    
    %Save function if 2. argument exist.
    if max(size(varargin))> 0
        name = cell2mat(varargin(2));
        %path = pwd;
        %save_name = [path name];
        saveas(f1,name,'png');
    end

    
end 