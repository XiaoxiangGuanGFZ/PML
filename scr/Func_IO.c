
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
                    printf("Error in opening global parameter file: unrecognized parameter field: %s!", token);
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
    printf("%-7s:%s\n%-7s:%s\n%-7s:%d\n%-7s:%d\n%-7s:%.2f\n",
           "FP_DATA", p_gp->FP_DATA,
           "FP_OUT", p_gp->FP_OUT,
           "CALC_N", p_gp->CALC_N,
           "PML_V", p_gp->PML_V,
           "Zm", p_gp->Zm);
}



void Write_ET2csv(
    char FP_OUT,
    ST_VAR_ET *p_Outs,
    int CALC_N)
{
    FILE *pf_out;
    if ((pf_out = fopen(FP_OUT, "w")) == NULL)
    {
        printf("Failed to create / open output file: %s\n", FP_OUT);
        exit(1);
    }
    for (size_t i = 0; i < CALC_N; i++)
    {
        fprintf(pf_out, "%.2f,%.2f,%.2f,%.2f\n",
               (p_Outs + i)->Ec,
               (p_Outs + i)->Ei,
               (p_Outs + i)->Es,
               (p_Outs + i)->Ec + (p_Outs + i)->Ei + (p_Outs + i)->Es);
    }
}

