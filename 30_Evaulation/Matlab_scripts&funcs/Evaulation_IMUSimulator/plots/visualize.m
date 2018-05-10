function visualize(ws,constellation,filename,def,gpsStationObj, galileoStationObj)

% %------------- BEGIN HEADER --------------
% Function: visualize
%
% Purpose: Generation of plots
%
% Inputs:
%   sol - p0ositioning solutions w.r.t. LSQ and KF
%
% Outputs:
%
% Comments:
%   sol contains:
%   sol.obsTime = timeGpsOut;
%   sol.leastSquaresPosition.ECEF = leastSquaresPositionECEF(1:3);
%   sol.leastSquaresPosition.clockError = leastSquaresPositionECEF(4);
%   sol.leastSquaresPosition.LLH = leastSquaresPositionLLH;
%   sol.leastSquaresVelocity.ECEF = leasSquaresVelocityECEF(1:3);
%   sol.leastSquaresVelocity.clockErrorDrift = leasSquaresVelocityECEF(4);
%   sol.kalmanFilter.x = x;
%   sol.kalmanFilter.P = P;
%   sol.kalmanFilter.dy = dy;
%   sol.kalmanFilter.dop = dop;
%
%   LLH = Latitude, Longitude, Height
% 
% Author:        CC-PS/ECS4, CC-AS/ESC5
% Created:       24-Feb-2016
% Last revision: 24-Feb-2016

% %------------- BEGIN CODE --------------

pltconf = visualization_settings();


if strcmp(constellation,'Gps')
    GNSSids =  def.SYS_GPS;
    gnss_satelite_id_first  = 1 ;
    gnss_satelite_id_last   = def.MAX_NUM_GPS_SAT;
    maxnumber_of_satelites  = def.MAX_NUM_GPS_SAT;
elseif strcmp(constellation,'Galileo')
    GNSSids =  def.SYS_GAL;
    maxnumber_of_satelites  = def.MAX_NUM_GALILEO_SAT; 
    gnss_satelite_id_first  = def.SVID_OFFSET_GALILEO_SAT + 1;
    gnss_satelite_id_last   = def.SVID_OFFSET_GALILEO_SAT + def.MAX_NUM_GALILEO_SAT;
else
    error('Wrong inputs for the visualization. Wrong constellation.')
end

figure_showup = false;

rho = 180/pi;
subplot_height = 0.15;



% Read temporary column-based observation file
rnxdata    = FileNameGenerator(gpsStationObj,galileoStationObj,'RawRinexData','Load');
disp(['Read Rinex Data ' rnxdata]);
obs_rnx = load(rnxdata);

% Get hours of day ()
hod = ws.hod;

% Count GPS PRNs per epoch
prns_detail = zeros(length(ws.sow),maxnumber_of_satelites);

for i=1:length(ws.sow)
    
    % Total PRNs
    % get idxs of all entries with current sow
    idxs1 = find(obs_rnx.GPSsow==round(ws.sow(i)));
    % get idxs of all entries with GNSS constellation
    idxs2 = find(obs_rnx.GNSSids==GNSSids ); 
    % Single PRNs
    for j = gnss_satelite_id_first:gnss_satelite_id_last 
        % get idxs of current prn
        idxs3 = find(obs_rnx.PRNs==j);
        aux = intersect( intersect(idxs1,idxs2), idxs3);
        prns_detail(i,j) = length(aux);
    end
    
end

% Preparation of reference data
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
idx_common=[];  % Vector for selecting only those positions that have also been determined by the iMar

