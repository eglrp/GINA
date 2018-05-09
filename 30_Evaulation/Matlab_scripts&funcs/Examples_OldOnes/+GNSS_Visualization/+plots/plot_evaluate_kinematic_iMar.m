function plot_evaluate_kinematic_iMar(  conf,       mode,       hod,            ...
                                        lat_ref,    lon_ref,	h_ref,          ...
                                        x_ref,      y_ref,      z_ref,          ...    
                                        lat_lsq,    lon_lsq,    h_lsq,       	...
                                        x_lsq,      y_lsq,      z_lsq,          ...
                                        lat_kf,     lon_kf,     h_kf,           ...
                                        x_kf,       y_kf,       z_kf,           ...
                                        filenamewithfullpath_kinematic_iMAR_LLH_residuals,      ...
                                        filenamewithfullpath_kinematic_iMAR_ECEF_residuals,     ...
                                        filenamewithfullpath_kinematic_iMAR_ENU_residuals,      ...
                                        filename,                                               ...
                                        ylimCell,                                               ...
                                        subplot_height, figure_showup, formattype )
%==================================================================
%==================================================================
%==================================================================
% KINEMATIC : Estimated vs. iMar position
%==================================================================
%==================================================================
%==================================================================

if ~strcmp(mode,'kinematic_w_reference')
    return
