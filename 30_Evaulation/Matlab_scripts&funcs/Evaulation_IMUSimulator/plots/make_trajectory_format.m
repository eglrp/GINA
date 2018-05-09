function  t_data = make_trajectory_format(data,format)
if strcmp('LLH',format) == 1
    
    t_data.time = [data(:,1) data(:,2)]
    
    t_data.lon = data(:,3);
    t_data.lat  = data(:,4);
    t_data.h = data(:,5);

    t_data.roll  = data(:,6);
    t_data.pitch = data(:,7);
    t_data.yaw   = data(:,8);
    
    t_data.format = format;
    
end

if strcmp('ECEF',format) == 1
    
    t_data.time = [data(:,1) data(:,2)]
    
    t_data.x = data(:,3);
    t_data.y = data(:,4);
    t_data.z = data(:,5);

    t_data.roll  = data(:,6);
    t_data.pitch = data(:,7);
    t_data.yaw   = data(:,8);
    
    t_data.format = format;
end

end 