% Fill position vectors
mode = GetDatafromStationObjects(gpsStationObj,galileoStationObj,'mode');
reference = GetDatafromStationObjects(gpsStationObj,galileoStationObj,'reference');
if strcmp(mode, 'kinematic_w_reference')
    
    secs_ref = reference.day1(:,2); 
    for i=1:length(ws.sow)
        
        % see if observed epoch is available in the iMar reference data
        idx = find(floor(secs_ref)==floor(ws.sow(i)));
        if isempty(idx)
            continue;
        elseif length(idx)==1
              
            idx_common = [idx_common; i];
            
            sow_ref = [sow_ref; secs_ref(idx)];
            lat_ref = [lat_ref; reference.day1(idx,3)*rho];
            lon_ref = [lon_ref; reference.day1(idx,4)*rho];
            h_ref = [h_ref; reference.day1(idx,5)];
            
            [x, y, z] = llh2ecef(lat_ref(end)/rho, lon_ref(end)/rho, h_ref(end));
            x_ref = [x_ref; x];
            y_ref = [y_ref; y];
            z_ref = [z_ref; z];
            
            [e, n, u] = xyz2enu(lat_ref(1)/rho,lon_ref(1)/rho,x_ref(end), y_ref(end), z_ref(end));
            e_ref = [e_ref; e];
            n_ref = [n_ref; n];
            u_ref = [u_ref; u];            
            
        elseif length(idx)>1
            
            idx_common = [idx_common; i];
            
            %disp('multiple idxs found - using first match')
            sow_ref = [sow_ref; secs_ref(idx(1))];
            lat_ref = [lat_ref; reference.day1(idx(1),3)*rho];
            lon_ref = [lon_ref; reference.day1(idx(1),4)*rho];
            h_ref = [h_ref; reference.day1(idx(1),5)];
            
            [x, y, z] = llh2ecef(lat_ref(end)/rho, lon_ref(end)/rho, h_ref(end));
            x_ref = [x_ref; x];
            y_ref = [y_ref; y];
            z_ref = [z_ref; z];
            
            [e, n, u] = xyz2enu(lat_ref(1)/rho,lon_ref(1)/rho,x_ref(end), y_ref(end), z_ref(end));
            e_ref = [e_ref; e];
            n_ref = [n_ref; n];
            u_ref = [u_ref; u];
        end
    end    
elseif strcmp(mode,'static_w_reference')
    x_ref = reference(1);
    y_ref = reference(2);
    z_ref = reference(3);
    [lat_ref, lon_ref, h_ref] = ecef2llh(x_ref,y_ref,z_ref);
    lat_ref = lat_ref*rho;
    lon_ref = lon_ref*rho;
end


% Least squares position
x_lsq = ws.ECEF_LSQ(:,1);
y_lsq = ws.ECEF_LSQ(:,2);
z_lsq = ws.ECEF_LSQ(:,3);
[lat_lsq, lon_lsq, h_lsq] = ecef2llh(x_lsq,y_lsq,z_lsq);
lat_lsq = lat_lsq*rho;
lon_lsq = lon_lsq*rho;

% Kalman filter position
x_kf = ws.ECEF_KF(:,1);
y_kf = ws.ECEF_KF(:,2);
z_kf = ws.ECEF_KF(:,3);
x_stddev = sqrt(ws.P(:,1));
y_stddev = sqrt(ws.P(:,2));
z_stddev = sqrt(ws.P(:,3));
[lat_kf, lon_kf, h_kf] = ecef2llh(x_kf,y_kf,z_kf);
lat_kf = lat_kf*rho;
lon_kf = lon_kf*rho;

% To compute ENU we refer to the tracking start as given by the iMAR
% reference 

% ENU coordinates where the tracking start is considered as the ENU origin
startPos_lat = ones(length(x_lsq),1)*lat_ref(1)/rho;
startPos_lon = ones(length(x_lsq),1)*lon_ref(1)/rho;
[e_lsq, n_lsq, u_lsq] = xyz2enu(startPos_lat,startPos_lon,x_lsq,y_lsq,z_lsq);
[e_kf, n_kf, u_kf] = xyz2enu(startPos_lat,startPos_lon,x_kf,y_kf,z_kf);    


pdop_ecef = ws.DOP(:,1);
hdop_ecef = ws.DOP(:,2);
vdop_ecef = ws.DOP(:,3);
tdop = ws.DOP(:,4);
pdop_enu = ws.DOP(:,5);
hdop_enu = ws.DOP(:,6);
vdop_enu = ws.DOP(:,7);



%==================================================================
% HTML map generation
%==================================================================
if pltconf.googlemaps

    try
        outFile.name = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_GoogleEarth');
        outFile.mode = mode;

        hours=round(hod);
        minutes=round((hod-hours)*60);
        seconds=round(((hod-hours)*60-minutes)*60);

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
            create_HTML(    outFile, [lat_kf lon_kf hours minutes seconds],     ...
                                     [lat_lsq lon_lsq hours minutes seconds],   ...
                                     [lat_ref_ lon_ref_ hours_common minutes_common seconds_common],'KF','LSQ','ITRF08/Epoch05')

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

