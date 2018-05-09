function plot_llh_kf(  conf, hod, lat_kf, lon_kf, h_kf, filenamewithfullpath, subplot_height, figure_showup )
%==================================================================
% 4.) LLH KF -------------------
%==================================================================

if conf.kfllh
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lat_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Lat [deg]');
    title('Lat (deg)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position');
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lon_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Lon [deg]');
    title('Lon (KF)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,h_kf,'.r');
    xlabel('Hours of Day');
    ylabel('H [m]');
    title('H (KF)');
    grid on;
    % End ECEF kF -------------------

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

