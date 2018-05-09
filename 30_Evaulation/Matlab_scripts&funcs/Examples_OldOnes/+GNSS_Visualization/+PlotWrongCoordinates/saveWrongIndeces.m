function saveWrongIndeces(myVariables,wrongidx)
     
        
 for i = 1:length(myVariables)      
        try  
           temp_variable = evalin('caller',myVariables{i}); 
        catch ME
           warning(ME.message)
           temp_variable = [];
        end
        
        [row,column] = size(temp_variable);
        
        if (row == 1 && column > 1) || (column == 1 && row > 1)
            saveMode =  'Vector';
        elseif row > 1 && column > 1
            saveMode = 'Matrix';
        else
            saveMode = 'Unknown_saveMode';
        end
            
        switch saveMode
            case 'Matrix'
                temp_matrix_new = zeros(length(wrongidx),column);
                for idx = 1 : column
                    temp_matrix_new(:,idx) = GNSS_Visualization.PlotWrongCoordinates.saveElementsofArraybyGivenIndeces(temp_variable(:,idx),wrongidx);
                end
                varName = strcat(myVariables{i},'_Wrong'); 
                assignin('caller',varName, temp_matrix_new);
            case 'Vector'
                 temp_variable  = GNSS_Visualization.PlotWrongCoordinates.saveElementsofArraybyGivenIndeces(temp_variable,wrongidx);
                 varName = strcat(myVariables{i},'_Wrong'); 
                 assignin('caller',varName,temp_variable);
            otherwise
                warning('Unknown save mode.')         
        end
            
 end     
end