#define GGAMEDLL_EXPORTS

#include "GGame_exports.h"

using namespace god;

GGAMEDLL_API error_t				god::DoNothing	( IGGame* pIGame ) { CGGame* pGame = (CGGame*)pIGame; return pGame->DoNothing(); };

