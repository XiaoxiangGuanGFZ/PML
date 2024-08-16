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


double Evaporation_soil(
    double Es_eq,
    double f,
    double lambda
)
{
    double Es;
    Es = f * Es_eq / lambda;
    return Es;
}

double SoilMoisture_factor(
    double *Prec,
    double *Ei,
    double *Es_eq,
    int N,
    int calc_ite
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


