set datafile separator ","
set yrange reverse
plot "input.txt" using 1:2 with lines

pause -1
