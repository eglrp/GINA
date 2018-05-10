function trajectroy3d_plot(t_data,varargin)
    f1 = figure('name','lat lon h / x y z ');
    
    position_format = cell2mat(varargin(1));
    
    if strcmp('llh',position_format) == 1
        plot3(t_data.lat,t_data.lon,t_data.h)
        grid on;
        
        title('Szélesség Hosszúság Magasság')
        xlabel('Szélesség [fok]')
        ylabel('Hosszúság [fok]')
        zlabel('Magasság [m]')
        
    end
    
    if strcmp('ecef',position_format) == 1
        plot3(t_data.x,t_data.y,t_data.z)
        grid on;
        
        title('ECEF koordináták')
        xlabel('x [m]')
        ylabel('y [m]')
        zlabel('z [m]')    
    end
    
    
    
    
    x=t_data.x;
    y=t_data.y;
    z=t_data.z;
    
    min_diff = 0.01; %[m]
    x_diff = max(x) - min(x);
    if x_diff < min_diff
        if max(x)> 0 %the moovment direction is pozitive 
            xmax = 2*max(x);
            xmin = 0;
            xlim([xmax xmin]);
        end

        if max(x)< 0%the moovment direction is negative
            xmax = 2*minx(x);
            xmin = 0;
            xlim([xmax xmin]);
        end 
    end 

    y_diff = max(y) - min(y);
    if y_diff < min_diff
          if max(y)> 0
            ymax = 2*max(y);
            ymin = 0;   
            ylim([ymax ymin]);
          end

          if max(y)< 0%the moovment direction is negative
            ymax = 2*minx(y);
            ymin = 0;
            ylim([ymax ymin]);
         end  
    end
    
    z_diff = max(z) - min(z);
    if z_diff < min_diff
          if max(z)> 0
            zmax = 2*max(n);
            zmin = 0;   
            zlim([zmax zmin]);
          end

          if max(z)< 0%the moovment direction is negative
            zmax = 2*min(z);
            zmin = 0;
            zlim([zmax zmin]);
         end  
    end
    
    
    
    
    
    
    
    
    %Save function if 3. argument exist.
    if max(size(varargin)) >=2 
        name = cell2mat(varargin(2));
        %path = pwd;
        %save_name = [path name];
        saveas(f1,name,'png');
    end

end 