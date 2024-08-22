# Penman-Monteith-Leuning


## How to use
### compile

the program is coded in C language. Take the following steps to build and implement: navigate to the `scr` folder, create directory `build` and navigate to it. call `cmake` and `make` to build the software. 

For example, to compile the program in the Windows OS with MinGW: 

```console
$ cd ./PML/scr/
$ mkdir build
$ cd build
$ cmake -G "MinGW Makefiles" ..
$ mingw32-make
```

then an executable is generated named `PML.exe` (in Windows OS).


### data preparation

The required variables to calculate the actual evapotranspiration with PML method include:

| VAR | variable |unit|
|-------|---------------|-------|
| Ta | air temperature | Cel degree|
| Rs_in| incoming shortwave radiation | w/m2 |
| Rl_in | incoming longwave radiation | w/m2 |
| Da | vapor pressure deficit | hPa|
| Pa | air pressure | kPa| 
| Prec | precipitation | mm |
| WS | wind speed | m/s |
| Ca | CO2 concentration | $\mu$ mol/mol |
| Albedo | albedo | - |
| emiss | emissitivity | - |
| LAI | leaf area index | m2/m2 |
| Hc | canopy height | m |
| Zm | suggested measurement height of wind speed and humidity | m |
| LE | latent heat | w/m2 |
| LE_code | quality code of LE | - |
| H | sensible heat | w/m2 |


an example data file is shown as following:

```
y,m,d,Ta,Rs_in,Rl_in,Da,Pa,Prec,WS,Ca,Albedo,emiss,LAI,Hc,Zm,LE,LE_code,H
2011,1,1,25.177,322.34,373.435,19.477,98.933,0.905,1.948,390.93,0.135,0.984,1.5,28,36,57.2376,0,127.758
2011,1,2,18.742,369.891,321.298,10.673,99.331,0,3.241,390.9435484,0.13475,0.984,1.5,28,36,55.6186,0,117.227
2011,1,3,16.626,372.732,302.404,9.774,99.409,0,3.667,390.9570968,0.1345,0.984,1.5,28,36,64.1627,0,136.383
2011,1,4,18.714,381.975,314.716,11.636,98.995,0,1.742,390.9706452,0.13425,0.984,1.5,28,36,57.2376,0,127.758
2011,1,5,18.471,342.724,327.956,9.841,98.774,0,3.264,390.9841935,0.134,0.984,1.5,28,36,55.6186,0,117.227
2011,1,6,21.912,372.203,341.696,14.362,98.771,0,1.683,390.9977419,0.13375,0.984,1.5,28,36,64.1627,0,136.383
...
```

Aside from the input variables for PML calculation, the PML parameters are also required to correctly simulate the actual evapotranspiration. See [Leuning et al., (2008)](https://doi.org/10.1029/2007WR006562) and [Gan et al., (2018)](https://doi.org/10.1002/eco.1974) for details. 

An example of parameter file is like below:

```
g_sx,Q50,D0,D50,k_Q,k_A,S_sls,f_ER0,beta,eta,m,A_m25
0.0038,30,0.536,0.7,0.832,0.899,0.098,0.085,0.048,0.024,10.602,13.875
0.0038,30,0.536,0.7,0.832,0.899,0.098,0.085,0.048,0.024,10.602,13.875
0.0038,30,0.536,0.7,0.832,0.899,0.098,0.085,0.048,0.024,10.602,13.875
0.0038,30,0.536,0.7,0.832,0.899,0.098,0.085,0.048,0.024,10.602,13.875
0.0038,30,0.536,0.7,0.832,0.899,0.098,0.085,0.048,0.024,10.602,13.875
...
```

Attention: the number of lines in the parameter file should be the same as that in data file, although the values of these variables can be temporally constant. 


### configure file

A configure file `gp.txt` is required to provide arguments controlling the behavior of this program, an example is:

```
# lines starting with hash symbol are comments
# field-value pairs, separated by comma

# FP_DATA, the file path and name of data file
FP_DATA,../../data/data_AU-Whr.csv

# the file path and fname of PML parameter file
FP_PARA,../../data/PMLpara_AU-Whr.csv

# the file base of name to store the simulated outputs
FP_OUT,../../data/fluxsim_AU-Whr.csv

# number of calculation steps
CALC_N,1461

# the version of PML model: 1 or 2
PML_V,2

# a flag, if MUTE == TRUE, then run the program silently; if MUTE == FALSE, print some info during running
MUTE,FALSE

# a flag, if HEAT_OBS == TRUE, means the observed LE and H are available in data file, the program 
# will conduct the data filtering and energy closure, the NSE is returned. 
# when HEAT_OBS == TRUE, and MUTE == FALSE, the NSE, R2, RMSE, and Re are printed in the end.

HEAT_OBS,TRUE
```

Then run the program by executing this in the command-line terminal (or shell):

``` console
./PML.exe path/to/gp.txt
```


### understand the output

In this program, the evaporation from canopy `Ec`, interception `Ei` and soil `Es` are simulated and net radiation `Rn` over canopy and equilibirum soil evaporation `Es_eq` are also returned in the output file. `ET` stands for the actual surface evapotranspiration. 

The output file should look like:

```
y,m,d,Ec,Ei,Es,Es_eq,ET,Rn,FILTER
2011,1,1,2.04,0.16,0.74,1.43,2.94,209.96,0
2011,1,2,1.70,0.00,0.37,1.45,2.07,235.99,0
2011,1,3,1.60,0.00,0.24,1.36,1.84,231.28,0
2011,1,4,1.84,0.00,0.19,1.47,2.04,240.21,0
2011,1,5,1.63,0.00,0.14,1.35,1.77,220.91,0
...
```

The last column in the output file indicates whether the value passes the data quality and energy closure control (yes: `1`; no: `0`). If the observed LE or H is not available, the `-1` is returned. 

When the observaed flux (LE and H) are available, namely `HEAT_OBS == TRUE`, the program also returns the estimated Nash–Sutcliffe efficiency coefficient to the calling evenronment. As the C program can also return integer value, so literally the returned value is `10000 * NSE`.

## References

Leuning, R., Zhang, Y.Q., Rajaud, A., Cleugh, H. and Tu, K.  2008.  A simple surface conductance model to estimate regional evaporation using MODIS leaf area index and the Penman-Monteith equation. Water Resources Research, 44(10). doi: https://doi.org/10.1029/2007WR006562.


Gan, R., Zhang, Y., Shi, H., Yang, Y., Eamus, D., Cheng, L., Chiew, F.H.S. and Yu, Q.  2018.  Use of satellite leaf area index estimating evapotranspiration and gross assimilation for Australian ecosystems. Ecohydrology, 11(5), e1974. doi: https://doi.org/10.1002/eco.1974.


## Contact

[Xiaoxiang Guan](https://www.gfz-potsdam.de/staff/guan.xiaoxiang/sec44)

