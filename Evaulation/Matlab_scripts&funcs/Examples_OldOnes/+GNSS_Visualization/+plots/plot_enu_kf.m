function plot_enu_kf( conf, hod, e_kf, n_kf, u_kf, filenamewithfullpath, subplot_height, figure_showup )
%==================================================================
% 6.) ENU KF -------------------
%==================================================================

if conf.kfenu
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,e_kf,'.r');
    xlabel('Hours of Day');
    ylabel('E [m]');
    title('E (KF)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position');
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,n_kf,'.r');
    xlabel('Hours of Day');
    ylabel('N [m]');
    title('N (KF)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,u_kf,'.r');
    xlabel('Hours of Day');
    ylabel('U [m]');
    title('U (KF)');
    grid on;
    % End ECEF kF -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    
    print(filenamewithfullpath,'-dpng')
    savefig(h,filenamewithfullpath)
    close all
    
end
end

