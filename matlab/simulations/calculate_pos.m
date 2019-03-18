% For now, this assumes a single tag: tag{1}

load('simdata1.mat')

averagingPeriod = 10; % average RSSI values for 10 seconds

t0 = 0;

numAnchors = 4;
% indexed using anchor id which is assumed to be 1 to numAnchors
averageRSSI = zeros(1, numAnchors);
numMeasurements = zeros(1, numAnchors);

i = 1;
period = 1;
while i <= numel(tags{1}.data) % while still processing data
    
    % average the measurements from each tag during the averaging period.
    while i <= numel(tags{1}.data) & tags{1}.data(i).time <= t0+averagingPeriod*period
        current = tags{1}.data(i);
        i = i+1;
        averageRSSI(current.id) = averageRSSI(current.id) + current.rssi;
        numMeasurements(current.id) = numMeasurements(current.id) + 1;
    end
    % compute distances, compute pose
    averageRSSI = averageRSSI ./ numMeasurements;
    distancePredictions = distance_model(averageRSSI);
    posPrediction = triangulate(...
        distancePredictions(1),...
        distancePredictions(2),...
        distancePredictions(3),...
        anchorLocations{1}.X,...
        anchorLocations{2}.X,...
        anchorLocations{3}.X,...
        anchorLocations{1}.Y,...
        anchorLocations{2}.Y,...
        anchorLocations{3}.Y)
    
    % reset for next period
    averageRSSI = zeros(1, numAnchors);
    numMeasurements = zeros(1, numAnchors);
    period = period+1;
end