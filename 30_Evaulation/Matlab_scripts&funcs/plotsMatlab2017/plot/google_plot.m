function google_plot(t_data,file_name)
    if strcmp(t_data.type,'PINA TRAJECTORY')~=1
        disp('Wrong type of data! Only trajectory type possible.')  
    end

    
    hours=round(hod);
    minutes=round((hod-hours)*60);
    seconds=round(((hod-hours)*60-minutes)*60);
    SoH =60; 
    SoD = 24*SoH;
    SoW = 7*SoD;
    t_data.time(:,1) = WN;
    t_data.time(:,2) = ToW;
    totalTime = ToW*WN;
    latng1=[t_data.lat t_data.lon];
    curves1 = 'curves1';
    create_HTML(file_name, latng1,curves1)
end