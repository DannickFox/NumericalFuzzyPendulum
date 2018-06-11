// gnuplotGen.c
// Generates output for gnuplot.
#include <stdlib.h>

int fileGen (char *filename, size_t n_results, size_t size, double **sim_results, char **tagNames)
{
    int check = 0;
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "# ");
        for (int i = 0; i < n_results; i++) {
            fprintf(f, "%s\t", tagNames[i]);
        }
        fprintf(f, "\n");
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < n_results; j++) {
                fprintf(f, "%lf", sim_results[j][i]);
                if (j + 1 < n_results) {
                    fprintf(f, "\t");
                } else {
                    fprintf(f, "\n");
                }
            }
        }
        fclose(f);
        check = 1;
    } else {
        perror("Disk error.");
        check = 0;
    }
    return check;
}