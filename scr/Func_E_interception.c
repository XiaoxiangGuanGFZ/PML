/*
 * SUMMARY:      Func_E_interception.c
 * USAGE:        Calculate the evaporation from canopy interception
 * AUTHOR:       Xiaoxiang Guan
 * ORG:          Section Hydrology, GFZ
 * E-MAIL:       guan@gfz-potsdam.de
 * ORIG-DATE:    Aug-2024
 * DESCRIPTION:  Calculate interception evaporation
 * DESCRIP-END.
 * FUNCTIONS:    Evaporation_Interception()
 * 
 * COMMENTS:
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "Func_E_interception.h"

/*********************************************************
 * VARIABLEs:
 * double Prec          - 日降水, mm/d     
 * double f_ER0         - 单位降雨期间截留叶片面积上平均蒸发 率和平均降水率的比值
 * double S_sls         - 单位叶面积的储水容量, mm/d
 * double P_wet         - 冠层饱和的降水量阈值, mm/d
 * double f_ER          - 降雨期间平均蒸发率和平均降水率的比值
 * double Ei            - interception evaporation, mm
 * double S_V           - 冠层储水容量, mm/d
 * double fV            - 截留叶片覆盖面积
*/



double Evaporation_Interception(
    double Prec,
    double LAI,
    double f_ER0,
    double S_sls
)
{
    double Ei;

    double fV, f_ER, S_V, P_wet;
    fV = 1 - exp(- LAI/ LAIref);
    f_ER = fV * f_ER0;
    S_V = S_sls * LAI;
    P_wet = -log(1 - f_ER / fV) * S_V / f_ER;

    if (Prec <= P_wet)
    {
        Ei = fV * Prec;
    } else {
        Ei = fV * P_wet + f_ER * (Prec - P_wet);
    }
    return Ei;
}
