
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "def_structs.h"
#include "Func_IO.h"

void import_global(
    char fname[], ST_GP *p_gp)
{
    /**************
     * import the global parameters
     *
     * -- Parameters:
     *      fname: a string (1-D character array), file path and name of the global parameters
     * -- Output:
     *      return a structure containing the key fields
     * ********************/

    char row[MAXCHAR];
    FILE *fp;
    char *token;
    char *token2;
    int i;

    if ((fp = fopen(fname, "r")) == NULL)
    {
        printf("cannot open global parameter file: %s\n", fname);
        exit(1);
    }
    while (fgets(row, MAXCHAR, fp) != NULL)
    {
        // the fgets() function comes from <stdbool.h>
        // Reads characters from stream and stores them as a C string

        /***
         * removeLeadingSpaces():
         * remove all the leading white spaces in the string if exist,
         * otherwise do nothing!
         */
        removeLeadingSpaces(row);

        if (row != NULL && strlen(row) > 1)
        {
            /*non-empty row(string)*/
            if (row[0] != '#')
            {
                /* the first character of row should not be # */
                for (i = 0; i < strlen(row); i++)
                {
                    /* remove (or hide) all the characters after # */
                    if (row[i] == '#')
                    {
                        row[i] = '\0';
                        break;
                    }
                }
                // puts(row);
                /*assign the values to the parameter structure: key-value pairs*/
                token = strtok(row, ",");       // the first column: key
                token2 = strtok(NULL, ",\r\n"); // the second column: value
                // printf("token: %s\n", token);
                /********
                 * file paths and file names
                 * *****/
                if (strncmp(token, "FP_DATA", 7) == 0)
                {
                    strcpy(p_gp->FP_DATA, token2);
                }
                else if (strncmp(token, "FP_OUT", 6) == 0)
                {
                    strcpy(p_gp->FP_OUT, token2);
                }
                else if (strncmp(token, "FP_PARA", 7) == 0)
                {
                    strcpy(p_gp->FP_PARA, token2);
                }
                else if (strncmp(token, "PML_V", 5) == 0)
                {
                    p_gp->PML_V = atoi(token2);
                }
                else if (strncmp(token, "CALC_N", 6) == 0)
                {
                    p_gp->CALC_N = atoi(token2);
                }
                else if (strncmp(token, "Zm", 2) == 0)
                {
                    p_gp->Zm = atof(token2);
                }
                else
                {
                    printf("Error in opening global parameter file: unrecognized parameter field: %s\n", token);
                    exit(1);
                }
            }
        }
    }
    fclose(fp);
}

void removeLeadingSpaces(char *str)
{
    if (str != NULL)
    {
        int i, j = 0;
        int len = strlen(str);
        // Find the first non-space character
        for (i = 0; i < len && isspace(str[i]); i++)
        {
            // Do nothing, just iterate until the first non-space character is found
        }
        // Shift the string to remove leading spaces
        for (; i < len; i++)
        {
            str[j++] = str[i];
        }
        // Null-terminate the modified string
        str[j] = '\0';
    }
}

void print_global(
    ST_GP *p_gp
)
{
    printf("----------- PML setup -----------\n");
    printf("%-7s:%s\n%-7s:%s\n%-7s:%s\n%-7s:%d\n%-7s:%d\n%-7s:%.2f\n",
           "FP_DATA", p_gp->FP_DATA,
           "FP_PARA", p_gp->FP_PARA,
           "FP_OUT", p_gp->FP_OUT,
           "CALC_N", p_gp->CALC_N,
           "PML_V", p_gp->PML_V,
           "Zm", p_gp->Zm);
}

void Write_ET2csv(
    char *FP_OUT,
    ST_DATE *ts_date,
    ST_VAR_IN * p_Vars,
    ST_VAR_ET *p_Outs,
    int CALC_N)
{
    FILE *pf_out;
    if ((pf_out = fopen(FP_OUT, "w")) == NULL)
    {
        printf("Failed to create / open output file: %s\n", FP_OUT);
        exit(1);
    }
    fprintf(pf_out, "%s,%s,%s,%s,%s,%s,%s,%s\n",
            "y", "m", "d", "Ec", "Ei", "Es", "ET", "Rn");
    for (size_t i = 0; i < CALC_N; i++)
    {
        fprintf(pf_out, "%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                (ts_date + i)->y, (ts_date + i)->m, (ts_date + i)->d,
                (p_Outs + i)->Ec,
                (p_Outs + i)->Ei,
                (p_Outs + i)->Es,
                (p_Outs + i)->Ec + (p_Outs + i)->Ei + (p_Outs + i)->Es,
                (p_Vars + i)->Rn);
    }

    fclose(pf_out);
    printf("***** output preview: the first 6 rows\n");
    printf("%4s %3s %3s %5s %5s %5s %5s %7s\n",
           "y", "m", "d", "Ec", "Ei", "Es", "ET", "Rn");
    for (size_t i = 0; i < 6; i++)
    {
        printf("%4d %3d %3d %5.2f %5.2f %5.2f %5.2f %7.2f\n",
               (ts_date + i)->y, (ts_date + i)->m, (ts_date + i)->d,
               (p_Outs + i)->Ec,
               (p_Outs + i)->Ei,
               (p_Outs + i)->Es,
               (p_Outs + i)->Ec + (p_Outs + i)->Ei + (p_Outs + i)->Es,
               (p_Vars + i)->Rn);
    }
    printf("...\n");
}



