
function h_data = load_h_data(info, data, h_data)
%% Double type data:
    if strcmp('START TIME',info) == 1
        h_data.startTime = data;
    end

    if strcmp('END TIME',info) == 1
       h_data.endTime = data;
    end

    if strcmp('START POSITION',info) == 1
       h_data.startPosition = data;
    end

    if strcmp('START VELOCITY',info) == 1
       h_data.startVelocity = data;
    end

    if strcmp('START ATTITUDE',info) == 1
       h_data.startAttitude = data;
    end

    if strcmp('EPOCH TIME',info) == 1
       h_data.epochTime = data;
    end
    
%% String type data:
    if strcmp('POSITION FORMAT',info) == 1
        h_data.positionFormat = data; 
    end

    if strcmp('CREATOR',info) == 1
        h_data.creator = data; 
    end

    if strcmp('TYPE',info) == 1
        h_data.type = data; 
    end

    if strcmp('TIME SYSTEM',info) == 1
        h_data.timeSystem = data; 
    end

    if strcmp('TIME OF CREATION',info) == 1
        h_data.timeOfCreation = data; 
    end
end
