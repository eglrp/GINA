
%enu_plot(trajectory_data,'enu'/ '2D' / '3D' /,'savename') 
function enu_plot(t_data,varargin) % ez jobb kezes ? 

    lat = t_data.startPosition_LLH(1);
    lon = t_data.startPosition_LLH(2);
    
   % start_xyz = ecef2geo([t_data.startPosition(1),t_data.startPosition(2),t_data.startPosition(3)],1);
    for i=1:length(t_data.x)
        [e(i),n(i),u(i)]= xyz2enu(lat,lon,t_data.x(i), t_data.y(i), t_data.z(i));
    end 
    % Eltolás, magasség LLH -ból a High. 
    e = e - e(1);
    n = n - n(1);
    u = t_data.h;
    
    %2D or 3D plot,
     switch length(varargin)
        case 0
            disp('There is no mod.');
            save_function = false;
        case 1
            mod = cell2mat(varargin(1));
            save_function = false;
        case 2
            mod = cell2mat(varargin(1));
            png_name = cell2mat(varargin(2));
            save_function = true;
     end
      
    if strcmp('2D',mod) == 1
        f1 = figure('Name','ENU 2D');

        hold on
        p1 = plot(e, n, '-*', 'MarkerSize', 10,'LineWidth', 5, 'Color','g');
        p2 = plot(e(1), n(1), 'Marker', 'o', ...
            'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize',10 );
        p3 = plot(e(2:(end-1)), n(2:(end-1)), 'Marker', 'o', ...
            'Color', 'g', 'MarkerEdgeColor', 'b', 'MarkerSize',10 );
        p4 = line(e(end), n(end),'Marker', 's', ...
              'MarkerFaceColor', 'r', 'MarkerSize',10 );
        hold off
        h = [p2(1),p4(1)];
        %Title, lables
        grid on;
        title('East-North koordináták')
        xlabel('East [m]');
        ylabel('North [m]');

        %ranges
        range_x = max(e)-min(e);
        
        if(range_x == 0)
            range_x = 1;
        end
        
        xlim([min(e)-(range_x*0.1) max(e)+(range_x*0.55)])
        legend(h,'Start pont','Vég pont')
      
        
    end
    
    if strcmp('3D',mod) == 1
        f1 = figure('Name','ENU 3D');
        view(3);
        hold on
        p1 = plot3(e, n,u, '-*', 'MarkerSize', 10,'LineWidth', 5, 'Color','g');
        p2 = plot3(e(1), n(1),u(1), 'Marker', 'o', ...
            'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize',10 );
        p3 = plot3(e(2:(end-1)), n(2:(end-1)), u(2:(end-1)),'Marker', 'o', ...
            'Color', 'g', 'MarkerEdgeColor', 'b', 'MarkerSize',10 );
        p4 = line(e(end), n(end),u(end),'Marker', 's', ...
              'MarkerFaceColor', 'r', 'MarkerSize',10 );
        hold off
        h = [p2(1),p4(1)];
        %Title, lables
        grid on;
        title('East-North-Up koordináták')
        xlabel('East [m]');
        ylabel('North [m]');
        zlabel('North [m]');
        h = [p2(1),p4(1)];
        legend(h,'Start pont','Vég pont')
        %Title, lables
        grid on;
        title('East North Up koordináták')
        xlabel('East [m]');
        ylabel('North [m]');
        zlabel('Up [m]');
    end
       
    if strcmp('enu',mod) == 1
        f1 = figure('Name','East North Up');
        title('East North UP')
        subplot(3,1,1);
        %Plot 
        plot(e);
        %Lables&Title
        xlabel('Idõ[s]');
        ylabel('East [m]');
        title('East');
        grid on;

        subplot(3,1,2);
        %Plot
        plot(n);
        %Lables&Title
        xlabel('Idõ');
        ylabel('North [m]');
        title('North');
        grid on;
        
        subplot(3,1,3);
        %Plot
        plot(u);
        %Lables&Title
        xlabel('Idõ');
        ylabel('Up [m]');
        title('Up');
        grid on;
    end
    
    %Save function if 3. argument exist.
    if save_function == true
        %png_name = cell2mat(varargin(2));
        folder_of_save = '\result\';
        save_name_with_path = [pwd folder_of_save png_name];
        saveas(f1,[save_name_with_path '.png'])
        saveas(f1,[save_name_with_path,'.fig'])
    end
    

end



   
% 
%     grid on; 
%     from = 2;
%     step = 35;
%     to = length(n);
%     number_of_point = [from:step:to];
% 
%     line(e(1),n(1),t_data.h(1), 'Marker', 'o', ...
%     'Color', 'b', 'MarkerFaceColor', 'g', 'MarkerSize', 5);
% 
%     line(e(number_of_point),n(number_of_point),t_data.h(number_of_point),...
%     'Marker', 'o','Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize', 5);
% 
%     line(e(end),n(end),t_data.h(end), 'Marker', '>', ...
%     'Color', 'b', 'MarkerFaceColor', 'r', 'MarkerSize', 5);
% 
%     %Title, lables
%     title('East-North-Up koordináta rendszer,East-North vetülte')
%     xlabel('East [m]');
%     ylabel('North [m]');
%     zlabel('Up [m]');
% 
%     %set the axes of limits 
%     r = 0.15;
%     delta_e = max(e)-min(e);
%     delta_e= delta_e*r;
%     
%     delta_n = max(n)-min(n);
%     delta_n= delta_n*r;
%     
%     delta_u = max(u)-min(u);
%     delta_u= delta_u*r;
%     
%     distance = [delta_e, delta_n, delta_u]
%     
%     %steady state case || total distance < 1mm
%     if abs(distance) <= 1*10e-3    
%         xlim([-1 1]);
%         ylim([-1 1]);
%         zlim([-1 1]);
%     end
%     
%     %total disctance > 1mm 
%     if abs(distance) >= 1*10e-3  
%         xlim([min(e)-delta_e max(e)+delta_e]);
%         ylim([min(n)-delta_n max(n)+delta_n]);
%         zlim([min(u)-delta_u max(u)+delta_u]);
%         
%         xlim([min(e)-max_delta max(e)+max_delta]);
%         ylim([min(n)-max_delta max(n)+max_delta]);
%         zlim([min(u)-max_delta max(u)+max_delta]);
%     end
  
    
       %path = pwd;
        %save_name = [path name];
        %saveas(f1,name,'png');



% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
%  h_diff = max(abs(u)) - min(abs(u));
%     
%     %If the differnce is too little, set the x,y ranges
%     min_diff = 0.01 %[m]
%     x_diff = max(e) - min(e);
%     if x_diff < min_diff
%         if max(e)> 0 %the moovment direction is pozitive 
%             xmax = 2*max(e);
%             xmin = 0;
%             xlim([xmax xmin]);
%         end
% 
%         if max(e)< 0%the moovment direction is negative
%             xmax = 2*minx(e);
%             xmin = 0;
%             xlim([xmax xmin]);
%         end 
%     end 
% 
%     y_diff = max(n) - min(n);
%     if y_diff < min_diff
%           if max(n)> 0
%             ymax = 2*max(n);
%             ymin = 0;   
%             ylim([ymax ymin]);
%           end
% 
%           if max(n)< 0%the moovment direction is negative
%             ymax = 2*minx(n);
%             ymin = 0;
%             ylim([ymax ymin]);
%          end  
%     end

