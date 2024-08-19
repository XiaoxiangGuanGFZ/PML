
#include <stdio.h>
#include <stdlib.h>
#include "def_structs.h"
#include "Func_IO.h"
#include "Func_PML.h"


int main(int argc, char * argv[])
{
    ST_GP Para_df;     // define the global-parameter structure
    ST_GP *p_gp;      // give a pointer to global_parameter structure
    p_gp = &Para_df;
    import_global(*(++argv), p_gp);
    print_global(p_gp);

    ST_DATE *ts_date;
    ST_VAR_IN *p_Vars;
    import_data(
        p_gp->FP_DATA,
        p_gp->CALC_N,
        &ts_date,
        &p_Vars);
    
    ST_PARA *p_Paras;
    import_PMLpara(
        p_gp->FP_PARA,
        p_gp->CALC_N,
        &p_Paras);

    

    /***************** PML simulation *******************/
    ST_VAR_ET * p_Outs;
    p_Outs = (ST_VAR_ET *)malloc(sizeof(ST_VAR_ET) * p_gp->CALC_N);

    PML(
        p_gp,
        p_Vars,
        p_Paras,
        p_Outs,
        p_gp->CALC_N);
    Write_ET2csv(
        p_gp->FP_OUT,
        ts_date,
        p_Vars,
        p_Outs,
        p_gp->CALC_N);
    
    printf("----------- simulation done!\n");
    /****************** end of the program ***************/
    free(ts_date);
    free(p_Paras);
    free(p_gp);
    free(p_Paras);
    free(p_Outs);
    return 0;
}

