#ifndef Func_ConductCanopy
#define Func_ConductCanopy

double ConductCanopy_V1(
    double Q_h,
    double Da,
    double LAI,
    double g_sx,
    double k_Q,
    double D50,
    double Q50
);


double ConductCanopy_V2(
    double Ta,
    double Da,
    double Pa,
    double Rs_in,
    double Ca,
    double LAI,
    double beta,
    double eta,
    double m,
    double Am_25,
    double D0,
    double k_Q
);

#endif