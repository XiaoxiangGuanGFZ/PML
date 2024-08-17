
#include <stdio.h>
#include <stdlib.h>
#include "def_structs.h"
#include "Func_IO.h"
#include "Func_PML.h"


int main(int argc, char * argv[])
{
    ST_GP Para_df;     // define the global-parameter structure
    ST_GP *p_gp;      // give a pointer to global_parameter structure
    p_gp = &Para_df;
    import_global(*(++argv), p_gp);
    print_global(p_gp);

    
    return 0;
}

