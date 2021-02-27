#ifndef TREE_CONTROLLER_H
#define TREE_CONTROLLER_H
#include <MEPWindow/Base.h>
#include <MEPWindow/Application.h>
#include "Tree.h"
const int aniation_time = 500;

class Position {
	//X and Y possition
	int X;
	int Y;
	//previous X and Y
	int prev_X;
	int prev_Y;
public:
	//Constructors
	//----------------
	Position();
	Position(int _x, int _y);
	Position(const Position& x);
	//----------------
	Position& operator=(const Position& update);
	//Changes the X and Y position.
	void change(int _x, int _y);
	//Updates the previous position parameter with the X and Y.
	void update();
	//Parameters output.
	//----------------
	const int& getX() const;
	const int& getY() const;
	const int& getX_prev() const;
	const int& getY_prev() const;
	const int getX_change() const;
	const int getY_change() const;
	//----------------
};

class TreeController : public Tree<int, Position>, public MEP::Window::BaseWindow {
private:
	//Static variables (text on a top ball and newestID)
	inline static std::string currentText;
	inline static unsigned int newestID;
	//Master app
	MEP::Window::Template::Application& master_base;
	//Variables
	//Entrance animation
	MEP::AnimationPosition entrance;
	MEP::TextureObject add_ball;
	//Text on the ball.
	std::string text_onScreen;
	MEP::Text textToDisp;
	//Master color of the tree.
	MEP::AnimationColor& master_color;
	//Balls container.
	std::list<MEP::TextureObject> textures;
	//Text container.
	std::list<MEP::Text> text;
	//Lines container.
	std::list<MEP::TextureObject> lines;
	//Local aniations
	std::list<MEP::AnimationPosition> animations;
	//Lines alpha
	MEP::AnimationColor lines_alpha;
private:
	//Lines and node generation
	bool generateNode(Node* node, MEP::Window::Template::Application& base);
	void generateLines(Node* curr, MEP::Window::Template::Application& base, const int constant, bool update = false);
	//Generate position on the graph
	void generatePosition(Node* node, int& X_axis, int Y_axis, bool _switch);
	void generatePosition();
	//initialization
	void init(MEP::Window::Template::Application& base);
public:
	TreeController(unsigned int ID, MEP::Window::Template::Application& base, MEP::AnimationColor& color);
	TreeController(const TreeController& x);
	//Main event method.
	void handleEvent(sf::RenderWindow& Window, sf::Event& event) override;
	//Activates and deactivates the event handler
	//---------------------
	void activateEvents();
	void deactivateEvents();
	//---------------------
	//Setting the current text.
	//---------------------
	static void setText(const std::string& string);
	static const std::string getText();
	void setDispText(const std::string& dispText);
	//---------------------
	//Newest ID 
	static unsigned int _newestID();
	//Sets the newest ID
	static void _setNewsetID(const int newID);
};

Position::Position() : X(0), Y(0), prev_X(0), prev_Y(0) {}
Position::Position(int _x, int _y) : X(_x), Y(_y), prev_X(_x), prev_Y(_y) {}
Position::Position(const Position& x) : X(x.X), Y(x.Y), prev_X(x.prev_X), prev_Y(x.prev_Y) {}
Position& Position::operator=(const Position& update) {
	if (&update != this) {
		prev_X = update.prev_X;
		prev_Y = update.prev_Y;
		X = update.X;
	}
	return *this;
}
void Position::change(int _x, int _y) {
	X = _x;
	Y = _y;
}
void  Position::update() {
	prev_X = X;
	prev_Y = Y;
}
const int& Position::getX() const {
	return X;
}
const int& Position::getY() const {
	return Y;
}
const int& Position::getX_prev() const {
	return prev_X;
}
const int& Position::getY_prev() const {
	return prev_Y;
}
const int  Position::getX_change() const {
	return prev_X - X;
}
const int Position::getY_change() const {
	return prev_Y - Y;
}

