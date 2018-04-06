function ReporterFileNameStoreObj = visualize(ws, InitObject, MaskObject, filename, def)

%%------------- BEGIN HEADER --------------
% Function: visualize
%
% Purpose: Generation of plots
%%------------- BEGIN CODE --------------

disp('Begin visualization!');
GenerateVariables();
pltconf = visualization_settings();

ReporterFileNameStoreObj        = Reporter.ReporterFileNameStore();

    gps_satelite_id_first   = 1 ;
    gps_satelite_id_last    = def.MAX_NUM_GPS_SAT;
    gpsConstellationID      = def.SYS_GPS;
    maxnumber_of_satelites  = max(def.MAX_NUM_GPS_SAT,def.SYS_GAL);
    
    galileo_satelite_id_first  = 1;
    galileo_satelite_id_last   = def.MAX_NUM_GALILEO_SAT;
    galileoConstellationID      = def.SYS_GAL;
    
figure_showup   = false;
subplot_height  = 0.15;
isPngSave       = true;
isPdfSave       = false;
isFigSave       = false;
Daimler_Mask    = [];
rho             = 180/pi;


    % Get hours of day ()
    sow = unique(ws.sow.Data);
    hod = unique(ws.hod.Data);
    

prns_detail_GPS     = InitObject.GetPrnAvaibility('GPS');
prns_detail_Galileo = InitObject.GetPrnAvaibility('Galileo');
prns_detail_All     = prns_detail_GPS + prns_detail_Galileo;

sowforPrns          = InitObject.GetsowforPrns;
hodforPrns          = GNSS_Visualization.compute_hours_of_day_from_seconds_of_week(sowforPrns);
  
toc;
%% Preparation of reference data
sow_ref=[];
lat_ref=[];
lon_ref=[];
h_ref=[];
x_ref=[];
y_ref=[];
z_ref=[];
e_ref=[];
n_ref=[];
u_ref=[];

%% Fill position vectors
mode        = filehandler.FileHandler.GetMeasInfoContainer.getAllMeasurementInfoinCellFormat{1}.GetMode();
reference   = filehandler.FileHandler.GetMeasInfoContainer.getAllMeasurementInfoinCellFormat{1}.GetReference();

myReferenceReader = GNSS_Visualization.ReferenceFactory.Create(mode);
myRerence = myReferenceReader.Read(reference);
if isa(myRerence,'GNSS_Visualization.kinematicReference') 
    
    mode = 'kinematic_w_reference';
    
    secs_ref_original     	= myRerence.GetRefGPSSoW(); 
    latdeg_ref_original     = myRerence.GetRefLatDeg();
    londeg_ref_original     = myRerence.GetRefLonDeg();
    high_ref_original       = myRerence.GetRefHigh();

    secs_ref            = makingCommonArray	(secs_ref_original,   	 secs_ref_original,   sow);
    sow_common_w_ref    = makingCommonArray (ws.sow.Data,    ws.sow.Data,   secs_ref);
    sow_common_w_ref    = unique(sow_common_w_ref);
    secs_ref            = makingCommonArray (secs_ref,    secs_ref,   sow_common_w_ref);

    lat_ref     = makingCommonArray (latdeg_ref_original,    secs_ref_original,   secs_ref);
    lon_ref     = makingCommonArray (londeg_ref_original,    secs_ref_original,   secs_ref);
    h_ref     	= makingCommonArray	(high_ref_original,      secs_ref_original,   secs_ref);
    
    
    for i=1:length(secs_ref)

       [x, y, z]   = llh2ecef(lat_ref(i)/rho, lon_ref(i)/rho, h_ref(i));
        x_ref       = [x_ref; x];
        y_ref       = [y_ref; y];
        z_ref       = [z_ref; z];

       [e, n, u] = xyz2enu(lat_ref(i),lon_ref(i),x_ref(end), y_ref(end), z_ref(end));
        e_ref = [e_ref; e];
        n_ref = [n_ref; n];
        u_ref = [u_ref; u];            
    end    

elseif isa(myRerence,'GNSS_Visualization.staticReference')
    
    mode = 'static_w_reference';    
    secs_ref = ws.sow.Data;
    
    x_ref  	= myRerence.GetRefX();
    y_ref 	= myRerence.GetRefY();
    z_ref	= myRerence.GetRefZ();

    [lat_ref, lon_ref, h_ref]   = ecef2llh(x_ref,y_ref,z_ref);
    lat_ref                     = lat_ref*rho;
    lon_ref                     = lon_ref*rho;
           
else
    mode = 'Unknown_wout_referenc';
    secs_ref = ws.sow.Data;
    x_ref  	= [];
    y_ref 	= [];
    z_ref	= [];

    [lat_ref, lon_ref, h_ref]   = ecef2llh(x_ref,y_ref,z_ref);
    lat_ref                     = lat_ref*rho;
    lon_ref                     = lon_ref*rho;
    
end

sow_common_w_ref    = makingCommonArray (ws.sow.Data,    ws.sow.Data,   secs_ref);
sow_common_w_ref    = unique(sow_common_w_ref);
secs_ref            = makingCommonArray (secs_ref,	secs_ref, 	sow_common_w_ref);
hod_common_w_ref  	= makingCommonArray (hod,       sow,        sow_common_w_ref);

%% Least squares position
[	x_lsq, ...
   	y_lsq, ...
   	z_lsq]         = LeverarmsCompenstion(  ws,                         ...
                                        	ws.ECEF_LSQ.Data(:,1),      ...
                                        	ws.ECEF_LSQ.Data(:,2),      ...
                                         	ws.ECEF_LSQ.Data(:,3)    );
                                        
x_lsq     = makingCommonArray (x_lsq,    ws.sow.Data,   sow);
y_lsq     = makingCommonArray (y_lsq,    ws.sow.Data,   sow);
z_lsq     = makingCommonArray (z_lsq,    ws.sow.Data,   sow);

[   lat_lsq, ...
    lon_lsq, ...
    h_lsq]          = ecef2llh( x_lsq, ...
                                y_lsq, ...
                                z_lsq);
                            
