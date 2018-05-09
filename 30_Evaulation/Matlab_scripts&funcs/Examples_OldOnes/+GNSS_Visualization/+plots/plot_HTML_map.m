function [      filename_static_w_reference,...
                filename_kinematic_reference,...
                filename_without_reference              ] = plot_HTML_map(      conf, mode, hod, idx_common, lat_ref, lon_ref,  ...
                                                                                lat_kf, lon_kf, lat_lsq, lon_lsq,               ...
                                                                                filenamewpath, subplot_height, figure_showup )
%   PLOT_HTML_MAP Summary of this function goes here
%   Detailed explanation goes here
if conf.googlemaps
    filename_static_w_reference = [];
    filename_kinematic_reference = [];
    filename_without_reference = [];
    
    try
        outFile.name = filenamewpath;
        outFile.mode = mode;

        hours=round(hod);
        minutes=round((hod-hours)*60);
        seconds=round(((hod-hours)*60-minutes)*60);

        if ~isempty(idx_common)
            hours_common    = hours(idx_common(1));
            minutes_common  = minutes(idx_common(1));
            seconds_common  = seconds(idx_common(1));
        else
            lat_ref_ = [lat_ref, lat_ref-0.0005, lat_ref, lat_ref,       lat_ref, lat_ref+0.0005  ]';
            lon_ref_ = [lon_ref, lon_ref,       lon_ref, lon_ref-0.0005, lon_ref, lon_ref,       ]';

            hours_common    = repmat(hours(1),length(lat_ref_),1);
            minutes_common  = repmat(minutes(1),length(lat_ref_),1); 
            seconds_common  = repmat(seconds(1),length(lat_ref_),1); 
            
        end

        try 
            outFileStatic       = outFile;
            outFileStatic.name = [outFileStatic.name,'_w_static_reference'];
            filename_static_w_reference = [outFileStatic.name,'.html'];
            
            strcmp(mode,'static_w_reference')
            create_HTML(    outFileStatic, [lat_kf lon_kf hours minutes seconds],     ...
                                     [lat_lsq lon_lsq hours minutes seconds],   ...
                                     [lat_ref_ lon_ref_ hours_common minutes_common seconds_common],'KF','LSQ','Static_Reference')

        catch ME
            filename_static_w_reference = [];
            warning(ME.message)
        end
        try 
            outFileKinematic       = outFile;
            outFileKinematic.name = [outFileKinematic.name,'_w_kinematic_reference'];
            filename_kinematic_reference = [outFileKinematic.name,'.html'];
            
            create_HTML(    outFileKinematic, [lat_kf lon_kf hours minutes seconds],     ...
                                     [lat_lsq lon_lsq hours minutes seconds],   ...
                                     [lat_ref lon_ref hours_common minutes_common seconds_common],'KF','LSQ','Reference')
        catch ME
            filename_kinematic_reference = [];
            warning(ME.message)
        end
        try
            create_HTML(outFile, [lat_kf lon_kf hours minutes seconds],[lat_lsq lon_lsq hours minutes seconds],'KF','LSQ')
            filename_without_reference = [outFile.name,'.html'];
        catch ME
            filename_without_reference = [];
            warning(ME.message)
        end    
    catch ME
        warning(ME.message)
        warning('Error at the Google maps html plotting.')
    end
end

end

