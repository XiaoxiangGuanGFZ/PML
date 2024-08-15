
/*
 * SUMMARY:      Func_radiation.c
 * USAGE:        Calculate radiation
 * AUTHOR:       Xiaoxiang Guan
 * ORG:          Section Hydrology, GFZ
 * E-MAIL:       guan@gfz-potsdam.de
 * ORIG-DATE:    Aug-2024
 * DESCRIPTION:  Calculate radiation, net radiation
 * DESCRIP-END.
 * FUNCTIONS:    ()
 * 
 * COMMENTS:
 *  模型运行的时间尺度为日，土壤热通量 G在这一时间尺度上约 为 0
 */



/*********************************************************
 * VARIABLEs:
 * double alpha         - 反射率                
 * double Rn            - 地面净辐射 (W m-2)
 * double Rs_in         - 向下短波辐射 (W m-2)
 * double Rl_in         - 向下长波辐射 (W m-2)
 * double Rl_out        - 向上长波辐射 (W m-2)
 * double Emiss         - 发射率
 * double Ta            - 气温（℃）
 * double Ac            - 冠层有效辐射
 * double As            - 土壤有效辐射
 * double tau           - 土壤有效辐射吸收比
 * double LAI           - 叶面积指数 m2 m-2
 * double kA            - 地表可供能量消光系数
*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "Func_radiation.h"

double Net_radiation(
    double alpha,
    double Rs_in,
    double Rl_in,
    double Rl_out
) 
{
    double Rn;
    Rn = (1 - alpha) * Rs_in + (Rl_in - Rl_out);
    return Rn;
}

double Radiation_long_out(
    double Emiss,
    double Ta
)
{
    double Rl_out;
    Rl_out = Emiss * Const_SB * pow(273.15 + Ta, 4) / 0.0864;
    return Rl_out;
}

void Radiation_effect(
    double LAI,
    double *Ac,
    double *As,
    double Rn,
    double kA
)
{
    double tau;
    tau = exp(- kA * LAI);
    *As =Rn * tau;
    *Ac = Rn - *As;
}

