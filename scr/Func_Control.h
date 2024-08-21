#ifndef FUNC_CONTROL
#define FUNC_CONTROL

extern int flag_MUTE;
extern int flag_HEATOBS;

void Data_Control(
    ST_VAR_IN * p_Vars,
    ST_PARA * p_Paras,
    ST_VAR_ET * p_Outs,
    int CALC_N,
    int *counts_valid,
    double **ET_sim,
    double **ET_obs
);

#endif

