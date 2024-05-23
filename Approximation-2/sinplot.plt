set xrange[-0.3:0.3]
set yrange[-0.3:0.3]
set grid
set key outside spacing 1.5
set size ratio 1
plot sin(x) w lines title "sin(x)", \
"sin1.dat" using 1:2 w lines title "Quadratic spline", \
"sin1.dat" using 1:4 w lines lt 7 title "Error"