function workspace = wsHandler_justVMPS(GNSS_SolOut,InitObject)

%%
    gpsSolution     = GNSS_SolOut;

    ws.SatIdGPS     = InitObject.GetPrnAvaibility('GPS');
    ws.SatIdGalileo = InitObject.GetPrnAvaibility('Galileo');
    
    obs             = InitObject.GetgnssObsFromRinexIn();
	obstypes        = InitObject.GetgnssObsTypeStr();

    ws.obstypes   	= obstypes;    
    ws.GPSweeksecs	= [];
    
    
%%
% ws.gnssId     = obs(:,3); 
ws.SatId = [];
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

ws.hod = timeseries(compute_hours_of_day_from_seconds_of_week(ws.sow.Data), 'Name', 'Hour of Day');
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