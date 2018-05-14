%plotting data with ecef2enu() function
function enu2_plot(t_data,varargin)
    lat0 = t_data.startPosition(1);
    lon0 = t_data.startPosition(2);
    h0 = t_data.startPosition(3);
%%
%llh->ecef
    for i=1:length(t_data.lat) 
    xyz(i,:) = ecef2geo([t_data.lat(i), t_data.lon(i), t_data.h(i)],1); % geo -> ecef 
    end

    x = xyz(:,1);
    y = xyz(:,2);
    z = xyz(:,3);
    %geodetic2enu()
%%
%ecef->enu
    spheroid = referenceEllipsoid('WGS 84');

    [e,n,u] = ecef2enu(x,y,z,lat0,lon0,h0,spheroid,'radians');
%%
%plot 
    figure('name','ENU');
    %2D or 3D plot,
    if size(varargin) > 0
        dimension = cell2mat(varargin(1));
        if strcmp('2D',dimension) == 1
            plot(e,n,'-*')
        end
        if strcmp('3D',dimension) == 1
            plot3(e,n,t_data.h,'-*')
        end
    else    
         plot(e,n,'-*');
    end
    grid on;
    
    %Matching axes scales
    tmpAspect=daspect();
    daspect(tmpAspect([1 2 2]))
    h_diff = max(abs(u)) - min(abs(u));

%%
%labeles,titles...stb
    title('ENU koordinátarendszer');
    xlabel('East [m]');
    ylabel('North [m]');
    zlabel('Up [m]');
%%
%Save function if 3. argument exist.
    if max(size(varargin))>= 2
        name = cell2mat(varargin(2));
        %path = pwd;
        %save_name = [path name];
        saveas(f1,name,'png');
    end

end