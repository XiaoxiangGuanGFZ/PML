#ifndef FUNC_RADIATION
#define FUNC_RADIATION

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