lat_lsq                     = lat_lsq*rho;
lon_lsq                   	= lon_lsq*rho;

%% Least squares position resampled with reference time
x_lsq_common_w_ref     = makingCommonArray (x_lsq,	sow,	secs_ref);
y_lsq_common_w_ref     = makingCommonArray (y_lsq, 	sow,    secs_ref);
z_lsq_common_w_ref     = makingCommonArray (z_lsq,	sow,    secs_ref);
[   lat_lsq_common_w_ref, ...
    lon_lsq_common_w_ref, ...
    h_lsq_common_w_ref]         = ecef2llh( x_lsq_common_w_ref,     ...
                                            y_lsq_common_w_ref ,    ...
                                            z_lsq_common_w_ref);
lat_lsq_common_w_ref = lat_lsq_common_w_ref*rho;
lon_lsq_common_w_ref = lon_lsq_common_w_ref*rho;

%% Kalman filter position

[	x_kf, ...
   	y_kf, ...
   	z_kf]         = LeverarmsCompenstion(	ws,                         ...
                                        	ws.ECEF_KF.Data(:,1),       ...
                                        	ws.ECEF_KF.Data(:,2),       ...
                                         	ws.ECEF_KF.Data(:,3)    );
                                        
x_kf     = makingCommonArray (x_kf,    ws.sow.Data,   sow);
y_kf     = makingCommonArray (y_kf,    ws.sow.Data,   sow);
z_kf     = makingCommonArray (z_kf,    ws.sow.Data,   sow);

x_stddev	= makingCommonArray (sqrt(ws.P.Data(:,1)),    ws.sow.Data,   sow);
y_stddev  	= makingCommonArray (sqrt(ws.P.Data(:,2)),    ws.sow.Data,   sow);
z_stddev  	= makingCommonArray (sqrt(ws.P.Data(:,3)),    ws.sow.Data,   sow);

[lat_kf, lon_kf, h_kf]  = ecef2llh(x_kf,y_kf,z_kf);
lat_kf                  = lat_kf*rho;
lon_kf                  = lon_kf*rho;

%% Kalman filter position resampled with reference time
x_kf_common_w_ref     = makingCommonArray (x_kf,    sow,   secs_ref);
y_kf_common_w_ref     = makingCommonArray (y_kf,    sow,   secs_ref);
z_kf_common_w_ref     = makingCommonArray (z_kf,    sow,   secs_ref);
[	lat_kf_common_w_ref, ...
	lon_kf_common_w_ref, ...
    h_kf_common_w_ref]          = ecef2llh( x_kf_common_w_ref, ...
                                            y_kf_common_w_ref, ...
                                            z_kf_common_w_ref);

lat_kf_common_w_ref = lat_kf_common_w_ref*rho;
lon_kf_common_w_ref = lon_kf_common_w_ref*rho;

%% ENU coordinates where the tracking start is considered as the ENU origin
[e_lsq, n_lsq,  u_lsq]	= xyz2enu(lat_lsq,	lon_lsq,	x_lsq,      y_lsq,  z_lsq);
[e_kf,  n_kf,   u_kf]  	= xyz2enu(lat_kf, 	lon_kf,     x_kf,       y_kf,   z_kf);    

pdop_ecef  	= makingCommonArray (ws.DOP.Data(:,1),    ws.sow.Data,   sow);
hdop_ecef  	= makingCommonArray (ws.DOP.Data(:,2),    ws.sow.Data,   sow);
vdop_ecef  	= makingCommonArray (ws.DOP.Data(:,3),    ws.sow.Data,   sow);
tdop      	= makingCommonArray (ws.DOP.Data(:,4),    ws.sow.Data,   sow);
pdop_enu   	= makingCommonArray (ws.DOP.Data(:,5),    ws.sow.Data,   sow);
hdop_enu  	= makingCommonArray (ws.DOP.Data(:,6),    ws.sow.Data,   sow);
vdop_enu   	= makingCommonArray (ws.DOP.Data(:,7),    ws.sow.Data,   sow);

pseudorange_residuals_KF = makingCommonArray(ws.pseudorange_residuals_KF.Data, ws.sow.Data,   sow);

display (['Plotting charts to: ' filehandler.FileHandler.GenerateFileName('General',filename,'')]);

wrongindx = SearchforWrongIndeces(  lat_lsq,    lon_lsq,                ...
                                    x_lsq,      y_lsq,      z_lsq,      ...
                                    x_kf,       y_kf,       z_kf);

Sat_GPS     = evalin('base','Nav_SolOut.GPS');
Sat_Galileo = evalin('base','Nav_SolOut.Galileo');
                                
if ~isempty(wrongindx)
    for idxer = 1:length(wrongindx)
        sprintf(  ['wrong result at (in sow): \t \t \t ', num2str(sow(wrongindx(idxer))) ])
    end
end

%%
try % Filter out the outlayers point
    cleanSolutionbyIndeces(myVariables,wrongindx);
    cleanSolutionbyIndeces(myVariablesRef,wrongindx);
catch ME
    warning(ME.message)
end

%==================================================================
% HTML map generation
%==================================================================
try
 if pltconf.googlemaps
     disp ('Plotting: Google Maps');
 end
 
 filenamewpath_GoogleEarth = filehandler.FileHandler.GenerateFileName('General',filename,'_GoogleEarth');
[       filename_static_w_reference,    ...
     	filename_kinematic_reference,   ...
     	filename_without_reference              ] =  GNSS_Visualization.plots.plot_HTML_map( pltconf,                   mode,                           ...
                                                                                            hod_common_w_ref,         	idx_common,                     ...
                                                                                            lat_ref,                    lon_ref,                        ...
                                                                                            lat_kf_common_w_ref,        lon_kf_common_w_ref,            ...
                                                                                            lat_lsq_common_w_ref,       lon_lsq_common_w_ref,           ...
                                                                                            filenamewpath_GoogleEarth,                                  ...
                                                                                            subplot_height,             figure_showup );
   
