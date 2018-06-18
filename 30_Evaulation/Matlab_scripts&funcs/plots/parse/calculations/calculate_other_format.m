function [new_h_data,new_d_data] = calculate_other_format(h_data,d_data)
wgs84 = wgs84Ellipsoid('meters');
new_d_data = d_data;
new_h_data = h_data;

new_h_data.startPosition_ECEF = [ NaN, NaN, NaN ];
new_h_data.startPosition_LLH = [ NaN, NaN, NaN ];

disp(d_data) %t_data = d_data
if strcmp('GINA TRAJECTORY',h_data.type) == 1
    %%LLH -> ECF
    if strcmp('LLH',h_data.positionFormat) == 1

        for i = 1 : max(size(d_data.lat))
            llh = [d_data.lat(i) d_data.lon(i) d_data.h(i)];
            xyz = ecef2geo(llh,1); %llh->ecef

            new_d_data.x(i,:) = xyz(1);
            new_d_data.y(i,:) = xyz(2);
            new_d_data.z(i,:) = xyz(3);
        end
        start_llh = [h_data.startPosition(1) h_data.startPosition(2) h_data.startPosition(3)];
        start_xyz =  ecef2geo(start_llh,1);

        new_h_data.startPosition_ECEF(1) = start_xyz(1);
        new_h_data.startPosition_ECEF(2) = start_xyz(2);
        new_h_data.startPosition_ECEF(3) = start_xyz(3);
        
        new_h_data.startPosition_LLH = h_data.startPosition;
    end

    %%ECEF -> LLH
    if strcmp('ECEF',h_data.positionFormat) == 1

        for i = 1 : max(size(d_data.x))
            xyz = [d_data.x(i) d_data.y(i) d_data.z(i)];
            llh = ecef2geo(xyz,0); %ecef->llh

            new_d_data.lat(i,:) = llh(1);
            new_d_data.lon(i,:) = llh(2);
            new_d_data.h(i,:) = llh(3);
        end
        start_xyz = [h_data.startPosition(1) h_data.startPosition(2) h_data.startPosition(3)];
        start_llh =  ecef2geo(start_xyz,0);

        new_h_data.startPosition_LLH(1) = start_llh(1);
        new_h_data.startPosition_LLH(2) = start_llh(2);
        new_h_data.startPosition_LLH(3) = start_llh(3);
        
        new_h_data.startPosition_ECEF = h_data.startPosition;
    end
end

if strcmp('GINA IMU BODY DATA',h_data.type) == 1
    %LLH -> ECEF
    if strcmp('LLH',h_data.positionFormat) == 1
        
        llh = [h_data.startPosition(1) h_data.startPosition(2) h_data.startPosition(3)];
        %xyz = lla2ecef(llh);
        xyz=ecef2geo(llh,2);
        
        new_h_data.startPosition_ECEF(1) = xyz(1);
        new_h_data.startPosition_ECEF(2) = xyz(2);
        new_h_data.startPosition_ECEF(3) = xyz(3);
        
        new_h_data.startPosition_LLH = h_data.startPosition;
        
    end
    
    %ECEF -> LLH
    if strcmp('ECEF',h_data.positionFormat) == 1
        
        [ new_h_data.startPosition_LLH(1),...
          new_h_data.startPosition_LLH(2),...
          new_h_data.startPosition_LLH(3) ]= ecef2geodetic( wgs84,...
                                            h_data.startPosition(1),...
                                            h_data.startPosition(2),...
                                            h_data.startPosition(3));
                                        
        new_h_data.startPosition_ECEF = h_data.startPosition;
  
    end  
end
    
end



