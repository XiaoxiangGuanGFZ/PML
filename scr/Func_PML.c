


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "def_structs.h"
#include "Func_ConductAir.h"
#include "Func_ConductCanopy.h"
#include "Func_E_canopy.h"
#include "Func_E_interception.h"
#include "Func_E_soil.h"
#include "Func_radiation.h"
#include "Func_PML.h"


void PML(
    ST_GP * p_GP,
    ST_VAR_IN * p_Vars,
    ST_PARA * p_Paras,
    ST_VAR_ET * p_Outs,
    int CALC_N
)
{
    double Ga;
    double Gc;
    double ee;
    double Ec;
    double Es;
    double Q_h;
    double lambda;
    double Ac, As;
    double f;

    double *Prec, *Es_eq, *Ei;
    Prec = (double *)malloc(sizeof(double) * CALC_N);
    Es_eq = (double *)malloc(sizeof(double) * CALC_N);
    Ei = (double *)malloc(sizeof(double) * CALC_N);
    for (size_t i = 0; i < CALC_N; i++)
    {
        *(Prec + i) = (p_Vars + i)->Prec;
    }
    
    // printf("%7s %7s %7s %7s %7s\n", "Rn", "Ac", "As", "Gc", "Ga");
    for (size_t i = 0; i < CALC_N; i++)
    {
        (p_Vars + i)->Da = (p_Vars + i)->Da * 0.1; // unit: hPa -> kPa
        if ((p_Vars + i)->Da < 0.001)
        {
            (p_Vars + i)->Da = 0.001;
        }
        
        Ga = Conductance_Air((p_Vars + i)->Hc, p_GP->Zm, (p_Vars + i)->u2); // unit of Ga: m/s
        Q_h = 0.45 * (p_Vars + i)->Rs_in;  // unit: w/m2
        if (Q_h < 0)
        {
            Q_h = 0;
        }
        
        if (p_GP->PML_V == 1)
        {
            // unit of Gc: m/s
            Gc = ConductCanopy_V1(
                Q_h,
                (p_Vars + i)->Da,
                (p_Vars + i)->LAI,
                (p_Paras + i)->g_sx,
                (p_Paras + i)->k_Q,
                (p_Paras + i)->D50,
                (p_Paras + i)->Q50);
            
        } else {
            Gc = ConductCanopy_V2(
                (p_Vars + i)->Ta,
                (p_Vars + i)->Da,
                (p_Vars + i)->Pa,
                (p_Vars + i)->Rs_in,
                (p_Vars + i)->Ca,
                (p_Vars + i)->LAI,
                (p_Paras + i)->beta,
                (p_Paras + i)->eta,
                (p_Paras + i)->m,
                (p_Paras + i)->Am_25,
                (p_Paras + i)->D0,
                (p_Paras + i)->k_Q);
        }
        Radiation(
            (p_Vars + i)->Albedo,
            (p_Vars + i)->Rs_in,
            (p_Vars + i)->Rl_in,
            (p_Vars + i)->Emiss,
            (p_Vars + i)->Ta,
            &((p_Vars + i)->Rn),
            &Ac,
            &As,
            (p_Paras + i)->k_A,
            (p_Vars + i)->LAI);
        Evaporation_canopy(
            &Ec,
            &lambda,
            &ee,
            (p_Vars + i)->Ta,
            Ac,
            Ga,
            Gc,
            (p_Vars + i)->Pa,
            (p_Vars + i)->Da,
            (p_Vars + i)->LAI);

        *(Ei + i) = Evaporation_Interception(
            (p_Vars + i)->Prec,
            (p_Vars + i)->LAI,
            (p_Paras + i)->f_ER0,
            (p_Paras + i)->S_sls);
        *(Es_eq + i) = Evaporation_soil_equilibrium(ee, lambda, As);
        f = SoilMoisture_factor(
            Prec,
            Ei,
            Es_eq,
            31,
            i);
        Es = Evaporation_soil(*(Es_eq + i), f);
        
        // if (i < 6)
        // {
        //     printf("%7.2f %7.2f %7.2f %7.5f %7.5f\n",
        //            (p_Vars + i)->Rn, Ac, As, Gc, Ga);
        // }

        // ---- out
        (p_Outs + i)->Ec = Ec;
        (p_Outs + i)->Ei = *(Ei + i);
        (p_Outs + i)->Es = Es;
        (p_Outs + i)->Es_eq = *(Es_eq + i);
    }
    free(Prec);
    free(Ei);
    free(Es_eq);
}