void import_data(
    char FP_DATA[],
    int CALC_N,
    ST_DATE **ts_date,
    ST_VAR_IN **p_Vars)
{
    FILE *fp;
    if ((fp = fopen(FP_DATA, "r")) == NULL)
    {
        printf("Cannot open data file: %s\n", FP_DATA);
        exit(1);
    }
    *ts_date = (ST_DATE *)malloc(sizeof(ST_DATE) * CALC_N);
    *p_Vars = (ST_VAR_IN *)malloc(sizeof(ST_VAR_IN) * CALC_N);
    
    char *token;
    char row[MAXCHAR];
    char row_first[MAXCHAR];
    int i;
    i = 0; // record the number of rows in the data file
    fgets(row_first, MAXCHAR, fp); // skip the first row
    
    while (fgets(row, MAXCHAR, fp) != NULL && i < CALC_N)
    {
        (*ts_date + i)->y = atoi(strtok(row, ",")); // df_rr_daily[i].
        (*ts_date + i)->m = atoi(strtok(NULL, ","));
        (*ts_date + i)->d = atoi(strtok(NULL, ","));
        (*p_Vars + i)->Ta = atof(strtok(NULL, ","));
        (*p_Vars + i)->Rs_in = atof(strtok(NULL, ","));
        (*p_Vars + i)->Rl_in = atof(strtok(NULL, ","));
        (*p_Vars + i)->Da = atof(strtok(NULL, ","));
        (*p_Vars + i)->Pa = atof(strtok(NULL, ","));
        (*p_Vars + i)->Prec = atof(strtok(NULL, ","));
        (*p_Vars + i)->u2 = atof(strtok(NULL, ","));
        (*p_Vars + i)->Albedo = atof(strtok(NULL, ","));
        (*p_Vars + i)->Emiss = atof(strtok(NULL, ","));
        (*p_Vars + i)->LAI = atof(strtok(NULL, ","));
        
        i++;
    }
    fclose(fp);
    if (i != CALC_N)
    {
        printf("conflict numbers of lines in data file: %s\n", FP_DATA);
        exit(1);
    }
    printf("***** data preview: the first 6 rows\n");
    printf("%s", row_first);
    for (size_t i = 0; i < 6; i++)
    {
        printf("%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
               (*ts_date + i)->y, (*ts_date + i)->m, (*ts_date + i)->d,
               (*p_Vars + i)->Ta, (*p_Vars + i)->Rs_in, (*p_Vars + i)->Rl_in, (*p_Vars + i)->Da, (*p_Vars + i)->Pa,
               (*p_Vars + i)->Prec, (*p_Vars + i)->u2, (*p_Vars + i)->Albedo, (*p_Vars + i)->Emiss, (*p_Vars + i)->LAI);
    }
    printf("...\n");
}


void import_PMLpara(
    char FP_PARA[],
    int CALC_N,
    ST_PARA **p_Paras)
{
    FILE *fp;
    if ((fp = fopen(FP_PARA, "r")) == NULL)
    {
        printf("Cannot open data file: %s\n", FP_PARA);
        exit(1);
    }
    *p_Paras = (ST_PARA *)malloc(sizeof(ST_PARA) * CALC_N);
    // struct df_rr_d df_rr_daily[10000];
    char *token;
    char row[MAXCHAR];
    char row_first[MAXCHAR];
    int i;
    i = 0; // record the number of rows in the data file
    fgets(row_first, MAXCHAR, fp); // skip the first row
    
    while (fgets(row, MAXCHAR, fp) != NULL && i < CALC_N)
    {
        (*p_Paras + i)->g_sx = atof(strtok(row, ","));
        (*p_Paras + i)->D0 = atof(strtok(NULL, ","));
        (*p_Paras + i)->D50 = atof(strtok(NULL, ","));
        (*p_Paras + i)->k_Q = atof(strtok(NULL, ","));
        (*p_Paras + i)->k_A = atof(strtok(NULL, ","));
        (*p_Paras + i)->S_sls = atof(strtok(NULL, ","));
        (*p_Paras + i)->f_ER0 = atof(strtok(NULL, ","));
        (*p_Paras + i)->beta = atof(strtok(NULL, ","));
        (*p_Paras + i)->eta = atof(strtok(NULL, ","));
        (*p_Paras + i)->m = atof(strtok(NULL, ","));
        (*p_Paras + i)->Am_25 = atof(strtok(NULL, ","));
        
        i++;
    }
    fclose(fp);
    if (i != CALC_N)
    {
        printf("conflict numbers of lines in Para file: %s\n", FP_PARA);
        exit(1);
    }
    printf("***** para preview: the first 6 rows\n");
    printf("%s", row_first);
    for (size_t i = 0; i < 6; i++)
    {
        printf("%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
               (*p_Paras + i)->g_sx, (*p_Paras + i)->D0, (*p_Paras + i)->D50, (*p_Paras + i)->k_Q, (*p_Paras + i)->k_A,
               (*p_Paras + i)->S_sls, (*p_Paras + i)->f_ER0, (*p_Paras + i)->beta, (*p_Paras + i)->eta, (*p_Paras + i)->m,
               (*p_Paras + i)->Am_25);
    }
    printf("...\n");
}

