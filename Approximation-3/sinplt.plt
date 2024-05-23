set xlabel "X" 
set ylabel "Y"
set xrange[-1:1]
set yrange[-1:1]
set zrange[*:*]
splot sin(3*x)+sin(2*y) title "sin(3*x)+sin(2*y)", \
"sin.dat" using 1:2:3 title "Approximation" with points lc rgb "red" pointtype 6 pointsize 0.1