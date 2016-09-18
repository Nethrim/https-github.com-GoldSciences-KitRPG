// This file will contain random code required in other projects that require to be unified
// We initialize the game before initializing any asset so we make sure the game is not dependant on the graphical representation.
#define KLIB_INIT_GAME(emptyPointerToSGame)	\
	ktools::initASCIIScreen();				\
											\
	emptyPointerToSGame = new klib::SGame;	\
	klib::initGame(*emptyPointerToSGame);

#define KLIB_UPDATE_GAME(pointerToSGame)			\
	ktools::pollInput(pointerToSGame->FrameInput);	\
	klib::drawAndPresentGame(*pointerToSGame);
