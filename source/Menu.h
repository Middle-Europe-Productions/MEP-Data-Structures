
#ifndef MENU_H
#define MENU_H

#include"MenuAnimations.h"
#include"ResNames.h"
#include"Modules.h"
#include"Data.h"
#include<MEPGraphics/AnimationManager.h>

class Menu : public MEP::BaseWindow, public Modules, public MenuAnimations {
	//Picked data structure
	enum Pick : MEP::U_int32 {
		AVL = 1,
		BST = 1 << 2
	};
	//Base
	MEP::Template::Application& master;
	//Currently picked.
	MEP::U_int32 picked = Pick::AVL;
	//Picked events
	MEP::U_int32 pickedEvents = Pick::AVL;
	//Animation for the buttons.
	MEP::AnimationColor* alpha_color[NUMBER_OF_COLORS];
	//Coming soon color.
	MEP::AnimationColor* gray;
	//1st itersection 
	MEP::TextureObject* inter_a[NUMBER_OF_INTER];
	//Data structures
	TextButton DS[NUMBER_OF_DS];
	//Color buttons
	TextButton color[NUMBER_OF_COLORS];
	//Draw button
	TextButton draw;
	//Reset and back buttons.
	TextButton setUp[NUMBER_OF_SETUP];
	//Animation of the logo entering the screen.
	MEP::AnimationPosition* logo_entrance;
	//Texture of the logo entering the screen.
	MEP::TextureObject* logo;
private:
	//Initialization of the resources.
	void init(MEP::Template::Application& base, MEP::Template::Hub& hub);
	//--------------------------------------
	//Generation of the enu objects 'lines'.
	void gen(TextButton* color, MEP::Template::Application& base, int pos, const Config& config);
	void generate(TextButton* color, MEP::Template::Application& base, int start) {}
	template<typename First, typename ...Args>
	void generate(TextButton* color, MEP::Template::Application& base, int start, First&& first, Args&& ...args);
	template<typename ...Args>
	void generateLine(TextButton* color, MEP::Template::Application& base, int start, Args&& ... args);
	//--------------------------------------
	void pressedEvent(TextButton* _array, int size);
	//Back and ESC Event
	void BackESC();
public:
	//Constructor
	Menu(unsigned int ID, MEP::Template::Application& base, MEP::Template::Hub& hub);
	//Ovveride of the update exit method.
	//Which executes then the Status is Exit
	void updateExit(sf::Time& currentTime) override;
	//Override of the event handling method.
	void handleEvent(sf::RenderWindow& Window, sf::Event& event) override;
	//Destructor.
	~Menu();
};
		
inline void Menu::gen(TextButton* color, MEP::Template::Application& base, int pos, const Config& config) {
	//Generate a Button & Text
	color[pos].button = new MEP::Button(24, base.get<MEP::Object>(Res::Menu::Button, Res::Group::Menu), 35, { 0, 0 }, { 0.5, 0.5 });
	color[pos].text = new MEP::Text(config.text, base.get<sf::Font>(1), 35);
	//The position animation
	color[pos].button->addMethodPos(config.method);
	//Color management
	if (config.globalColor) {
		color[pos].button->setFollow(m_color);
	}
	else {
		//Alpha channel animation
		alpha_color[pos] = new MEP::AnimationColor({ config.color.r, config.color.g, config.color.b, 0 },
			{ config.color.r, config.color.g, config.color.b, 255 }, sf::milliseconds(300), 120, 0, 10,
			[](double x)->double { return std::pow(x, 4); });
		alpha_color[pos]->changeTag(MEP::AdditionalTag::RunAtEntryAndEnd);
		alpha_color[pos]->setDirection(MEP::Direction::Forward);
		//Setting the following.
		color[pos].button->setFollow(*alpha_color[pos]);
		color[pos].button->setColor(config.color);
	}
	color[pos].text->setFollow(m_color, MEP::ColorChannel::A);
	color[pos].button->setDrawTag(MEP::DrawTag::Resize_Pos);
	//Text update
	color[pos].text->addMethodPos(config.method_text);
	color[pos].text->setDrawTag(MEP::DrawTag::Resize_Pos);
	color[pos].text->changePositionTag(MEP::Text::PositionTag::XMiddle);
	//End of text update
}

