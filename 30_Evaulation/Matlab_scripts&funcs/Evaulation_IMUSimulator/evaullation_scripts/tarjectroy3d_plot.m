function trajectroy3d_plot(t_data,varargin)
    f1 = figure('name','lat lon h / x y z ')
   
    if strcmp('llh',t_data.positionFormat) == 1
        plot3(t_data.lat,t_data.lon,t_data.h)
        
        title('Szélesség Hosszúság Magasság')
        xlabel('Szélesség [fok]')
        ylabel('Hosszúság [fok]')
        zlabel('Magasság [m]')
        
    end
    
    if strcmp('ecef',t_data.positionFormat) == 1
        plot3(t_data.x,t_data.y,t_data.z)
        
        title('ECEF koordináták')
        xlabel('x [m]')
        ylabel('y [m]')
        zlabel('z [m]')
        
    end
    
    %Save function if 2. argument exist.
    if max(size(varargin)) > 0 
        name = cell2mat(varargin(2));
        %path = pwd;
        %save_name = [path name];
        saveas(f1,name,'png');
    end

end 