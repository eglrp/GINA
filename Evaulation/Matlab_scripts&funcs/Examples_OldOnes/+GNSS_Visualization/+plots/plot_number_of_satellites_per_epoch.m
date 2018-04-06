function plot_number_of_satellites_per_epoch( conf, hod, prns_detail, filenamewithfullpath, figure_showup, subplot_height )
%==================================================================
% Count of PRNs
%==================================================================

% as the data is tranfered through the simulink bus with 1hz, there are
% epoch duplicates within 30s arcs according to the rinex sampling.
% By selecting unique epochs, we obtain a epoch value every 30s:

if conf.numbersatellites

    if isempty(hod) || isempty(prns_detail) || isempty(filenamewithfullpath) || isempty(subplot_height) || isempty(figure_showup) 
        warning('Some input arg are empty.')
        return
    end
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    pos = get(gca,'position');
    prn_sum_per_epoch = sum(prns_detail,2);

    scatter(hod,prn_sum_per_epoch,'.r');
    xlabel('Hours of Day');
    ylabel('Count of PRNs [#]');
    title('Number of satellites per epoch');
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