inline void Menu::init(MEP::Template::Application& base, MEP::Template::Hub& hub) {
	/*
	* Initialization of a master color.
	*/
	m_color.changeTag(MEP::AdditionalTag::RunAtEntryAndEnd);
	/*
	* Initalization of a coming soon colors.
	*/
	gray = new MEP::AnimationColor({ 100, 100, 100, 0 }, { 100, 100, 100, 255 }, sf::milliseconds(300), 120, 0, 10,
		[](double x)->double { return std::pow(x, 4); });
	gray->changeTag(MEP::AdditionalTag::RunAtEntryAndEnd);
	/*
	* By default the animation direction is Backwards.
	* That is bacause of the common sense. Frame is 0 so our animation needed to be backwards (size -> 0) to achive that state.
	*/
	m_color.setDirection(MEP::Direction::Forward);
	/*
	* Setting the follow of all of the Hub elements.
	*/
	hub.setFollow(m_color, MEP::Template::HubElements::All, 
		MEP::ColorChannel::R | MEP::ColorChannel::G | MEP::ColorChannel::B);
	/*
	* Initialization of the top logo.
	* 1. Animation
	*/
	logo_entrance = new MEP::AnimationPosition(-100, 0, sf::milliseconds(300));
	logo_entrance->changeTag(MEP::AdditionalTag::RunAtEntryAndEnd);
	/*
	* 2. Texture
	*/
	logo = new MEP::TextureObject(base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu),
		{ 0, 0 });
	logo->setFollow(m_color);
	logo->setFollow(*logo_entrance, MEP::Following::FollowType::Y_Pos);
	/*
	* 3. Reposition.
	*/
	logo->addMethodPos([&base]()->sf::Vector2f {
		return { (float)(base.getResolution().x / 2) - (float)(base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().x/2), 0 };
		});
	logo->addDrawTag(MEP::DrawTag::Resize_Pos);
	/*
	* Initialization of the color buttons (itersections will need that too)
	* 1.Constants.
	*/
	sf::Vector2u objSize = base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize();
	const float y_const = 330.f;
	const float y_second = 70.f;
	/*
	* Intersection a)
	*/
	inter_a[0] = new MEP::TextureObject(base.get<MEP::Object>(Res::Intersection, Res::Group::Menu));
	inter_a[0]->addMethodPos([&base]()->sf::Vector2f {
		return { (float)(base.getResolution().x / 2) - (float)(base.get<MEP::Object>(Res::Intersection, Res::Group::Menu).getSize().x / 2),
			(float)(base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y) };
		});
	/*
	* Intersection b)
	*/
	inter_a[1] = new MEP::TextureObject(base.get<MEP::Object>(Res::Intersection, Res::Group::Menu));
	inter_a[1]->addMethodPos([&base, objSize, y_const]()->sf::Vector2f {
		return { (float)(base.getResolution().x / 2) - (float)(base.get<MEP::Object>(Res::Intersection, Res::Group::Menu).getSize().x / 2),
			y_const - (float)objSize.y / 32 - 40};
		});
	/*
	* Intersection c)
	*/
	inter_a[2] = new MEP::TextureObject(base.get<MEP::Object>(Res::Intersection, Res::Group::Menu));
	inter_a[2]->addMethodPos([&base, objSize, y_const, y_second]()->sf::Vector2f {
		return { (float)(base.getResolution().x / 2) - (float)(base.get<MEP::Object>(Res::Intersection, Res::Group::Menu).getSize().x / 2),
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second + 60};
		});
	for (int i = 0; i < 3; ++i) {
		inter_a[i]->addDrawTag(MEP::DrawTag::Resize_Pos);
		inter_a[i]->setFollow(m_color);
	}
	/*
	* 2. Actual generation.
	*/
	generateLine(color, base, 0, 
		Config([&base, objSize, y_const]()->sf::Vector2f { return
			{ 
			(float)base.getResolution().x/2 - (float)objSize.x / 4,
			y_const
			}; },
			[&base, objSize, y_const]()->sf::Vector2f { return
			{ 
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)objSize.x/8,
			y_const + (float)objSize.y / 32
			}; },
			"CYAN", sf::Color::Cyan, false),

		Config([&base, objSize, y_const]()->sf::Vector2f { return
			{ 
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 - (float)(objSize.x/4 + 10),
			y_const
			}; }, 
			[&base, objSize, y_const]()->sf::Vector2f { return
			{ 
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 - (float)(objSize.x / 4 + 10) + (float)objSize.x / 8 - 5,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32
			}; },
			"GREEN", { 0, 190, 0 }, false),

		Config([&base, objSize, y_const]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 4 + 10),
			y_const
			}; },
			[&base, objSize, y_const]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 4 + 10) + (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32
			}; },
			"RED", {190, 0, 0}, false),

		Config([&base, objSize, y_const]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 2 + 20),
			y_const
			}; },
			[&base, objSize, y_const]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 2 + 20) + (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32
			}; },
			"BROWN", { 222, 184, 135 }, false),

		Config([&base, objSize, y_const, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4,
			y_const + y_second
			}; },
			[&base, objSize, y_const, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second
			}; },
			"ORANGE", { 255, 140, 0 }, false),

		Config([&base, objSize, y_const, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 4 + 10),
			y_const + y_second
			}; },
			[&base, objSize, y_const, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 4 + 10) + (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second
			}; },
			"PINK", { 255, 105, 180 }, false));
	/*
	* Generating data structures.
	*/
	const float y_const_DS = 150;
	generateLine(DS, base, 0,
		Config([&base, objSize, y_const_DS]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8,
			y_const_DS
			}; },
			[&base, objSize, y_const_DS]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8 + (float)objSize.x / 8,
			y_const_DS + (float)objSize.y / 32
			}; },
			"AVL", sf::Color::Cyan, true), 
		Config([&base, objSize, y_const_DS]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8 - (float)(objSize.x / 4 + 10),
			y_const_DS
			}; },
			[&base, objSize, y_const_DS]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8 + (float)objSize.x / 8 - (float)(objSize.x / 4 + 10),
			y_const_DS + (float)objSize.y / 32
			}; },
			"BST", sf::Color::Cyan, true),
		Config([&base, objSize, y_const_DS, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8 + (float)(objSize.x / 4 + 10),
			y_const_DS
			}; },
			[&base, objSize, y_const_DS, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8 + (float)objSize.x / 8 + (float)(objSize.x / 4 + 10),
			y_const_DS + (float)objSize.y / 32
			}; },
			"LIST", sf::Color::Cyan, true),
		Config([&base, objSize, y_const_DS, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8,
			y_const_DS + y_second
			}; },
			[&base, objSize, y_const_DS, y_second]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8 + (float)objSize.x / 8,
			y_const_DS + (float)objSize.y / 32 + y_second
			}; },
			"RING", sf::Color::Cyan, true));
	/*
	* Finally draw button it is also going to be a line.
	*/
	generateLine(&draw, base, 0,
		Config([&base, y_const, y_second, objSize]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second + 100
			}; },
			[&base, y_const, y_second, objSize]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 8 + (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second + 100 + (float)objSize.y / 32
			}; },
			"DRAW", sf::Color::Cyan, true));
	/*
	* Conditional button generation.
	*/
	generateLine(setUp, base, 0,
		Config([&base, y_const, y_second, objSize]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second + 100
			}; },
			[&base, y_const, y_second, objSize]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second + 100 + (float)objSize.y / 32
			}; },
			"RESET", sf::Color::Cyan, true),
		Config([&base, y_const, y_second, objSize]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 4 + 10),
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second + 100
			}; },
			[&base, y_const, y_second, objSize]()->sf::Vector2f { return
			{
			(float)base.getResolution().x / 2 - (float)objSize.x / 4 + (float)(objSize.x / 4 + 10) + (float)objSize.x / 8,
			y_const + (float)base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu).getSize().y / 32 + y_second + 100 + (float)objSize.y / 32
			}; },
			"BACK", sf::Color::Cyan, true));
	//Disabling the buttons
	setUp[0].changeVisibility();
	setUp[1].changeVisibility();
	/*
	* Setting the gray buttons.
	*/
	DS[2].button->clearFollow();
	DS[2].button->setFollow(*gray);
	DS[3].button->clearFollow();
	DS[3].button->setFollow(*gray);
	/*
	* Blocking the Cyan button.
	*/
	color[0].button->forcePressed();
	color[0].button->block();
	/*
	* Blocking coming soon options.
	*/
	DS[2].button->block();
	DS[3].button->block();
	/*
	* 
	*/
	DS[0].button->forcePressed();
	DS[0].button->block();
	DS[0]._picked = true;
	DS[1].button->muteFollowGroup(1);
	/*
	* Initialziation of the aniamtions
	*/
	initMenu(DS);
	/*
	* Assigning the U_int32 values to the DS buttons.
	*/
	DS[1]._data = Pick::BST;
	DS[0]._data = Pick::AVL;
}