ReporterFileNameStoreObj.setGoogleMaps_static_w_reference(filename_static_w_reference);
ReporterFileNameStoreObj.setGoogleMaps_kinematic_reference(filename_kinematic_reference);
ReporterFileNameStoreObj.setGoogleMaps_without_reference(filename_without_reference);
                                                                      
catch ME
    warning(ME.message)
end

%==================================================================
% Plot wrong coordinates -------------------
%==================================================================
try

    Path = 'C:\Users\RCO2BP\Desktop\SVNwc\10_Trunk\VMPS_DevelopmentEnvironment\01NavigationAlgorithm\GNSSRawDataProcessing\plots\WrongPlots';   
    WrongPlotNames = GNSS_Visualization.PlotWrongCoordinates.MakeWrongCoordinatePlots(  x_lsq_common_w_ref, y_lsq_common_w_ref, z_lsq_common_w_ref,     ...
                                                                                        x_kf_common_w_ref, y_kf_common_w_ref, z_kf_common_w_ref,        ...
                                                                                        x_ref, y_ref, z_ref,                                            ...
                                                                                        30, 20, 'Coordinate',                                           ...
                                                                                        myVariables, myVariablesRef, filename,                          ...
                                                                                        pltconf, mode, idx_common, subplot_height, figure_showup,       ...
                                                                                        maxnumber_of_satelites, Path);
catch ME
    warning(ME.message)
end

%==================================================================
% Create Ionosphere monitoring related plots ----------------------
%==================================================================

try
    
    Rootfolder      = filehandler.FileHandler.GenerateFileName('General','','');
    SubfolderName   = 'IonosphereRelated';
    Subfolder       = [Rootfolder,'\',SubfolderName]; 
    
     GNSS_Visualization.plots.CreateIonoPlots(  pltconf,                            ...
                                                Subfolder,                          ...
                                                filename,                           ...
                                                figure_showup,                      ...
                                                ws,                                 ...
                                                [isPngSave,isPdfSave,isFigSave])
                
                
catch ME
    warning(ME.message)
end




%==================================================================
% Plot raw measurements -------------------
%==================================================================
try
 if pltconf.rawmeas
     disp ('Plotting: raw measurements');
 end
 
savedfilenamewfullpath_InputSignals = filehandler.FileHandler.GenerateFileName('General',filename,'_InputSignals_GPS');
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig                         ]       = GNSS_Visualization.plots.plot_raw_measurements(   pltconf,                    ...
                                                                                                    gpsConstellationID,         ...
                                                                                                    gps_satelite_id_first,      ...
                                                                                                    gps_satelite_id_last,       ...
                                                                                                    ws.GPSweeksecs.Data(:,2),   ...
                                                                                                    ws.C1.Data, ws.C2.Data,     ...
                                                                                                    ws.L1.Data, ws.L2.Data,     ...
                                                                                                    ws.D1.Data, ws.D2.Data,     ...
                                                                                                    ws.S1.Data, ws.S2.Data,     ...
                                                                                                    ws.SatId.Data(:,1),         ...
                                                                                                    ws.SatId.Data(:,2),         ...
                                                                                                    savedfilenamewfullpath_InputSignals,    ...
                                                                                                    figure_showup, subplot_height,          ...
                                                                                                    [isPngSave,isPdfSave,isFigSave] );
                                            
ReporterFileNameStoreObj.setInputSignals_GPS([savedfilenamewfullpath_InputSignals,'.png']);  

savedfilenamewfullpath_InputSignals = filehandler.FileHandler.GenerateFileName('General',filename,'_InputSignals_Galileo');
[   fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig                         ]       = GNSS_Visualization.plots.plot_raw_measurements(   pltconf,                    ...
                                                                                                    galileoConstellationID,     ...
                                                                                                    galileo_satelite_id_first, 	...
                                                                                                    galileo_satelite_id_last, 	...
                                                                                                    ws.GPSweeksecs.Data(:,2),   ...
                                                                                                    ws.C1.Data, ws.C2.Data,     ...
                                                                                                    ws.L1.Data, ws.L2.Data,     ...
                                                                                                    ws.D1.Data, ws.D2.Data,     ...
                                                                                                    ws.S1.Data, ws.S2.Data,     ...
                                                                                                    ws.SatId.Data(:,1),         ...
                                                                                                    ws.SatId.Data(:,2),         ...
                                                                                                    savedfilenamewfullpath_InputSignals,    ...
                                                                                                    figure_showup, subplot_height,          ...
                                                                                                    [isPngSave,isPdfSave,isFigSave]);
                                            
ReporterFileNameStoreObj.setInputSignals_Galileo([savedfilenamewfullpath_InputSignals,'.png']);  
catch ME
    warning(ME.message)
end

%==================================================================
% Code Noise Ratio per SVid -------------------
%==================================================================
try
filenamewithfullpath = filehandler.FileHandler.GenerateFileName('General',filename,'_Code_Noise_Ratio');
[	fileNamePng,                ...
 	fileNamePdf,                ...
  	fileNameFig                         ]           =  GNSS_Visualization.plots.plot_raw_measurements_per_satellite(	true,                       ...
                                                                                                                        ws.GPSweeksecs.Data(:,2),	...
                                                                                                                        ws.SatId.Data(:,1),         ...
                                                                                                                        ws.S1.Data,                 ...
                                                                                                                        gps_satelite_id_first,      ...
                                                                                                                        gps_satelite_id_last,       ...
                                                                                                                        filenamewithfullpath,       ...
                                                                                                                        figure_showup,              ...
                                                                                                                        [isPngSave,isPdfSave,isFigSave]);
                                                                                                                    
ReporterFileNameStoreObj.setCodeNoiseRation(fileNamePng);  
catch ME
    warning(ME.message)
end
                                                                                                               
%==================================================================
% Pseudorange residuals per SVid -------------------
%==================================================================
try
 if pltconf.pseudorange_residuals
     disp ('Plotting: pseudorange residuals');
 end
 
