/*
 * SUMMARY:      Func_E_soil.c
 * USAGE:        Calculate the soil evaporation
 * AUTHOR:       Xiaoxiang Guan
 * ORG:          Section Hydrology, GFZ
 * E-MAIL:       guan@gfz-potsdam.de
 * ORIG-DATE:    Aug-2024
 * DESCRIPTION:  Calculate soil evaporation
 * DESCRIP-END.
 * FUNCTIONS:    Evaporation_soil()
 * 
 * COMMENTS:
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "Func_E_soil.h"

/*********************************************************
 * VARIABLEs:
 * double Ta            - air temperature,  celsuis degree     
 * double As            - Available energy on soil, 有效辐射（ W m-2
 * double Es_eq         - equilibrium soil evaporation, mm/d
 * double Ei            - canopy interception evaporation, mm/d
 * double Prec          - precipitation, mm/d
 * double Es            - soil evaporation, mm/d
 * double lambda        - 蒸发潜热 KJ kg-1, related to air temperature
 * double ee            - 
 * 
*/



double Evaporation_soil_equilibrium(
    double ee,
    double lambda,
    double As
)
{
    double Es_eq;
    Es_eq = ee * As / (ee + 1) / lambda * 86.4;
    if (Es_eq < 0.0001)
    {
        Es_eq = 0.0001;
    }
    
    return Es_eq;
}

double Evaporation_soil(
    double Es_eq,
    double f
)
{
    double Es;
    Es = f * Es_eq;
    return Es;
}

double SoilMoisture_factor(
    double *Prec,
    double *Ei,
    double *Es_eq,
    int N,
    size_t calc_ite
)
{
    double f;
    double Pi, Pi_sum, Es_eq_sum;
    Pi_sum = 0.0;
    Es_eq_sum = 0.0;
    if (calc_ite == 0)
    {
        Pi_sum = *(Prec + calc_ite) - *(Ei + calc_ite);
        Es_eq_sum = *(Es_eq + calc_ite);
    } else if (calc_ite < N)
    {
        for (size_t i = 0; i < calc_ite; i++)
        {
            Pi_sum += *(Prec + i) - *(Ei + i);
            Es_eq_sum += *(Es_eq + i);
        }
    } else {
        for (size_t i = (calc_ite - N); i < calc_ite; i++)
        {
            Pi_sum += *(Prec + i) - *(Ei + i);
            Es_eq_sum += *(Es_eq + i);
        }
    }
    
    f = Pi_sum / Es_eq_sum;

    if (f < 0)
    {
        f = 0.0; // to secure, although the Prec should always be greater than interception evaporation, Pi >= 0
    }
    else if (f > 1)
    {
        f = 1.0;
    }
    
    return f;
}