TreeController::TreeController(unsigned int ID,
	MEP::Window::Template::Application& base,
	MEP::AnimationColor& color) :
	MEP::Window::BaseWindow(ID, base.getMepView(), base.getMepView()),
	master_base(base),
	entrance(MEP::AnimationPosition(100, 0, sf::milliseconds(300), 120, 0, 10, [](double x) -> double { return std::pow(x, 4); })),
	add_ball(MEP::TextureObject(base.get<MEP::Object>(1, Res::Group::Tree))),
	textToDisp(text_onScreen, base.get<sf::Font>(1), 40),
	master_color(color),
	lines_alpha({0,0,0,0}, {0,0,0,255}, sf::milliseconds(200))
{
	newestID = getID();
	init(base);
	m_view.move(-640 + 50, -100);
	/*
	insert(11, Position(-11, -11));
	insert(12, Position(-11, -11));
	insert(13, Position(-11, -11));
	insert(14, Position(-11, -11));
	insert(15, Position(-11, -11));
	insert(16, Position(-11, -11));
	insert(17, Position(-11, -11));
	insert(18, Position(-11, -11));
	insert(1, Position(-11, -11));
	insert(2, Position(-11, -11));
	insert(3, Position(-11, -11));
	generatePosition();
	*/
	generateLines(head, master_base, 100, true);
	generateLines(head, master_base, 100);
	entrance.run(MEP::Direction::Forward);
}

TreeController::TreeController(const TreeController& x) :
	Tree<int, Position>(x),
	MEP::Window::BaseWindow(x.getID() + 1, x.getView(), x.getMasterView()),
	master_base(x.master_base),
	entrance(MEP::AnimationPosition(100, 0, sf::milliseconds(300), 120, 0, 10, [](double x) -> double { return std::pow(x, 4); })),
	add_ball(MEP::TextureObject(master_base.get<MEP::Object>(1, Res::Group::Tree))),
	textToDisp(text_onScreen, master_base.get<sf::Font>(1), 40),
	master_color(x.master_color),
	lines_alpha({ 0,0,0,0 }, { 0,0,0,255 }, sf::milliseconds(aniation_time), 120, 0, 10, [](double x)->double { return std::pow(x, 4); })
{
	newestID = getID();
	init(master_base); 
	generatePosition();
	generateLines(head, master_base, 100);
	for (auto& it : animations)
		it.setDelay(sf::milliseconds(aniation_time), MEP::Direction::Forward);
	lines_alpha.setDelay(sf::milliseconds(aniation_time*2) , MEP::Direction::Forward);
	entrance.setDelay(sf::milliseconds(aniation_time),MEP::Direction::Forward);
	//lines_alpha.run(MEP::Direction::Forward);
}

