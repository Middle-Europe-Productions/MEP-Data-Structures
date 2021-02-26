
#ifndef MENU_H
#define MENU_H
#define NUMBER_OF_COLORS 6
#define NUMBER_OF_INTER 3
#define NUMBER_OF_DS 4
#define NUMBER_OF_SETUP 4

#include<MEP.h>
#include"ResNames.h"
#include"Modules.h"

class Menu : public Modules, public MEP::Window::BaseWindow {
	//Picked data structure
	enum Pick : MEP::U_int32 {
		AVL = 0,
		BST = 1
	};
	//Individual 'button'
	struct TextButton {
		MEP::Button* button;
		MEP::Text* text;
		void changeVisibility();
		bool isVisible() const;
	};
	//Configuration for folding initializatiom method.
	struct Config {
		//Method of calculating the position of the button.
		std::function<sf::Vector2f()> method;
		//Method of calculating the position of the text on the button.
		std::function<sf::Vector2f()> method_text;
		//Text content.
		std::string text;
		//Color
		sf::Color color;
		//Use global color
		bool globalColor = false;
		Config(std::function<sf::Vector2f()> m, std::function<sf::Vector2f()> m_t, std::string t, sf::Color c, bool g = false) :
			method_text(m_t),
			method(m),
			text(t),
			color(c),
			globalColor(g) {}
	};
	//Base
	MEP::Window::Template::Application& master;
	//Currently picked.
	Pick picked;
	//Master color
	MEP::AnimationColor* m_color;
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
	void init(MEP::Window::Template::Application& base, MEP::Window::Template::Hub& hub);
	//--------------------------------------
	//Generation of the enu objects 'lines'.
	void gen(TextButton* color, MEP::Window::Template::Application& base, int pos, const Config& config);
	void generate(TextButton* color, MEP::Window::Template::Application& base, int start) {}
	template<typename First, typename ...Args>
	void generate(TextButton* color, MEP::Window::Template::Application& base, int start, First&& first, Args&& ...args);
	template<typename ...Args>
	void generateLine(TextButton* color, MEP::Window::Template::Application& base, int start, Args&& ... args);
	//--------------------------------------
	void pressedEvent(TextButton* _array, int size);
public:
	//Constructor
	Menu(unsigned int ID, MEP::Window::Template::Application& base, MEP::Window::Template::Hub& hub);
	//Ovveride of the update exit method.
	//Which executes then the Window::Status is Exit
	void updateExit(sf::Time& currentTime) override;
	//Override of the event handling method.
	void handleEvent(sf::RenderWindow& Window, sf::Event& event) override;
	//Destructor.
	~Menu();
};

void Menu::TextButton::changeVisibility() {
	if (!isVisible()) {
		button->removeDrawTag(MEP::DrawTag::Unactive);
		text->removeDrawTag(MEP::DrawTag::Unactive);
	}
	else {
		button->addDrawTag(MEP::DrawTag::Unactive);
		text->addDrawTag(MEP::DrawTag::Unactive);
	}
}

bool Menu::TextButton::isVisible() const {
	return !(button->getDrawTag() & MEP::DrawTag::Unactive);
}
		
inline void Menu::gen(TextButton* color, MEP::Window::Template::Application& base, int pos, const Config& config) {
	//Generate a Button & Text
	color[pos].button = new MEP::Button(24, base.get<MEP::Object>(Res::Menu::Button1, Res::Group::Menu), 35, { 0, 0 }, { 0.5, 0.5 });
	color[pos].text = new MEP::Text(config.text, base.get<sf::Font>(1), 35);
	//The position animation
	color[pos].button->addMethodPos(config.method);
	//Color management
	if (config.globalColor) {
		color[pos].button->setFollow(*m_color);
	}
	else {
		//Alpha channel animation
		alpha_color[pos] = new MEP::AnimationColor({ config.color.r, config.color.g, config.color.b, 0 },
			{ config.color.r, config.color.g, config.color.b, 255 }, sf::milliseconds(300), 120, 0, 10,
			[](double x)->double { return std::pow(x, 4); });
		alpha_color[pos]->changeTag(MEP::Animation::AdditionalTag::RunAtEntryAndEnd);
		alpha_color[pos]->setDirection(MEP::Direction::Forward);
		//Setting the following.
		color[pos].button->setFollow(*alpha_color[pos]);
		color[pos].button->setColor(config.color);
	}
	color[pos].text->setFollow(*m_color, MEP::ColorChannel::A);
	color[pos].button->setDrawTag(MEP::DrawTag::Resize_Pos);
	//Text update
	color[pos].text->addMethodPos(config.method_text);
	color[pos].text->setDrawTag(MEP::DrawTag::Resize_Pos);
	color[pos].text->changePositionTag(MEP::Text::PositionTag::XMiddle);
	//End of text update
}