template<typename First, typename ...Args>
inline void Menu::generate(TextButton* color, MEP::Template::Application& base, int start, First&& first, Args&& ...args) {
	gen(color, base, start, std::forward<First>(first));
	generate(color, base, start + 1, std::forward<Args>(args) ...);
}

template<typename ...Args>
inline void Menu::generateLine(TextButton* color, MEP::Template::Application& base, int start, Args&& ... args) {
	generate(color, base, start, std::forward<Args>(args)...);
}

inline void Menu::pressedEvent(TextButton* _array, int size) {
	for (int i = 0; i < size; ++i) {
		_array[i].button->release();
		_array[i].button->forceRelease();
	}
}

inline void Menu::BackESC()
{
	if (picked & Pick::AVL) {
		if (!avl_module_v2) {
			//avl drawing module
			avl_module_v2 = new TreeController(0, master, m_color, false, Res::Windows::AVL);
			master.addWindow(avl_module_v2, Res::Windows::AVL);
			if (current_pos == 0)
				master.latestWindow().changeStatus(MEP::BaseWindow::Status::Entrance);
			else
				master.latestWindow().changeStatus(MEP::BaseWindow::Status::LowEntrance);
		}
		else {
			if (current_pos == 0)
				master.getWindow(master.getWindowGroup(Res::Windows::AVL).rbegin()->first, Res::Windows::AVL).changeStatus(MEP::BaseWindow::Status::Entrance);
			else
				master.getWindow(master.getWindowGroup(Res::Windows::AVL).rbegin()->first, Res::Windows::AVL).changeStatus(MEP::BaseWindow::Status::LowEntrance);
		}
	}
	if (picked & Pick::BST) {
		if (!bst_module_v2) {
			//avl drawing module
			bst_module_v2 = new TreeController<BST_Tree<int, Position>>(0, master, m_color, false, Res::Windows::BST);
			master.addWindow(bst_module_v2, Res::Windows::BST);
			if (current_pos == 1)
				master.latestWindow().changeStatus(MEP::BaseWindow::Status::Entrance);
			else 
				master.latestWindow().changeStatus(MEP::BaseWindow::Status::LowEntrance);
		}
		else {
			if (current_pos == 1)
				master.getWindow(master.getWindowGroup(Res::Windows::BST).rbegin()->first, Res::Windows::BST).changeStatus(MEP::BaseWindow::Status::Entrance);
			else
				master.getWindow(master.getWindowGroup(Res::Windows::BST).rbegin()->first, Res::Windows::BST).changeStatus(MEP::BaseWindow::Status::LowEntrance);
		}
	}
	if (!(picked & Pick::AVL)) {
		if (avl_module_v2) {
			MEP::BaseWindow& x = master.getWindow(master.getWindowGroup(Res::Windows::AVL).rbegin()->first,
				Res::Windows::AVL);
			if(x.getStatus() == MEP::BaseWindow::Status::Main)
				x.changeStatus(MEP::BaseWindow::Status::Exit);
			else
				x.changeStatus(MEP::BaseWindow::Status::LowExit);
		}
	}
	if (!(picked & Pick::BST)) {
		if (bst_module_v2) {
			MEP::BaseWindow& x = master.getWindow(master.getWindowGroup(Res::Windows::BST).rbegin()->first,
				Res::Windows::BST);
			if (x.getStatus() == MEP::BaseWindow::Status::Main)
				x.changeStatus(MEP::BaseWindow::Status::Exit);
			else
				x.changeStatus(MEP::BaseWindow::Status::LowExit);
		}
	}

	//Exiting the menu
	m_color.changeEntryColor({
		m_color.getFrameAsColor().r,
		m_color.getFrameAsColor().g,
		m_color.getFrameAsColor().b,
		0,
		});
	m_color.changeExitColor(m_color.getFrameAsColor());
	this->changeStatus(MEP::BaseWindow::Status::Exit);
	debugOutput(std::cout);
}

