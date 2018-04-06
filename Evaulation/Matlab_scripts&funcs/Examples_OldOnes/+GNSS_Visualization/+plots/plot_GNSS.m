function [  fileNamePng,                ...
            fileNamePdf,                ...
            fileNameFig,                ...
            outputfigureHanlde]                     = plot_GNSS(    plotcondidition,                                                ...
                                                                    time,                                                           ...
                                                                    first_coor,             second_coor,        third_coor,         ...
                                                                    time_common_w_ref,                                              ...
                                                                    first_coor_ref,         second_coor_ref,    third_coor_ref,     ...
                                                                    filenamewithfullpath,                                           ...
                                                                    subplot_height,         figure_showup,                      	...
                                                                    xlabelstr,                                                      ...          
                                                                    ylabelCell,                                                    	...   
                                                                    titleCell,                                                    	...                                                           
                                                                    plotlegendCell,                                                	... 
                                                                    ylimCell,                                                     	...
                                                                    saveformatLOGIC,                                                ...
                                                                    logic_mask,...
                                                                    figureHandle)
                                                            
%==================================================================
% 
%==================================================================


fileNamePdf = [];
fileNamePng = [];
fileNameFig = [];
isMaskValid = false;

if ~plotcondidition
   return
end

if ~isempty(logic_mask)
    if strcmp(class(logic_mask),'timeseries')
        isMaskValid = true;
    end
end
%% Input Validation
isValid0    = isDataExist(time, first_coor, second_coor, third_coor);
isValid1    = inputVectorValidation(time,              first_coor,     second_coor,        third_coor);
isValid2    = inputVectorValidation(time_common_w_ref, first_coor_ref, second_coor_ref,    third_coor_ref);
isValid3    = islogical(figure_showup);
isValid4    = isnumeric(subplot_height);
isValid5    = ischar(filenamewithfullpath);
plotlegendCell  = plotLegendValidation(plotlegendCell);

isValid = isValid0 & isValid1 & isValid2 & isValid3 & isValid4 & isValid5;

if ~isValid
    warning('Invalid input parameters for plotting.')
    return
end

[   savePngOK, savePdfOK, saveFigOK	] 	= saveLogicCheck(saveformatLOGIC);
    xlabelstr                         	= inputXLabelValidation(xlabelstr);
    ylabelCell                         	= inputYLabelValidation(ylabelCell);
    titleCell                         	= inputTitleStrValidation(titleCell);
    
    [   isylimValid, ...
        firstyYLimit, ...
        secondYLimit, ...
        thirdYLimit]                    = inputYLimValidation(ylimCell);
    
    referenceOK    = isDataExist(time_common_w_ref, first_coor_ref, second_coor_ref,    third_coor_ref);
%%
if isempty(figureHandle)
    figureHandle = figure;
else
    
