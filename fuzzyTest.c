// fuzzyTest.c
#define USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fuzzyControl.c"

#define NUM_MEMS 5




int main (int argc, char **argv) {
    if (argc > 2) {
        theta = atof(argv[1]);
        d_theta = atof(argv[2]);
        double (*member[NUM_MEMS])(double) = {PB, PS, Z, NS, NB};
        char *tags[NUM_MEMS] = {"PB", "PS", " Z", "NS", "NB"};

        for (int i = 0; i < NUM_MEMS; i++) {
            double A = (*member[i])(theta);
            double B = (*member[i])(d_theta);

            printf("%s(%f) = %f;\t", tags[i], theta, A);
            printf("%s(%f) = %f\n", tags[i], d_theta, B);
        }

        double centroid = integral(iNumF, -2, 2, 0.1) / integral(iDenF, -2, 2, 0.1);
        printf("<x> = %f", centroid);       
    }

    return 0;
}