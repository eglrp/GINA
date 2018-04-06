function [  fileNamePng,                ...
            fileNamePdf,                ...
            fileNameFig                         ] =  plot_raw_measurements( conf,                                           ...
                                                                            constelationId,                                 ...
                                                                            gnss_satelite_id_first, gnss_satelite_id_last,  ...
                                                                            GPSsecs,                                        ...
                                                                            C1,C2,                                          ...
                                                                            L1,L2,                                          ...
                                                                            D1,D2,                                          ...
                                                                            S1,S2,                                          ...
                                                                            prns,                                           ...
                                                                            gnssId,                                         ...
                                                                            filenamewithfullpath,                         ...
                                                                            figure_showup, subplot_height,                  ...
                                                                            saveformatLOGIC)
if ~conf.rawmeas
    return
end  

[   savePngOK, savePdfOK, saveFigOK	] 	= saveLogicCheck(saveformatLOGIC);

    secs = GPSsecs;
    days_sinceGpsWeek = secs/(24.0*3600.);
    days_sinceGpsWeek = round(days_sinceGpsWeek,10); % Round to 10th digit due to issues with the floor computation
    fulldays = floor(days_sinceGpsWeek);
    hod_raw = (days_sinceGpsWeek - fulldays)*24;

    figure;

    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    %C1
    subplot(4,2,1);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),C1(idxs),'.');
        ylabel('C1 [m]');
        title('C1 Signal');
        grid on;
        hold on;
    end
    %C2
    subplot(4,2,2);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),C2(idxs),'.');
        ylabel('C2 [m]');
        title('C2 Signal');
        grid on;
        hold on;
    end
    %L1
    subplot(4,2,3);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),L1(idxs),'.');
        ylabel('L1 [m]');
        title('L1 Signal');
        grid on;
        hold on;
    end
    %L2
    subplot(4,2,4);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),L2(idxs),'.');
        ylabel('L2 [m]');
        title('L2 Signal');
        grid on;
        hold on;
    end
    %D1
    subplot(4,2,5);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),D1(idxs),'.');
        ylabel('D1 [Hz]');
        title('D1 Signal');
        grid on;
        hold on;
    end
    %D2
    subplot(4,2,6);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),D2(idxs),'.');
        ylabel('D2 [Hz]');
        title('D2 Signal');
        grid on;
        hold on;
    end
    %S1
    subplot(4,2,7);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),S1(idxs),'.');
        xlabel('Hours of Day');
        ylabel('S1 [dbHz]');
        title('S1 Signal');
        grid on;
        hold on;
    end
    %S2
    subplot(4,2,8);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        searchCond = @(x,y) (x==i & y == constelationId);
        idxs    = find(searchCond(prns, gnssId));
        scatter(hod_raw(idxs),S2(idxs),'.');
        xlabel('Hours of Day');
        ylabel('S2 [dbHz]');
        title('S2 Signal');
        grid on;
        hold on;
    end

    % Save to file
    if savePngOK
        try
            print(filenamewithfullpath,'-dpng')
            fileNamePng = [filenamewithfullpath,'.png'];
        catch ME
            warning(ME.message)
        end
    end
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    
    if savePdfOK
        try
            print(filenamewithfullpath,'-dpdf')
            fileNamePdf = [filenamewithfullpath,'.pdf'];
        catch ME
            warning(ME.message)
        end
    end
    if saveFigOK
        try
            savefig(h,filenamewithfullpath)
            fileNameFig = [filenamewithfullpath,'.fig'];
       	catch ME
            warning(ME.message)
        end
    end
   
    close all


end


function [  savePngOK, ...
            savePdfOK, ...
            saveFigOK           ]      = saveLogicCheck(saveformatLOGIC)

     if isempty(saveformatLOGIC)
        savePngOK = true;      
        savePdfOK = true;      
        saveFigOK = true; 
   	 elseif islogical(saveformatLOGIC) || isvector(saveformatLOGIC)
        if length(saveformatLOGIC)==3
        	savePngOK = saveformatLOGIC(1);      
            savePdfOK = saveformatLOGIC(2);      
            saveFigOK = saveformatLOGIC(3); 
        end
     end
    
end
