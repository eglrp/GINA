% 2 D-s ploton ábrázolnal a lat longot. Matlabos 2D-s plot.
function lat_long_plot(t_data,varargin)
    f1 = figure('name','Lattiude Longitude') 
    plot(t_data.lat,t_data.lon)
    grid on;
    title('Hosszúsági és szelességi koordináták')
    xlabel('Szélesség [rad]')
    ylabel('Hosszúság [rad]')
    
    %Save function if 2. argument exist.
    if max(size(varargin))> 0
        name = cell2mat(varargin(2));
        %path = pwd;
        %save_name = [path name];
        saveas(f1,name,'png');
    end

    
end 