inline void Menu::init(MEP::Window::Template::Application& base, MEP::Window::Template::Hub& hub) {
	/*
	* Initialization of a master color.
	*/
	m_color = new MEP::AnimationColor({sf::Color::Cyan.r, sf::Color::Cyan.g, sf::Color::Cyan.b, 0 }, sf::Color::Cyan, sf::milliseconds(300), 120, 0, 10,
		[](double x)->double { return std::pow(x, 4); });
	m_color->changeTag(MEP::Animation::AdditionalTag::RunAtEntryAndEnd);
	/*
	* Initalization of a coming soon colors.
	*/
	gray = new MEP::AnimationColor({ 100, 100, 100, 0 }, { 100, 100, 100, 255 }, sf::milliseconds(300), 120, 0, 10,
		[](double x)->double { return std::pow(x, 4); });
	gray->changeTag(MEP::Animation::AdditionalTag::RunAtEntryAndEnd);
	/*
	* By default the animation direction is Backwards.
	* That is bacause of the common sense. Frame is 0 so our animation needed to be backwards (size -> 0) to achive that state.
	*/
	m_color->setDirection(MEP::Direction::Forward);
	/*
	* Setting the follow of all of the Hub elements.
	*/
	hub.setFollow(*m_color, MEP::Window::Template::HubElements::All, 
		MEP::ColorChannel::R | MEP::ColorChannel::G | MEP::ColorChannel::B);
	/*
	* Initialization of the top logo.
	* 1. Animation
	*/
	logo_entrance = new MEP::AnimationPosition(-100, 0, sf::milliseconds(300));
	logo_entrance->changeTag(MEP::Animation::AdditionalTag::RunAtEntryAndEnd);
	/*
	* 2. Texture
	*/
	logo = new MEP::TextureObject(base.get<MEP::Object>(Res::Menu::Logo, Res::Group::Menu),
		{ 0, 0 });
	logo->setFollow(*m_color);
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
		inter_a[i]->setFollow(*m_color);
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
	DS[1].button->clearFollow();
	DS[1].button->setFollow(*gray);
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
}

template<typename First, typename ...Args>
inline void Menu::generate(TextButton* color, MEP::Window::Template::Application& base, int start, First&& first, Args&& ...args) {
	gen(color, base, start, std::forward<First>(first));
	generate(color, base, start + 1, std::forward<Args>(args) ...);
}

template<typename ...Args>
inline void Menu::generateLine(TextButton* color, MEP::Window::Template::Application& base, int start, Args&& ... args) {
	generate(color, base, start, std::forward<Args>(args)...);
}

inline void Menu::pressedEvent(TextButton* _array, int size) {
	for (int i = 0; i < size; ++i) {
		_array[i].button->release();
		_array[i].button->forceRelease();
	}
}

