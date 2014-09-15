/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef symbolTab
#define symbolTab

#include <stdio.h>
#include "symbolTabDef.h"

extern Tables initSymTab();
extern Tables addToSymTab(symbolTab st, symTabEntry ste);
extern Tables loadTransTableAndSymTab();
extern void printSymTab(Tables T);

#endif
