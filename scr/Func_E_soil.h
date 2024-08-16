#ifndef Func_E_soil
#define Func_E_soil

double Evaporation_soil(
    double Es_eq,
    double f,
    double lambda
);

double SoilMoisture_factor(
    double *Prec,
    double *Ei,
    double *Es_eq,
    int N,
    int calc_ite
);


#endif

