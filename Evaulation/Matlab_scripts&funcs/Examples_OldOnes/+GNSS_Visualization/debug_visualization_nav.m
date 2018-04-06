function debug_visualization_nav(hod, nav, filename, dest)

    figure_showup = false;
    folder = ['Nav_' filename '\'];
    
    %'satPos','satVel',
    fieldnames = {'recTow','svId','gnssId','timeOfSignalTransmission_tsat','timeOfSignalTransmission_tgnss','satClockError','satClockErrorDrift'};
   
    for ifieldnames = 1:length(fieldnames)
        for isv = 1:32
           	
          	figure;
            if ~figure_showup
                set(gcf, 'Visible', 'off')
            end
            
            hold on
            scatter(hod,nav.Values(isv).(fieldnames{ifieldnames}).Data(:),'filled')       
            xlabel('Hours of Day');
            ylabel([fieldnames{ifieldnames}]);
            title([fieldnames{ifieldnames} '-' int2str(isv)]);
            grid on;      
            
            clear idx_subplot
            % Save to file
            % h=gcf;
            % set(h,'PaperOrientation','landscape');
            % set(h,'PaperUnits','normalized');
            % set(h,'PaperPosition', [0 0 1 1]);
            subfol = [dest.plotpath folder fieldnames{ifieldnames} '\'];
            if ~exist(subfol, 'dir')
                mkdir(subfol)
            end
            saveas(gcf,[subfol filename '_' fieldnames{ifieldnames} '_svId_' int2str(isv) '.png'])  
            close all
        end
    end

end
















