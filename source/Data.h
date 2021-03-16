#ifndef DATA_H
#define DATA_H

#define NUMBER_OF_COLORS 6
#define NUMBER_OF_INTER 3
#define NUMBER_OF_DS 4
#define NUMBER_OF_SETUP 4

#include<MEP.h>
//Individual 'button'
struct TextButton {
	MEP::Button* button;
	MEP::Text* text;
	MEP::U_int32 _data = 0;
	bool _picked = false;
	TextButton() : button(nullptr), text(nullptr) {}
	~TextButton() {
		if (button)
			delete button;
		if (text)
			delete text;
	}
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
		method(m),
		method_text(m_t),
		text(t),
		color(c),
		globalColor(g) {}
};

void TextButton::changeVisibility() {
	if (!isVisible()) {
		button->removeDrawTag(MEP::DrawTag::Unactive);
		text->removeDrawTag(MEP::DrawTag::Unactive);
	}
	else {
		button->addDrawTag(MEP::DrawTag::Unactive);
		text->addDrawTag(MEP::DrawTag::Unactive);
	}
}

bool TextButton::isVisible() const {
	return !(button->getDrawTag() & MEP::DrawTag::Unactive);
}

#endif