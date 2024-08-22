# ------------ introduction -----------
# PML evapotranspiration model, 
# coupled with SCE-UA algorithm (from library rtop)
# parameter optimization.
# author: Xiaoxiang Guan (guan@gfz-potsdam.de)
# Last updated: Aug-2024



# ---- set work space, load libraries
library(tidyverse)
library(rtop)
# source(paste0(ws, "Rscrs/metrics.R"))



# ---- file path and name 
ws <- "D:/Xie/PML/"
fp_PML <<- paste0(ws, "scr/build/PML.exe ")  # the PML c program
fp_gp <<- paste0(ws, "data/gp_SCEUA.txt")   # the PML.exe program configure file
fp_data <- paste0(ws, "data/data_AU-Whr.csv")  # the weather variable data file
fp_out <- paste0(ws, "data/fluxsim_AU-Whr.csv") # the output file 
fp_para <<- paste0(ws, "data/PMLpara_AU-Whr.csv") # the PML parameter file
CALC_N <<- 1461     # the number of lines (observations) in data file, excluding the header line

# ------- 
df_gp <- data.frame(
  field_name = c("FP_DATA", "FP_PARA", "FP_OUT", "CALC_N", "PML_V", "MUTE", "HEAT_OBS"),
  field_value = c(
    fp_data, fp_para, fp_out, CALC_N, 2, "TRUE", "TRUE"
  )
)

write.table(
  df_gp,
  file = fp_gp, col.names = F, row.names = F, quote = F, append = F, sep = ","
)

# df_data <<- read.table(
#   fp_data, header = T, sep = ","
# )

df_para <<- data.frame(
  g_sx = rep(0.0038, CALC_N),
  Q50 = rep(30, CALC_N),
  D0 = rep(0.536, CALC_N),
  D50 = rep(0.7, CALC_N),
  k_Q = rep(0.832, CALC_N),
  k_A = rep(0.899, CALC_N),
  S_sls = rep(0.098, CALC_N),
  f_ER0 = rep(0.085, CALC_N),
  beta = rep(0.048, CALC_N),
  eta = rep(0.024, CALC_N),
  m = rep(10.602, CALC_N),
  A_m25 = rep(13.875, CALC_N)
)

# fp_obs <- paste0(ws,"data/fluxobs_AU-Whr.csv")  # 
# df_obs <<- read.table(
#   fp_obs, header = T, sep = ","
# )

fit = function(x) {
  
  # ---------
  # update parameter file
  # beta, eta, m, A_m25, D0, k_Q, k_A, S_sls, f_ER0
  beta = x[1]
  eta = x[2]
  m = x[3]
  A_m25 = x[4]
  D0 = x[5]
  k_Q = x[6]
  k_A = x[7]
  S_sls = x[8]
  f_ER0 = x[9]
  
  df_para <- df_para %>% mutate(
    beta = rep(x[1], CALC_N),
    eta = rep(x[2], CALC_N),
    m = rep(x[3], CALC_N),
    A_m25 = rep(x[4], CALC_N),
    D0 = rep(x[5], CALC_N),
    k_Q = rep(x[6], CALC_N),
    k_A = rep(x[7], CALC_N),
    S_sls = rep(x[8], CALC_N),
    f_ER0 = rep(x[9], CALC_N)
    
  )

  write.table(
    df_para, file = fp_para,
    col.names = T, row.names = F, quote = F, sep = ",", append = F
  )
  
  # ----- 
  # run the PML model
  
  NSE = system(paste0(fp_PML, fp_gp)) / 10000
  
  return(1 - NSE)
}


# define the PML V2 parameter ranges --------
#                 beta,  eta,     m,  A_m25,    D0,  k_Q,  k_A, S_sls, f_ER0
para_lower =   c(0.010, 0.010,    2,      5,   0.2, 0.02, 0.05, 0.001, 0.001)
para_upper =   c(0.100, 0.070,  100,    120,   5.0, 1.00, 1.00, 1.000, 0.500)
para_initial = c(0.048, 0.024, 10.6, 13.875, 0.536, 0.80, 0.90, 0.098, 0.085)

# 
# % bl = [0.01, 0.01,   2,   5, 0.5,  0.1, 0.5,  1e-2, 1e-2,   0.65,   3.5]; %参数的下边界
# % bu = [0.10, 0.07, 100, 120,   2,    1, 0.9,     1,  0.5,    1.5,   6.5]; %参数的上边界
# % x0 = [0.06, 0.04,  10,  50, 0.7, 0.45, 0.7,   0.1,  0.1,    0.9,     4]; %参数的初始值
#       Alpha, Thelta, m,   Am,  D0,   kQ,  kA, S_sls, fER0, VPDmin, VPDmax,

# optimizing 
sceua_PML = sceua(
  fit, 
  pars = para_initial, lower = para_lower, upper = para_upper, 
  maxn = 6000, pcento = 0.0001, iprint = 1
)

# check the optimization result
sceua_PML

# # optimized parameters 
# para = sceua_PML$par
# 
# 
# # validating: plots, and metrics
# df_sim <- read.table(
#   fp_out, header = T, sep = ","
# )
# 
# df_com <- cbind(
#   df_obs,
#   Rs_in = df_data$Rs_in,
#   Prec = df_data$Prec,
#   Da = df_data$Da,
#   Ta = df_data$Ta,
#   ET_sim = df_sim$ET,
#   Rn = df_sim$Rn
# ) %>% mutate(
#   lambda = Ta * (-2.2) + 2500
# ) %>% filter(
#   LE_code > 0.75,
#   Prec <= 0,
#   LE > 0,
#   Da > 0,
#   H > 5,
#   Rs_in > 50,
#   Rn > 0,
#   abs(LE + H - Rn) < 25
# ) %>% mutate(
#   EBR = (LE + H) / Rn
# ) %>% mutate(
#   LE_corrected = LE / EBR
# ) %>% mutate(
#   ET_obs = round(LE_corrected / lambda * 86.4, 2)
# )
# 
# 
# ggplot(data = df_com) +
#   geom_point(aes(x = ET_obs, ET_sim)) +
#   geom_abline(slope = 1, intercept = 0) + 
#   labs(x = "Observed ET [mm/d]", y = "Simulated ET [mm/d]")
# 
# df_com %>% mutate(x = seq(1, dim(df_com)[1])) %>% 
#   ggplot() +
#   geom_point(aes(x = x, y = ET_obs)) +
#   geom_line(aes(x = x, y = ET_sim), color = "blue") + 
#   labs(x = "time", y = "ET [mm/d]")
# 
# NSE = Func_NSE(df_com$ET_sim, df_com$ET_obs)
# RMSE = Func_RMSE(df_com$ET_sim, df_com$ET_obs)
# R2 = Func_R2(df_com$ET_sim, df_com$ET_obs)
# Re = Func_Re(df_com$ET_sim, df_com$ET_obs)
# data.frame(
#   metric = c("NSE", "R2", "RMSE", "Re(%)"),
#   value = c(NSE, R2, RMSE, Re)
# )


##   metric  value
## 1    NSE  0.705
## 2     R2  0.706
## 3   RMSE  0.360
## 4  Re(%) 20.482

