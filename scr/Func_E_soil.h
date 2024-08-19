#ifndef Func_E_soil
#define Func_E_soil

double Evaporation_soil_equilibrium(
    double ee,
    double lambda,
    double As
);

double Evaporation_soil(
    double Es_eq,
    double f    
);

double SoilMoisture_factor(
    double *Prec,
    double *Ei,
    double *Es_eq,
    int N,
    size_t calc_ite
);


#endif