inline Menu::Menu(unsigned int ID, MEP::Template::Application& base, MEP::Template::Hub& hub) :
	MEP::BaseWindow(ID),
	Modules(base),
	MenuAnimations(this),
	master(base){
	init(base, hub);
	newObjects(m_color,
		alpha_color[0],
		alpha_color[1],
		alpha_color[2],
		alpha_color[3],
		alpha_color[4],
		alpha_color[5],
		color[0].button,
		color[0].text,
		color[1].button,
		color[1].text,
		color[2].button,
		color[2].text,
		color[3].button,
		color[3].text,
		color[4].button,
		color[4].text,
		color[5].button,
		color[5].text,
		DS[0].button,
		DS[0].text,
		DS[1].button,
		DS[1].text,
		DS[2].button,
		DS[2].text,
		DS[3].button,
		DS[3].text,
		logo,
		logo_entrance,
		inter_a[0],
		inter_a[1],
		inter_a[2],
		setUp[0].button,
		setUp[0].text,
		setUp[1].button,
		setUp[1].text,
		gray,
		draw.button,
		draw.text);
}
void Menu::updateExit(sf::Time& currentTime) {
	bool isActive = false;
	_execute([&currentTime, &isActive](auto& x) {
		if (x.isActive()) {
			x.get()->exitUpdate(currentTime);
			if (x.get()->isActive())
				isActive = true;
		}
		});
	if (!isActive) {
		if (!disableDraw) {
			setUp[0].changeVisibility();
			setUp[1].changeVisibility();
			draw.changeVisibility();
			disableDraw = true;
		}
		changeStatus(MEP::BaseWindow::Status::NullAction);
	}
}
void Menu::handleEvent(sf::RenderWindow& Window, sf::Event& event){
	sf::Vector2i pos = sf::Mouse::getPosition(Window);
	if (event.type == sf::Event::KeyReleased) {
		//Escape button disables the menu the menu
		if (event.key.code == sf::Keyboard::Escape) {
			if (!m_color.isActive()) {
				BackESC();
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			//Data structures buttons
			for (int i = 0; i < NUMBER_OF_DS; ++i)
				if (DS[i].button->mousePress(pos)) {
				}
			//-----------------------
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left) {
			//Data structures buttons
			for (int i = 0; i < NUMBER_OF_DS; ++i)
				if (!DS[i].button->isTansparent(pos.x, pos.y)) {
					if (DS[i].button->isBlocked()) {
						manager.runGroup(MEP::Direction::Forward, AnimationGroups::ButtonSizeUp);
						bool _block = false;
						if (i == 0) {
							picked ^= Pick::AVL;
							if (picked == 0) {
								picked |= Pick::AVL;
								_block = true;
							}
						}
						if (i == 1) {
							picked ^= Pick::BST;
							if (picked == 0) {
								picked |= Pick::BST;
								_block = true;
							}
						}
						if (!_block) {
							DS[i].button->release();
							DS[i].button->forceReleased();
							DS[i]._picked = false;	
							if(i == current_pos)
								buttonEvents(DS, i, true);
						}
					}
					else {
						DS[i].button->forcePress();
						DS[i].button->block();
						DS[i]._picked = true;
						buttonEvents(DS, i);
						if (i == 0) {
							picked |= Pick::AVL;
						}
						if (i == 1) {
							picked |= Pick::BST;
						}
					}
				}
			//-----------------------
		}
		else if (event.mouseButton.button == sf::Mouse::Right) {
			for (int i = 0; i < NUMBER_OF_DS; ++i)
				if (!DS[i].button->isTansparent(pos.x, pos.y)) {
					if (DS[i].button->isBlocked()) {
						buttonEvents(DS, i);
					}
				}
		}
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		//Data structures buttons
		for (int i = 0; i < NUMBER_OF_DS; ++i)
			DS[i].button->mouseActivity(pos);
		//-----------------------

	}
	if (setUp[0].isVisible() and setUp[0].button->handleEvent(event, pos)) {
		if (picked & Pick::AVL) {
			if (avl_module_v2) {
				if (master.getWindowGroup(Res::Windows::AVL).rbegin()->first != 0) {
					master.getWindow(master.getWindowGroup(Res::Windows::AVL).rbegin()->first, Res::Windows::AVL).changeStatus(MEP::BaseWindow::Status::NullAction);
					std::string text = avl_module_v2->getText();
					master.deleteWindowGroup(Res::Windows::AVL);
					avl_module_v2 = new TreeController(0, master, m_color, false, Res::Windows::AVL);
					master.addWindow(avl_module_v2, Res::Windows::AVL);
					avl_module_v2->changeStatus(MEP::BaseWindow::Status::Main);
				}
			}
		}
		if (picked & Pick::BST) {
			if (bst_module_v2) {
				if (master.getWindowGroup(Res::Windows::BST).rbegin()->first != 0) {
					master.getWindow(master.getWindowGroup(Res::Windows::BST).rbegin()->first, Res::Windows::BST).changeStatus(MEP::BaseWindow::Status::NullAction);
					std::string text = bst_module_v2->getText();
					master.deleteWindowGroup(Res::Windows::BST);
					bst_module_v2 = new TreeController<BST_Tree<int, Position>>(0, master, m_color, false, Res::Windows::BST);
					master.addWindow(bst_module_v2, Res::Windows::BST);
					bst_module_v2->changeStatus(MEP::BaseWindow::Status::Main);
				}
			}
		}
	}
	else if (setUp[1].isVisible() and setUp[1].button->handleEvent(event, pos)) {
		if (!m_color.isActive()) {
			BackESC();
		}
	}
	else if (draw.isVisible() and draw.button->handleEvent(event, pos)) {
		if (!m_color.isActive()) {
			BackESC();
		}
	}
	else {
		for (int i = 0; i < NUMBER_OF_COLORS; ++i)
			if (color[i].button->handleEvent(event, pos)) {
				if (m_color.reset()) {
					pressedEvent(color, NUMBER_OF_COLORS);
					color[i].button->forcePress();
					color[i].button->block();
					m_color.changeEntryColor(m_color.getFrameAsColor());
					m_color.changeExitColor(color[i].button->getColor());
					m_color.run(MEP::Direction::Forward);
				}
			}
	}
}

Menu::~Menu() {
	for (int i = 0; i < NUMBER_OF_COLORS; ++i)
		delete alpha_color[i];
	delete gray;
	for(int i = 0; i < NUMBER_OF_INTER; ++i)
		delete inter_a[i];
	delete logo;
	delete logo_entrance;
}

#endif