end
    if ~figure_showup
        set(figureHandle, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos     = get(gca,'position'); 
    pos(4)  = subplot_height;
    set(gca,'position',pos);
    plot(time,first_coor,'.r')
    if referenceOK
        hold on
        plot(time_common_w_ref,first_coor_ref,'.g');
        hold off
        legend(plotlegendCell{1},plotlegendCell{2})
    else
        legend(plotlegendCell{1})
    end
    if isylimValid(1)
        ylim([firstyYLimit{1} firstyYLimit{2}])
    end
    if isMaskValid
        hold on
        plot(logic_mask.Time,logic_mask.Data,'.b');
        hold off
        if referenceOK
            legend(plotlegendCell{1},plotlegendCell{2},'Validation Mask')
        else
            legend(plotlegendCell{1},'Validation Mask')
        end
    end
    xlabel(xlabelstr);
    ylabel(ylabelCell{1});
    title(titleCell{1});
    grid on;

    subplot(3,1,2)
    pos     = get(gca,'position'); 
    pos(4)  = subplot_height;
    set(gca,'position',pos);
    plot(time,second_coor,'.r')
    if referenceOK
        hold on
        plot(time_common_w_ref,second_coor_ref,'.g');
        hold off
        legend(plotlegendCell{1},plotlegendCell{2})
    else
        legend(plotlegendCell{1})
    end
    if isylimValid(2)
        ylim([secondYLimit{1} secondYLimit{2}])
    end
    if isMaskValid
        hold on
        plot(logic_mask.Time,logic_mask.Data,'.b');
        hold off
        if referenceOK
            legend(plotlegendCell{1},plotlegendCell{2},'Validation Mask')
        else
            legend(plotlegendCell{1},'Validation Mask')
        end
    end
    xlabel(xlabelstr);
    ylabel(ylabelCell{2});
    title(titleCell{2});
    grid on;

    subplot(3,1,3)
    pos     = get(gca,'position'); 
    pos(4)  = subplot_height;
    set(gca,'position',pos);
    plot(time,third_coor,'.r')
    if referenceOK
        hold on
        plot(time_common_w_ref,third_coor_ref,'.g');
        hold off
        legend(plotlegendCell{1},plotlegendCell{2})
    else
        legend(plotlegendCell{1})
    end
    if isylimValid(3)
        ylim([thirdYLimit{1} thirdYLimit{2}])
    end
    if isMaskValid
        hold on
        plot(logic_mask.Time,logic_mask.Data,'.b');
        hold off
        if referenceOK
            legend(plotlegendCell{1},plotlegendCell{2},'Validation Mask')
        else
            legend(plotlegendCell{1},'Validation Mask')
        end
    end
    xlabel(xlabelstr);
    ylabel(ylabelCell{3});
    title(titleCell{3});
    grid on;
    
    % End ECEF LSQ -------------------
    % Save to file
    if savePngOK
        try
            print(filenamewithfullpath,'-dpng')
            fileNamePng = [filenamewithfullpath,'.png'];
        catch ME
            warning(ME.message)
        end
    end
    h=figureHandle;
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
    outputfigureHanlde = h;
%     close all

end

function isThereData = isDataExist(time, first_coor, second_coor, third_coor)
    isThereData = true;
    if isempty(time) || isempty(first_coor) || isempty(second_coor) || isempty(third_coor)
        isThereData = false;
    end
end
function isValid = inputVectorValidation(time, first_coor, second_coor, third_coor)
    isValid = true;

    if length(time) ~= length(first_coor) && length(time) ~=  length(second_coor) && length(time) ~=  length(third_coor)
        isValid = false;
    end

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
function outPlotlegend  = plotLegendValidation(plotlegendCell)

outPlotlegend = {};
    if ischar(plotlegendCell)
        outPlotlegend{end+1} = plotlegendCell;
        outPlotlegend{end+1} = 'Missing plot legend';
    end
    
	if iscell(plotlegendCell) && ~isempty(plotlegendCell)
        if ischar(plotlegendCell{1})
            outPlotlegend{end+1} = plotlegendCell{1};
        else
            outPlotlegend{end+1} = 'Missing plot legend';
        end
        if length(plotlegendCell)>=2
            if ischar(plotlegendCell{2})
                outPlotlegend{end+1} = plotlegendCell{2};
            end
        else
            outPlotlegend{end+1} = 'Missing plot legend';
        end
    else
     	outPlotlegend{end+1} = 'Missing plot legend';
        outPlotlegend{end+1} = 'Missing plot legend';
	end

end
function xlabelstr	= inputXLabelValidation(xlabelstr)
    if ~ischar(xlabelstr)
        xlabelstr = 'Missing x label legend';
    end
end
function ylabelCell = inputYLabelValidation(ylabelCell)
    
    if iscell(ylabelCell)
        if length(ylabelCell) == 3
            if all(cellfun(@(x) ischar(x),ylabelCell))
                validYLabel = true;
            end
        end
    end
    if validYLabel==false
        ylabelCell = {};
        ylabelCell{end+1} = 'y label';
        ylabelCell{end+1} = 'y label';
        ylabelCell{end+1} = 'y label';
    end
    
end
function titleCell = inputTitleStrValidation(titleCell)
    
    if iscell(titleCell)
        if length(titleCell) == 3
            if all(cellfun(@(x) ischar(x),titleCell))
                validTitle = true;
            end
        end
    end
    if validTitle==false
        titleCell = {};
        titleCell{end+1} = 'Title Missing';
        titleCell{end+1} = 'Title Missing';
        titleCell{end+1} = 'Title Missing';
    end
    
end
function [isylimValid, firstyYLimit,secondYLimit, thirdYLimit] = inputYLimValidation(ylimCell)
    isylimValid = [false, false, false];
    firstyYLimit    = [];
    secondYLimit    = [];
    thirdYLimit     = [];
     if iscell(ylimCell)
         if length(ylimCell) == 3
            isylimValid1 = inputYLimCellValidation(ylimCell{1});
            isylimValid2 = inputYLimCellValidation(ylimCell{2});
            isylimValid3 = inputYLimCellValidation(ylimCell{3});
            isylimValid = [isylimValid1, isylimValid2, isylimValid3];
         end
     end
     if isylimValid(1)
         firstyYLimit = ylimCell{1};
     end
     if isylimValid(2) 
         secondYLimit = ylimCell{2};
     end
     if isylimValid(3) 
         thirdYLimit = ylimCell{3};
     end
end
function isylimValid= inputYLimCellValidation(ylimCell)
    isylimValid = false;
    
    if iscell(ylimCell)
        if length(ylimCell) == 2
            if all(cellfun(@(x) isnumeric(x),ylimCell))
                isylimValid = true;
            end
        end
    end   
end



