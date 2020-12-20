
/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "irqs.hpp"


/*******************************************************************************
 * GLOBAL DEFINITIONS
 ******************************************************************************/

__attribute__((
    aligned(0x200))) volatile InterruptHandler g_vtable[hardware::nb_irqs];


/*******************************************************************************
 * EXTERN FUNCTION IMPLEMENTATIONS
 ******************************************************************************/

void handleError(void)
{
    while (1) {};
}
