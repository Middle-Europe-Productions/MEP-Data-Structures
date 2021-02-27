#include<MEP.h>

#include"ResNames.h"
#include"Menu.h"

class SimpleApp : public MEP::Window::Template::Application {
public:
	SimpleApp(const std::string& resPath, const std::string& title, MEP::U_int32 style) :
		MEP::Window::Template::Application(title.c_str(), resPath.c_str(), { 1280, 720 }, style) {}
	void createWindows() override {
		//Creating the hub
		MEP::Window::Template::Hub* hub = new MEP::Window::Template::Hub(0, *this, sf::Color::Cyan);
		//Adding the hub to the master windows.
		addWindowMaster(hub);
		//Creating the menu.
		addWindow(new Menu(1, *this, *hub));
		//Changing the windows status. (entrance means that it is going to activate the RunAtEntrance animations)
		latestWindow().changeStatus(MEP::Window::BaseWindow::Status::Entrance);
		latestWindowMaster().changeStatus(MEP::Window::BaseWindow::Status::Main);
	};
	void createResources() override {
		//Initalization of all of the app resources.
		initResources(MEP::Resource(1, Res::Group::Tree, "kw"));
		initResources(MEP::Resource(2, Res::Group::Tree, "topress", 3));
		initResources(MEP::Resource(Res::Menu::Button, Res::Group::Menu, "button/button", 37, true));
		initResources(MEP::Resource(Res::Menu::Logo, Res::Group::Menu, "logo"));
		initResources(MEP::Resource(Res::Menu::Intersection, Res::Group::Menu, "inside"));
		initResources(MEP::Resource(MEP::ResourceType::Font, 1, "Arialn.ttf"));;
	}

};
#if defined (MEP_WINDOWS)
	#if _DEBUG
		int main()
		{
			SimpleApp app("res/", "MEP Data Structures", sf::Style::Modern_Resize);
			try {
				app.initApp();		
				app.setFramerateLimit(120);
			}
			catch (const MEP::Window::WindowException& x) {
				std::cout << x;
			}
			try {
				app.run();
			}
			catch (const MEP::Window::WindowException& x) {
				std::cout << x;
			}
			return 0;
		}
	#else
		int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
		{
			SimpleApp app("res/", "MEP Data Structures", sf::Style::Modern_Resize); 
			try {
				app.initApp();
				app.setFramerateLimit(180);
			}
			catch (const MEP::Window::WindowException& x) {
				std::cout << x;
			}
			try {
				app.run();
			}
			catch (const MEP::Window::WindowException& x) {
				std::cout << x;
			}
			return 0;
		}
	#endif
#elif defined(MEP_LINUX)
	int main()
	{
		SimpleApp app("res/", "MEP Data Structures", sf::Style::None);
		try {
			app.initApp();		
			app.setFramerateLimit(120);
		}
		catch (const MEP::Window::WindowException& x) {
			std::cout << x;
		}
		try {
			app.run();
		}
		catch (const MEP::Window::WindowException& x) {
			std::cout << x;
		}
		return 0;
	}
#endif
