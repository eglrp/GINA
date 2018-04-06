function plotGoogleMaps(isPlot,             mode,       hod,    ...
                        lat_ref,            lon_ref,            ...
                        lat_kf,             lon_kf,             ...
                        lat_lsq,            lon_lsq,            ...
                        fileName,                               ...
                        idx_common)

if isPlot == false   
    return 
end
   
    
    try
        outFile.name =  fileName;   
        outFile.mode = mode;        

        hours=floor(hod);
        minutes=floor((hod-hours)*60);
        seconds=floor(((hod-hours)*60-minutes)*60);

        if ~isempty(idx_common)
            hours_common    = hours(idx_common);
            minutes_common  = minutes(idx_common);
            seconds_common  = seconds(idx_common);
        else
            lat_ref_ = [lat_ref, lat_ref-0.0005, lat_ref, lat_ref,       lat_ref, lat_ref+0.0005  ]';
            lon_ref_ = [lon_ref, lon_ref,       lon_ref, lon_ref-0.0005, lon_ref, lon_ref,       ]';

            hours_common    = repmat(hours(1),length(lat_ref_),1);
            minutes_common  = repmat(minutes(1),length(lat_ref_),1); 
            seconds_common  = repmat(seconds(1),length(lat_ref_),1); 
            % Temporary data cosmetic for the google maps
            if length(lat_kf)>1
                lat_kf(1)   = lat_kf(2);
                lon_kf(1)   = lon_kf(2);
                lat_lsq(1)  = lat_lsq(2);
                lon_lsq(1)  = lon_lsq(2);
            end
        end

        if strcmp(mode,'static_w_reference')
            lat_lsq = reshape(lat_lsq,[],1);
            lon_lsq = reshape(lon_lsq,[],1);
            hours = reshape(hours,[],1);
            minutes = reshape(minutes,[],1);
            seconds = reshape(seconds,[],1);
            lat_kf = reshape(lat_kf,[],1);
            lon_kf = reshape(lat_lsq,[],1);
            lat_ref_ = reshape(lat_ref_,[],1);
            lon_ref_ = reshape(lon_ref_,[],1);
            hours_common = reshape(hours_common,[],1);
            minutes_common = reshape(minutes_common,[],1);
            seconds_common = reshape(seconds_common,[],1);            
            
            try
                outFile.name = [outFile.name '_justLLH'];
                create_HTML(    outFile, [lat_lsq lon_lsq hours minutes seconds],   ...
                                         [lat_ref_ lon_ref_ hours_common minutes_common seconds_common],'LSQ','ITRF08/Epoch05')
            catch
                warning('The LLH LSQ html plotting had an error.')
            end
            try
                outFile.name = [outFile.name '_justKF'];
                create_HTML(    outFile, [lat_kf lon_kf hours minutes seconds],     ...  
                                         [lat_ref_ lon_ref_ hours_common minutes_common seconds_common],'KF','ITRF08/Epoch05')
            catch
                warning('The LLH KF html plotting had an error.')
            end
            try
                outFile.name = [outFile.name '_LLHandKF'];
                create_HTML(    outFile, [lat_kf lon_kf hours minutes seconds],     ...
                                         [lat_lsq lon_lsq hours minutes seconds],   ...
                                         [lat_ref_ lon_ref_ hours_common minutes_common seconds_common],'KF','LSQ','ITRF08/Epoch05')
            catch
                warning('The LLH KF with LLH LSQ html plotting had an error.')
            end

        elseif strcmp(mode,'kinematic_w_reference')
            create_HTML(    outFile, [lat_kf lon_kf hours minutes seconds],     ...
                                     [lat_lsq lon_lsq hours minutes seconds],   ...
                                     [lat_ref lon_ref hours_common minutes_common seconds_common],'KF','LSQ','iMAR')
        else    
            create_HTML(outFile, [lat_kf lon_kf hours minutes seconds],[lat_lsq lon_lsq hours minutes seconds],'KF','LSQ')
        end    
    catch
        warning('Error at the Google maps html plotting.')
    end
    
end