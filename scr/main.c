
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
    // for (size_t i = 0; i < 10; i++)
    // {
    //     printf("%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
    //            (ts_date + i)->y, (ts_date + i)->m, (ts_date + i)->d,
    //            (p_Vars + i)->Ta, (p_Vars + i)->Rs_in, (p_Vars + i)->Rl_in, (p_Vars + i)->Da, (p_Vars + i)->Pa,
    //            (p_Vars + i)->Prec, (p_Vars + i)->u2, (p_Vars + i)->Albedo, (p_Vars + i)->Emiss, (p_Vars + i)->LAI);
    // }

    ST_PARA *p_Paras;
    import_PMLpara(
        p_gp->FP_PARA,
        p_gp->CALC_N,
        &p_Paras);

    // for (size_t i = 0; i < 5; i++)
    // {
    //     printf("%d,%d,%d,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
    //            (ts_date + i)->y, (ts_date + i)->m, (ts_date + i)->d,
    //            (p_Paras + i)->g_sx, (p_Paras + i)->D0, (p_Paras + i)->D50, (p_Paras + i)->k_Q, (p_Paras + i)->k_A,
    //            (p_Paras + i)->S_sls, (p_Paras + i)->f_ER0, (p_Paras + i)->beta, (p_Paras + i)->eta, (p_Paras + i)->m,
    //            (p_Paras + i)->Am_25);
    // }

    /***************** PML simulation *******************/
    


    /****************** end of the program ***************/
    free(ts_date);
    free(p_Paras);
    free(p_gp);
    free(p_Paras);
    return 0;
}

