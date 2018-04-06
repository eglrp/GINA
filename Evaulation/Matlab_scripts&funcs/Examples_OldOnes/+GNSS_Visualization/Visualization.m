function Visualization()
try
    
    def = initGnss();
    GNSS_SolOut = evalin('base','GNSS_SolOut');
    InitObject  = SimulinkInputInit.VisualizationObjectTranpsorter.GetVisualization();
    ws          = GetSimulationOutput(InitObject.GetRunType(),GNSS_SolOut,InitObject);
    ws          = AddData2LeverarmsCompensation(ws);
    ws          = AddData2deltaSTECplots(ws);
    
    gpsMeasurementInfo      = getMeasurementInfobyProvider(filehandler.FileHandler.GetMeasInfoContainer().getAllMeasurementInfoinCellFormat(),'GPS');
    galileoMeasurementInfo  = getMeasurementInfobyProvider(filehandler.FileHandler.GetMeasInfoContainer().getAllMeasurementInfoinCellFormat(),'Galileo');
    maskData                = LoadandProcessLogicMasks(gpsMeasurementInfo,galileoMeasurementInfo);
    
    MaskObject              = GNSS_Visualization.LogicMask(maskData);
    filename_for_plots     	= filehandler.FileHandler.GenerateFileName('Visualization');
	ReporterFileNameStore 	= GNSS_Visualization.visualize(ws, InitObject, MaskObject, filename_for_plots, def);
    
    %% Dummy Report Construct
        myReporter = Reporter.GNSSReportGenerator();      
        myReporter.setReporterFileNameStoreObj(ReporterFileNameStore);
        myReporter.setNameofTheReport('Report GPS and Galileo');
        myReporter.setDirectoryofReport(getReportPath(gpsMeasurementInfo,galileoMeasurementInfo));
        myReporter.setFileNameofReport('Report_GPS_and_Galileo');
        myReporter.setExcelFile(which('\00DevelopmentEnvironement\ReportGeneration\+Reporter\GPS_Galileo_Text_Sheet.xlsx'));  
        myReporter.setReportScriptName('Reporter.buildGPS_and_Galileo');
        
        myReporter.setMeasurementInfoObjforGPS(gpsMeasurementInfo);
        myReporter.setMeasurementInfoObjforGalileo(galileoMeasurementInfo);

        myReporter.Report();
    
catch ME
    disp(['WARNING: <a href="matlab:opentoline(which(''GNSS_Visualization.Visualization''),50,0);">GNSS_Visualization::Visualization</a>: ' ME.message])
end
end


function measurementInfo = getMeasurementInfobyProvider(measContainersinCell,provider)

    measurementInfo = [];
    
    for i = 1:length(measContainersinCell)
        temp = measContainersinCell{i};
        if strcmp(temp.GetProvider(),provider);
            measurementInfo = temp;
            return;
        end
    end

end

function Path = getReportPath(gpsMeasurementInfo, galileoMeasurementInfo)
    Path = pwd;
    
    if ~isempty(gpsMeasurementInfo())
        Path = gpsMeasurementInfo.plotpath();
    end
    if ~isempty(galileoMeasurementInfo())
        Path = galileoMeasurementInfo.plotpath();
    end

end

function maskData = LoadandProcessLogicMasks(gpsMeasurementInfo,galileoMeasurementInfo)
    try
        maskData = load(fullfile(gpsMeasurementInfo.GetMaskLogicPath(),gpsMeasurementInfo.GetMaskLogicFile()));
    catch ME
        maskData = [];
    end
end

function ws = GetSimulationOutput(runType,GNSS_SolOut,InitObject)
%     switch runType
%         
%         case 'justRinex'
            ws        	= GNSS_Visualization.wsHandler_justRinex(GNSS_SolOut,InitObject);   
%             
%         case 'justVMPS'
%             ws        	= GNSS_Visualization.wsHandler_justVMPS(GNSS_SolOut,InitObject);             
%             
%         otherwise
%             warning('Unknow Init GNSS Simulink type.')  
%             ws = [];
%     end
end


function ws = AddData2LeverarmsCompensation(ws)
    
   try
        Chassis     = evalin('base','Parameters.VMPS.Car.Chassis');
        try
            Roll    = evalin('base','objRef.roll.Data');
            Pitch   = evalin('base','objRef.pitch.Data');
            Heading = evalin('base','objRef.Heading_rad.Data');
        catch ME
            Roll    = [];
            Pitch   = [];
            Heading = [];
            warning(ME.message)
        end
        
        ws.leverarms.Roll               = Roll*pi/180;
        ws.leverarms.Pitch              = Pitch*pi/180;
        ws.leverarms.Heading            = Heading;
        ws.leverarms.Chassis            = Chassis;
        
   catch ME
       warning(ME.message)
   end
end

function ws = AddData2deltaSTECplots(ws)
    
   try
        LogSVelevation = SimulinkVariableLogger.handleInstance('get');
        def = initGnss();

        TimeElevationSVxx = cell(def.MAX_NUM_obs,3);
        for i = 1:def.MAX_NUM_obs
            svIDsring   = num2str(i);
            TimeSV      = strcat('timeSV', svIDsring);
            ElevationSV = strcat('elevationSV', svIDsring);
            GnssIDSV    = strcat('gnssIDSV', svIDsring);
            try
                TimeElevationSVxx{i,1} = LogSVelevation.targets(GnssIDSV);
            catch
                TimeElevationSVxx{i,1} = [];
            end
            try
                TimeElevationSVxx{i,2} = LogSVelevation.targets(TimeSV);
            catch
                TimeElevationSVxx{i,2} = [];
            end
            try
                TimeElevationSVxx{i,3} = LogSVelevation.targets(ElevationSV);
            catch
                TimeElevationSVxx{i,3} = [];
            end    
        end
        
        DeltaSTEC_SolOut = evalin('base','DeltaSTEC_SolOut');
                
        ws.TimeElevationSVxx    = TimeElevationSVxx;
        ws.DeltaSTEC_SolOut     = DeltaSTEC_SolOut;
        
        ws.def                  = def;
        
                
   catch ME
       warning(ME.message)
   end
end





