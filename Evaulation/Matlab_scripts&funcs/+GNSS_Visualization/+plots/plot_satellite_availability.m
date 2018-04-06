function plot_satellite_availability( conf, hod, gnss_satelite_id_first, gnss_satelite_id_last, prns_detail, filenamewithfullpath, figure_showup)
%==================================================================
% PRN Availability
%==================================================================

if conf.satelliteavailability

    if isempty(hod) || isempty(prns_detail) || isempty(filenamewithfullpath) || isempty(gnss_satelite_id_first) || isempty(gnss_satelite_id_last) || isempty(figure_showup) 
        warning('Some input arg are empty.')
        return
    end
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')

        pos = get(gca,'position');
        pos(4) = 0.2;                               % plot subplot_height
        set(gca,'position',pos);
    end

    idx_subplot = 1;
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        subplot(16,2,idx_subplot);
        idx = find(prns_detail(:,i)>0);
        scatter(hod,prns_detail(:,i),'.b'); 
        hold on
        scatter(hod(idx),prns_detail(idx,i),'.r'); 
        ylim([0 1])
        set(gca,'ytick',0:1)                        % only integer labels along y axis 
        ylabel(sprintf('#%02d', i));
        grid on;
        idx_subplot = idx_subplot + 1;
    end
    clear idx_subplot
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

