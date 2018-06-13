%% GNSS Simulator Data Evaluator 0.1
% Plots the Pseudorange and Navigation solution deviaton
% from the output of the GNSSSIM program.
%% Important variables: 
%   navsol_cell     Navigation Solution for each epoch
%   rover_cell      Rover position for each epoch
%   eval_cell       Contains parsed data - bad format for plotting, needs
%                   reworking
%   SolutionMatrix  Contains the solutions sorted by epoch and gpsID.
%   FinalCell       Contains transformed parse data - good format for plot


clearvars;

%% Config
% Todo: Parse config output file from GNSSSIM

% filename = 'C:\Local WorkSpace\Cpp\GnssSimulator\Simulator\TrajectoryTestFiles\output_RaimSolution_test.txt';
filename = 'C:\Local WorkSpace\Cpp\CompetenceProject\GNSSSimulator\TrajectoryTestFiles\output_RaimSolution_test.txt';

fileID = fopen(filename,'r');

epoch_iterator = 1;
epochnum = 0;
epochend = 0;
solutionread = 0;
roverposread = 0;

%% Parse GNSS Sim Output file
while ~feof(fileID)                     
    tline = fgets(fileID);
    currline = sprintf('%s %u %f %s',tline);
    try
        
        if ~solutionread                                                    %% If solution is read, only one more parse(Rover pos) until next epoch
            [solx soly solz] = strread(tline,'%f %f %f');                   % try-catch -> if solution fails, there are still data to be read
            solutionread = 1;
            navsol_cell(epochnum+1,1:5) = [gpsweek gpssec solx soly solz];
        elseif ~roverposread && solutionread                                    %% Rover position, last line in epoch
            [tempvar roverx rovery roverz] = strread(tline,'%s %f %f %f');
            rover_cell(epochnum+1,1:5) = [gpsweek gpssec roverx rovery roverz];
            roverposread = 1;
            epochend = 1;
        end
%         epochend = 1;
        if epochend
            eval_cell(epochnum+1,1:3) = [gpsweek gpssec {SolutionMatrix}];  % Cell to fill with all the data
            solutionread = 0;
            roverposread = 0;                                               % Last line in epoch is read, proceed to next line
            epoch_iterator = 1;
            epochnum = epochnum + 1;
            SolutionMatrix = [];
            continue;
        end
        continue;
    catch
        epochend = 0;
    end
    
    if epoch_iterator == 1      % Header
        [head gpsweek gpssec sys] = strread(tline,'%s %u %f %s');
    elseif  epochend            % Last Epoch Data - ECEF Solution %% This line might be redundant.
       continue;
    elseif ~solutionread && ~roverposread && ~epochend                    % GPS ID - Pseudorange -> Data line, ~middle of epoch
%         [sys gpsID Prange satposx satposy satposz] = strread(tline,'%s %u %f %f %f %f');
%         SolutionMatrix(epoch_iterator-1,1:5) = [gpsID Prange satposx satposy satposz];
        [sys gpsID Prange] = strread(tline,'%s %u %f');
        SolutionMatrix(epoch_iterator-1,1:2) = [gpsID Prange];
    end

    epoch_iterator = epoch_iterator+1;   
end
%% Transform Data Cell

EpochMatrix = [eval_cell{:,1};eval_cell{:,2}]';
EMsize = size(EpochMatrix);
EMsize = EMsize(1,1);

for id_it = 1:32                                                            %Loop through satellites
    epochnum = 1;
    for epoch_it = 1:EMsize                                                 %Loop through epochs
        DataMatrix =  eval_cell{epoch_it,3}(:,1);
        Datasize = size(DataMatrix);
        Datasize = Datasize(1,1);
        
        for data_it = 1:Datasize
            current_data = eval_cell{epoch_it,3}(data_it,1);          
            if current_data == id_it
                data = eval_cell{epoch_it,3}(data_it,2);
                timestamp(epochnum,1:2) = EpochMatrix(epoch_it,:);
                timestamp(epochnum,3) = data;
                epochnum = epochnum+1;
            end
        end
    end
    if exist('timestamp','var')
        FinalCell(id_it,1:2) = [id_it {timestamp}];
        clearvars timestamp;
    end
end

