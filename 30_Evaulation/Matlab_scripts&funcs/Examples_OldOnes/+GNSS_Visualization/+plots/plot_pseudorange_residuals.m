function plot_pseudorange_residuals( conf,hod,pseudorange_residuals_KF,maxnumber_of_satelites, subfol, filename, figure_showup, subplot_height)
%==================================================================
% Pseudorange residuals per SVid -------------------
%==================================================================

% The residuals are plotted on multiple pdfs

if conf.pseudorange_residuals
    if ~exist(subfol, 'dir')
        mkdir(subfol)
    end

    pdfnr = 0;
    figure_index = 1;
    for i=1:maxnumber_of_satelites

        % initialize new figure
        if mod(figure_index-1,3)==0
            figure;
            if ~figure_showup
                set(gcf, 'Visible', 'off')
            end
            pos = get(gca,'position'); 
            pos(4) = subplot_height;
            set(gca,'position',pos);
            plotidx = 1;
        end

        subplot(3,1,plotidx);
        scatter(hod,pseudorange_residuals_KF(:,i),'.');
        hold on
        xlabel('Hours of Day');
        ylabel('\Delta\rho [m]');
        title(sprintf('Pseudorange residuals (KF) #%02d',i));
        grid on;

        % after every third subplot, save to file
        if mod(figure_index,3)==0 && i~=1
            h=gcf;
            set(h,'PaperOrientation','landscape');
            set(h,'PaperUnits','normalized');
            set(h,'PaperPosition', [0 0 1 1]);
            filenamewfullpath = fullfile(subfol,filename);
            print([filenamewfullpath sprintf('_PRN_%02d-%02d',i-2,i)],'-dpdf')
            print([filenamewfullpath sprintf('_PRN_%02d-%02d',i-2,i)],'-dpng')
            close all
            pdfnr=pdfnr+1;
        end

        plotidx=plotidx+1;
        figure_index = figure_index + 1;

    end
    clear figure_index

end

end

