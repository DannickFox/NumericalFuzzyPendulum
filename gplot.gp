# Gnuplot graph
#
set multiplot layout 3, 1
set title "{/Symbol q}"
plot "pendulo.dat" using 1:2 with lines
set title "{/Symbol w}"
plot "pendulo.dat" using 1:3 with lines
set title "u"
plot "pendulo.dat" using 1:4 with lines