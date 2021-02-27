#ifndef RES_NAMES
#define RES_NAMES
#include<MEPGraphics/Config.h>

namespace Res {
	enum Group: MEP::U_int32 {
		Tree = (MEP::U_int32)1,
		Menu = (MEP::U_int32)2
	};
	enum Menu : MEP::U_int32 {
		Button = (MEP::U_int32)1,
		Logo = (MEP::U_int32)2,
		Intersection = (MEP::U_int32)3
	};
}

#endif
