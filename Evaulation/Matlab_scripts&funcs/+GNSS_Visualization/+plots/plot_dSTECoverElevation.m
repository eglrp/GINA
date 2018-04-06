function [  fileNamePng,                ...
            fileNamePdf,                ...
            fileNameFig                         ] =  plot_dSTECoverElevation(   conf,                           ...
                                                                                filenamewithfullpath,           ...
                                                                                figure_showup,                  ...
                                                                                STECtime,                       ...
                                                                                STECdata,                       ...
                                                                                gnssIDSV,                       ...
                                                                                timeSV,                         ...
                                                                                elevationSV,                    ...
                                                                                gnss,                           ...
                                                                                xLabel,                         ...
                                                                                yLabel,                         ...
                                                                                Title,                          ...
                                                                                savePngOK,savePdfOK,saveFigOK)
                                                                            
    if ~conf
        fileNamePng = [];
        fileNamePdf = [];
        fileNameFig = [];
        return
    end  

    figure;

    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end
    
    elevation_sv_corr = NaN(length(STECtime),1);
    for i = 1 : length(STECtime)
        actTimeStep = STECtime(i); 
        foundIndeces = find(timeSV == actTimeStep & gnssIDSV == gnss);
        if ~isempty(foundIndeces)>0 
            elevation_sv_corr(i) = elevationSV(foundIndeces(1));
        end
    end
    
    STECdata(isnan(elevation_sv_corr)) = NaN; % return if plot would be empty
    if sum(~isnan(STECdata)) == 0
        fileNamePng = [];
        fileNamePdf = [];
        fileNameFig = [];
        return
    end
    
    plot(elevation_sv_corr,STECdata)
    xlabel(xLabel);
    ylabel(yLabel);

    title(Title)

    % Save to file
    if savePngOK
        try
            print(filenamewithfullpath,'-dpng')
            fileNamePng = [filenamewithfullpath,'.png'];
        catch ME
            warning(ME.message)
        end
    else
        fileNamePng = [];
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
    else
        fileNamePdf = [];
    end
    if saveFigOK
        try
            savefig(h,filenamewithfullpath)
            fileNameFig = [filenamewithfullpath,'.fig'];
        catch ME
            warning(ME.message)
        end
    else
        fileNameFig = [];
    end

    close all
end

