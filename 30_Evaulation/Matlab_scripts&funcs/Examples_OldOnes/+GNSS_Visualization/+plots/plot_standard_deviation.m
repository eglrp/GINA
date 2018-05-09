function plot_standard_deviation( conf, hod, x_stddev, y_stddev, z_stddev, filenamewithfullpath, figure_showup, subplot_height )
%==================================================================
% Formal errors, i.e. standard deviation w.r.t. X, Y, Z
%==================================================================

if conf.stddev

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,x_stddev,'.b');
    xlabel('Hours of Day');
    ylabel('stddev [m]');
    title('Standard deviation X');
    grid on;

    subplot(3,1,2);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,y_stddev,'.b');
    xlabel('Hours of Day');
    ylabel('stddev [m]');
    title('Standard deviation Y');
    grid on;

    subplot(3,1,3);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,z_stddev,'.b');
    xlabel('Hours of Day');
    ylabel('stddev [m]');
    title('Standard deviation Z');
    grid on;


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

