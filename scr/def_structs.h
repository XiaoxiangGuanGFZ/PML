#ifndef STRUCTS_DEF
#define STRUCTS_DEF

#define MAXCHAR 400

typedef struct 
{
    int y;
    int m;
    int d;
} ST_DATE;


typedef struct 
{
    double Ta;          // deg C, air temperature
    double Pa;          // air pressure, kPa
    double Prec;        // precipitation, mm
    double Da;          // vapor pressure deficit, hPa
    double Rs_in;       // shortwave solar radiation, w/m2
    double Rl_in;       // longwave radiation, w/m2
    double u2;          // wind speed, m/s
    double LAI;         // leaf area index, m2/m2
    double Hc;          // canopy height, m
    double Ca;          // C02 concentration, 𝜇mol mol-1
    double Emiss;       // 地表发射率, emissitivity
    double Albedo;      // 地表反照率
    double Zm;          // suggested measurement height of wind speed and humidity, m
    double LE;          // observed latent heat, w/m2
    double LE_QC;       // quality code of LE
    double H;           // observed sensible heat, w/m2
    double Rn;          // net radiation, w/m2; estimated (simulated) from program
    double ET_obs;      // estimated from observed LE, mm
} ST_VAR_IN;

typedef struct 
{
    double Ei;          // canopy interception evaporation, mm
    double Ec;          // canopy transpiration, mm
    double Es;          // soil evaporation, mm
    double Es_eq;       // soil equilibrium evaporation, mm
    double ET;          // surface (total) evapotranspiration, mm
    int FILTER;
} ST_VAR_ET;



typedef struct 
{
    double k_Q;
    double k_A;
    double Q50;
    double D50;
    double g_sx;
    double beta;
    double eta;
    double m;
    double Am_25;
    double D0;
    double S_sls;
    double f_ER0;

} ST_PARA;


typedef struct 
{
    /* data */
    char FP_DATA[MAXCHAR];
    char FP_OUT[MAXCHAR];
    char FP_PARA[MAXCHAR];
    char MUTE[MAXCHAR];
    char HEAT_OBS[MAXCHAR];
    int CALC_N;
    int PML_V;
} ST_GP;


#endif

