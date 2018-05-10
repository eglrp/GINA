% Create 2D or 3D plot and save in .png format (opcionally).
% enu_plot() require a trajectory data ( struct ) and it is calculate from
% trayectory data the actualy enu coordinates, and make a 2 or 3D plot.

%enu_plot(trajectory_data,'2D' or '3D' plot,'savename') 
function enu_plot(t_data,varargin) % ez jobb kezes ? 

    lat = t_data.startPosition_LLH(1);
    lon = t_data.startPosition_LLH(2);
    for i=1:length(t_data.x)
    %xyz2enu()-ban 10 és 11 sorból kivettem a %-t
    [e(i),n(i),u(i)]= xyz2enu(lat,lon,t_data.x(i), t_data.y(i), t_data.z(i));
    end 

    f1 = figure('Name','ENU');
    %2D or 3D plot,
    if size(varargin) > 0
        dimension = cell2mat(varargin(1));
        if strcmp('2D',dimension) == 1
            plot(e,n,'-*')
        end
        if strcmp('3D',dimension) == 1
            plot3(e,n,u)
        end
    end

    if size(varargin) == 0
      plot(e,n,'-*');
    end

    grid on; 

    %Title, lables
    title('East-North-Up koordináta rendszer,East-North vetülte')
    xlabel('east [m]')
    ylabel('north [m]')
    %If the differnce is too little, set the x,y ranges
    min_diff = 0.01 %[m]
    x_diff = max(e) - min(e);
    if x_diff < min_diff
        if max(e)> 0 %the moovment direction is pozitive 
            xmax = 2*max(e);
            xmin = 0;
            xlim([xmax xmin]);
        end

        if max(e)< 0%the moovment direction is negative
            xmax = 2*minx(e);
            xmin = 0;
            xlim([xmax xmin]);
        end 
    end 

    y_diff = max(n) - min(n);
    if y_diff < min_diff
          if max(n)> 0
            ymax = 2*max(n);
            ymin = 0;   
            ylim([ymax ymin]);
          end

          if max(n)< 0%the moovment direction is negative
            ymax = 2*minx(n);
            ymin = 0;
            ylim([ymax ymin]);
         end  
    end

    %Save function if 3. argument exist.
    if max(size(varargin))>= 2
        name = cell2mat(varargin(2));
        %path = pwd;
        %save_name = [path name];
        saveas(f1,name,'png');
    end

end