bool TreeController::generateNode(Node* node, MEP::Window::Template::Application& base) {
	try {
		//Creating a line.
		if (node->up) {
			const sf::Vector2u& size = base.getObject(MEP::HUB::Box, MEP::AssetsGroup::HUB).getSize();
			//Line
			float out = sqrt(std::pow(node->m_info.getX() - node->up->m_info.getX(), 2) + std::pow(node->m_info.getY() - node->up->m_info.getY(), 2));
			lines.push_back(MEP::TextureObject(base.getObject(MEP::HUB::Box, MEP::AssetsGroup::HUB),
				sf::Vector2f((float)node->m_info.getX() + 50, node->m_info.getY() + 50)));
			//Scale
			lines.back().setScale({ (float)out / size.x, 0.5 });
			//Calculation of an angle
			float angleOut = atan2(node->m_info.getY() - node->up->m_info.getY(), node->m_info.getX() - node->up->m_info.getX());
			lines.back().setRotation(180 + angleOut * 180.0 / 3.141);
			//Setting the following color animation for RGB
			lines.back().setFollow(master_color, MEP::ColorChannel::R | MEP::ColorChannel::G | MEP::ColorChannel::B);
			lines.back().setFollow(lines_alpha, MEP::ColorChannel::A);
			newObject(lines.back(), true);
		}
		const sf::Vector2u& local = base.getObject(1, Res::Group::Tree).getSize();
		text.push_back(MEP::Text(std::to_string(node->m_key), base.getFont(1), 40, sf::Vector2f(node->m_info.getX() + (float)local.x / 2, node->m_info.getY() + (float)local.y / 2),
			MEP::Text::PositionTag::Middle));
		//Local size.
		//Texture generation.
		textures.push_back(MEP::TextureObject(base.getObject(1, Res::Group::Tree), sf::Vector2f(node->m_info.getX(), node->m_info.getY())));
		//Aniamtions	
		//X--------------------
		float x;
		if (node->m_info.getX_prev() == 0 and node->m_info.getY_prev() == 0)
			x = (float)(getView().getCenter().x - getMasterView().getCenter().x + add_ball.getPosition().x) - node->m_info.getX();
		else
			x = node->m_info.getX_change();
		//--------------------
		animations.push_back(MEP::AnimationPosition(x, 0, sf::milliseconds(aniation_time),
				120, 0, 10, [](double x)->double { return std::pow(x, 4); }));
		
		textures.back().setFollow(animations.back(), MEP::Following::FollowType::X_Pos);
		text.back().setFollow(animations.back(), MEP::Following::FollowType::X_Pos);
		newObject(animations.back());

		//Y--------------------
		float y;
		if (node->m_info.getX_prev() == 0 and node->m_info.getY_prev() == 0)
			y = (float)(getView().getCenter().y - getMasterView().getCenter().y + add_ball.getPosition().y) - node->m_info.getY() - 100;
		else
			y = node->m_info.getY_change();
		//--------------------

		animations.push_back(MEP::AnimationPosition(y, 0, sf::milliseconds(aniation_time),
			120, 0, 10, [](double x)->double { return std::pow(x, 4); }));
		newObject(animations.back());
		textures.back().setFollow(animations.back(), MEP::Following::FollowType::Y_Pos);
		text.back().setFollow(animations.back(), MEP::Following::FollowType::Y_Pos);
		//Setting the RGB animation
		textures.back().setFollow(master_color, MEP::ColorChannel::R | MEP::ColorChannel::G | MEP::ColorChannel::B);
		//
		newObject(textures.back());
		//Text generation.
		newObject(text.back());
	}
	catch (MEP::ResourceException& x) {
		std::cout << x.Message << std::endl;
		return false;
	}
	return true;
}

void TreeController::generateLines(Node* curr, MEP::Window::Template::Application& base, const int constant, bool update) {
	if (!curr)
		return;
	generateLines(curr->right, base, constant, update);
	if (update)
		curr->m_info.update();
	else
		generateNode(curr, base);
	generateLines(curr->left, base, constant, update);
}

void TreeController::generatePosition(Node* curr, int& X_axis, int Y_axis, bool _switch) {
	if (!curr)
		return;
	Y_axis += 1;
	curr->m_info.change((int)100 * -X_axis, (int)100 * Y_axis);
	if (!_switch)
		generatePosition(curr->right, X_axis, Y_axis, _switch);
	else
		generatePosition(curr->left, X_axis, Y_axis, _switch);
	if (!_switch)
		++X_axis;
	else
		--X_axis;
	curr->m_info.change((int)100 * -X_axis, (int)100 * Y_axis);
	if (!_switch)
		generatePosition(curr->left, X_axis, Y_axis, _switch);
	else
		generatePosition(curr->right, X_axis, Y_axis, _switch);
}

void TreeController::generatePosition() {
	if (!head)
		return;
	const int constant = 100;
	int X = 0, Y = 1;
	head->m_info.change(constant * X, constant * Y);
	generatePosition(head->left, X, Y, false);
	X = 0;
	generatePosition(head->right, X, Y, true);
}

void TreeController::init(MEP::Window::Template::Application& base) {
	//Updating the add_ball texture.
	add_ball.addMethodPos([&base]()->sf::Vector2f {
		return { (float)(base.getResolution().x / 2 - (float)base.get<MEP::Object>(1, Res::Group::Tree).getSize().x / 2),
			(float)(base.getResolution().y - (float)(base.get<MEP::Object>(1, Res::Group::Tree).getSize().y)) };
		});
	add_ball.addDrawTag(MEP::DrawTag::Resize_Pos | MEP::DrawTag::ViewLock);
	add_ball.setFollow(master_color, MEP::ColorChannel::R | MEP::ColorChannel::G | MEP::ColorChannel::B);
	add_ball.setFollow(entrance, MEP::Following::FollowType::Y_Pos);
	add_ball.updatePosition();
	//Changing the text.
	textToDisp << add_ball;
	textToDisp.changePositionTag(MEP::Text::PositionTag::Middle);
	textToDisp.movePosition({ 50, 50 });
	textToDisp.addDrawTag(MEP::DrawTag::Resize_Pos | MEP::DrawTag::ViewLock);
	lines_alpha.changeTag(MEP::Animation::AdditionalTag::RunAtEnd);
	newObjects(add_ball, entrance, textToDisp, lines_alpha);
}

