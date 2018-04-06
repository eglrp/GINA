function debug_visualization_eph(hod, eph, filename, dest)

    figure_showup = false;
    folder = ['Eph_' filename '\'];

    %'satPos','satVel',
    fieldnames = {'svId','af2','M0','roota','deltan','ecc','omega','cuc','cus','crc','crs','i0','idot','cic','cis','Omega0','Omegadot','toe',...
                         'af0','af1','WN','toc_in_TW','tgd'};
   
    for ifieldnames = 1:length(fieldnames)
        for isv = 1:32
           	
          	figure;
            if ~figure_showup
                set(gcf, 'Visible', 'off')
            end
            
            hold on
            scatter(hod,eph.Values(isv).(fieldnames{ifieldnames}).Data(:),'filled')       
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
















