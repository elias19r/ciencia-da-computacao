set border linewidth 1.0
set style line 1 linecolor rgb '#0000FF' linetype 1 linewidth 1.5
set xrange [0:2]
set yrange [0:10]
set key center top

y(x) = (exp(x) * (1 + 2*x) + 3*exp(-x))/4

plot y(x) title "Solucao Analitica" with lines linestyle 1, \
     '../out_k1.dat' title "Euler Mod. k = 1" pt 5 ps 1.5, \
     '../out_k2.dat' title "Euler Mod. k = 2" pt 3 ps 1.5, \
     '../out_k4.dat' title "Euler Mod. k = 4" pt 7 ps 0.4


