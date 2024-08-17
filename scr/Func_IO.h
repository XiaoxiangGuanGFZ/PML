#ifndef FUNC_IO
#define FUNC_IO


void import_global(
    char fname[], ST_GP *p_gp);

void removeLeadingSpaces(char *str);

void print_global(
    ST_GP *p_gp
);

void Write_ET2csv(
    char FP_OUT,
    ST_VAR_ET *p_Outs,
    int CALC_N);

#endif