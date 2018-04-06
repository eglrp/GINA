function plot_enu_lsq( conf, hod, e_lsq, n_lsq, u_lsq, filenamewithfullpath, subplot_height, figure_showup )
%==================================================================
% 3.) ENU LSQ -------------------
%==================================================================

if conf.lsqenu
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,e_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('E [m]');
    title('E (LSQ)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,n_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('N [m]');
    title('N (LSQ)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,u_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('U [m]');
    title('U (LSQ)');
    grid on;
    % End ECEF LSQ -------------------

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

