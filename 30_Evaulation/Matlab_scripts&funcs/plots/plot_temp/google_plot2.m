function google_plot2(varargin)
    switch length(varargin)
        case 1
            t_data = varargin{1};
            save_switch = false;
        case 2
            t_data = varargin{1};
            png_name = varargin{2};
            save_switch = true;
    end
    lat = t_data.lat;
    lon = t_data.lon;
    f1 = figure('Name','Google plot');
    hold on
    p1 = plot(lon, lat, '-*', 'MarkerSize', 10,'LineWidth', 5, 'Color','g');
    p2 = plot(lon(1), lat(1), 'Marker', 'o', ...
        'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize',10 );
    p3 = plot(lon(2:(end-1)), lat(2:(end-1)), 'Marker', 'o', ...
        'Color', 'g', 'MarkerEdgeColor', 'b', 'MarkerSize',10 );
    p4 = line(lon(end), lat(end),'Marker', 's', ...
          'MarkerFaceColor', 'r', 'MarkerSize',10 );
    hold off
    h = [p2(1),p4(1)];
    xlabel('lon [fok]')
    ylabel('lat [fok]')
    range_x = max(lon)-min(lon);
    
    if(range_x == 0)
    	range_x = 1;
    end
        
    xlim([min(lon)-(range_x*0.1) max(lon)+(range_x*0.55)])
    legend(h,'Start pont','Vég pont')
    plot_google_map('maptype','hybrid','MapScale',1);

    if save_switch == true
        folder_of_save = '\result\';
        save_name_with_path = [pwd folder_of_save png_name];
        saveas(f1,[save_name_with_path '.png'])
        saveas(f1,[save_name_with_path,'.fig'])
    end 
end