%==================================================================
% Plot raw measurements -------------------
%==================================================================
if pltconf.rawmeas
  
    secs = ws.GPSweeksecs(:,2);
    days_sinceGpsWeek = secs/(24.0*3600.);
    days_sinceGpsWeek = round(days_sinceGpsWeek,10); % Round to 10th digit due to issues with the floor computation
    fulldays = floor(days_sinceGpsWeek);
    hod_raw = (days_sinceGpsWeek - fulldays)*24;

    figure;

    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    %C1
    subplot(4,2,1);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.C1(idxs),'.');
        ylabel('C1 [m]');
        title('C1 Signal');
        grid on;
        hold on;
    end
    %C2
    subplot(4,2,2);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.C2(idxs),'.');
        ylabel('C2 [m]');
        title('C2 Signal');
        grid on;
        hold on;
    end
    %L1
    subplot(4,2,3);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.L1(idxs),'.');
        ylabel('L1 [m]');
        title('L1 Signal');
        grid on;
        hold on;
    end
    %L2
    subplot(4,2,4);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.L2(idxs),'.');
        ylabel('L2 [m]');
        title('L2 Signal');
        grid on;
        hold on;
    end
    %D1
    subplot(4,2,5);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.D1(idxs),'.');
        ylabel('D1 [Hz]');
        title('D1 Signal');
        grid on;
        hold on;
    end
    %D2
    subplot(4,2,6);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.D2(idxs),'.');
        ylabel('D2 [Hz]');
        title('D2 Signal');
        grid on;
        hold on;
    end
    %S1
    subplot(4,2,7);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.S1(idxs),'.');
        xlabel('Hours of Day');
        ylabel('S1 [dbHz]');
        title('S1 Signal');
        grid on;
        hold on;
    end
    %S2
    subplot(4,2,8);
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        pos = get(gca,'position'); 
        pos(4) = subplot_height;
        set(gca,'position',pos);
        idxs = find(ws.prns==i);
        scatter(hod_raw(idxs),ws.S2(idxs),'.');
        xlabel('Hours of Day');
        ylabel('S2 [dbHz]');
        title('S2 Signal');
        grid on;
        hold on;
    end

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    savedfilenamewfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_InputSignals.pdf');
    print(savedfilenamewfullpath,'-dpdf')

    close all

end

%==================================================================
% Pseudorange residuals per SVid -------------------
%==================================================================

% The residuals are plotted on multiple pdfs

if pltconf.pseudorange_residuals
    subfol =  FileNameGenerator(gpsStationObj,galileoStationObj,'SubfolderforResiduals','Save');
    if ~exist(subfol, 'dir')
        mkdir(subfol)
    end

    pdfnr = 0;
    figure_index = 1;
    for i=1:maxnumber_of_satelites

        % initialize new figure
        if mod(figure_index-1,3)==0
            figure;
            if ~figure_showup
                set(gcf, 'Visible', 'off')
            end
            pos = get(gca,'position'); 
            pos(4) = subplot_height;
            set(gca,'position',pos);
            plotidx = 1;
        end

        subplot(3,1,plotidx);
        scatter(hod,ws.pseudorange_residuals_KF(:,i),'.');
        hold on
        xlabel('Hours of Day');
        ylabel('\Delta\rho [m]');
        title(sprintf('Pseudorange residuals (KF) #%02d',i));
        grid on;

        % after every third subplot, save to file
        if mod(figure_index,3)==0 && i~=1
            h=gcf;
            set(h,'PaperOrientation','landscape');
            set(h,'PaperUnits','normalized');
            set(h,'PaperPosition', [0 0 1 1]);
            filenamewfullpath = fullfile(subfol,filename);
            print([filenamewfullpath sprintf('_PRN_%02d-%02d.pdf',i-2,i)],'-dpdf')          
            close all
            pdfnr=pdfnr+1;
        end

        plotidx=plotidx+1;
        figure_index = figure_index + 1;

    end
    clear figure_index

end

%==================================================================
% Count of PRNs
%==================================================================

% as the data is tranfered through the simulink bus with 1hz, there are
% epoch duplicates within 30s arcs according to the rinex sampling.
% By selecting unique epochs, we obtain a epoch value every 30s:

if pltconf.numbersatellites

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    pos = get(gca,'position');
    pos(4) = subplot_height;
    set(gca,'position',pos);
    prn_sum_per_epoch = sum(prns_detail,2);

    scatter(hod,prn_sum_per_epoch,'.r');
    xlabel('Hours of Day');
    ylabel('Count of PRNs [#]');
    title('Number of satellites per epoch');
    grid on;

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_NumberSats.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end

%==================================================================
% PRN Availability
%==================================================================

