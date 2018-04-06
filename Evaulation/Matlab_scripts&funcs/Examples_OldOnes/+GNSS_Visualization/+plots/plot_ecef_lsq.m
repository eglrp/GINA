function plot_ecef_lsq( conf, hod, x_lsq, y_lsq, z_lsq, filenamewithfullpath, subplot_height, figure_showup )
%==================================================================
% 2.) ECEF LSQ -------------------
%==================================================================

if conf.lsqecef

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,x_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('X [m]');
    title('X (LSQ)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,y_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('Y [m]');
    title('Y (LSQ)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,z_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('Z [m]');
    title('Z (LSQ)');
    grid on;
    % End ECEF LSQ -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    
    print(filenamewithfullpath,'-dpdf')
    savefig(h,filenamewithfullpath)
    close all
    
end

end

