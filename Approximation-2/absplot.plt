set xrange[-0.02:0.02]
set yrange[0:0.02]
set key outside spacing 1.5
set size ratio 1
plot abs(x) w lines title "abs(x)", \
"abs.dat" using 1:2 w lines title "Akima spline", \
"abs.dat" using 1:4 w lines lt 7 title "Error"