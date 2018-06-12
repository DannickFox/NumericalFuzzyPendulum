// ctrlPendInv.c
#define USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N_RESULTS 4

#include "fuzzyControl.c"
#include "gnuplotGen.c"


// Pendulum paramters.
double u = 0;
double l = 1;
double g = -9.81;

double intg_rk4 (double, double, double, double (*fcn)(double, double));
double d_x1 (double, double);
double d_x2 (double, double);

void plotMembership(void);

int main(void) {
    // Simulation paramters.
    double sim_time = 15; // Seconds.
    double delta = 0.1; // dt
    int N_steps = (int) (sim_time / delta);
    double t = 0;
    double x1 = M_PI / 4; // angle
    double x2 = 0; // angular velocity

    plotMembership(); // Plots membership ranges.

    // For graph generation.
    double **pendOut = malloc(N_RESULTS * sizeof(double*));
    for (int i = 0; i < N_RESULTS; i++) {
        pendOut[i] = malloc((N_steps + 1) * sizeof(double));
    }
    double *outputRes[N_RESULTS] = {&t, &x1, &x2, &u};
    char *tagNames[N_RESULTS] = {"t", "theta", "d_theta", "d_d_x"};
    
    // Simulation Loop
    for (int i = 0; i <= N_steps; i++) {
        t = (double) (i * delta);
        for (int j = 0; j < N_RESULTS; j++) pendOut[j][i] = *outputRes[j];
        x2 += intg_rk4(t, x1, delta, d_x2);
        x1 += intg_rk4(t, x2, delta, d_x1);
        
        // Calculation for acceleration by fuzzy controller.
        u = ctrl_u(x1, x2);
    }
    // Generate output file.
    fileGen("pendulo.dat", N_RESULTS, N_steps + 1, pendOut, tagNames);
    
    // Clear memory.
    for (int i = 0; i < N_RESULTS; i++) free(pendOut[i]);
    free(pendOut);

    return 0;
}

double intg_rk4 (double x_i, double y_i, double h, double (*fcn)(double, double))
{   
    // Runge-Kutta 4th order integration.
    double k1 = h * fcn(x_i, y_i);
    double k2 = h * fcn(x_i + (h / 2), y_i + (k1 / 2));
    double k3 = h * fcn(x_i + (h / 2), y_i + (k2 / 2));
    double k4 = h * fcn(x_i + h, y_i + k3);

    return (k1 + (2 * k2) + (2 * k3) + k4) / 6;
}

double d_x1 (double t, double x2) {
    // function for change in angular position.
    return x2;
}
double d_x2 (double t, double x1) {
    // function for change in angular velocity.
    return (u * cos(x1) + g * sin(x1)) / l;
}

void plotMembership (void) {
    // generates file for membership functions.
    double limA = -2;
    double limB = 2;
    double step_sz = 0.02;
    int steps = (int) ((limB - limA) / step_sz);

    // Reserve memory for output file generation.
    double **MemberOut = malloc(6 * sizeof(double*));
    for (int i = 0; i < 6; i++) {
        MemberOut[i] = malloc((steps + 1) * sizeof(double));
    }

    double (*memFuncs[5])(double) = {PB, PS, Z, NS, NB};
    char *tagNames[6] = {"x", "PB", "PS", "Z", "NS", "NB"};

    for (int i = 0; i <= steps; i++) {
        // Create data for membership functions.
        double x = i * step_sz + limA;
        for (int j = 0; j < 6; j++) MemberOut[j][i] = j == 0 ? x : (*memFuncs[j - 1])(x);
    }
    // Generate file.
    fileGen("membership.dat", 6, steps + 1, MemberOut, tagNames);

    for (int i = 0; i < 6; i++) free(MemberOut[i]);
    free(MemberOut);
}