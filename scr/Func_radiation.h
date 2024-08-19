#ifndef FUNC_RADIATION
#define FUNC_RADIATION


void Radiation(
    double alpha,
    double Rs_in,
    double Rl_in,
    double Emiss,
    double Ta,
    double *Rn,
    double *Ac,
    double *As,
    double k_A,
    double LAI
);


double Net_radiation(
    double alpha,
    double Rs_in,
    double Rl_in,
    double Rl_out
);

double Radiation_long_out(
    double Emiss,
    double Ta
);

void Radiation_effect(
    double LAI,
    double *Ac,
    double *As,
    double Rn,
    double kA
);

#endif
