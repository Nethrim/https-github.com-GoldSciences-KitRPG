#include "GGame.h"

#ifndef __GGAME_EXPORTS_H
#define __GGAME_EXPORTS_H

#if defined(ANDROID) || defined(__linux__)
#	define GGAMEDLL_API
#elif defined(WIN32) || defined(_WIN32)
#	ifdef GGAMEDLL_EXPORTS
#		define GGAMEDLL_API __declspec(dllexport)
#	else
#		define GGAMEDLL_API __declspec(dllimport)
#	endif
#else
#	define GGAMEDLL_API
#endif

namespace god
{
	GGAMEDLL_API error_t				DoNothing	( IGGame* );
	// Main loop function calls
};

#endif // __GGAME_EXPORTS_H
