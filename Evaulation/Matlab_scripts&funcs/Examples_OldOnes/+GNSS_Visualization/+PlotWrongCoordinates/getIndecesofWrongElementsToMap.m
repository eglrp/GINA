function WrongIndeces = getIndecesofWrongElementsToMap(xLsq, yLsq, zLsq,    ...
                                                       xKf, yKf, zKf,       ...
                                                       x_ref, y_ref, z_ref, ...
                                                       tolerance, borderRange, searchMode)
    WrongIndeces = [];
  
    if borderRange < 1
        borderRange = 1;
    end
    if strcmp(searchMode,'Distance') == 0 && strcmp(searchMode,'Coordinate') == 0
        warning('Unknown search mode, default is used: Distance')
    end
    
    wrongGroups = {};
    Group = [];
    j = 1;
    newGroup = true;
    for i = 1 : length(xLsq)
        switch searchMode
            case 'Coordinate'
                error = abs(xLsq(i) - x_ref(i));
                if abs(yLsq(i) - y_ref(i)) > error
                    error = abs(yLsq(i) - y_ref(i));
                end
                if abs(zLsq(i) - z_ref(i)) > error
                    error = abs(zLsq(i) - z_ref(i));
                end
                
                if abs(xKf(i) - x_ref(i)) > error
                    error = abs(xKf(i) - x_ref(i));
                end
                if abs(yKf(i) - y_ref(i)) > error
                    error = abs(yKf(i) - y_ref(i));
                end
                if abs(zKf(i) - z_ref(i)) > error
                    error = abs(zKf(i) - z_ref(i));
                end
                
            otherwise
                errorLsq = sqrt((xLsq(i)-x_ref(i))^2 + (yLsq(i)-y_ref(i))^2 + (zLsq(i)-z_ref(i))^2);
                errorKf  = sqrt((xKf(i)-x_ref(i))^2 + (yKf(i)-y_ref(i))^2 + (zKf(i)-z_ref(i))^2);
                if errorLsq > errorKf
                    error = errorLsq;
                else
                    error = errorKf;
                end
        end
           
        if error > tolerance
            if i < borderRange && newGroup == true
                if isempty(Group)
                    for ii = 1 : i - 1
                        Group = [Group;ii];   
                    end
                else
                    for ii = size(Group) : i - 1  
                        Group = [Group;ii];   
                    end
                end
            elseif i >= borderRange && newGroup == true
                for ii = i - (borderRange+1) : i - 1 
                    Group = [Group;ii];
                end
            end
            newGroup = false;
            Group = [Group;i];
        else
            if newGroup == false
                if length(xLsq) - i > borderRange
                    for ii = i + 1 : i + borderRange
                        Group = [Group;ii];
                    end
                else     
                    for ii = i + 1 : length(xLsq)
                        Group = [Group;ii];
                    end
                end
                wrongGroups{j} = Group; 
                if j > 1 && sum(ismember(wrongGroups{j-1},wrongGroups{j})) > 0
                    wrongGroups{j-1} = unique([wrongGroups{j-1};wrongGroups{j}]);
                    wrongGroups{j} = [];
                    wrongGroups = wrongGroups(~cellfun(@isempty,wrongGroups));
                    j = j - 1;
                end
                Group = [];
                j = j + 1;
                newGroup = true;                
            end
        end
    end

   WrongIndeces = wrongGroups; 

end