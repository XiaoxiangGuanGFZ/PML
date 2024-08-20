# ------------ introduction -----------
# PML evapotranspiration model, 
# coupled with SCE-UA algorithm (from library rtop)
# parameter optimization.
# author: Xiaoxiang Guan (guan@gfz-potsdam.de)
# Last updated: Aug-2024



# ---- set work space, load libraries
library(tidyverse)
library(rtop)

ws <- "D:/Xie/PML/"
source(paste0(ws, "Rscrs/metrics.R"))


# ---- file path and name 
fp_PML <<- paste0(ws, "scr/build/PML.exe ")  # the PML c program

fp_data = paste0(ws, "data/data_AU-Whr.csv")
fp_out <- paste0(ws, "data/fluxsim_AU-Whr.csv")
fp_obs <- paste0(ws,"data/fluxobs_AU-Whr.csv")

CALC_N <<- 1461

df_gp <- data.frame(
  field_name = c("FP_DATA", "FP_PARA", "FP_OUT", "CALC_N", "PML_V", "Zm"),
  field_value = c(
    fp_data, fp_para, fp_out, CALC_N, 2, 36
  )
)

fp_para <<- paste0(ws, "data/PMLpara_AU-Whr.csv")
fp_gp <<- paste0(ws, "data/gp_SCEUA.txt")
write.table(
  df_gp,
  file = fp_gp, col.names = F, row.names = F, quote = F, append = F, sep = ","
)

df_data <<- read.table(
  fp_data, header = T, sep = ","
)
df_para <<- read.table(
  fp_para, header = T, sep = ","
)
df_obs <<- read.table(
  fp_obs, header = T, sep = ","
)

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
  # df_para <- df_para %>% mutate(
  #   D0 = rep(D0, CALC_N),
  #   k_Q = rep(k_Q, CALC_N),
  #   k_A = rep(k_A, CALC_N),
  #   S_sls = rep(S_sls, CALC_N),
  #   f_ER0 = rep(f_ER0, CALC_N),
  #   beta = rep(beta, CALC_N),
  #   eta = rep(eta, CALC_N),
  #   m = rep(m, CALC_N),
  #   A_m25 = rep(A_m25, CALC_N)
  # )
  # 
  write.table(
    df_para, file = fp_para,
    col.names = T, row.names = F, quote = F, sep = ",", append = F
  )
  
  # ----- 
  # run the PML model
  
  system(paste0(fp_PML, fp_gp))
  
  # ---- datat filtering
  df_sim <- read.table(
    fp_out, header = T, sep = ","
  )
  
  df_com <- cbind(
    df_obs,
    Rs_in = df_data$Rs_in,
    Prec = df_data$Prec,
    Da = df_data$Da,
    Ta = df_data$Ta,
    ET_sim = df_sim$ET,
    Rn = df_sim$Rn
  ) %>% mutate(
    lambda = Ta * (-2.2) + 2500
  ) %>% filter(
    LE_code > 0.75,
    Prec <= 0,
    LE > 0,
    Da > 0,
    H > 5,
    Rs_in > 50,
    Rn > 0,
    abs(LE + H - Rn) < 25
  ) %>% mutate(
    EBR = (LE + H) / Rn
  ) %>% mutate(
    LE_corrected = LE / EBR
  ) %>% mutate(
    ET_obs = round(LE_corrected / lambda * 86.4, 2)
  )
  
  NSE = Func_NSE(df_com$ET_sim, df_com$ET_obs)
  
  return(1 - NSE)
}


# define the PML V2 parameter ranges --------
#                 beta,  eta,     m,  A_m25,    D0,  k_Q,  k_A, S_sls, f_ER0
para_lower =   c(0.010, 0.001,    1,      1,   0.1, 0.02, 0.05, 0.001, 0.001)
para_upper =   c(0.099, 0.099,   30,     50,   5.0, 1.00, 1.00, 0.500, 0.500)
para_initial = c(0.048, 0.024, 10.6, 13.875, 0.536, 0.80, 0.90, 0.098, 0.085)

# 
# % bl = [0.01, 0.01, 2, 5, 0.5, 0.1, 0.5, 1e-2, 1e-2, 0.65, 3.5]; %参数的下边界
# % bu = [0.1, 0.07, 100, 120, 2, 1, 0.9, 1, 0.5, 1.5, 6.5]; %参数的上边界
# % x0 = [0.06, 0.04, 10, 50, 0.7, 0.45, 0.7, 0.1, 0.1, 0.9, 4]; %参数的初始值
# Alpha, Thelta, m, Am, D0, kQ, kA, S_sls, fER0, VPDmin, VPDmax,

# optimizing 
sceua_PML = sceua(
  fit, 
  pars = para_initial, lower = para_lower, upper = para_upper, 
  maxn = 6000, pcento = 0.00001, iprint = 0
)

# optimized parameters 
para = sceua_PML$par


# validating: plots, and metrics
df_sim <- read.table(
  fp_out, header = T, sep = ","
)

df_com <- cbind(
  df_obs,
  Rs_in = df_data$Rs_in,
  Prec = df_data$Prec,
  Da = df_data$Da,
  Ta = df_data$Ta,
  ET_sim = df_sim$ET,
  Rn = df_sim$Rn
) %>% mutate(
  lambda = Ta * (-2.2) + 2500
) %>% filter(
  LE_code > 0.75,
  Prec <= 0,
  LE > 0,
  Da > 0,
  H > 5,
  Rs_in > 50,
  Rn > 0,
  abs(LE + H - Rn) < 25
) %>% mutate(
  EBR = (LE + H) / Rn
) %>% mutate(
  LE_corrected = LE / EBR
) %>% mutate(
  ET_obs = round(LE_corrected / lambda * 86.4, 2)
)


ggplot(data = df_com) +
  geom_point(aes(x = ET_obs, ET_sim)) +
  geom_abline(slope = 1, intercept = 0) + 
  labs(x = "Observed ET [mm/d]", y = "Simulated ET [mm/d]")

df_com %>% mutate(x = seq(1, dim(df_com)[1])) %>% 
  ggplot() +
  geom_point(aes(x = x, y = ET_obs)) +
  geom_line(aes(x = x, y = ET_sim), color = "blue") + 
  labs(x = "time", y = "ET [mm/d]")

NSE = Func_NSE(df_com$ET_sim, df_com$ET_obs)
RMSE = Func_RMSE(df_com$ET_sim, df_com$ET_obs)
R2 = Func_R2(df_com$ET_sim, df_com$ET_obs)
Re = Func_Re(df_com$ET_sim, df_com$ET_obs)
data.frame(
  metric = c("NSE", "R2", "RMSE", "Re(%)"),
  value = c(NSE, R2, RMSE, Re)
)


# metric  value
# 1    NSE  0.693
# 2     R2  0.709
# 3   RMSE  0.360
# 4  Re(%) 19.580

