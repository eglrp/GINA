function [  fileNamePng,                ...
            fileNamePdf,                ...
            fileNameFig                         ] =  plot_raw_measurements_per_satellite(    plotcondidition,        ...
                                                                                            GPSsecs,              	...
                                                                                            prns,                   ...
                                                                                            rawObsData,                     ...
                                                                                            gnss_satelite_id_first, ...
                                                                                            gnss_satelite_id_last,  ...
                                                                                            filenamewithfullpath,   ...
                                                                                            figure_showup,          ...
                                                                                            saveformatLOGIC)

                                            
fileNamePdf = [];
fileNamePng = [];
fileNameFig = [];

if ~plotcondidition
   return
end

[ savePngOK, savePdfOK, saveFigOK ] 	= saveLogicCheck(saveformatLOGIC);    

    days_sinceGpsWeek   = GPSsecs/(24.0*3600.);
    days_sinceGpsWeek   = round(days_sinceGpsWeek,10);                      % Round to 10th digit due to issues with the floor computation
    fulldays            = floor(days_sinceGpsWeek);
    hod_raw             = (days_sinceGpsWeek - fulldays)*24;

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')

        pos = get(gca,'position');
        pos(4) = 0.2;                                                       % plot subplot_height
        set(gca,'position',pos);
    end
    
    idx_subplot = 1;
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        subplot(16,2,idx_subplot);
        idxs = find(prns==i);
        scatter(hod_raw(idxs),rawObsData(idxs),'.');
        set(gca,'ytick',0:1)                        % only integer labels along y axis 
        ylabel(sprintf('#%02d', i));
        grid on;
        idx_subplot = idx_subplot + 1;
        grid on;
        hold on;
    end
    
    clear idx_subplot
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

