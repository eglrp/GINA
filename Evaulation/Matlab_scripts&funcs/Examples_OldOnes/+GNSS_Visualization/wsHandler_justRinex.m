function workspace = wsHandler_justRinex(GNSS_SolOut,InitObject)

%%
    obs             = InitObject.GetgnssObsFromRinexIn();
	obstypes        = InitObject.GetgnssObsTypeStr();
    gpsSolution     = GNSS_SolOut;
    ws.obstypes   	= obstypes;    
    ws.GPSweeksecs	= timeseries([obs(:,1) obs(:,2)],'Name','GPS weeks and secs');
    
    
%%
% ws.gnssId     = obs(:,3); 
ws.SatId = timeseries([obs(:,3) obs(:,4)],obs(:,2),'Name','Satellite ID and gnss ID');
% ws.Galileo.prns = obs(:,3); 

%%
% ws.svId =  obs(:,4);

%% fill observation vectors

    fieldnames_from_gnssObsFromRinexIn     = {	'C1', ...
                                                'L1', ...
                                                'D1', ...
                                                'S1', ...
                                                'C2', ...
                                                'L2', ...
                                                'D2', ...
                                                'S2'};

    for a=1:length(ws.obstypes)
        for b=1:length(fieldnames_from_gnssObsFromRinexIn)
            if strcmp(ws.obstypes(a,:),fieldnames_from_gnssObsFromRinexIn{b})           
                ws.(fieldnames_from_gnssObsFromRinexIn{b})        = timeseries(obs(:,a+4),obs(:,2), 'Name', fieldnames_from_gnssObsFromRinexIn{b});
            end
        end
    end

    for b=1:length(fieldnames_from_gnssObsFromRinexIn)
        if ~isfield(ws,fieldnames_from_gnssObsFromRinexIn{b})
            Nobs = size(obs,1);
         	ws.(fieldnames_from_gnssObsFromRinexIn{b})        = timeseries(zeros(Nobs,1),obs(:,2), 'Name', fieldnames_from_gnssObsFromRinexIn{b});
        end
    end
% Read position and corresponding epochs under consideration of the
% user-defined sampling

 %%
try
% 	ws.sow      = gpsSolution.obsTime;%evalin('base','resObsTime_Gps');
    ws.sow = timeseries(gpsSolution.obsTime.Data, 'Name', 'Second of Week');
% 	ws.Galileo.sow  = galileoSolution.Values.obsTime.Data;%evalin('base','resObsTime_Galileo');
catch
        error('There is no "resObsTime" or "resObsTime_Gps" or "resObsTime_Galileo" in the base workspace.')
end
% [ws.sow, idxs_Gps]        	= unique(ws.sow);
% [ws.Galileo.sow, idxs_Galileo] 	= unique(ws.Galileo.sow);

ws.hod = timeseries(GNSS_Visualization.compute_hours_of_day_from_seconds_of_week(ws.sow.Data), 'Name', 'Hour of Day');
% ws.hod.Data         = compute_hours_of_day_from_seconds_of_week(ws.sow.Data);
% ws.Galileo.hod     = compute_hours_of_day_from_seconds_of_week(ws.Galileo.sow);

%% Gps Interface
ws.LLH_LSQ                  = gpsSolution.leastSquaresPosition.LLH;
ws.ECEF_LSQ                 = gpsSolution.leastSquaresPosition.ECEF;
ws.ECEF_KF                  = gpsSolution.kalmanFilter.x;  
ws.DOP                      = gpsSolution.kalmanFilter.dop;
ws.P                       	= gpsSolution.kalmanFilter.P;
ws.pseudorange_residuals_KF = gpsSolution.kalmanFilter.dy;

% ws.ECEF_LSQ                 = ws.ECEF_LSQ(idxs_Gps,:);
% ws.ECEF_KF                  = ws.ECEF_KF(idxs_Gps,:);
% ws.DOP                      = ws.DOP(idxs_Gps,:);
% ws.P                       	= ws.P(idxs_Gps,:);
% ws.pseudorange_residuals_KF = ws.pseudorange_residuals_KF(idxs_Gps,:);
       
%% Galileo Interface                                   
% ws.Galileo.LLH_LSQ                  = galileoSolution.Values.leastSquaresPosition.LLH.Data;
% ws.Galileo.ECEF_LSQ                 = galileoSolution.Values.leastSquaresPosition.ECEF.Data;
% ws.Galileo.ECEF_KF                  = galileoSolution.Values.kalmanFilter.x.Data;
% ws.Galileo.DOP                      = galileoSolution.Values.kalmanFilter.dop.Data;
% ws.Galileo.P                        = galileoSolution.Values.kalmanFilter.P.Data;
% ws.Galileo.pseudorange_residuals_KF = galileoSolution.Values.kalmanFilter.dy.Data;
% ws.Galileo.stec_KF                  = galileoSolution.Values.kalmanFilter.iono_biased.Data;
% 
% ws.Galileo.LLH_LSQ                  = ws.Galileo.LLH_LSQ(idxs_Galileo,:);
% ws.Galileo.ECEF_LSQ                 = ws.Galileo.ECEF_LSQ(idxs_Galileo,:);
% ws.Galileo.ECEF_KF                  = ws.Galileo.ECEF_KF(idxs_Galileo,:);
% ws.Galileo.DOP                      = ws.Galileo.DOP(idxs_Galileo,:);
% ws.Galileo.P                        = ws.Galileo.P(idxs_Galileo,:);
% ws.Galileo.pseudorange_residuals_KF = ws.Galileo.pseudorange_residuals_KF(idxs_Galileo,:);
% ws.Galileo.stec_KF                  = ws.Galileo.stec_KF(idxs_Galileo,:);

%% return parameters
workspace = ws;

end