if pltconf.satelliteavailability

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')

    pos = get(gca,'position');
    pos(4) = 0.2; % plot subplot_height
    set(gca,'position',pos);
    end

    idx_subplot = 1;
    for i=gnss_satelite_id_first:gnss_satelite_id_last
        subplot(16,2,idx_subplot);
        idx = find(prns_detail(:,i)>0);
        scatter(hod,prns_detail(:,i),'.b'); 
        hold on
        scatter(hod(idx),prns_detail(idx,i),'.r'); 
        ylim([0 1])
        set(gca,'ytick',0:1) % only integer labels along y axis 
        ylabel(sprintf('#%02d', i));
        grid on;
        idx_subplot = idx_subplot + 1;
    end
    clear idx_subplot
    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_PRNavailability.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end


%==================================================================
% DOP values -------------------
%==================================================================

if pltconf.dop
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(4,2,1);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,pdop_ecef,'.b');
    xlabel('Hours of Day');
    ylabel('PDOP ECEF');
    title('PDOP ECEF');
    grid on;

    subplot(4,2,2);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,pdop_enu,'.b');
    xlabel('Hours of Day');
    ylabel('PDOP ENU');
    title('PDOP ENU');
    grid on;

    subplot(4,2,3);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,hdop_ecef,'.b');
    xlabel('Hours of Day');
    ylabel('HDOP ECEF');
    title('HDOP ECEF');
    grid on;

    subplot(4,2,4);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,hdop_enu,'.b');
    xlabel('Hours of Day');
    ylabel('HDOP ENU');
    title('HDOP ENU');
    grid on;

    subplot(4,2,5);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,vdop_ecef,'.b');
    xlabel('Hours of Day');
    ylabel('VDOP ECEF');
    title('VDOP ECEF');
    grid on;

    subplot(4,2,6);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,vdop_enu,'.b');
    xlabel('Hours of Day');
    ylabel('VDOP ENU');
    title('VDOP ENU');
    grid on;

    subplot(4,2,7);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,tdop,'.b');
    xlabel('Hours of Day');
    ylabel('TDOP');
    title('TDOP');
    grid on;

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_KF_DOP.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end

%==================================================================
% Formal errors, i.e. standard deviation w.r.t. X, Y, Z
%==================================================================

if pltconf.stddev

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,x_stddev,'.b');
    xlabel('Hours of Day');
    ylabel('stddev [m]');
    title('Standard deviation X');
    grid on;

    subplot(3,1,2);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,y_stddev,'.b');
    xlabel('Hours of Day');
    ylabel('stddev [m]');
    title('Standard deviation Y');
    grid on;

    subplot(3,1,3);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,z_stddev,'.b');
    xlabel('Hours of Day');
    ylabel('stddev [m]');
    title('Standard deviation Z');
    grid on;


    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_KF_stddev.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
     
end


%==================================================================
% 1.) LLH LSQ -------------------
%==================================================================

if pltconf.lsqllh

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lat_lsq,'.b');
    xlabel('Hours of Day');
    ylabel('Latitude [deg]');
    title('Latitude (LSQ)');
    grid on;

    subplot(3,1,2);
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lon_lsq,'.b');
    xlabel('Hours of Day');
    ylabel('Longitude [deg]');
    title('Longitude (LSQ)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,h_lsq,'.b');
    xlabel('Hours of Day');
    ylabel('Height [m]');
    title('Height (LSQ)');
    grid on;
    % End LLH LSQ -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_LLH_LSQ.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end

%==================================================================
% 2.) ECEF LSQ -------------------
%==================================================================

