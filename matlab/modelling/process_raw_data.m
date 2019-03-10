%% Load data into these variables
distance = []; % N x 1
RSSI = []; % N x 1

%% Process data to estimate parameters for the following model:
% RSSI = -10*n*log(d) + A
assert(length(distance) == length(RSSI));
assert(size(distance, 2) == size(RSSI, 2));
N = length(distance);
% form into y = Ax form
A = [log(distance) ones(N,1)];
% solve least squares problem using pseudo-inverse
x = pinv(A)*RSSI;
n = x(1)/-10
A = x(2)

%% Plot regression
hold on
scatter(log(distance), RSSI)
distance_pred = [min(distance);max(distance)];
RSSI_pred = -10*n*log(distance_pred)+A;
plot(log(distance_pred), RSSI_pred)
xlabel('Log Distance')
ylabel('RSSI')
title('RSSI v.s. log Distance, Model v.s. Actual')
%% Compute confidence intervals at each x
% That is, we are 95% confident that true RSSI value is within this range
y = [];
x = unique(distance);
ylims = []
for i=1:numel(x)
    d = x(i)
    y = RSSI(distance == d); % gets all the RSSI values for a specific distance.
    N = size(y,1)
    yMean = mean(y)
    ySEM = std(y)/sqrt(N)
    CI95 = tinv([0.025 0.975], N-1)
    yCI95 = bsxfun(@times, ySEM, CI95(:))
    ylims = [ylims yMean+yCI95]; % append
end
plot(log(x), ylims)
