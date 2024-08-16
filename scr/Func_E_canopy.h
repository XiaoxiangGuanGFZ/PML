#ifndef Func_E_canopy
#define Func_E_canopy

void Evaporation_canopy(
    double *Ec,
    double *lambda,
    double *ee,
    double Ta,
    double Ac,
    double Ga,
    double Gc,
    double Pa,
    double Da,
    double LAI
);

double Vapor_pressure(
    double T
);

#endif
