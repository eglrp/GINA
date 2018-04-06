function plot_ecef_kf( pltconf, hod, x_kf, y_kf, z_kf, filenamewithfullpath, subplot_height,figure_showup)
%==================================================================
% 5.) ECEF KF -------------------
%==================================================================

if pltconf.kfecef

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,x_kf,'.r');
    xlabel('Hours of Day');
    ylabel('X [m]');
    title('X (KF)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position');
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,y_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Y [m]');
    title('Y (KF)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,z_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Z [m]');
    title('Z (KF)');
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

