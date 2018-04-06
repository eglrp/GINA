function [  fileNamePng,                ...
            fileNamePdf,                ...
            fileNameFig                         ] =  plot_dSTECoverTime     (   conf,                           ...
                                                                                filenamewithfullpath,           ...
                                                                                figure_showup,                  ...
                                                                                STECtime,                       ...
                                                                                STECdata,                       ...
                                                                                filter,                         ...
                                                                                gnssIDSV,                       ...
                                                                                timeSV,                         ...
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
    
    
    STECtime_corr = STECtime;
    
    if filter == true
        Length = length(STECtime);
        for i = 1: Length
            actTimeStep = STECtime(i);
            foundIndeces = find(timeSV == actTimeStep & gnssIDSV == gnss);
            if isempty(foundIndeces)
                    STECtime_corr(i) = NaN;
            end
            if i > 2
                if isnan(STECtime_corr(i)) && ~isnan(STECtime_corr(i-1)) && isnan(STECtime_corr(i-2))
                    STECtime_corr(i-2) = STECtime_corr(i-1);
                end
            end
        end
    end
    
    STECdata(isnan(STECtime_corr)) = NaN; % return if plot would be empty
    if sum(~isnan(STECdata)) == 0
        fileNamePng = [];
        fileNamePdf = [];
        fileNameFig = [];
        return
    end
    
    plot(STECtime_corr,STECdata)

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