if pltconf.lsqecef

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,x_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('X [m]');
    title('X (LSQ)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,y_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('Y [m]');
    title('Y (LSQ)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,z_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('Z [m]');
    title('Z (LSQ)');
    grid on;
    % End ECEF LSQ -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_ECEF_LSQ.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end


%==================================================================
% 3.) ENU LSQ -------------------
%==================================================================

if pltconf.lsqenu
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,e_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('E [m]');
    title('E (LSQ)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,n_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('N [m]');
    title('N (LSQ)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,u_lsq,'.r');
    xlabel('Hours of Day');
    ylabel('U [m]');
    title('U (LSQ)');
    grid on;
    % End ECEF LSQ -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_ENU_LSQ.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
end

%==================================================================
% 4.) LLH KF -------------------
%==================================================================

if pltconf.kfllh
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lat_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Lat [deg]');
    title('Lat (deg)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position');
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,lon_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Lon [deg]');
    title('Lon (KF)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,h_kf,'.r');
    xlabel('Hours of Day');
    ylabel('H [m]');
    title('H (KF)');
    grid on;
    % End ECEF kF -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_LLH_KF.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end

%==================================================================
% 5.) ECEF KF -------------------
%==================================================================

if pltconf.kfecef

    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,x_kf,'.r');
    xlabel('Hours of Day');
    ylabel('X [m]');
    title('X (KF)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position');
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,y_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Y [m]');
    title('Y (KF)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,z_kf,'.r');
    xlabel('Hours of Day');
    ylabel('Z [m]');
    title('Z (KF)');
    grid on;
    % End ECEF kF -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_ECEF_KF.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end

%==================================================================
% 6.) ENU KF -------------------
%==================================================================

if pltconf.kfenu
    
    figure;
    if ~figure_showup
        set(gcf, 'Visible', 'off')
    end

    subplot(3,1,1)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,e_kf,'.r');
    xlabel('Hours of Day');
    ylabel('E [m]');
    title('E (KF)');
    grid on;

    subplot(3,1,2)
    pos = get(gca,'position');
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,n_kf,'.r');
    xlabel('Hours of Day');
    ylabel('N [m]');
    title('N (KF)');
    grid on;

    subplot(3,1,3)
    pos = get(gca,'position'); 
    pos(4) = subplot_height;
    set(gca,'position',pos);
    scatter(hod,u_kf,'.r');
    xlabel('Hours of Day');
    ylabel('U [m]');
    title('U (KF)');
    grid on;
    % End ECEF kF -------------------

    % Save to file
    h=gcf;
    set(h,'PaperOrientation','landscape');
    set(h,'PaperUnits','normalized');
    set(h,'PaperPosition', [0 0 1 1]);
    filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_ENU_KF.pdf');
    print(filenamewithfullpath,'-dpdf')

    close all
    
end

%==================================================================
%==================================================================
%==================================================================
% STATIC : Estimated vs. ITRF08 position
%==================================================================
%==================================================================
%==================================================================

