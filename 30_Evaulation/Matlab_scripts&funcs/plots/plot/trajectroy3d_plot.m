
% trajectroy3d_plot(t_data, llh/ecef, llh/xyz/2d/3d, save name )
%trajectroy3d_plot(t_data,'llh','llh','circle_trajectroy3d_llh_llh_plot');
function trajectroy3d_plot(t_data,varargin)
   
    switch length(varargin)
         case 1 
            position_format = cell2mat(varargin(1));
            mod = 'Not a Mod';
            save_function = false;
        case 2
            position_format = cell2mat(varargin(1));
            mod = cell2mat(varargin(2));
            save_function = false;
        case 3
            position_format = cell2mat(varargin(1));
            mod = cell2mat(varargin(2));
            png_name = cell2mat(varargin(3));
            save_function = true;
    end
    
    if strcmp('llh',position_format) == 1
        if strcmp('llh',mod) == 1
            f1 = figure('Name','Lat Lon High');
            title('Lateral Longitudnila High')
            subplot(3,1,1);
            %Plot 
            plot(t_data.lat);
            %Lables&Title
            xlabel('Idõ[s]');
            ylabel('Lat [rad]');
            title('Lat');
            grid on;

            subplot(3,1,2);
            %Plot
            plot(t_data.lon);
            %Lables&Title
            xlabel('Idõ');
            ylabel('Lon [rad]');
            title('Longitudnial');
            grid on;

            subplot(3,1,3);
            %Plot
            plot(t_data.h);
            %Lables&Title
            xlabel('Idõ');
            ylabel('High [m]');
            title('High');
            grid on;
        end
        if strcmp('2D',mod) == 1
            lat = t_data.lat;
            lon = t_data.lon;
            h = t_data.h;

            f1 = figure('name','lat lon h');
           
           
            p1 = plot(lat,lon, '-*', 'MarkerSize', 10,'LineWidth', 5, 'Color','g');
            hold on 
            p2 = plot(lat(1),lon(1),'Marker', 'o', ...
                'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize',10 );
            p3 = plot(lat(2:(end-1)),lon(2:(end-1)), 'Marker', 'o', ...
                'Color', 'g', 'MarkerEdgeColor', 'b', 'MarkerSize',10 );
            p4 = line(lat(end), lon(end),'Marker', 's', ...
                  'MarkerFaceColor', 'r', 'MarkerSize',10 );
            hold off
            grid on;
            title('LLH koordináták')
            xlabel('lat [fok]')
            ylabel('lon [fok]')
            zlabel('h [m]')

