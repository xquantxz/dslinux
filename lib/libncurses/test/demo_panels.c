/*
 * $Id: demo_panels.c 317 2005-10-02 21:32:49Z pepsiman $
 *
 * Demonstrate a variety of functions from the panel library.
 * Thomas Dickey - 2003/4/26
 */
/*
panel_above			-
panel_below			-
panel_hidden			-
replace_panel			-
*/

#include <test.priv.h>

#if USE_LIBPANEL

#include <panel.h>

int
main(int argc GCC_UNUSED, char *argv[]GCC_UNUSED)
{
    printf("Not implemented - demo for panel library\n");
    return EXIT_SUCCESS;
}
#else
int
main(void)
{
    printf("This program requires the curses panel library\n");
    ExitProgram(EXIT_FAILURE);
}
#endif
