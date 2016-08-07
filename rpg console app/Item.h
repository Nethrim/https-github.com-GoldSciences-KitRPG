#pragma once

#include <string>
   

struct SItem 
{
	std::string	Name;
	int			Price;
};



#define MAX_ITEM_DESCRIPTIONS 3

static const SItem itemDescriptions[MAX_ITEM_DESCRIPTIONS] = {
	{"Small HP Potion",		50},
	{"Medium HP Potion",	100},
	{"Large HP Potion",		150},
};