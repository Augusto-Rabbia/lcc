#include <stdlib.h>
#include <setjmp.h>

#define TPILA   4096
#define NPILAS  10  // Hasta 10 corrutinas.

static void hace_stack(jmp_buf buf, void (*pf)(), unsigned prof, char *dummy)
{
    // `prof` va a estar dentro del marco de `act` porque pido la dirección.
    if (dummy - (char *) &prof >= prof) {
        // Si evalúa a verdadero, llegué a la `prof` que quería.
        if (setjmp(buf) != 0) {
            pf();
            abort();
        }
    } else {
        hace_stack(buf, pf, prof, dummy);
    }
}

void stack(jmp_buf buf, void (*pf)())
{
    static int ctas = NPILAS;
    char dummy;
    hace_stack(buf, pf, TPILA * ctas, &dummy);
    ctas--;
}
