#ifndef MODULES_H
#define MODULES_H
#include "TreeController.h"

class Modules {
protected:
	struct IDs {
		const int begin_ID;
		const int end_ID;
		IDs(const int bID, const int endID) : begin_ID(bID), end_ID(endID) {}
	};
	IDs AVL_ID = { 9000, 10000 };

	bool disableDraw;
	//structure generated
	bool isGen;
	//avl tree graphical menu
	TreeController* avl_module_v2;
public:	
	Modules(): avl_module_v2(nullptr), disableDraw(false), isGen(false) {}

	bool isGenerated() const {
		return isGen;
	}
	void genDS() {
		isGen = true;
	}
};

#endif
