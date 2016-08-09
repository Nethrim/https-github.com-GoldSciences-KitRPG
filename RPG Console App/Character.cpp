#include "Character.h"

void rest(SCharacterPoints& points)
{
	printf("\nYou decide to get some rest.\n");
	points.HP = points.MaxHP;
	printf("Your HP is: %u.\n", points.HP);
}