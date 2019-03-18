function pos = triangulate(da, db, dc, xa, xb, xc, ya, yb, yc)
    va = ((db*db-dc*dc) - (xb*xb-xc*xc) - (yb*yb-yc*yc)) / 2;
    vb = ((db*db-da*da) - (xb*xb-xa*xa) - (yb*yb-ya*ya)) / 2;
    temp1 = vb*(xc-xb) - va*(xa-xb);
    temp2 = (ya-yb)*(xc-xb) - (yc-yb)*(xa-xb);
    % Estimated user position:
    y = temp1 / temp2;
    x = (va - y*(yc-yb)) / (xc-xb)
    pos = [x;y];
end