subfol_SubfolderforResiduals = filehandler.FileHandler.GenerateFileName('SubfolderforResiduals');
GNSS_Visualization.plots.plot_pseudorange_residuals( pltconf,                   hod,                    ...
                                                    pseudorange_residuals_KF,   maxnumber_of_satelites, ...
                                                    subfol_SubfolderforResiduals,                       ...
                                                    filename,                   figure_showup,      	...
                                                    subplot_height)
catch ME
	warning(ME.message)
end

%==================================================================
% Count of PRNs
%==================================================================
try 
 if pltconf.numbersatellites
     disp ('Plotting: number of satellites per epoch');
 end
 
% All
filenamewithfullpath_NumberSats = filehandler.FileHandler.GenerateFileName('General',filename,'_NumberSatsAll');
GNSS_Visualization.plots.plot_number_of_satellites_per_epoch(   pltconf,        hodforPrns,                           	...
                                                                prns_detail_All,    filenamewithfullpath_NumberSats,    ...
                                                                figure_showup,  subplot_height )
ReporterFileNameStoreObj.setNumberSatsAll([filenamewithfullpath_NumberSats,'.png']);  

% GPS only
filenamewithfullpath_NumberSats = filehandler.FileHandler.GenerateFileName('General',filename,'_NumberSats_GPS');
GNSS_Visualization.plots.plot_number_of_satellites_per_epoch(   pltconf,        hodforPrns,                           	...
                                                                prns_detail_GPS,    filenamewithfullpath_NumberSats,    ...
                                                                figure_showup,  subplot_height )
ReporterFileNameStoreObj.setNumberSats_GPS([filenamewithfullpath_NumberSats,'.png']); 

% Galileo only
filenamewithfullpath_NumberSats = filehandler.FileHandler.GenerateFileName('General',filename,'_NumberSats_Galileo');
GNSS_Visualization.plots.plot_number_of_satellites_per_epoch(   pltconf,        hodforPrns,                                 ...
                                                                prns_detail_Galileo,    filenamewithfullpath_NumberSats,    ...
                                                                figure_showup,  subplot_height )
ReporterFileNameStoreObj.setNumberSats_Galileo([filenamewithfullpath_NumberSats,'.png']);          

catch ME
    warning(ME.message)
end

%==================================================================
% PRN Availability
%==================================================================
try
 if pltconf.satelliteavailability
     disp ('Plotting: satellite availibility');
 end
 
% GPS
filenamewithfullpath_PRNavailability = filehandler.FileHandler.GenerateFileName('General',filename,'_PRNavailability_GPS');
GNSS_Visualization.plots.plot_satellite_availability(   pltconf,                hodforPrns,                                    ...
                                                        gps_satelite_id_first, gps_satelite_id_last,                  ...
                                                        prns_detail_GPS,            filenamewithfullpath_PRNavailability,   ...
                                                        figure_showup)
ReporterFileNameStoreObj.setsatellite_availabilityGPS([filenamewithfullpath_PRNavailability,'.png']);                 

% Galileo
filenamewithfullpath_PRNavailability = filehandler.FileHandler.GenerateFileName('General',filename,'_PRNavailability_Galileo');
GNSS_Visualization.plots.plot_satellite_availability(   pltconf,                hodforPrns,                                    ...
                                                        galileo_satelite_id_first, galileo_satelite_id_last,                  ...
                                                        prns_detail_Galileo,            filenamewithfullpath_PRNavailability,   ...
                                                        figure_showup)                                                  
ReporterFileNameStoreObj.setsatellite_availabilityGalileo([filenamewithfullpath_PRNavailability,'.png']); 

catch ME
    warning(ME.message)
end   

%==================================================================
% DOP values -------------------
%==================================================================
try
 if pltconf.dop
     disp ('Plotting: DOP');
 end
    
filenamewithfullpath_KF_DOP = filehandler.FileHandler.GenerateFileName('General',filename,'_KF_DOP');
GNSS_Visualization.plots.plot_dop_values(   pltconf,                    hod,        ...
                                            pdop_ecef,                  pdop_enu,   ...
                                            hdop_ecef,                  hdop_enu,   ...
                                            vdop_ecef,                  vdop_enu,   ...
                                            tdop,                                   ...
                                            filenamewithfullpath_KF_DOP,            ...
                                            subplot_height,             figure_showup  )
                                        
ReporterFileNameStoreObj.setKF_DOP([filenamewithfullpath_KF_DOP,'.png']);                                      
catch ME
    warning(ME.message)
end

%==================================================================
% Formal errors, i.e. standard deviation w.r.t. X, Y, Z
%==================================================================
try
  if pltconf.stddev
     disp ('Plotting: Formal errors (std.dev)');
  end
  
  isMakePlot            = pltconf.stddev;
  time                  = hod;
  first_coor            = x_stddev;
  second_coor           = y_stddev;
  third_coor            = z_stddev;        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_KF_stddev');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'stddev [m]',	'stddev [m]',    'stddev [m]'};
  titles                = {'Standard deviation X',  	'Standard deviation Y',    'Standard deviation Z'};
  legends               = {'sim','ref','mask'};
  plotRange             = {{-1,20},{-1,20},{-1,20}};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
  logicMask             = timeseries(ones(length(hod),1), hod, 'name', 'Daimler Mask');
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
ReporterFileNameStoreObj.setKF_stddev(fileNamePng);
catch ME
    warning(ME.message)
end

