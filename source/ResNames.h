#ifndef RES_NAMES
#define RES_NAMES
#include<MEPGraphics/Config.h>

namespace Res {
	enum Group: MEP::U_int32 {
		Tree = 1,
		Menu = 2
	};
	enum Menu : MEP::U_int32 {
		Button = 1,
		Logo = 2,
		Intersection = 3
	};
	enum Windows : MEP::U_int32 {
		AVL = 1,
		BST = 2
	};
}

#endif
