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
    double Ta;
    double Pa;
    double Prec;
    double Da;
    double Rs_in;
    double Rl_in;
    double u2;
    double LAI;
    double Hc;
    double Ca;
    double Emiss;
    double Albedo;
} ST_VAR_IN;

typedef struct 
{
    double Ei;
    double Ec;
    double Es;
    double Es_eq;
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
    int CALC_N;
    int PML_V;
    double Zm;

} ST_GP;


#endif

