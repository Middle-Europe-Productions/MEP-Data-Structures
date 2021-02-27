#ifndef MODULES_H
#define MODULES_H
#include "TreeController.h"

class Modules {
protected:
	//Master color
	MEP::AnimationColor m_color;
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
	Modules(MEP::Window::Template::Application& base): 
	m_color({sf::Color::Cyan.r, sf::Color::Cyan.g, sf::Color::Cyan.b, 0 }, sf::Color::Cyan, sf::milliseconds(300), 120, 0, 10, [](double x)->double { return std::pow(x, 4); }),
	disableDraw(false), 
	isGen(false),
	avl_module_v2(nullptr)
	{
		
	}

	bool isGenerated() const {
		return isGen;
	}
	void genDS() {
		isGen = true;
	}
};

#endif
