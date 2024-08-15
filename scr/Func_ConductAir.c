
/*
 * SUMMARY:      Func_ConductAir.c
 * USAGE:        Calculate air dynamic conductance
 * AUTHOR:       Xiaoxiang Guan
 * ORG:          Section Hydrology, GFZ
 * E-MAIL:       guan@gfz-potsdam.de
 * ORIG-DATE:    Aug-2024
 * DESCRIPTION:  Calculate air dynamic conductance
 * DESCRIP-END.
 * FUNCTIONS:    ()
 * 
 * COMMENTS:
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "Func_ConductAir.h"

/*********************************************************
 * VARIABLEs:
 * double Hc            - 冠层高度, m     
 * double u2            - 2m风速, m/s
 * double Zm            - 15m, 风速和湿度测量的高度
 * double Zom, Zoh      - 控制动量和水蒸气转移的粗糙度长度,m
 * double d             - 零平面位移,m
 * double um            - 高度 𝑍𝑚处的风速 m s-1
 * double Ga            - 空气动力学导度, m s-1
*/


double Conductance_Air(
    double Hc,
    double Zm,
    double u2
)
{
    double Ga;
    double d, Zom, Zoh;
    d = 0.64 * Hc;
    Zom = 0.13 * Hc;
    Zoh = 0.1 * Zom;
    double um;
    um = log(67.8 * Zm - 5.42) / 4.87 * u2;
    Ga = Const_Karman * Const_Karman * um / (log((Zm - d) / Zom) * log((Zm - d) / Zoh));
    return Ga;
}


