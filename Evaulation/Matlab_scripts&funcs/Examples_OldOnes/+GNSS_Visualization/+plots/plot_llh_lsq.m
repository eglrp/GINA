function plot_llh_lsq( conf, hod, lat_lsq, lon_lsq, h_lsq, filenamewithfullpath, subplot_height, figure_showup )

%==================================================================
% 1.) LLH LSQ -------------------
%==================================================================

if conf.lsqllh

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lat_lsq,'.b');
    xlabel('Hours of Day');
    ylabel('Latitude [deg]');
    title('Latitude (LSQ)');
    grid on;

    subplot(3,1,2);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lon_lsq,'.b');
    xlabel('Hours of Day');
    ylabel('Longitude [deg]');
    title('Longitude (LSQ)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,h_lsq,'.b');
    xlabel('Hours of Day');
    ylabel('Height [m]');
    title('Height (LSQ)');
    grid on;
    % End LLH LSQ -------------------

    % Save to file
         
    print(filenamewithfullpath,'-dpng')
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    print(filenamewithfullpath,'-dpdf')
    savefig(h,filenamewithfullpath)
    close all
    
end
end