%==================================================================
% 1.) LLH LSQ -------------------
%==================================================================
try
  if pltconf.lsqllh
     disp ('Plotting: Least Squares: LLH');
  end
  
  isMakePlot            = pltconf.lsqllh;
  time                  = hod;
  first_coor            = lat_lsq;
  second_coor           = lon_lsq;
  third_coor            = h_lsq;        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_LLH_LSQ');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'Lat [deg]',	'Lon [deg]',    'H [m]'};
  titles                = {'Lat (LSQ)',	'Lon (LSQ)',    'H (LSQ)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
  logicMask             = MaskObject.getDummyMask();
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
ReporterFileNameStoreObj.setLLH_LSQ(fileNamePng);
catch ME
    warning(ME.message)
end
% With Reference
try
  if pltconf.lsqllh
     disp ('Plotting: Least Squares: LLH with Reference');
  end
    
  isMakePlot            = pltconf.lsqllh;
  time                  = hod;
  first_coor            = lat_lsq;
  second_coor           = lon_lsq;
  third_coor            = h_lsq;        
  time_common_w_ref     = hod_common_w_ref;                                           
  first_coor_ref        = lat_ref;
  second_coor_ref       = lon_ref;
  third_coor_ref        = h_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_LLH_LSQ_w_ref');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'Lat [deg]',	'Lon [deg]',    'H [m]'};
  titles                = {'Lat (LSQ)',	'Lon (LSQ)',    'H (LSQ)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setLLH_LSQ(fileNamePng);
catch ME
    warning(ME.message)
end

%==================================================================
% 2.) ECEF LSQ -------------------
%==================================================================
try
  if pltconf.lsqecef
     disp ('Plotting: Least Squares: ECEF');
  end
    
  isMakePlot            = pltconf.lsqecef;
  time                  = hod;
  first_coor            = x_lsq;
  second_coor           = y_lsq;
  third_coor            = z_lsq;        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = h_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ECEF_LSQ');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'X [m]',       'Y [m]',        'Z [m]'};
  titles                = {'X (LSQ)',  	'Y (LSQ)',      'Z (LSQ)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setECEF_LSQ(fileNamePng);
catch ME
    warning(ME.message)
end
% With Reference
try
  if pltconf.lsqecef
     disp ('Plotting: Least Squares: ECEF with reference');
  end
    
  isMakePlot            = pltconf.lsqecef;
  time                  = hod;
  first_coor            = x_lsq;
  second_coor           = y_lsq;
  third_coor            = z_lsq;        
  time_common_w_ref     = hod_common_w_ref;                                           
  first_coor_ref        = x_ref;
  second_coor_ref       = y_ref;
  third_coor_ref        = z_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ECEF_LSQ_w_ref');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'X [m]',       'Y [m]',        'Z [m]'};
  titles                = {'X (LSQ)',  	'Y (LSQ)',      'Z (LSQ)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setECEF_LSQ(fileNamePng);
catch ME
    warning(ME.message)
end

%==================================================================
% 3.) ENU LSQ -------------------
%==================================================================
try
  if pltconf.lsqenu
     disp ('Plotting: Least Squares: ECEF');
  end
    
  isMakePlot            = pltconf.lsqenu;
  time                  = hod;
  first_coor            = e_lsq;
  second_coor           = n_lsq;
  third_coor            = u_lsq;        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ENU_LSQ');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'East [m]',    	'North [m]',        'Up [m]'};
  titles                = {'East (LSQ)',  	'North (LSQ)',      'Up (LSQ)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setENU_LSQ(fileNamePng);
catch ME
    warning(ME.message)
end
% With Reference
try
  if pltconf.lsqenu
     disp ('Plotting: Least Squares: ECEF with reference');
  end
    
  isMakePlot            = pltconf.lsqenu;
  time                  = hod;
  first_coor            = e_lsq;
  second_coor           = n_lsq;
  third_coor            = u_lsq;        
  time_common_w_ref     = hod_common_w_ref;                                           
  first_coor_ref        = e_ref;
  second_coor_ref       = n_ref;
  third_coor_ref        = u_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ENU_LSQ_w_ref');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'East [m]',    	'North [m]',        'Up [m]'};
  titles                = {'East (LSQ)',  	'North (LSQ)',      'Up (LSQ)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setENU_LSQ(fileNamePng);
catch ME
    warning(ME.message)
end

%==================================================================
% 4.) LLH KF -------------------
%==================================================================
try
  if pltconf.kfllh
     disp ('Plotting: Kalman filter: LLH');
  end
    
  isMakePlot            = pltconf.kfllh;
  time                  = hod;
  first_coor            = lat_kf;
  second_coor           = lon_kf;
  third_coor            = h_kf;        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_LLH_KF');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'Lat [deg]',	'Lon [deg]',    'H [m]'};
  titles                = {'Lat (KF)',  	'Lon (KF)',     'H (KF)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setLLH_KF(fileNamePng);
catch ME
    warning(ME.message)
end
% With Reference
try
  if pltconf.kfllh
     disp ('Plotting: Kalman filter: LLH with reference');
  end
    
  isMakePlot            = pltconf.kfllh;
  time                  = hod;
  first_coor            = lat_kf;
  second_coor           = lon_kf;
  third_coor            = h_kf;        
  time_common_w_ref     = hod_common_w_ref;                                           
  first_coor_ref        = lat_ref;
  second_coor_ref       = lon_ref;
  third_coor_ref        = h_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_LLH_KF_w_ref');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'Lat [deg]',	'Lon [deg]',    'H [m]'};
  titles                = {'Lat (KF)',	'Lon (KF)',     'H (KF)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setLLH_KF(fileNamePng);
catch ME
    warning(ME.message)
end

%==================================================================
% 5.) ECEF KF -------------------
%==================================================================
try
  if pltconf.kfecef
     disp ('Plotting: Kalman filter: ECEF');
  end
    
  isMakePlot            = pltconf.kfecef;
  time                  = hod;
  first_coor            = x_kf;
  second_coor           = y_kf;
  third_coor            = z_kf;        
  time_common_w_ref     = [];                                           
  first_coor_ref        = [];
  second_coor_ref       = [];
  third_coor_ref        = [];  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ECEF_KF');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'X [m]',   	'Y [m]',        'Z [m]'};
  titles                = {'X (KF)',  	'Y (KF)',       'Z (KF)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setECEF_KF(fileNamePng);
catch ME
    warning(ME.message)
end
% With Reference
try
  if pltconf.kfecef
     disp ('Plotting: Kalman filter: ECEF');
  end
    
  isMakePlot            = pltconf.kfecef;
  time                  = hod;
  first_coor            = x_kf;
  second_coor           = y_kf;
  third_coor            = z_kf;        
  time_common_w_ref     = hod_common_w_ref;                                           
  first_coor_ref        = x_ref;
  second_coor_ref       = y_ref;
  third_coor_ref        = z_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ECEF_KF_w_ref');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'X [m]',   	'Y [m]',        'Z [m]'};
  titles                = {'X (KF)',  	'Y (KF)',       'Z (KF)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setECEF_KF(fileNamePng);
catch ME
    warning(ME.message)
end

%==================================================================
% 6.) ENU KF -------------------
%==================================================================
try
  if pltconf.kfenu
     disp ('Plotting: Kalman filter: ENU ');
  end
    
  isMakePlot            = pltconf.kfenu;
  time                  = hod;
  first_coor            = e_kf;
  second_coor           = n_kf;
  third_coor            = u_kf;        
  time_common_w_ref     = hod_common_w_ref;                                           
  first_coor_ref        = e_ref;
  second_coor_ref       = n_ref;
  third_coor_ref        = u_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ENU_KF');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'East [m]',        'North [m]',    	'Up [m]'};
  titles                = {'East (KF)',       'North (KF)',   	'Up (KF)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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
                          
ReporterFileNameStoreObj.setENU_KF(fileNamePng);
catch ME
    warning(ME.message)
end
% With Reference
try
  if pltconf.kfenu
     disp ('Plotting: Kalman filter: ENU with reference');
  end
    
  isMakePlot            = pltconf.kfenu;
  time                  = hod;
  first_coor            = e_kf;
  second_coor           = n_kf;
  third_coor            = u_kf;        
  time_common_w_ref     = hod_common_w_ref;                                           
  first_coor_ref        = e_ref;
  second_coor_ref       = n_ref;
  third_coor_ref        = u_ref;  
  filenamewithfullpath  = filehandler.FileHandler.GenerateFileName('General',filename,'_ENU_KF_w_ref');
  
  subplot_heightin      = subplot_height;
  figure_showupin       = figure_showup;
  xlabelstr             = ['Hours of Day'];
  ylabelCell            = {'East [m]',        'North [m]',    	'Up [m]'};
  titles                = {'East (KF)',       'North (KF)',   	'Up (KF)'};
  legends               = {'sim','ref','mask'};
  plotRange             = {};
  plotSaveLogic         = [isPngSave,isPdfSave,isFigSave];
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

                          
ReporterFileNameStoreObj.setENU_KF(fileNamePng);
catch ME
    warning(ME.message)
end

%==================================================================
%==================================================================
%==================================================================
% STATIC : Estimated vs. static reference position
%==================================================================
%==================================================================
%==================================================================
try
    
  if pltconf.kfdllh_static || pltconf.kfdecef_static || pltconf.kfdenu_static
    disp ('Plotting: Static measurement evaluation: Estim. vs static reference');
  end
  
    
filenamewithfullpath_static_ITRF08_ECEF_residuals = filehandler.FileHandler.GenerateFileName('General',filename,'_static_ITRF08_ECEF_residuals');
filenamewithfullpath_static_ITRF08_LLH_residuals = filehandler.FileHandler.GenerateFileName('General',filename,'_static_ITRF08_LLH_residuals');
filenamewithfullpath_static_ITRF08_ENU_residuals = filehandler.FileHandler.GenerateFileName('General',filename,'_static_ITRF08_ENU_residuals');

GNSS_Visualization.plots.plot_evaluate_static_ITRF08(   pltconf,    mode,                                                           ...       
                                                        hod_common_w_ref,                                                           ...
                                                        lat_kf_common_w_ref,  	lon_kf_common_w_ref,        h_kf_common_w_ref,  	...
                                                        x_kf_common_w_ref,     	y_kf_common_w_ref,          z_kf_common_w_ref,  	...
                                                        lat_lsq_common_w_ref,  	lon_lsq_common_w_ref,       h_lsq_common_w_ref, 	...
                                                        x_lsq_common_w_ref,    	y_lsq_common_w_ref,         z_lsq_common_w_ref, 	...
                                                        lat_ref,                lon_ref,                    h_ref,                  ...
                                                        x_ref,                  y_ref,                      z_ref,                  ...
                                                        filenamewithfullpath_static_ITRF08_ECEF_residuals,                          ...
                                                        filenamewithfullpath_static_ITRF08_LLH_residuals,                           ...
                                                        filenamewithfullpath_static_ITRF08_ENU_residuals,                           ...
                                                        filename,                                                                   ...
                                                        {{},{-20,20},{-20,20}},...
                                                        subplot_height, figure_showup )
                                                    
ReporterFileNameStoreObj.setResiduals_LLH([filenamewithfullpath_static_ITRF08_ECEF_residuals,'.png']);
ReporterFileNameStoreObj.setResiduals_ECEF([filenamewithfullpath_static_ITRF08_LLH_residuals,'.png']);
ReporterFileNameStoreObj.setResiduals_ENU([filenamewithfullpath_static_ITRF08_ENU_residuals,'.png']);
catch ME
    warning(ME.message)
end

%==================================================================
%==================================================================
%==================================================================
% KINEMATIC : Estimated vs. kinematic reference position
%==================================================================
%==================================================================
%==================================================================
try 
        
  if pltconf.kfdllh_kinematic || pltconf.kfdecef_kinematic || pltconf.kfdenu_kinematic
    disp ('Plotting: Kinematic measurement evaluation: Estim. vs kinematic reference');
  end
    
filenamewithfullpath_kinematic_iMAR_LLH_residuals   = filehandler.FileHandler.GenerateFileName('General',filename,'_kinematic_iMAR_LLH_residuals');
filenamewithfullpath_kinematic_iMAR_ECEF_residuals  = filehandler.FileHandler.GenerateFileName('General',filename,'_kinematic_iMAR_ECEF_residuals');
filenamewithfullpath_kinematic_iMAR_ENU_residuals   = filehandler.FileHandler.GenerateFileName('General',filename,'_kinematic_iMAR_ENU_residuals');

GNSS_Visualization.plots.plot_evaluate_kinematic_iMar(  pltconf,                mode,                       hod_common_w_ref,  	...
                                                        lat_ref,                lon_ref,                    h_ref,              ...
                                                        x_ref,                  y_ref,                      z_ref,              ...
                                                        lat_lsq_common_w_ref, 	lon_lsq_common_w_ref,       h_lsq_common_w_ref,	...
                                                        x_lsq_common_w_ref,    	y_lsq_common_w_ref,         z_lsq_common_w_ref,	...
                                                        lat_kf_common_w_ref,   	lon_kf_common_w_ref,        h_kf_common_w_ref,	...
                                                        x_kf_common_w_ref,     	y_kf_common_w_ref,          z_kf_common_w_ref,	...
                                                        filenamewithfullpath_kinematic_iMAR_LLH_residuals,                      ...
                                                        filenamewithfullpath_kinematic_iMAR_ECEF_residuals,                     ...
                                                        filenamewithfullpath_kinematic_iMAR_ENU_residuals,                      ...
                                                        filename,                                                               ...
                                                         {{},{-20,20},{}},...
                                                        subplot_height, figure_showup, {'-dpdf'})
                                                    
ReporterFileNameStoreObj.setResiduals_LLH([filenamewithfullpath_kinematic_iMAR_LLH_residuals,'.png']);
ReporterFileNameStoreObj.setResiduals_ECEF([filenamewithfullpath_kinematic_iMAR_ECEF_residuals,'.png']);  
ReporterFileNameStoreObj.setResiduals_ENU([filenamewithfullpath_kinematic_iMAR_ENU_residuals,'.png']);

catch ME
    warning(ME.message)
end

%==================================================================
% World Map (Map toolbox required)
%==================================================================
try %% World Map (Map toolbox required)
 filenamewithfullpath_GloMap = filehandler.FileHandler.GenerateFileName('General',filename,'_GloMap');
 GNSS_Visualization.plots.plot_world_map(   lat_kf,                         lon_kf,         ...
                                            filenamewithfullpath_GloMap,    figure_showup)
catch ME
    warning(ME.message)
end

end

function newMatchedData = makingCommonArray(Data,DataTime,refTime)

    newMatchedData  = [];
    
    [row,column] = size(Data);
     if (row == 1 && column > 1) || (column == 1 && row > 1)
            cleanMode =  'Vector';
        elseif row > 1 && column > 1
            cleanMode = 'Matrix';
        else
            cleanMode = 'Unknown_cleanMode';
     end
            
        
        switch cleanMode
            case  'Vector'
                for i=1:length(refTime)

                    % see if observed epoch is available in the iMar reference data
                    idx = find(abs(refTime(i)-(DataTime))<01e-6);
                    if isempty(idx)
                        continue;
                    elseif length(idx)>=1

                        newMatchedData = [newMatchedData; Data(idx(1))];

                    end
                end    

                
            case 'Matrix'
        
                if column > row
                    temp_matrix_for_size = makingCommonArray(Data(1,:),DataTime,refTime);
                    temp_matrix_new      	= zeros(length(temp_matrix_for_size),row);
                    for idx = 1 : row
                        temp_matrix_new(idx,:) = makingCommonArray(Data(idx,:),DataTime,refTime);
                    end
                    newMatchedData = temp_matrix_new;
                else
                   temp_matrix_for_size    = makingCommonArray(Data(:,1),DataTime,refTime);
                    temp_matrix_new      	= zeros(length(temp_matrix_for_size),column);
                    for idx = 1 : column
                        temp_matrix_new(:,idx) = makingCommonArray(Data(:,idx),DataTime,refTime);
                    end
                    newMatchedData = temp_matrix_new;
                end
                
            otherwise
                warning('Unknown input data format.')
                
        end
        
        if length(newMatchedData) ~= length(refTime)
         	warning('The length of the refTime and the given Data is not equal.')
      	end
        
       
end
function myArray = throwElementsfromArraybyGivenIndeces(myArray,idx)
    if ~isempty(myArray)
        myArray(idx) = [];
    else
        disp(['WARNING: <a href="matlab:opentoline(which(''GNSS_Visualization.visualize''),654,0);">GNSS_Visualization::visualize</a>: The given array is empty.'])
    end
end
function idx = findElementsbyCondition(myArray,myCond)
    try
        idx = find(myCond(myArray));
    catch ME
        warning(ME.message)
        idx = [];
    end
end
function idx = findElementsbyConditionfromArray(x,y,z,myCond)
    try
        idx = find(myCond(x,y,z));
    catch ME
        warning(ME.message)
        idx = [];
    end
end
function indx = getIndecesofWrongElementsfromECEF(x,y,z)

    indx    = [];
 
    Cond1     = @(x,y,z) (x.^2+y.^2+z.^2) < 6200000;
    indx      = findElementsbyConditionfromArray(x,y,z,Cond1);

    indx = unique(indx);
end
function indx = getIndecesofWrongElementsfromLatLong(mylat, mylong)

    indx    = [];
    indxlat = [];    
    
    Cond180     = @(x) abs(x-180)<01e-5;
    CondZero    = @(x) abs(x)<01e-5;
    idx180      = findElementsbyCondition(mylat,Cond180);
    idxZero     = findElementsbyCondition(mylat,CondZero);
    indxlat     = [idx180,idxZero];
    indxlat     = unique(indxlat);
    
    indxlong    = [];    
    Cond180     = @(x) abs(x-180)<01e-5;
    CondZero    = @(x) abs(x)<01e-5;
    idx180      = findElementsbyCondition(mylong,Cond180);
    idxZero     = findElementsbyCondition(mylong,CondZero);
    indxlong    = [idx180,idxZero];
    indxlong    = unique(indxlong);
    
    
    indx = [indxlong,indxlat];
    indx = unique(indx);
end
function cleanSolutionbyIndeces(myVariables,wrongidx)
     
        
 for i = 1:length(myVariables)      
        try  
           temp_variable = evalin('caller',myVariables{i}); 
        catch ME
           warning(ME.message)
           temp_variable = [];
        end
        
        [row,column] = size(temp_variable);
        
        if (row == 1 && column > 1) || (column == 1 && row > 1)
            cleanMode =  'Vector';
        elseif row > 1 && column > 1
            cleanMode = 'Matrix';
        else
            cleanMode = 'Unknown_cleanMode';
        end
            
        switch cleanMode
            case 'Matrix'
                
                if column > row
                    temp_matrix_for_size    = throwElementsfromArraybyGivenIndeces(temp_variable(1,:),wrongidx);
                    temp_matrix_new      	= zeros(length(temp_matrix_for_size),row);
                    for idx = 1 : row
                        temp_matrix_new(idx,:) = throwElementsfromArraybyGivenIndeces(temp_variable(idx,:),wrongidx);
                    end
                    assignin('caller',myVariables{i}, temp_matrix_new);
                else
                   temp_matrix_for_size    = throwElementsfromArraybyGivenIndeces(temp_variable(:,1),wrongidx);
                    temp_matrix_new      	= zeros(length(temp_matrix_for_size),column);
                    for idx = 1 : column
                        temp_matrix_new(:,idx) = throwElementsfromArraybyGivenIndeces(temp_variable(:,idx),wrongidx);
                    end
                    assignin('caller',myVariables{i}, temp_matrix_new); 
                end

            case 'Vector'
                 temp_variable  = throwElementsfromArraybyGivenIndeces(temp_variable,wrongidx);
                 assignin('caller',myVariables{i},temp_variable);
            otherwise
                warning('Unknown clean mode.')
                
        end
            
 end     
end
function GenerateVariables()

    myVariables = { 'sow',          'hod',          'prns_detail_All',    	...
                    'prns_detail_Galileo',          'prns_detail_GPS',      ...
                    'pseudorange_residuals_KF',                           	...
                    'x_lsq',        'y_lsq',        'z_lsq',                ...
                    'lon_lsq',    	'lat_lsq',     	'h_lsq',                ...
                    'x_kf',         'y_kf',         'z_kf',                 ...
                    'x_stddev',     'y_stddev',    	'z_stddev',             ...
                    'lat_kf',       'lon_kf',       'h_kf',                 ...
                    'e_lsq',        'n_lsq',        'u_lsq',                ...
                    'e_kf',         'n_kf',         'u_kf',                 ...
                    'pdop_ecef',    'hdop_ecef',    'vdop_ecef',            ...
                    'tdop',                                                 ...
                    'pdop_enu',     'hdop_enu',     'vdop_enu'};
                
	myVariablesRef = {   	'sow_ref',                                                                  ...
                            'lat_ref',              'lon_ref',                  'h_ref',                ...
                            'x_ref',                'y_ref',                    'z_ref',                ...
                            'e_ref',                'n_ref',                    'u_ref',                ...
                            'sow_common_w_ref',     'hod_common_w_ref',                                 ...
                            'x_lsq_common_w_ref',   'y_lsq_common_w_ref',       'z_lsq_common_w_ref',   ...
                            'lat_lsq_common_w_ref', 'lon_lsq_common_w_ref',     'h_lsq_common_w_ref',   ...
                            'x_kf_common_w_ref',   	'y_kf_common_w_ref',        'z_kf_common_w_ref',   	...
                            'lat_kf_common_w_ref',  'lon_kf_common_w_ref',      'h_kf_common_w_ref' };
   
   assignin('caller','myVariables',myVariables);         
   assignin('caller','myVariablesRef',myVariablesRef);  

   assignin('caller','sow_ref',[]);  
   assignin('caller','lat_ref',[]);  
   assignin('caller','lon_ref',[]);  
   assignin('caller','h_ref',[]);  
   assignin('caller','x_ref',[]);  
   assignin('caller','y_ref',[]);  
   assignin('caller','z_ref',[]);  
   assignin('caller','e_ref',[]);  
   assignin('caller','n_ref',[]);  
   assignin('caller','u_ref',[]);  
   assignin('caller','idx_common',[]);  % Vector for selecting only those positions that have also been determined by the iMar
end
function wrongindx = SearchforWrongIndeces( lat_lsq,    lon_lsq,                ...
                                            x_lsq,      y_lsq,      z_lsq,      ...
                                            x_kf,       y_kf,       z_kf)

    wrongindx           = [];                        
    wrongindxlatlonLSQ 	= getIndecesofWrongElementsfromLatLong(lat_lsq, lon_lsq);
    wrongindxxyzLSQ     = getIndecesofWrongElementsfromECEF(x_lsq,y_lsq,z_lsq);
    wrongindxKF         = getIndecesofWrongElementsfromECEF(x_kf,y_kf,z_kf);
    wrongindx           = [wrongindxlatlonLSQ,wrongindxxyzLSQ,wrongindxKF];
    wrongindx           = unique(wrongindx);
    
end

function [	position_E2POI_E_x, ...
            position_E2POI_E_y, ...
            position_E2POI_E_z]         = LeverarmsCompenstion( ws,             ...
                                                                position_E_x,   ...
                                                                position_E_y,   ...
                                                                position_E_z    )

    try
    
        Chassis = ws.leverarms.Chassis;

        Roll    = ws.leverarms.Roll;
        Pitch   = ws.leverarms.Pitch;
        Heading = ws.leverarms.Heading;

        position_ACTUAL2POI_b = Chassis.leverarm_VMPS2POI - Chassis.leverarm_IMU_ANT_m;

        [   position_E2POI_E_x, ...
            position_E2POI_E_y, ...
            position_E2POI_E_z]         = lib_transformPOS_E2POI_E( position_E_x,           position_E_y, 	position_E_z,	...
                                                                    Roll,                   Pitch,          Heading,        ...
                                                                    position_ACTUAL2POI_b);
                                                                
    catch ME
        warning(ME.message)
        position_E2POI_E_x = position_E_x;
        position_E2POI_E_y = position_E_y;
        position_E2POI_E_z = position_E_z;
    end


end
