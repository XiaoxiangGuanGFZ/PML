


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "def_structs.h"
#include "Func_Control.h"


void Data_Control(
    ST_VAR_IN * p_Vars,
    ST_PARA * p_Paras,
    ST_VAR_ET * p_Outs,
    int CALC_N,
    int *counts_valid,
    double **ET_sim,
    double **ET_obs
)
{
    /*********************
     * estimate the observed ET from LE,
     * LE is firstly corrected by energy closure
     * *******************/
    double EBR;
    double LE_corrected;
    for (size_t i = 0; i < CALC_N; i++)
    {
        EBR = ((p_Vars + i)->H + (p_Vars + i)->LE) / (p_Vars + i)->Rn;
        if (EBR > 0 || EBR < 0) // make sure EBR != 0.0
        {
            if (EBR < 0.8 || EBR > 1.2)
            {
                // only corrct the LE for those severely unclosed 
                LE_corrected = (p_Vars + i)->LE / EBR;
            } else {
                LE_corrected = (p_Vars + i)->LE;
            }
        } else {
            // here: EBR == 0.0, namely LE + H == 0.0
            LE_corrected = 0;
        }
        (p_Vars + i)->ET_obs = LE_corrected / (-2.2 * (p_Vars + i)->Ta + 2500) * 86.4;
    }
    
    /*******************
     * data quality control and filtering
     * *****************/
    *counts_valid = 0;
    for (size_t i = 0; i < CALC_N; i++)
    {
        if (
            (p_Vars + i)->LE_QC > 0.75 &&
            (p_Vars + i)->LE > 0.0 &&
            (p_Vars + i)->Da > 0.0 &&
            (p_Vars + i)->Prec <= 0.0 &&
            (p_Vars + i)->H > 5.0 &&
            (p_Vars + i)->Rs_in > 50 &&
            (p_Vars + i)->Rn > 0.0 &&
            abs((p_Vars + i)->LE + (p_Vars + i)->H - (p_Vars + i)->Rn) < 25
        )
        {
            (p_Outs + i)->FILTER = 1;
            *counts_valid += 1;
        } else {
            (p_Outs + i)->FILTER = 0;
        }
    }
    if (flag_MUTE == 0)
    {
        printf("After data quality control, there are in total %d out of %d valid ET values in the series!\n", *counts_valid, CALC_N);
    }

    *ET_obs = (double *)malloc(sizeof(double) * *counts_valid);
    *ET_sim = (double *)malloc(sizeof(double) * *counts_valid);
    int index = 0;
    for (size_t i = 0; i < CALC_N; i++)
    {
        if ((p_Outs + i)->FILTER == 1)
        {
            *(*ET_obs + index) = (p_Vars + i)->ET_obs;
            *(*ET_sim + index) = (p_Outs + i)->ET;
            index += 1;
        }
    }
}

