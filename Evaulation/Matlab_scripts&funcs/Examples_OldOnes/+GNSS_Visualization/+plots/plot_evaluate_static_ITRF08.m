function plot_evaluate_static_ITRF08(   conf,       mode,               ...
                                        hod,                            ...
                                        lat_kf,     lon_kf,     h_kf,   ...
                                        x_kf,       y_kf,       z_kf,   ...
                                        lat_lsq,    lon_lsq,    h_lsq,  ...
                                        x_lsq,    	y_lsq,     	z_lsq,   ...
                                        lat_ref,    lon_ref,    h_ref,  ...
                                        x_ref,      y_ref,      z_ref,  ...
                                        filenamewithfullpath_static_ITRF08_ECEF_residuals,  ...
                                        filenamewithfullpath_static_ITRF08_LLH_residuals ,  ...
                                        filenamewithfullpath_static_ITRF08_ENU_residuals,   ...
                                        filename,                                           ...
                                        ylimCell,                                           ...
                                        subplot_height, figure_showup )

%==================================================================
%==================================================================
%==================================================================
% STATIC : Estimated vs. ITRF08 position
%==================================================================
%==================================================================
%==================================================================

if ~strcmp(mode,'static_w_reference')
    return
end
    [   isylimValid, ...
        firstyYLimit, ...
        secondYLimit, ...
        thirdYLimit]                    = inputYLimValidation(ylimCell);
    
    % Difference vectors
    len = length(lat_kf);
    
    dlat_kf = lat_kf - ones(len,1)*lat_ref;
    dlon_kf = lon_kf - ones(len,1)*lon_ref;
    dh_kf = h_kf - ones(len,1)*h_ref;
    
    len = length(lat_lsq);
    dlat_lsq = lat_lsq - ones(len,1)*lat_ref;
    dlon_lsq = lon_lsq - ones(len,1)*lon_ref;
    dh_lsq = h_lsq - ones(len,1)*h_ref;
    
    len = length(x_kf);
    dx_kf = x_kf - ones(len,1)*x_ref;
    dy_kf = y_kf - ones(len,1)*y_ref;
    dz_kf = z_kf - ones(len,1)*z_ref;
    
    len = length(x_lsq);
    dx_lsq = x_lsq - ones(len,1)*x_ref;
    dy_lsq = y_lsq - ones(len,1)*y_ref;
    dz_lsq = z_lsq - ones(len,1)*z_ref;
    
    [de_kf, dn_kf, du_kf] = xyz2enu(ones(len,1)*lat_ref, ones(len,1)*lon_ref, dx_kf, dy_kf, dz_kf);
    [de_lsq, dn_lsq, du_lsq] = xyz2enu(ones(len,1)*lat_ref, ones(len,1)*lon_ref, dx_lsq, dy_lsq, dz_lsq);

    hod_common = hod;    

    %==================================================================
    % STATIC: Estimated vs. ITRF08 position in ECEF -------------------
    %==================================================================
    
    if conf.kfdecef_static
        
        figure;
        if ~figure_showup
            set(gcf, 'Visible', 'off')
        end

        subplot(3,1,1)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod,dx_kf,'.r');
        hold on
        s2=plot(hod,dx_lsq,'.b');
        legend([s1,s2], {'KF', 'LSQ'})  
        if isylimValid(1)
            ylim([firstyYLimit{1} firstyYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaX [m]');
        t=title(sprintf('dX w.r.t. ITRF08: avg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dx_kf),std(dx_kf),mean(dx_lsq),std(dx_lsq)));
        set(t, 'FontSize', 12);
        grid on;

        subplot(3,1,2)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod,dy_kf,'.r');
        hold on
        s2=plot(hod,dy_lsq,'.b');
        legend([s1,s2], {'KF', 'LSQ'})  
        if isylimValid(1)
            ylim([firstyYLimit{1} firstyYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaY [m]');
        t=title(sprintf('dY w.r.t. ITRF08: avg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dy_kf),std(dy_kf),mean(dy_lsq),std(dy_lsq)));
        set(t, 'FontSize', 12);
        grid on;

        subplot(3,1,3)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod,dz_kf,'.r');
        hold on
        s2=plot(hod,dz_lsq,'.b');
        legend([s1,s2], {'KF', 'LSQ'})  
        if isylimValid(1)
            ylim([firstyYLimit{1} firstyYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaZ [m]');
        t=title(sprintf('dZ w.r.t. ITRF08: avg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dz_kf),std(dz_kf),mean(dz_lsq),std(dz_lsq)));
        set(t, 'FontSize', 12);
        grid on;   

        % Save to file
        print(filenamewithfullpath_static_ITRF08_ECEF_residuals,'-dpng')
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        
        print(filenamewithfullpath_static_ITRF08_ECEF_residuals,'-dpdf')
        savefig(h,filenamewithfullpath_static_ITRF08_ECEF_residuals)
        close all
    end
 
    %==================================================================
    % STATIC: Estimated vs. ITRF08 position in LLH -------------------
    %==================================================================
    
    if conf.kfdllh_static
    
        figure;
        if ~figure_showup
            set(gcf, 'Visible', 'off')
        end

        subplot(3,2,1)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod, dlat_kf,'.r');
        hold on;
        s2=plot(hod, dlat_lsq,'.b');
        legend([s1,s2], {'KF', 'LSQ'})  
        if isylimValid(2)
            ylim([secondYLimit{1} secondYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaLat [deg]');
        t=title(sprintf('dLat w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dlat_kf),std(dlat_kf),mean(dlat_lsq),std(dlat_lsq)));
        set(t, 'FontSize', 12);    
        grid on;

        subplot(3,2,3)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod, dlon_kf,'.r');
        hold on;
        s2=plot(hod, dlon_lsq,'.b');    
        legend([s1,s2], {'KF', 'LSQ'}) 
        if isylimValid(2)
            ylim([secondYLimit{1} secondYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaLon [deg]');
        t=title(sprintf('dLon w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dlon_kf),std(dlon_kf),mean(dlon_lsq),std(dlon_lsq)));
        set(t, 'FontSize', 12);   
        grid on;

        subplot(3,2,5)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod, dh_kf,'.r');
        hold on;
        s2=plot(hod, dh_lsq,'.b');    
        legend([s1,s2], {'KF', 'LSQ'}) 
        if isylimValid(2)
            ylim([secondYLimit{1} secondYLimit{2}])
        end
        xlabel('Hours of Day');
        ylabel('\DeltaH [m]');
        t=title(sprintf('dH w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dh_kf),std(dh_kf),mean(dh_lsq),std(dh_lsq)));
        set(t, 'FontSize', 12);    
        grid on;


        subplot(3,2,[2 4])
        s1=plot(dlon_lsq, dlat_lsq,'.b');
        hold on
        s2=plot(dlon_kf, dlat_kf,'.r');
        legend([s1,s2], {'LSQ', 'KF'})
        xlabel('dLon [deg]');
        ylabel('dLat [deg]');
        max_x = max([abs(dlon_lsq); abs(dlon_kf)]);
        max_y = max([abs(dlat_lsq); abs(dlat_kf)]);
        xlim([-max_x max_x]);
        ylim([-max_y max_y]);
        if isylimValid
            ylim([ylimCell{1} ylimCell{2}])
        end
        hor_pos_err_kf = sqrt(dlon_kf.^2+dlat_kf.^2);
        hor_pos_err_lsq = sqrt(dlon_lsq.^2+dlat_lsq.^2);
        t=title(sprintf('dLon/dLat w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(hor_pos_err_kf),std(hor_pos_err_kf),mean(hor_pos_err_lsq),std(hor_pos_err_lsq)));
        set(t, 'FontSize', 12);    
        grid on



        % Save to file
        print(filenamewithfullpath_static_ITRF08_LLH_residuals,'-dpng')
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        
        print(filenamewithfullpath_static_ITRF08_LLH_residuals,'-dpdf')
        savefig(h,filenamewithfullpath_static_ITRF08_LLH_residuals)
        close all
        
    end
    
    %==================================================================
    % STATIC: Estimated vs. iMar position in ENU -------------------
    %==================================================================      
    
    if conf.kfdenu_static
        
        % ENU
        figure;
        if ~figure_showup
            set(gcf, 'Visible', 'off')
        end

        % hod with data observed           
        subplot(3,2,1)
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        s1=plot(hod_common, de_kf,'.r');
        hold on;
        s2=plot(hod_common, de_lsq,'.b');  
        legend([s1,s2],'KF','LSQ');
        xlabel('Hours of Day');
        ylabel('\DeltaE [m]');
        t=title(sprintf('dE w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(de_kf),std(de_kf),mean(de_lsq),std(de_lsq)));
        set(t, 'FontSize', 12);    
        grid on;

        subplot(3,2,3)
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
        t=title(sprintf('dN w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dn_kf),std(dn_kf),mean(dn_lsq),std(dn_lsq)));
        set(t, 'FontSize', 12);    
        grid on;

        subplot(3,2,5)
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
        t=title(sprintf('dU w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(du_kf),std(du_kf),mean(du_lsq),std(du_lsq)));
        set(t, 'FontSize', 12);    
        grid on;    

        subplot(3,2,[2 4])
        s1=plot(de_lsq, dn_lsq,'.b');
        hold on
        s2=plot(de_kf, dn_kf,'.r');
            legend([s1,s2], {'LSQ', 'KF'})    
        xlabel('dE [m]');
        ylabel('dN [m]');
        max_x = max([abs(de_lsq); abs(de_kf)]);
        max_y = max([abs(dn_lsq); abs(dn_kf)]);
        xlim([-max_x max_x]);
        ylim([-max_y max_y]);
        if isylimValid(3)
            ylim([thirdYLimit{1} thirdYLimit{2}])
        end
        hor_pos_err_kf = sqrt(de_kf.^2+dn_kf.^2);
        hor_pos_err_lsq = sqrt(de_lsq.^2+dn_lsq.^2);
        t=title(sprintf('dE/dN w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(hor_pos_err_kf),std(hor_pos_err_kf),mean(hor_pos_err_lsq),std(hor_pos_err_lsq)));
        set(t, 'FontSize', 12);    
        grid on    

        % Save to file
        print(filenamewithfullpath_static_ITRF08_ENU_residuals,'-dpng')
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        
        print(filenamewithfullpath_static_ITRF08_ENU_residuals,'-dpdf')
        savefig(h,filenamewithfullpath_static_ITRF08_ENU_residuals)
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
    ~ ]                                         = GNSS_Visualization.plots.plot_GNSS(   isMakePlot,                                                   	...
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
