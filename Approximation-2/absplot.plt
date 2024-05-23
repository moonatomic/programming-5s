set xrange[-0.2:0.2]
set yrange[-0.2:0.2]
set grid
set key outside spacing 1.5
set size ratio 1
plot abs(x) w lines title "abs(x)", \
"abs1.dat" using 1:2 w lines title "Quadratic spline", \
"abs1.dat" using 1:4 w lines lt 7 title "Error"