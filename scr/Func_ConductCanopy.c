

/*
 * SUMMARY:      Func_ConductCanopy.c
 * USAGE:        Calculate the canopy conductance
 * AUTHOR:       Xiaoxiang Guan
 * ORG:          Section Hydrology, GFZ
 * E-MAIL:       guan@gfz-potsdam.de
 * ORIG-DATE:    Aug-2024
 * DESCRIPTION:  Calculate Gc
 * DESCRIP-END.
 * FUNCTIONS:    ()
 * 
 * COMMENTS:
 * PML-V1 and V2
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "Func_ConductCanopy.h"

/*********************************************************
 * VARIABLEs:
 * double Ta            - air temperature,  celsuis degree     
 * double Da            - 水汽压差, kPa
 * double Q_h           - 冠层上方的可见光辐射能量, the flux density of visible radiation at the top of the canopy (approximately half of incoming solar radiation)
 * double g_sx          - 为最大气孔导度
 * double k_A           - 地表可供能量消光系数 
 * double k_Q           - 光合有效辐射的消光系数, extinction coefficient for shortwave radiation
 * double Q50           - 30 W/m2, 𝑔𝑠=0.5𝑔𝑠𝑥时的可见光辐射, the visible radiation flux when stomatal conductance is half its maximum value.
 * double D50           - 0.7 kpa 𝑔𝑠=0.5𝑔𝑠𝑥时的水汽压差, the humidity deficit at which stomatal conductance is half its maximum value.
 * double Rs_in         - 向下短波辐射 (W m-2)
 * double Ca            - 二氧化碳浓度, 𝜇mol mol-1
 * double beta          - 同化效率与光响应曲线的初始斜率（ 𝜇mol CO2 [mol PAR]-1 )
 * double eta           - 是同化效率与CO2响应曲线的初始斜率（ 𝜇mol m-2 s-1 [𝜇mol m-2 s-1]-1 )
 * double m             - 气孔导度系数
 * double Am_25         - 25degree下光照和二氧化碳浓度达到饱和时的最高光合速率, 𝜇mol m-2 s-1
 * double D0            - water vapor pressure deficit of the air 
*/



double ConductCanopy_V1(
    double Q_h,
    double Da,
    double LAI,
    double g_sx,
    double k_A,
    double k_Q,
    double D50,
    double Q50
)
{
    double Gc;
    Gc = g_sx / k_Q * log(
        (Q_h + Q50) / (Q_h * exp(- k_A * LAI) + Q50)
    ) * (1 / (1 + Da / D50));
    if (Gc < 1e-6)
    {
        Gc = 1e-6;
    }
    return Gc;
}


double ConductCanopy_V2(
    double Ta,
    double Da,
    double Pa,
    double Rs_in,
    double Ca,
    double LAI,
    double beta,
    double eta,
    double m,
    double Am_25,
    double D0,
    double k_Q
)
{
    double Gc;
    double P1, P2, P3, P4;
    double Am, I0;
    double v_temp;
    v_temp = exp(0.031 *(Ta - 25)) / (1 + exp(0.115 * (Ta - 41)));
    if (v_temp > 1)
    {
        v_temp = 1.0;
    }
    // Am是光照和二氧化碳浓度达到饱和时的最高光合速率（ 𝜇mol m-2 s-1
    Am = Am_25 * v_temp;

    v_temp = Rs_in * 0.45 * 4.57;
    if (v_temp < 0)
    {
        v_temp = 0;
    }
    I0 = v_temp;  // I0是冠层顶部有效光合辐射 PAR的通量密度 𝜇mol m-2 s-1）

    P1 = Am * beta * I0 * eta;
    P2 = Am * beta * I0;
    P3 = Am * eta * Ca;
    P4 = beta * I0 * eta * Ca;
    Gc = m * P1 / k_Q / (P2 + P4) * (k_Q * LAI + 
        log((P1 + P3 + P4) / (P2 + P3 * exp(k_Q * LAI) + P4)) * 1 / (1 + Da/D0)
    );

    // 1.6 = conductance of water / conductance of CO2 (mol m-2 s-1)
    Gc = 1.6 * Gc * 1e-2 / (0.446 * (273 / (273 + Ta)) * (Pa/101.3));
    if (Gc < 1e-6)
    {
        Gc = 1e-6;
    }
    
    return Gc;
}

