set xrange[-1:1]
set yrange[-1:1]
set key outside spacing 1.5
set size ratio 1
plot sin(x) w lines title "sin(x)", \
"sin1.dat" using 1:2 w lines title "Akima spline", \
"sin1.dat" using 1:4 w lines lt 7 title "Error"