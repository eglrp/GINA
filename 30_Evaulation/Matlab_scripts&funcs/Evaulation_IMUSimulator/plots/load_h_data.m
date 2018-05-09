function h_data = load_h_data(info, data, h_data)

if strcmp('START TIME',info) == 1
h_data.startTime = data;
end

if strcmp('END TIME',info) == 1
   h_data.endTime = data;
end
if strcmp('POSITION FORMAT',info) == 1
   h_data.positonFormat = data;
end

if strcmp('START POSITION',info) == 1
   h_data.startPositon = data;
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

% if strcmp('POSITION FORMAT',info) == 1
%    h_data.positonFormat = data; %bõvítsd ki a keywords{}-t! 
% end
end
