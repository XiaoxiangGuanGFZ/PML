
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def_structs.h"
#include "Func_IO.h"
#include "Func_PML.h"
#include "Func_Control.h"
#include "Func_Metric.h"

int flag_MUTE;
int flag_HEATOBS;

int main(int argc, char * argv[])
{
    ST_GP Para_df;     // define the global-parameter structure
    ST_GP *p_gp;      // give a pointer to global_parameter structure
    p_gp = &Para_df;
    import_global(*(++argv), p_gp);
    if (strncmp(p_gp->MUTE, "TRUE", 4) == 0)
    {
        flag_MUTE = 1;
    } else {
        flag_MUTE = 0;
    }
    
    if (strncmp(p_gp->HEAT_OBS, "TRUE", 4) == 0)
    {
        flag_HEATOBS = 1;
    } else {
        flag_HEATOBS = 0;
    }
    
    if (flag_MUTE == 0)
    {
        print_global(p_gp);
    }

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
    
    
    if (flag_MUTE == 0)
    {
        printf("----------- ET simulation done!\n");
    }
    
    double nse = 0.0;
    int nse_int = 0;
    if (flag_HEATOBS == 1)
    {
        double *ET_sim;
        double *ET_obs;
        int counts_valid;
        Data_Control(
            p_Vars,
            p_Paras,
            p_Outs,
            p_gp->CALC_N,
            &counts_valid,
            &ET_sim,
            &ET_obs);
        
        double RMSE, R2, Re;
        Metrics(
            ET_sim, ET_obs, counts_valid,
            &nse, &R2, &RMSE, &Re);
        if (flag_MUTE == 0)
        {
            printf("----------- Metrics (obs vs. sim): \n");
            printf("%-4s:%6.3f\n%-4s:%6.3f\n%-4s:%6.3f\n%-4s:%6.2f%%\n",
                "NSE", nse,
                "R2", R2,
                "RMSE", RMSE,
                "Re", Re * 100
            );
        }
        
    }

    Write_ET2csv(
        p_gp->FP_OUT,
        ts_date,
        p_Vars,
        p_Outs,
        p_gp->CALC_N);
    /****************** end of the program ***************/
    free(ts_date);
    free(p_Paras);
    free(p_gp);
    free(p_Paras);
    free(p_Outs);
    if (nse > 0.0)
    {
        nse_int = (int) (nse * 10000);
    }
    // printf("returned nse: %d\n", nse_int);
    return nse_int;
}

