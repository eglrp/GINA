function  t_data = make_trajectory_format(d_data,h_data)
if strcmp('GINA TRAJECTORY',h_data.type) == 1
    if strcmp('LLH',h_data.positionFormat) == 1
        
        t_data.t_time = [d_data(:,1) d_data(:,2)]
        t_data.lon = d_data(:,3);
        t_data.lat  = d_data(:,4);
        t_data.h = d_data(:,5);

    %ha hiányoznak a roll, pitch yaw adatok akkor, akkor a t_data-ban ezek az
    %elemek legyenek "empty"
    size_of_d_data = size(d_data);
        if size_of_d_data(2) == 5 
            t_data.roll  = [];
            t_data.pitch = [];
            t_data.yaw   = [];
        else
            t_data.roll  = d_data(:,6);
            t_data.pitch = d_data(:,7);
            t_data.yaw   = d_data(:,8);
            t_data.format = h_data.positionFormat;
        end
    end

    if strcmp('ECEF',h_data.positionFormat) == 1

        t_data.t_time = [d_data(:,1) d_data(:,2)]

        t_data.x = d_data(:,3);
        t_data.y = d_data(:,4);
        t_data.z = d_data(:,5);

        size_of_d_data = size(d_data);
        if size_of_d_data(2) == 5 
            t_data.roll  = [];
            t_data.pitch = [];
            t_data.yaw   = [];
        else
            t_data.roll  = d_data(:,6);
            t_data.pitch = d_data(:,7);
            t_data.yaw   = d_data(:,8);
            t_data.format = h_data.positionFormat;
        end
    end
end

if strcmp('GINA IMU BODY DATAS',h_data.type) == 1
        t_data.t_time = [d_data(:,1) d_data(:,2)]

        t_data.ax = d_data(:,3);
        t_data.ay = d_data(:,4);
        t_data.az = d_data(:,5);

        size_of_d_data = size(data);
        if size_of_d_data(2) == 5 
            t_data.wx  = [];
            t_data.wy = [];
            t_data.wz  = [];
        else
            t_data.wx  = d_data(:,6);
            t_data.wy = d_data(:,7);
            t_data.wz   = d_data(:,8);
            t_data.format = h_data.positionFormat;
        end
end

end 