function plot_dop_values( conf, hod, pdop_ecef, pdop_enu, hdop_ecef, hdop_enu, vdop_ecef, vdop_enu, tdop, filenamewithfullpath, subplot_height, figure_showup  )
%==================================================================
% DOP values -------------------
%==================================================================

if conf.dop
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(4,2,1);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,pdop_ecef,'.b');
    xlabel('Hours of Day');
    ylabel('PDOP ECEF');
    title('PDOP ECEF');
    grid on;

    subplot(4,2,2);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,pdop_enu,'.b');
    xlabel('Hours of Day');
    ylabel('PDOP ENU');
    title('PDOP ENU');
    grid on;

    subplot(4,2,3);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,hdop_ecef,'.b');
    xlabel('Hours of Day');
    ylabel('HDOP ECEF');
    title('HDOP ECEF');
    grid on;

    subplot(4,2,4);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,hdop_enu,'.b');
    xlabel('Hours of Day');
    ylabel('HDOP ENU');
    title('HDOP ENU');
    grid on;

    subplot(4,2,5);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,vdop_ecef,'.b');
    xlabel('Hours of Day');
    ylabel('VDOP ECEF');
    title('VDOP ECEF');
    grid on;

    subplot(4,2,6);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,vdop_enu,'.b');
    xlabel('Hours of Day');
    ylabel('VDOP ENU');
    title('VDOP ENU');
    grid on;

    subplot(4,2,7);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,tdop,'.b');
    xlabel('Hours of Day');
    ylabel('TDOP');
    title('TDOP');
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