%% Evaluate and plot Data %%
%
%
%% Pseudorange and Solution Deviance
figure;
Cellsize = size(FinalCell);
Cellsize = Cellsize(1,1);
%%%%%% Plot Pseudoranges for PRNs %%%%%%
for satid_it = 1:Cellsize
    if ~isempty(FinalCell{satid_it})
        data = FinalCell{satid_it,2};
        timematrix = data(:,1:2);
        datavector = data(:,3);
        
        graphlegend = sprintf('PRN: %d',satid_it);
        plot(timematrix(:,2),datavector,'x-','DisplayName',graphlegend,'color',rand(1,3))
        hold all;
        legend('-DynamicLegend','location','best');
        
        text(timematrix(end,2),datavector(end),graphlegend);       
        text(timematrix(1,2),datavector(1),graphlegend,'HorizontalAlignment','right');
    end
end
ylabel('Pseudorange [m]');

%%%%%%% Plot the Solution deviation from the Rover reference point %%%%%%%%
% Solution_Deviation = sqrt((navsol_cell(:,3)-rover_cell(:,3)).^2+(navsol_cell(:,4)-rover_cell(:,3)).^2+(navsol_cell(:,5)-rover_cell(:,3)).^2);
Solution_Deviation = arrayfun(@sqrt,(navsol_cell(:,3)-rover_cell(:,3)).^2+(navsol_cell(:,4)-rover_cell(:,4)).^2+(navsol_cell(:,5)-rover_cell(:,5)).^2);

yyaxis right

plot(navsol_cell(:,2),Solution_Deviation,'o:','DisplayName','Solution Deviance')
legend('-DynamicLegend','location','best');
title('Visualization of Pseudorange variations and absolute Solution deviance');
xlabelformat = sprintf('GPS Time axis for GpsWeek %d ',navsol_cell(1,1));
xlabel(xlabelformat);

ylabel('Deviance of the navigation solution [m]');

hold off;
 
%% Satellite Trajectory Plot
%GNSS_SatTrajectory;


%% Plot per-axis Deviance
wgs84 = wgs84Ellipsoid('meter');
[reflat reflon refh] = ecef2geodetic(wgs84,rover_cell(:,3),rover_cell(:,4),rover_cell(:,5));
[calclat calclon calch] = ecef2geodetic(wgs84,navsol_cell(:,3),navsol_cell(:,4),navsol_cell(:,5));


figure;
subplot(2,2,1);
plot(navsol_cell(:,2),abs(rover_cell(:,3)-navsol_cell(:,3)))
xlabel('GPS SOW');
ylabel('Deviance [m]');
title('X coordinate Deviation');

subplot(2,2,2);
plot(navsol_cell(:,2),abs(rover_cell(:,4)-navsol_cell(:,4)))
xlabel('GPS SOW');
ylabel('Deviance [m]');
title('Y coordinate Deviation');

subplot(2,2,3);
plot(navsol_cell(:,2),abs(rover_cell(:,5)-navsol_cell(:,5)))
xlabel('GPS SOW');
ylabel('Deviance [m]');
title('Z coordinate Deviation');

figure;
subplot(2,2,1);
plot(navsol_cell(:,2),abs(reflat-calclat));
title('Latitude deviation');

subplot(2,2,2);
plot(navsol_cell(:,2),abs(reflon-calclon));
title('Longitude deviation');

subplot(2,2,3);
plot(navsol_cell(:,2),abs(refh-calch));
title('Height deviation');


%% Create HTML Plot

html_name = 'EVAL_HhtmlPlot';
file.name = html_name;
file.mode = [file.name,'_w_kinematic_reference'];  %_w_kinematic_reference , _w_static_reference

% Convert GPS Time
wk = rover_cell(:,1);
sow = rover_cell(:,2);
dow = sow/86400;
hr = floor(24 * (dow - floor(dow)));
min = rem(24 * (dow - floor(dow)),1)*60;
sec = rem(min,1)*60;
min = floor(min);
sec = round(sec);

%  create_HTML(    outFileStatic, [lat_kf lon_kf hours minutes seconds],     ...
%                                      [lat_lsq lon_lsq hours minutes seconds],   ...
%                                      [lat_ref_ lon_ref_ hours_common minutes_common seconds_common],'KF','LSQ','Static_Reference')
create_HTML(file,[reflat reflon hr min sec],...
                        [calclat calclon hr min sec],...
                        'Reference','Calculated');
winopen([file.name,'.html']);
