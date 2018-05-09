function plot_world_map(lat_kf, lon_kf, filenamewithfullpath, figure_showup)

   

    try
        figure;
        if ~figure_showup
            set(gcf, 'Visible', 'off')
        end

        % World map with contintental shpes
        load coastlines;
        worldmap world;
        geoshow('landareas.shp', 'FaceColor', [0.9 0.9 0.9]);
        plotm(coastlat, coastlon);

        % add lat/lon data
        plotm(lat_kf,lon_kf,'+r');

        % Save to file
        print(filenamewithfullpath,'-dpng')
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        
        print(filenamewithfullpath,'-dpdf')
    catch
        warning('World Map (Map toolbox required) had an error.')
    end



end