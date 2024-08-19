

Func_RMSE <- function(sim, obs)
{
  round(sqrt(mean((sim - obs)^2)), 2)
}


Func_R2 <- function(sim, obs)
{
  round(cor(sim, obs)^2, 3)
}


Func_NSE <- function(sim,obs)
{
  round(1 - (sum((obs - sim)^2) / sum((obs - mean(obs))^2)), 3)
}


Func_Re <- function(sim, obs)
{
  round(sum(abs(sim - obs)) / sum(abs(obs)) * 100, 3)
}