inline Menu::Menu(unsigned int ID, MEP::Window::Template::Application& base, MEP::Window::Template::Hub& hub) :
	MEP::Window::BaseWindow(ID),
	master(base) {
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
	for (auto& x : m_objects) {
		x->exitUpdate(currentTime);
	}
	for (auto& x : m_objects) {
		if (x->isActive())
			isActive = true;
	}
	if (!isActive) {
		if (disableDraw) {
			setUp[0].changeVisibility();
			setUp[1].changeVisibility();
			draw.changeVisibility();
			disableDraw = false;
		}
		changeStatus(MEP::Window::BaseWindow::Status::NullAction);
	}
}
void Menu::handleEvent(sf::RenderWindow& Window, sf::Event& event){
	sf::Vector2i pos = sf::Mouse::getPosition(Window);
	if (event.type == sf::Event::KeyReleased) {
		//Escape button disables the menu the menu
		if (event.key.code == sf::Keyboard::Escape) {
			if (isGenerated() and !m_color->isActive()) {
				this->changeStatus(MEP::Window::BaseWindow::Status::Exit);
				m_color->changeEntryColor({
				m_color->getFrameAsColor().r,
				m_color->getFrameAsColor().g,
				m_color->getFrameAsColor().b,
				0,
					});
				m_color->changeExitColor(m_color->getFrameAsColor());
				if (picked == Pick::AVL) {
					if (avl_module_v2) {
						master.getBaseWindow(avl_module_v2->_newestID()).changeStatus(MEP::Window::BaseWindow::Status::Entrance);
					}
				}
			}
		}
	}
	else if (setUp[0].isVisible() and setUp[0].button->handleEvent(event, pos)) {
		if (picked == Pick::AVL) {
			master.getBaseWindow(avl_module_v2->_newestID()).changeStatus(MEP::Window::BaseWindow::Status::NullAction);
			for (int i = AVL_ID.begin_ID + 1; i <= avl_module_v2->_newestID(); ++i) {
				master.deleteWindow(i);
			}
			avl_module_v2->setDispText(avl_module_v2->getText());
			avl_module_v2->_setNewsetID(AVL_ID.begin_ID);
			avl_module_v2->clear();
			avl_module_v2->changeStatus(MEP::Window::BaseWindow::Status::InProgress);
		}
	}
	else if (setUp[1].isVisible() and setUp[1].button->handleEvent(event, pos)) {
		if (!m_color->isActive()) {
			this->changeStatus(MEP::Window::BaseWindow::Status::Exit);
			m_color->changeEntryColor({
					m_color->getFrameAsColor().r,
					m_color->getFrameAsColor().g,
					m_color->getFrameAsColor().b,
					0,
				});
			m_color->changeExitColor(m_color->getFrameAsColor());
			master.getBaseWindow(avl_module_v2->_newestID()).changeStatus(MEP::Window::BaseWindow::Status::Entrance);
		}
	}
	else if (draw.isVisible() and draw.button->handleEvent(event, pos)) {
		if (!m_color->isActive()) {
			if (picked == Pick::AVL) {
				if (!avl_module_v2) {
					//Chngin the generate tag.
					genDS();
					//avl drawing module
					avl_module_v2 = new TreeController(AVL_ID.begin_ID, master, *m_color);
					master.addWindow(avl_module_v2);
					//Graph is generated.
					disableDraw = true;
					master.latestWindow().changeStatus(MEP::Window::BaseWindow::Status::Entrance);
				}
				else {
					master.getBaseWindow(avl_module_v2->_newestID()).changeStatus(MEP::Window::BaseWindow::Status::Main);
				}
			}
			//Exiting the menu
			m_color->changeEntryColor({
				m_color->getFrameAsColor().r,
				m_color->getFrameAsColor().g,
				m_color->getFrameAsColor().b,
				0,
				});
			m_color->changeExitColor(m_color->getFrameAsColor());
			this->changeStatus(MEP::Window::BaseWindow::Status::Exit);
		}
	}
	else {
		for (int i = 0; i < NUMBER_OF_COLORS; ++i)
			if (color[i].button->handleEvent(event, pos)) {
				if (m_color->reset()) {
					pressedEvent(color, NUMBER_OF_COLORS);
					color[i].button->forcePress();
					color[i].button->block();
					m_color->changeEntryColor(m_color->getFrameAsColor());
					m_color->changeExitColor(color[i].button->getColor());
					m_color->run(MEP::Direction::Forward);
				}
			}
		for (int i = 0; i < NUMBER_OF_DS; ++i)
			if (DS[i].button->handleEvent(event, pos)) {
				pressedEvent(DS, 2);
				DS[i].button->forcePress();
				DS[i].button->block();
				picked = (Pick)i;
			}
	}
}

Menu::~Menu() {
	delete m_color;
	for (int i = 0; i < NUMBER_OF_COLORS; ++i) {
		delete color[i].button;
		delete color[i].text;
	}
	delete logo;
	//delete logo_entranc;
	for (int i = 0; i < NUMBER_OF_INTER; ++i)
		delete inter_a[i];
}

#endif