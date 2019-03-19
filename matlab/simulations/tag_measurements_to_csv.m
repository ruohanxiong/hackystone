% station_sim.m produces a 'tags' and 'anchorLocations' object which 
% contains measurements for a single tag. This file produces two CSV files,
% one containing the tag measurements and one containing anchor data

% Intend to be used for initializing redis for testing purposes

tagId = 1;
tagHeaders = {'tagId' 'anchorId' 'rssi' 'timestamp'};
% store [tagId, anchorId, rssi, timestamp]
tagM = [tagId*ones(numel(tags{1}.data),1)...
    [tags{1}.data.id]'...
    [tags{1}.data.rssi]'...
    [tags{1}.data.time]'];


csvwrite_with_headers('tagdata.csv', tagM, tagHeaders);

anchorM = []
for i=1:numel(anchorLocations)
    anchor = anchorLocations{i};
    anchorM = [anchorM; [i anchor.X anchor.Y]]
end

anchorHeaders = {'anchorId', 'X', 'Y'}

csvwrite_with_headers('anchordata.csv', anchorM, anchorHeaders);