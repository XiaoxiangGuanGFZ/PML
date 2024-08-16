
/*
 * SUMMARY:      Func_E_canopy.c
 * USAGE:        Calculate the evapotranspiration from canopy
 * AUTHOR:       Xiaoxiang Guan
 * ORG:          Section Hydrology, GFZ
 * E-MAIL:       guan@gfz-potsdam.de
 * ORIG-DATE:    Aug-2024
 * DESCRIPTION:  Calculate Ec
 * DESCRIP-END.
 * FUNCTIONS:    ()
 * 
 * COMMENTS:
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "Func_E_canopy.h"

/*********************************************************
 * VARIABLEs:
 * double Ta            - air temperature,  celsuis degree     
 * double Ac            - Available energy, 冠层有效辐射（ W m-2
 * double Ga            - 空气动力学导度 m s-1
 * double Gc            - 冠层导度 m s-1 
 * double Pa            - kPa 大气压
 * double Da            - 水汽压差, kPa
 * double lambda        - 蒸发潜热 KJ kg-1, related to air temperature
 * double ee            - 
 * 
*/


void Evaporation_canopy(
    double *Ec,
    double *lambda,
    double *ee,
    double Ta,
    double Ac,
    double Ga,
    double Gc,
    double Pa,
    double Da,
    double LAI
)
{
    double LE;
    *lambda = Ta * (-2.2) + 2500; // 蒸发潜热 KJ kg-1
    double gamma;
    gamma = SpecificHeat_air * Pa / (0.622 * *lambda); // 干湿表常数 kPa ℃-1
    double s;
    // 是饱和水汽压和温度关系曲线的斜率 kPa ℃-1
    s = 4098 * Vapor_pressure(Ta) / pow(273.15 + Ta, 2);
    
    *ee = s / gamma;
    if (LAI <= 0)
    {
        LE = 0.0;
        *Ec = 0.0;
    } else {
        double density_air;
        density_air = 3846 * Pa / (273.15 + Ta);
        LE = (*ee * Ac + density_air * SpecificHeat_air / gamma * Da * Ga) / (*ee + 1 + Ga / Gc);
        *Ec = LE / *lambda;
    }
    
}

double Vapor_pressure(
    double T
)
{
    double e;
    e = 0.6108 * exp(17.27 * T / (237.3 + T));
    return e;
}
