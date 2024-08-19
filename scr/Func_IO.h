#ifndef FUNC_IO
#define FUNC_IO


void import_global(
    char fname[], ST_GP *p_gp);

void removeLeadingSpaces(char *str);

void print_global(
    ST_GP *p_gp
);

void Write_ET2csv(
    char *FP_OUT,
    ST_DATE *ts_date,
    ST_VAR_IN * p_Vars,
    ST_VAR_ET *p_Outs,
    int CALC_N);


void import_data(
    char FP_DATA[],
    int CALC_N,
    ST_DATE **ts_date,
    ST_VAR_IN **p_Vars);

void import_PMLpara(
    char FP_PARA[],
    int CALC_N,
    ST_PARA **p_Paras);


#endif