end

    [   isylimValid, ...
        firstyYLimit, ...
        secondYLimit, ...
        thirdYLimit]                    = inputYLimValidation(ylimCell);
    
    
    
    dlat_kf = lat_kf - lat_ref;
    dlon_kf = lon_kf - lon_ref;
    dh_kf   = h_kf - h_ref;
    
    dlat_lsq    = lat_lsq - lat_ref;
    dlon_lsq    = lon_lsq - lon_ref;
    dh_lsq      = h_lsq - h_ref;
    
    dx_kf = x_kf - x_ref;
    dy_kf = y_kf - y_ref;
    dz_kf = z_kf - z_ref;
    
    dx_lsq = x_lsq - x_ref;
    dy_lsq = y_lsq - y_ref;
    dz_lsq = z_lsq - z_ref;
    
    [de_kf, dn_kf, du_kf]       = xyz2enu(lat_ref, lon_ref, dx_kf,  dy_kf,  dz_kf);
    [de_lsq, dn_lsq, du_lsq]    = xyz2enu(lat_ref, lon_ref, dx_lsq, dy_lsq, dz_lsq);

    hod_common = hod(:);
    
    
    %==================================================================
    % KINEMATIC: Estimated vs. iMar position in LLH -------------------
    %==================================================================    
    
    if conf.kfdllh_kinematic
        
        figure;
        if ~figure_showup
            set(gcf, 'Visible', 'off')
        end

        % LLH
        subplot(3,1,1)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, dlat_kf,'.r');
        hold on;
        s2=plot(hod_common, dlat_lsq,'.b');
        legend([s1,s2],'KF','LSQ');
         if isylimValid(1)
            ylim([firstyYLimit{1} firstyYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaLat [deg]');
        title(sprintf('dLat (KF) w.r.t. iMAR: avg=%3.3f*1e-5, stddev=%3.3f*1e-5\ndLat (LSQ)  w.r.t. iMAR: avg=%3.3f*1e-5, stddev=%3.3f*1e-5',mean(dlat_kf)*1e5,std(dlat_kf)*1e5,mean(dlat_lsq)*1e5,std(dlat_lsq)*1e5));
        grid on;

        subplot(3,1,2)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, dlon_kf,'.r');
        hold on;
        s2=plot(hod_common, dlon_lsq,'.b');    
        legend([s1,s2],'KF','LSQ');
         if isylimValid(1)
            ylim([firstyYLimit{1} firstyYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaLon [deg]');
        title(sprintf('dLon (KF) w.r.t. iMAR: avg=%3.3f*1e-5, stddev=%3.3f*1e-5\ndLon (LSQ) w.r.t. iMAR: avg=%3.3f*1e-5, stddev=%3.3f*1e-5',mean(dlon_kf)*1e5,std(dlon_kf)*1e5,mean(dlon_lsq)*1e5,std(dlon_lsq)*1e5));
        grid on;

        subplot(3,1,3)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, dh_kf,'.r');
        hold on;
        s2=plot(hod_common, dh_lsq,'.b');
        legend([s1,s2],'KF','LSQ');
         if isylimValid(1)
            ylim([firstyYLimit{1} firstyYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaH [m]');
        title(sprintf('dH (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndH (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(dh_kf),std(dh_kf),mean(dh_lsq),std(dh_lsq)));
        grid on;

        % Save to file
        print(filenamewithfullpath_kinematic_iMAR_LLH_residuals,'-dpng')
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        savefig(h,filenamewithfullpath_kinematic_iMAR_LLH_residuals)
        
        for i = 1 : length(formattype)
            if ~isempty(formattype{i})
                if strcmp(formattype{i}, '-djpeg') || strcmp(formattype{i}, '-dpng') || strcmp(formattype{i}, '-dtiff') || strcmp(formattype{i}, '-dpdf') || strcmp(formattype{i}, '-deps')
                    if formattype{i} == '-dpng'
                        set(h,'PaperOrientation','portrait');
                    else
                        set(h,'PaperOrientation','landscape');
                    end
                    print(filenamewithfullpath_kinematic_iMAR_LLH_residuals, formattype{i})
                else
                    warning('Unknown formattype, default is used: pdf')
                    print(filenamewithfullpath_kinematic_iMAR_LLH_residuals,'-dpdf')
                end    
            end
        end
        close all
        
    end
    
    %==================================================================
    % KINEMATIC: Estimated vs. iMar position in ECEF -------------------
    %==================================================================    

    if conf.kfdecef_kinematic
        
        % ECEF
        figure;
        if ~figure_showup
            set(gcf, 'Visible', 'off')
        end

        subplot(3,1,1)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, dx_kf,'.r');
        hold on;
        s2=plot(hod_common, dx_lsq,'.b');
        legend([s1,s2],'KF','LSQ');
       if isylimValid(2)
            ylim([secondYLimit{1} secondYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\Deltax [m]');
        title(sprintf('dx (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndx (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(dx_kf),std(dx_kf),mean(dx_lsq),std(dx_lsq)));
        grid on;

        subplot(3,1,2)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, dy_kf,'.r');
        hold on;
        s2=plot(hod_common, dy_lsq,'.b');    
        legend([s1,s2],'KF','LSQ');
        if isylimValid(2)
            ylim([secondYLimit{1} secondYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\Deltay [m]');
        title(sprintf('dy (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndy (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(dy_kf),std(dy_kf),mean(dy_lsq),std(dy_lsq)));
        grid on;

        subplot(3,1,3)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, dz_kf,'.r');
        hold on;
        s2=plot(hod_common, dz_lsq,'.b');
        legend([s1,s2],'KF','LSQ');
        if isylimValid(2)
            ylim([secondYLimit{1} secondYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\Deltaz [m]');
        title(sprintf('dz (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndz (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(dz_kf),std(dz_kf),mean(dz_lsq),std(dz_lsq)));
        grid on;

        % Save to file
        print(filenamewithfullpath_kinematic_iMAR_ECEF_residuals,'-dpng')
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]); 
        savefig(h,filenamewithfullpath_kinematic_iMAR_ECEF_residuals)
        
        for i = 1 : length(formattype)
            if ~isempty(formattype{i})
                if strcmp(formattype{i}, '-djpeg') || strcmp(formattype{i}, '-dpng') || strcmp(formattype{i}, '-dtiff') || strcmp(formattype{i}, '-dpdf') || strcmp(formattype{i}, '-deps')
                    if formattype{i} == '-dpng'
                        set(h,'PaperOrientation','portrait');
                    else
                        set(h,'PaperOrientation','landscape');
                    end
                    print(filenamewithfullpath_kinematic_iMAR_ECEF_residuals, formattype{i})
                else
                    warning('Unknown formattype, default is used: pdf')
                    print(filenamewithfullpath_kinematic_iMAR_ECEF_residuals,'-dpdf')
                end    
            end
        end
        
        close all      
        
    end

    %==================================================================
    % KINEMATIC: Estimated vs. iMar position in ENU -------------------
    %==================================================================      
    
    if conf.kfdenu_kinematic
        
        % ENU
        figure;
        if ~figure_showup
            set(gcf, 'Visible', 'off')
        end

        subplot(3,1,1)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, de_kf,'.r');
        hold on;
        s2=plot(hod_common, de_lsq,'.b');
        legend([s1,s2],'KF','LSQ');
        if isylimValid(3)
            ylim([thirdYLimit{1} thirdYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaE [m]');
        title(sprintf('dE (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndE (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(de_kf),std(de_kf),mean(de_lsq),std(de_lsq)));
        grid on;

        subplot(3,1,2)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, dn_kf,'.r');
        hold on;
        s2=plot(hod_common, dn_lsq,'.b');    
        legend([s1,s2],'KF','LSQ');
        if isylimValid(3)
            ylim([thirdYLimit{1} thirdYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaN [m]');
        title(sprintf('dN (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndN (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(dn_kf),std(dn_kf),mean(dn_lsq),std(dn_lsq)));
        grid on;

        subplot(3,1,3)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, du_kf,'.r');
        hold on;
        s2=plot(hod_common, du_lsq,'.b');
        legend([s1,s2],'KF','LSQ');
        if isylimValid(3)
            ylim([thirdYLimit{1} thirdYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaU [m]');
        title(sprintf('dU (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndU (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(du_kf),std(du_kf),mean(du_lsq),std(du_lsq)));
        grid on;    

        % Save to file
        print(filenamewithfullpath_kinematic_iMAR_ENU_residuals,'-dpng')
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        savefig(h,filenamewithfullpath_kinematic_iMAR_ENU_residuals)
        
        for i = 1 : length(formattype)
            if ~isempty(formattype{i})
                if strcmp(formattype{i}, '-djpeg') || strcmp(formattype{i}, '-dpng') || strcmp(formattype{i}, '-dtiff') || strcmp(formattype{i}, '-dpdf') || strcmp(formattype{i}, '-deps')
                    if formattype{i} == '-dpng'
                        set(h,'PaperOrientation','portrait');
                    else
                        set(h,'PaperOrientation','landscape');
                    end
                    print(filenamewithfullpath_kinematic_iMAR_ENU_residuals, formattype{i})
                else
                    warning('Unknown formattype, default is used: pdf')
                    print(filenamewithfullpath_kinematic_iMAR_ENU_residuals,'-dpdf')
                end    
            end
        end

        close all  
        
    end
  

 %%    Empirical cumulative distribution function plot LSQ ECEF
  isMakePlot            = true;
  time                  = hod;
  first_coor            = abs(dx_lsq);
  second_coor           = abs(dy_lsq);
  third_coor            = abs(dz_lsq);        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_CDF_ECEF_LSQ');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Probability of |sim-ref|'];
  ylabelCell            = {'Probability of |dx_lsq|',	'Probability of |dy_lsq|',    'Probability of |dz_lsq|'};
  titles                = {'cumulative distribution of |dx_lsq|',	'cumulative distribution of |dy_lsq|',    'cumulative distribution of |dz_lsq|'};
  legends               = {'sim'};
  plotRange             = {};
  plotSaveLogic         = [true,true,true];
  logicMask             = [];
  infigureHandle        = [];
  
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig,                ...                         
    ~ ]                                         = GNSS_Visualization.plots.plot_cdf_GNSS(   isMakePlot,                                                   	...
                                                                                        time,                                                            ...
                                                                                        first_coor,         	second_coor,     	third_coor,       	...
                                                                                        time_common_w_ref,                                              ...
                                                                                        first_coor_ref,         second_coor_ref,	third_coor_ref,     ...
                                                                                        filenamewithfullpath,                                           ...
                                                                                        subplot_heightin,         figure_showupin,                      	...
                                                                                        xlabelstr,                                                          ...      	
                                                                                        ylabelCell,                                                         ...        
                                                                                        titles,                                                             ...
                                                                                        legends,                                                            ... 
                                                                                        plotRange,                                                          ...
                                                                                        plotSaveLogic,                                                    	...
                                                                                        logicMask,                                                          ...
                                                                                        infigureHandle) ;  
                                                                                    
%%    Empirical cumulative distribution function plot KF ECEF
  isMakePlot            = true;
  time                  = hod;
  first_coor            = abs(dx_kf);
  second_coor           = abs(dy_kf);
  third_coor            = abs(dz_kf);        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_CDF_ECEF_KF');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Probability of |sim-ref|'];
  ylabelCell            = {'Probability of |dx_kf|',	'Probability of |dy_kf|',    'Probability of |dz_kf|'};
  titles                = {'cumulative distribution of |dx_kf|',	'cumulative distribution of |dy_kf|',    'cumulative distribution of |dz_kf|'};
  legends               = {'sim'};
  plotRange             = {};
  plotSaveLogic         = [true,true,true];
  logicMask             = [];
  infigureHandle        = [];
  
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig,                ...                         
    ~ ]                                         = GNSS_Visualization.plots.plot_cdf_GNSS(   isMakePlot,                                                   	...
                                                                                        time,                                                            ...
                                                                                        first_coor,         	second_coor,     	third_coor,       	...
                                                                                        time_common_w_ref,                                              ...
                                                                                        first_coor_ref,         second_coor_ref,	third_coor_ref,     ...
                                                                                        filenamewithfullpath,                                           ...
                                                                                        subplot_heightin,         figure_showupin,                      	...
                                                                                        xlabelstr,                                                          ...      	
                                                                                        ylabelCell,                                                         ...        
                                                                                        titles,                                                             ...
                                                                                        legends,                                                            ... 
                                                                                        plotRange,                                                          ...
                                                                                        plotSaveLogic,                                                    	...
                                                                                        logicMask,                                                          ...
                                                                                        infigureHandle) ;  
                                                                                    
 %%    Empirical cumulative distribution function plot LSQ LLH
  isMakePlot            = true;
  time                  = hod;
  first_coor            = abs(dlat_lsq);
  second_coor           = abs(dlon_lsq);
  third_coor            = abs(h_lsq);        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_CDF_LLH_LSQ');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Probability of |sim-ref|'];
  ylabelCell            = {'Probability of |dlat_lsq|',	'Probability of |dlon_lsq|',    'Probability of |h_lsq|'};
  titles                = {'cumulative distribution of |dlat_lsq|',	'cumulative distribution of |dlon_lsq|',    'cumulative distribution of |h_lsq|'};
  legends               = {'sim'};
  plotRange             = {};
  plotSaveLogic         = [true,true,true];
  logicMask             = [];
  infigureHandle        = [];
  
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig,                ...                         
    ~ ]                                         = GNSS_Visualization.plots.plot_cdf_GNSS(   isMakePlot,                                                   	...
                                                                                        time,                                                            ...
                                                                                        first_coor,         	second_coor,     	third_coor,       	...
                                                                                        time_common_w_ref,                                              ...
                                                                                        first_coor_ref,         second_coor_ref,	third_coor_ref,     ...
                                                                                        filenamewithfullpath,                                           ...
                                                                                        subplot_heightin,         figure_showupin,                      	...
                                                                                        xlabelstr,                                                          ...      	
                                                                                        ylabelCell,                                                         ...        
                                                                                        titles,                                                             ...
                                                                                        legends,                                                            ... 
                                                                                        plotRange,                                                          ...
                                                                                        plotSaveLogic,                                                    	...
                                                                                        logicMask,                                                          ...
                                                                                        infigureHandle) ;  
                                                                                    
%%    Empirical cumulative distribution function plot KF LLH
  isMakePlot            = true;
  time                  = hod;
  first_coor            = abs(dlat_kf);
  second_coor           = abs(dlon_kf);
  third_coor            = abs(h_kf);        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_CDF_LLH_KF');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Probability of |sim-ref|'];
  ylabelCell            = {'Probability of |dlat_kf|',	'Probability of |dlon_kf|',    'Probability of |h_kf|'};
  titles                = {'cumulative distribution of |dlat_kf|',	'cumulative distribution of |dlon_kf|',    'cumulative distribution of |h_kf|'};
  legends               = {'sim'};
  plotRange             = {};
  plotSaveLogic         = [true,true,true];
  logicMask             = [];
  infigureHandle        = [];
  
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig,                ...                         
    ~ ]                                         = GNSS_Visualization.plots.plot_cdf_GNSS(   isMakePlot,                                                   	...
                                                                                        time,                                                            ...
                                                                                        first_coor,         	second_coor,     	third_coor,       	...
                                                                                        time_common_w_ref,                                              ...
                                                                                        first_coor_ref,         second_coor_ref,	third_coor_ref,     ...
                                                                                        filenamewithfullpath,                                           ...
                                                                                        subplot_heightin,         figure_showupin,                      	...
                                                                                        xlabelstr,                                                          ...      	
                                                                                        ylabelCell,                                                         ...        
                                                                                        titles,                                                             ...
                                                                                        legends,                                                            ... 
                                                                                        plotRange,                                                          ...
                                                                                        plotSaveLogic,                                                    	...
                                                                                        logicMask,                                                          ...
                                                                                        infigureHandle) ; 
                                                                                    
%%    Empirical cumulative distribution function plot LSQ ENU
  isMakePlot            = true;
  time                  = hod;
  first_coor            = abs(de_lsq);
  second_coor           = abs(dn_lsq);
  third_coor            = abs(du_lsq);        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_CDF_ENU_LSQ');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Probability of |sim-ref|'];
  ylabelCell            = {'Probability of |de_lsq|',	'Probability of |dn_lsq|',    'Probability of |du_lsq|'};
  titles                = {'cumulative distribution of |de_lsq|',	'cumulative distribution of |dn_lsq|',    'cumulative distribution of |du_lsq|'};
  legends               = {'sim'};
  plotRange             = {};
  plotSaveLogic         = [true,true,true];
  logicMask             = [];
  infigureHandle        = [];
  
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig,                ...                         
    ~ ]                                         = GNSS_Visualization.plots.plot_cdf_GNSS(   isMakePlot,                                                   	...
                                                                                        time,                                                            ...
                                                                                        first_coor,         	second_coor,     	third_coor,       	...
                                                                                        time_common_w_ref,                                              ...
                                                                                        first_coor_ref,         second_coor_ref,	third_coor_ref,     ...
                                                                                        filenamewithfullpath,                                           ...
                                                                                        subplot_heightin,         figure_showupin,                      	...
                                                                                        xlabelstr,                                                          ...      	
                                                                                        ylabelCell,                                                         ...        
                                                                                        titles,                                                             ...
                                                                                        legends,                                                            ... 
                                                                                        plotRange,                                                          ...
                                                                                        plotSaveLogic,                                                    	...
                                                                                        logicMask,                                                          ...
                                                                                        infigureHandle) ; 
                                                                                    
%%    Empirical cumulative distribution function plot KF ENU
  isMakePlot            = true;
  time                  = hod;
  first_coor            = abs(de_kf);
  second_coor           = abs(dn_kf);
  third_coor            = abs(du_kf);        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_CDF_ENU_KF');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Probability of |sim-ref|'];
  ylabelCell            = {'Probability of |de_kf|',	'Probability of |dn_kf|',    'Probability of |du_kf|'};
  titles                = {'cumulative distribution of |de_kf|',	'cumulative distribution of |dn_kf|',    'cumulative distribution of |du_kf|'};
  legends               = {'sim'};
  plotRange             = {};
  plotSaveLogic         = [true,true,true];
  logicMask             = [];
  infigureHandle        = [];
  
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig,                ...                         
    ~ ]                                         = GNSS_Visualization.plots.plot_cdf_GNSS(   isMakePlot,                                                   	...
                                                                                        time,                                                            ...
                                                                                        first_coor,         	second_coor,     	third_coor,       	...
                                                                                        time_common_w_ref,                                              ...
                                                                                        first_coor_ref,         second_coor_ref,	third_coor_ref,     ...
                                                                                        filenamewithfullpath,                                           ...
                                                                                        subplot_heightin,         figure_showupin,                      	...
                                                                                        xlabelstr,                                                          ...      	
                                                                                        ylabelCell,                                                         ...        
                                                                                        titles,                                                             ...
                                                                                        legends,                                                            ... 
                                                                                        plotRange,                                                          ...
                                                                                        plotSaveLogic,                                                    	...
                                                                                        logicMask,                                                          ...
                                                                                        infigureHandle) ; 
                                                                                                                                  
                                                                                    
end




function isylimValid= inputYLimCellValidation(ylimCell)
    isylimValid = false;
    
    if iscell(ylimCell)
        if length(ylimCell) == 2
            if all(cellfun(@(x) isnumeric(x),ylimCell))
                isylimValid = true;
            end
        end
    end
    
    
   
end
function [isylimValid, firstyYLimit,secondYLimit, thirdYLimit] = inputYLimValidation(ylimCell)
    isylimValid = [false, false, false];
    firstyYLimit    = [];
    secondYLimit    = [];
    thirdYLimit     = [];
     if iscell(ylimCell)
         if length(ylimCell) == 3
            isylimValid1 = inputYLimCellValidation(ylimCell{1});
            isylimValid2 = inputYLimCellValidation(ylimCell{2});
            isylimValid3 = inputYLimCellValidation(ylimCell{3});
            isylimValid = [isylimValid1, isylimValid2, isylimValid3];
         end
     end
     if isylimValid(1)
         firstyYLimit = ylimCell{1};
     end
     if isylimValid(2) 
         secondYLimit = ylimCell{2};
     end
     if isylimValid(3) 
         thirdYLimit = ylimCell{3};
     end
end
