function [PlotNames] = MakeWrongCoordinatePlots(x_lsq, y_lsq, z_lsq,                                        ...
                                                x_kf, y_kf, z_kf,                                           ...
                                                x_ref, y_ref, z_ref,                                        ...
                                                Tolerance, Borderange, SearchMode,                          ...
                                                MyVariables, MyVariablesRef, Filename,                      ...
                                                Pltconf, Mode, idx_Common, Subplot_height, Figure_showup,   ...
                                                Maxnumber_of_satelites, varargin)


     PlotNames{1,1} = {};
     PlotNames{1,2} = {};
     
     wrongGroupsIndeces = GNSS_Visualization.PlotWrongCoordinates.getIndecesofWrongElementsToMap(   x_lsq, y_lsq, z_lsq,                ...
                                                                                                    x_kf, y_kf, z_kf,                   ...
                                                                                                    x_ref, y_ref, z_ref,                ...
                                                                                                    Tolerance, Borderange, SearchMode);

    [~,numberOfGroups] = size(wrongGroupsIndeces);

    if numberOfGroups > 0

        for i = 1:length(MyVariables)
           try 
               tmpVar = evalin('caller', MyVariables{i});
               createVariablefromDataandName(tmpVar, MyVariables{i});
           catch ME
               warning(ME.message); 
               continue
           end
        end
        
        for i = 1:length(MyVariablesRef)
           try 
               tmpVar = evalin('caller', MyVariablesRef{i});
               createVariablefromDataandName(tmpVar, MyVariablesRef{i});
           catch ME
               warning(ME.message); 
               continue
           end
        end
       
        
        for i = 1 : numberOfGroups

            %==================================================================
            % HTML map generation
            %==================================================================
           try

                wrongindx = wrongGroupsIndeces{1,i};
                GNSS_Visualization.PlotWrongCoordinates.saveWrongIndeces(MyVariables,wrongindx);
                GNSS_Visualization.PlotWrongCoordinates.saveWrongIndeces(MyVariablesRef,wrongindx);
                plotName = strcat('_GoogleEarth_Wrong_Elements_', num2str(i));

                if i == 1
                    if nargin == 21 + 1 && ~isempty(varargin{1})
                        if ischar(varargin{1})
                            tmpString = varargin{1};
                        else
                            warning('Wrong path format, current path is used instead.')
                            
                            tmpString = filehandler.FileHandler.GenerateFileName('General', Filename,plotName);
                            tmpString = strrep(tmpString, [Filename, plotName],''); 
                        end
                    else
                        tmpString = filehandler.FileHandler.GenerateFileName('General', Filename,plotName);
                        tmpString = strrep(tmpString, [Filename, plotName],''); 
                    end
                    FolderName = ['Wrong Coordinate Plots - ', datestr(now, 'dd mmm yyyy HH-MM-SS')];
                    Folder = [tmpString, '\', FolderName];
                    if ~exist(Folder, 'dir')
                        mkdir(Folder);
                    end
                end

                filenamewpath_GoogleEarth = filehandler.FileHandler.GenerateFileName('SubfolderforWrongCoordinatePlots',Filename,plotName, Folder);

               [filename_static_w_reference,    ...
                filename_kinematic_reference,   ...
                filename_without_reference         ] = GNSS_Visualization.plots.plot_HTML_map(  Pltconf, Mode,                                                  ...
                                                                                                hod_common_w_ref_Wrong,         idx_Common,                     ...
                                                                                                lat_ref_Wrong,                  lon_ref_Wrong,                  ...
                                                                                                lat_kf_common_w_ref_Wrong,      lon_kf_common_w_ref_Wrong,      ...
                                                                                                lat_lsq_common_w_ref_Wrong,     lon_lsq_common_w_ref_Wrong,     ...
                                                                                                filenamewpath_GoogleEarth,                                      ...
                                                                                                Subplot_height,                 Figure_showup );
                                                    
                if ~isempty(filename_without_reference)
                    PlotNames{1,1} = [PlotNames{1,1}; {filename_without_reference}];
                end
                if ~isempty(filename_kinematic_reference)
                    PlotNames{1,1} = [PlotNames{1,1}; {filename_kinematic_reference}];
                end
                if ~isempty(filename_static_w_reference)
                    PlotNames{1,1} = [PlotNames{1,1}; {filename_static_w_reference}];
                end
                
           catch ME
                warning(ME.message)
           end

            %==================================================================
            % Pseudorange residuals per SVid -------------------
            %==================================================================

           try

                subfol_SubfolderforResidualsWrongCoordinates = filehandler.FileHandler.GenerateFileName('SubfolderforResidualsWrongCoordinates',Folder, num2str(i));
                GNSS_Visualization.plots.plot_pseudorange_residuals(    Pltconf,                   hod_Wrong,                               ...
                                                                        pseudorange_residuals_KF_Wrong,   Maxnumber_of_satelites,           ...
                                                                        subfol_SubfolderforResidualsWrongCoordinates,                       ...
                                                                        Filename,                   Figure_showup,                          ...
                                                                        Subplot_height);
            catch ME
                disp(['WARNING: <a href="matlab:opentoline(which(''GNSS_Visualization.visualize''),303,0);">GNSS_Visualization::visualize</a>: ' ME.message])
           end

            %==================================================================
            % KINEMATIC : Estimated vs. iMar position
            %==================================================================
            try %% Plotting: Kinematic measurement evaluation: Estim. vs iMar

            plotName = strcat('_kinematic_iMAR_LLH_residuals_', num2str(i));
            filenamewithfullpath_kinematic_iMAR_LLH_residuals = filehandler.FileHandler.GenerateFileName('SubfolderforWrongCoordinatePlots',Filename,plotName, Folder);
            
            plotName = strcat('_kinematic_iMAR_ECEF_residuals_', num2str(i));
            filenamewithfullpath_kinematic_iMAR_ECEF_residuals = filehandler.FileHandler.GenerateFileName('SubfolderforWrongCoordinatePlots',Filename,plotName, Folder);
            
            plotName = strcat('_kinematic_iMAR_ENU_residuals_', num2str(i));
            filenamewithfullpath_kinematic_iMAR_ENU_residuals = filehandler.FileHandler.GenerateFileName('SubfolderforWrongCoordinatePlots',Filename,plotName, Folder);
            

            GNSS_Visualization.plots.plot_evaluate_kinematic_iMar(  Pltconf,                    Mode,                       hod_common_w_ref_Wrong,     ...
                                                                    lat_ref_Wrong,              lon_ref_Wrong,              h_ref_Wrong,                ...
                                                                    x_ref_Wrong,                y_ref_Wrong,                z_ref_Wrong,                ...
                                                                    lat_lsq_common_w_ref_Wrong, lon_lsq_common_w_ref_Wrong, h_lsq_common_w_ref_Wrong,	...
                                                                    x_lsq_common_w_ref_Wrong,   y_lsq_common_w_ref_Wrong,   z_lsq_common_w_ref_Wrong,	...
                                                                    lat_kf_common_w_ref_Wrong,  lon_kf_common_w_ref_Wrong,  h_kf_common_w_ref_Wrong,	...
                                                                    x_kf_common_w_ref_Wrong,    y_kf_common_w_ref_Wrong,    z_kf_common_w_ref_Wrong,    ...
                                                                    filenamewithfullpath_kinematic_iMAR_LLH_residuals,                                  ...
                                                                    filenamewithfullpath_kinematic_iMAR_ECEF_residuals,                                 ...
                                                                    filenamewithfullpath_kinematic_iMAR_ENU_residuals,                                  ...
                                                                    Subplot_height, Figure_showup, {'-dpdf', '-dpng'})
                                                                
            if i == 1
                PlotNames{1,2} = [filenamewithfullpath_kinematic_iMAR_LLH_residuals, '.png'];
                PlotNames{1,2} = [PlotNames{1,2}; {[filenamewithfullpath_kinematic_iMAR_ECEF_residuals, '.png']}];
                PlotNames{1,2} = [PlotNames{1,2}; {[filenamewithfullpath_kinematic_iMAR_ENU_residuals, '.png']}];
            else
                PlotNames{1,2} = [PlotNames{1,2}; {[filenamewithfullpath_kinematic_iMAR_LLH_residuals, '.png']}];
                PlotNames{1,2} = [PlotNames{1,2}; {[filenamewithfullpath_kinematic_iMAR_ECEF_residuals, '.png']}];
                PlotNames{1,2} = [PlotNames{1,2}; {[filenamewithfullpath_kinematic_iMAR_ENU_residuals, '.png']}];
            end
            
            catch ME
                disp(['WARNING: <a href="matlab:opentoline(which(''GNSS_Visualization.visualize''),559,0);">GNSS_Visualization::visualize</a>: ' ME.message])
            end      
       end
    else   
       disp('No wrong coordinates found.');
    end
end


    
function [] = createVariablefromDataandName(Data, Name)
    if ischar(Name)
        assignin('caller', Name, Data);
    else
        warning('Name must be a string!');
    end
end