void TreeController::handleEvent(sf::RenderWindow& Window, sf::Event& event) {
	//entering the text.
	if (event.type == sf::Event::MouseMoved) {
		//Finally, we need to change the view.
		//Build in method does that.
		if (m_grabbedWindow) {
			moveView(Window);
		}
	} 
	else if (event.type == sf::Event::TextEntered) {
		if ((event.text.unicode >= 48 and event.text.unicode <= 57) or event.text.unicode == 45)
		{
			if (text_onScreen.size() <= 3) {
				text_onScreen += static_cast<char>(event.text.unicode);
				textToDisp.setText(text_onScreen);
			}
		}
	} 
	else if (event.type == sf::Event::MouseButtonPressed) {
		//To move the view we do need to create an event.
		//Left click to grab in our case.
		if (event.mouseButton.button == sf::Mouse::Left) {
			//First one indicates change of the position.
			m_windowPossChange = sf::Mouse::getPosition(Window);
			//Second keeps track of the window.
			m_grabbedWindow = true;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
	//If the left button is released we do need to stop the dragging.
		if (event.mouseButton.button == sf::Mouse::Left) {
			m_grabbedWindow = false;
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		//Escape button enables the menu
		if (event.key.code == sf::Keyboard::Escape) {
			deactivateEvents();
			setText(textToDisp.getString());
			master_base.getBaseWindow(1).changeStatus(MEP::Window::BaseWindow::Status::Entrance);
		} 
		//Backspace key deletes the text from a button.
		else if (event.key.code == sf::Keyboard::Backspace) {
			if (text_onScreen.size() >= 1) {
				text_onScreen.pop_back();
				textToDisp.setText(text_onScreen);
			}
		}
		//Enter button inserts the text on the screen.
		else if (event.key.code == sf::Keyboard::Enter) {
			//m_color->run(MEP::Animation::Direction::Forward);
			if (text_onScreen.size() > 0) {
				int x = 0;
				bool exception = false;
				try {
					x = stoi(text_onScreen);
					exception = !insert(x, Position());
				}
				catch (const std::invalid_argument& x) {
					std::cout << x.what();
					exception = true;
				}
				if (!exception) {
					//Let's update the position of prev layer.
					generateLines(head, master_base, 100, true);
					if (this->getID() < 10000) {
						master_base.addWindow(new TreeController(*this));
						master_base.latestWindow().changeStatus(MEP::Window::BaseWindow::Status::Entrance);
						this->changeStatus(MEP::Window::BaseWindow::Status::Exit);
					}
				}
				else {
					text_onScreen.clear();
					textToDisp.setText(text_onScreen);
				}
				
			}
		}
	}
}
void TreeController::activateEvents() {
	if (getStatus() != MEP::Window::BaseWindow::Status::Main)
		changeStatus(MEP::Window::BaseWindow::Status::Main);
}
void TreeController::deactivateEvents() {
	if (getStatus() != MEP::Window::BaseWindow::Status::InProgress)
		changeStatus(MEP::Window::BaseWindow::Status::InProgress);
}
//Setting the current text.
void TreeController::setText(const std::string& string) {
	currentText = string;
}
const std::string TreeController::getText() {
	return currentText;
}
void TreeController::setDispText(const std::string& dispText) {
	textToDisp.setText(dispText);
	text_onScreen = dispText;
}
//Newest ID 
unsigned int TreeController::_newestID() {
	return newestID;
}
//Sets the newest ID
void TreeController::_setNewsetID(const int newID) {
	newestID = newID;
}
#endif