if strcmp(mode,'static_w_reference')
    
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
    
    if pltconf.kfdecef_static
        
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
        xlabel('Hours of Day');
        ylabel('\DeltaZ [m]');
        t=title(sprintf('dZ w.r.t. ITRF08: avg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(dz_kf),std(dz_kf),mean(dz_lsq),std(dz_lsq)));
        set(t, 'FontSize', 12);
        grid on;   

        % Save to file
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_static_ITRF08_ECEF_residuals.pdf');
        print(filenamewithfullpath,'-dpdf')

        close all
    end
 
    %==================================================================
    % STATIC: Estimated vs. ITRF08 position in LLH -------------------
    %==================================================================
    
    if pltconf.kfdllh_static
    
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
        hor_pos_err_kf = sqrt(dlon_kf.^2+dlat_kf.^2);
        hor_pos_err_lsq = sqrt(dlon_lsq.^2+dlat_lsq.^2);
        t=title(sprintf('dLon/dLat w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(hor_pos_err_kf),std(hor_pos_err_kf),mean(hor_pos_err_lsq),std(hor_pos_err_lsq)));
        set(t, 'FontSize', 12);    
        grid on



        % Save to file
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_static_ITRF08_LLH_residuals.pdf');
        print(filenamewithfullpath,'-dpdf')

        close all
        
    end
    
    %==================================================================
    % STATIC: Estimated vs. iMar position in ENU -------------------
    %==================================================================      
    
    if pltconf.kfdenu_static
        
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
        hor_pos_err_kf = sqrt(de_kf.^2+dn_kf.^2);
        hor_pos_err_lsq = sqrt(de_lsq.^2+dn_lsq.^2);
        t=title(sprintf('dE/dN w.r.t. ITRF2008/Epoch2005:\navg=%3.2f, stddev=%3.2f (KF), avg=%3.2f, stddev=%3.2f (LSQ)',mean(hor_pos_err_kf),std(hor_pos_err_kf),mean(hor_pos_err_lsq),std(hor_pos_err_lsq)));
        set(t, 'FontSize', 12);    
        grid on    

        % Save to file
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_static_ITRF08_ENU_residuals.pdf');
        print(filenamewithfullpath,'-dpdf')

        close all   
        
    end
    
end

%==================================================================
%==================================================================
%==================================================================
% KINEMATIC : Estimated vs. iMar position
%==================================================================
%==================================================================
%==================================================================

if strcmp(mode,'kinematic_w_reference')
    
        
    dlat_kf = lat_kf(idx_common) - lat_ref;
    dlon_kf = lon_kf(idx_common) - lon_ref;
    dh_kf = h_kf(idx_common) - h_ref;
    
    dlat_lsq = lat_lsq(idx_common) - lat_ref;
    dlon_lsq = lon_lsq(idx_common) - lon_ref;
    dh_lsq = h_lsq(idx_common) - h_ref;
    
    dx_kf = x_kf(idx_common) - x_ref;
    dy_kf = y_kf(idx_common) - y_ref;
    dz_kf = z_kf(idx_common) - z_ref;
    
    dx_lsq = x_lsq(idx_common) - x_ref;
    dy_lsq = y_lsq(idx_common) - y_ref;
    dz_lsq = z_lsq(idx_common) - z_ref;
    
    [de_kf, dn_kf, du_kf] = xyz2enu(lat_ref, lon_ref, dx_kf, dy_kf, dz_kf);
    [de_lsq, dn_lsq, du_lsq] = xyz2enu(lat_ref, lon_ref, dx_lsq, dy_lsq, dz_lsq);

    hod_common = hod(idx_common);
    
    
    %==================================================================
    % KINEMATIC: Estimated vs. iMar position in LLH -------------------
    %==================================================================    
    
    if pltconf.kfdllh_kinematic
        
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
        xlabel('Hours of Day');
        ylabel('\DeltaH [m]');
        title(sprintf('dH (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndH (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(dh_kf),std(dh_kf),mean(dh_lsq),std(dh_lsq)));
        grid on;

        % Save to file
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_kinematic_iMAR_LLH_residuals.pdf');
        print(filenamewithfullpath,'-dpdf')

        close all
        
    end
    
    %==================================================================
    % KINEMATIC: Estimated vs. iMar position in ECEF -------------------
    %==================================================================    

    if pltconf.kfdecef_kinematic
        
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
        xlabel('Hours of Day');
        ylabel('\Deltax [deg]');
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
        xlabel('Hours of Day');
        ylabel('\Deltay [deg]');
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
        xlabel('Hours of Day');
        ylabel('\Deltaz [m]');
        title(sprintf('dz (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndz (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(dz_kf),std(dz_kf),mean(dz_lsq),std(dz_lsq)));
        grid on;

        % Save to file
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_kinematic_iMAR_ECEF_residuals.pdf');
        print(filenamewithfullpath,'-dpdf')

        close all      
        
    end

    %==================================================================
    % KINEMATIC: Estimated vs. iMar position in ENU -------------------
    %==================================================================      
    
    if pltconf.kfdenu_kinematic
        
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
        xlabel('Hours of Day');
        ylabel('\DeltaE [deg]');
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
        xlabel('Hours of Day');
        ylabel('\DeltaN [deg]');
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
        xlabel('Hours of Day');
        ylabel('\DeltaU [m]');
        title(sprintf('dU (KF) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f\ndU (LSQ) w.r.t. iMAR: avg=%3.2f, stddev=%3.2f',mean(du_kf),std(du_kf),mean(du_lsq),std(du_lsq)));
        grid on;    

        % Save to file
        h=gcf;
        set(h,'PaperOrientation','landscape');
        set(h,'PaperUnits','normalized');
        set(h,'PaperPosition', [0 0 1 1]);
        filenamewithfullpath = FileNameGenerator(gpsStationObj,galileoStationObj,'General','Save',filename,'_kinematic_iMAR_ENU_residuals.pdf');
        print(filenamewithfullpath,'-dpdf')

        close all  
        
    end
  
end

%==================================================================
% World Map (Map toolbox required)
%==================================================================

% figure;
% if ~figure_showup
%     set(gcf, 'Visible', 'off')
% end
% 
% % World map with contintental shpes
% load coastlines;
% worldmap world;
% geoshow('landareas.shp', 'FaceColor', [0.9 0.9 0.9]);
% plotm(coastlat, coastlon);
% 
% % add lat/lon data
% plotm(lat_kf,lon_kf,'+r');
% 
% % Save to file
% h=gcf;
% set(h,'PaperOrientation','landscape');
% set(h,'PaperUnits','normalized');
% set(h,'PaperPosition', [0 0 1 1]);
% print([dest.plotpath filename '_GloMap.pdf'],'-dpdf')

end