%%
% This file simulates rssi measurements from multiple anchors (anchorLocations)
% for multiple tags (tags). The position fields x and y are self-explanatory.
% tags{i}.data is a struct array, where each struct contains a
% time, rssi, and id field, indicating the time of recieval, rssi value,
% and the anchor id (matches the index in anchorLocations).

%% Load data
anchorLocations{1}.X = 0;
anchorLocations{1}.Y = 0;

anchorLocations{2}.X = 10;
anchorLocations{2}.Y = 0;

anchorLocations{3}.X = 10;
anchorLocations{3}.Y = 10;

anchorLocations{4}.X = 0;
anchorLocations{4}.Y = 10;

tags{1}.X = 5;
tags{1}.Y = 5;
tags{1}.data = struct([]);

%% Generate simulated data
% model: RSSI = -10*n*log(d) + A
n = 1.4267;
A = -59.0636;
rssiVar = 10;

% data rate
t0 = 0;
frequency = 1; % 1 hz
tEnd = 60; % 60 seconds

numCycles = tEnd*frequency;
for i=1:numCycles
    t = t0 + i/frequency;
    for j=1:numel(tags)
        for k=1:numel(anchorLocations)
            distance = norm([tags{j}.X; tags{j}.Y] - [anchorLocations{k}.X; anchorLocations{k}.Y]);
            tags{j}.data(end+1).time = t;
            tags{j}.data(end).rssi = -10*n*log(distance)+A + random('norm', 0, sqrt(rssiVar));
            tags{j}.data(end).id = k;
        end
    end
end

%% Save data above
save('simdata.mat', 'anchorLocations', 'tags')