%             range_z = max(h)-min(h);
%             zlim( [(min(h)-(range_z*0.2))-3 max(h)+(range_z*0.2)+3])

            h = [p2(1),p4(1)];
            legend(h,'Strat pont','Vég pont')
        end
        
        
        if strcmp('3D',mod) == 1
            lat = t_data.lat;
            lon = t_data.lon;
            h = t_data.h;

            f1 = figure('name','lat lon h');
            view(3);
            hold on 
            grid on;
            p1 = plot3(lat,lon,h, '-*', 'MarkerSize', 10,'LineWidth', 5, 'Color','g');
            p2 = plot3(lat(1),lon(1),h(1),'Marker', 'o', ...
                'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize',10 );
            p3 = plot3(lat(2:(end-1)),lon(2:(end-1)),h(2:(end-1)), 'Marker', 'o', ...
                'Color', 'g', 'MarkerEdgeColor', 'b', 'MarkerSize',10 );
            p4 = line(lat(end), lon(end),h(end),'Marker', 's', ...
                  'MarkerFaceColor', 'r', 'MarkerSize',10 );

            hold off

            title('LLH koordináták')
            xlabel('lat [fok]')
            ylabel('lon [fok]')
            zlabel('h [m]')

            range_z = max(h)-min(h);
            zlim( [(min(h)-(range_z*0.2))-3 max(h)+(range_z*0.2)+3])

            h = [p2(1),p4(1)];
            legend(h,'Strat pont','Vég pont')
        end
    end
    
    if strcmp('ecef',position_format) == 1
        if strcmp('xyz',mod) == 1
            f1 = figure('Name','Lat Lon High');
            title('Lateral Longitudnila High')
            
            subplot(3,1,1);
            %Plot 
            plot(t_data.x);
            %Lables&Title
            xlabel('Idõ[s]');
            ylabel('X [m]');
            title('X_{ECEF}');
            grid on;
            
            subplot(3,1,2);
            %Plot
            plot(t_data.y);
            %Lables&Title
            xlabel('Idõ');
            ylabel('Y [m]');
            title('Y_{ECEF}');
            grid on;
            
            subplot(3,1,3);
            %Plot
            plot(t_data.z);
            %Lables&Title
            xlabel('Idõ');
            ylabel('Z [m]');
            title('Z_{ECEF}');
            grid on;
        end
        if strcmp('3D',mod) == 1
            f1 = figure('name','ECEF 3D');
            view(3);
            x = t_data.x;
            y = t_data.y;
            z = t_data.z;

            hold on 
            grid on;
            p1 = plot3(x,y,z, '-*', 'MarkerSize', 10,'LineWidth', 5, 'Color','g');
            p2 = plot3(x(1),y(1),z(1),'Marker', 'o', ...
                'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize',10 );
            p3 = plot3(x(2:(end-1)),y(2:(end-1)),z(2:(end-1)), 'Marker', 'o', ...
                'Color', 'g', 'MarkerEdgeColor', 'b', 'MarkerSize',10 );
            p4 = line(x(end), y(end),z(end),'Marker', 's', ...
                  'MarkerFaceColor', 'r', 'MarkerSize',10 );
            hold off
            grid on;     
            title('ECEF koordináták')
            xlabel('x [m]')
            ylabel('y [m]')
            zlabel('z [m]')

            h = [p2(1),p4(1)];
            legend(h,'Strat pont','Vég pont')  
        end
    end
    
    if save_function == true
        %png_name = cell2mat(varargin(3));
        folder_of_save = '\result\';
        save_name_with_path = [pwd folder_of_save png_name];
        saveas(f1,[save_name_with_path '.png'])
        saveas(f1,[save_name_with_path,'.fig'])
    end 
    %Save function if 3. argument exist.
%     if max(size(varargin)) >=2 
%         png_name = cell2mat(varargin(2));
%        
%         folder_of_save = '\result\';
%         save_name_with_path = [pwd folder_of_save png_name];
%         saveas(f1,[save_name_with_path '.png'])
%         saveas(f1,[save_name_with_path,'.fig'])
%     end

end 




%     
%     tmpAspect=daspect();
%     daspect(tmpAspect([ 2 2 2]))

    
%     
%     
%     x=t_data.x;
%     y=t_data.y;
%     z=t_data.z;
%     
%     min_diff = 0.01; %[m]
%     x_diff = max(x) - min(x);
%     if x_diff < min_diff
%         if max(x)> 0 %the moovment direction is pozitive 
%             xmax = 2*max(x);
%             xmin = 0;
%             xlim([xmax xmin]);
%         end
% 
%         if max(x)< 0%the moovment direction is negative
%             xmax = 2*minx(x);
%             xmin = 0;
%             xlim([xmax xmin]);
%         end 
%     end 
% 
%     y_diff = max(y) - min(y);
%     if y_diff < min_diff
%           if max(y)> 0
%             ymax = 2*max(y);
%             ymin = 0;   
%             ylim([ymax ymin]);
%           end
% 
%           if max(y)< 0%the moovment direction is negative
%             ymax = 2*minx(y);
%             ymin = 0;
%             ylim([ymax ymin]);
%          end  
%     end
%     
%     z_diff = max(z) - min(z);
%     if z_diff < min_diff
%           if max(z)> 0
%             zmax = 2*max(n);
%             zmin = 0;   
%             zlim([zmax zmin]);
%           end
% 
%           if max(z)< 0%the moovment direction is negative
%             zmax = 2*min(z);
%             zmin = 0;
%             zlim([zmax zmin]);
%          end  
%     end
%     
%     
    
    
    
    