// fuzzyControl.c

// Functions for fuzzy controller.

double sigma = 0.15;

double bound1 = 0.25;
double bound2 = 0.5;

double PB (double x) {
    // Positive-Big membership.
    double m = x - bound2;
    return x <= bound2 ? exp(-(m * m) / (2 * sigma * sigma)) : 1;
}
double PS (double x) {
    // Positive-Small membership-
    double m = x - bound1;
    return exp(-(m * m) / (2 * sigma * sigma));
}
double Z (double x) {
    // Zero membership.
    return exp(-(x * x) / (2 * sigma * sigma));
}
double NS (double x) {
    // Negative-Small membership.
    double m = x + bound1;
    return exp(-(m * m) / (2 * sigma * sigma));
}
double NB (double x) {
    // Negative-Big membership.
    double m = x + bound2;
    return x <= -bound2 ? 1 : exp(-(m * m) / (2 * sigma * sigma));
}

double integral (double (*fcn)(double), double x1, double x2, double delta) {
    // Trapezoidal integration.
    int M = (int) ((x2 - x1) / delta);
    double intg = 0;
    double a = 0;
    for (int i = 0; i < M; i++) {
        double b = fcn(i * delta + x1);
        intg += (3 * b - a) * delta / 2;
        a = b;
    }

    return intg;
}

double max3 (double n1, double n2, double n3) {
    if (n1 >= n2 && n1 >= n3) {
        return n1; 
    } else if(n2 >= n1 && n2 >= n3) {
        return n2;
    } else {
        return n3;
    }
}

double min3 (double n1, double n2, double n3) {
    // Finds minimum of 3 numbers.
    if (n1 <= n2 && n1 <= n3) {
        return n1;
    } else if (n2 <= n1 && n2 <= n3) {
        return n2;
    } else {
        return n3;
    }
}

double theta, d_theta; // placeholders for integration.
double iDenF (double x) {
    // Denominator calculation for centroid.
    /*
        Ruleset:
        theta   d_theta |   x
        PB      PB      |   NB   
        PS      PS      |   NS
        Z       Z       |   Z
        NS      NS      |   PS
        NB      NB      |   PB
    */

    double (*ruleSet[5])(double) =  {PB, PS, Z, NS, NB};
    double maxVal = 0;
    for (int i = 0; i < 5; i++) {
        double val = min3((*ruleSet[i])(theta), (*ruleSet[i])(d_theta), (*ruleSet[4 - i])(x));
        
        if (val > maxVal) { // find max value during loop
            maxVal = val;
        }
    }

    return maxVal;
}

double iNumF (double x) {
    // Numerator calculation for centroid.
    return x * iDenF(x);
}



double ctrl_u (double ang, double d_ang) {
    theta = ang;
    d_theta = d_ang;

    // Calculates centroid of rule.
    return integral(iNumF, -2, 2, 0.08) / integral(iDenF, -2, 2, 0.08);
}