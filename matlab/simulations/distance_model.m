% model: RSSI = -10*n*log(d) + A
function distance = distance_model(rssi)
    n = 1.4267;
    A = -59.0636;
    distance = exp((A-rssi)/(10*n));
end