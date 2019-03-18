% accepts distances from each anchor and the anchor locations
% and returns a loss function to minimize
function lossfct = getLossFct(distances, anchors)
    anchorX = cellfun(@(anchor) anchor.X , anchors)';
    anchorY = cellfun(@(anchor) anchor.Y , anchors)';
    lossfct = @(pos) (distances.^2 - (anchorX-pos(1)).^2 - (anchorY-pos(2)).^2);
end