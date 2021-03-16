#ifndef MENU_ANIMATION_H
#define MENU_ANIMATION_H
#define general_time 100
#include<MEPGraphics/AnimationManager.h>
#include"Data.h"

class MenuAnimations {
protected:
	enum AnimationGroups : MEP::U_int32 {
		ButtonSizeUp = 10,
		ButtonSizeDown = 11
	};
	enum ButtonSize : MEP::U_int32 {
		Scale = 10,
		X_pos = 11,
		Y_pos = 12
	};
	/**
	* Menu animation manager.
	*/
	MEP::AnimationManager manager;
	/**
	* Currently pressed button.
	*/
	int current_pos;
	/**
	* Initializatio method.
	*/
	void initMenu(TextButton x[NUMBER_OF_DS]) {
		manager.add(new MEP::AnimationPosition(0, 0.05, sf::milliseconds(general_time)), ButtonSize::Scale, AnimationGroups::ButtonSizeUp);
		manager.add(new MEP::AnimationPosition(0, -8, sf::milliseconds(general_time)), ButtonSize::X_pos, AnimationGroups::ButtonSizeUp);
		manager.add(new MEP::AnimationPosition(0, -2, sf::milliseconds(general_time)), ButtonSize::Y_pos, AnimationGroups::ButtonSizeUp);
		manager.executeGroup([](auto& x) { x.second->setDirection(MEP::Direction::Backwards); x.second->reset(); }, AnimationGroups::ButtonSizeUp);
		manager.add(new MEP::AnimationPosition(0.05, 0, sf::milliseconds(general_time)), ButtonSize::Scale, AnimationGroups::ButtonSizeDown);
		manager.add(new MEP::AnimationPosition(-8, 0, sf::milliseconds(general_time)), ButtonSize::X_pos, AnimationGroups::ButtonSizeDown);
		manager.add(new MEP::AnimationPosition(-2, 0, sf::milliseconds(general_time)), ButtonSize::Y_pos, AnimationGroups::ButtonSizeDown);
		manager.debugOutput(std::cout);

		MEP::AnimationPosition& local = manager.get<MEP::AnimationPosition>(ButtonSize::Scale, AnimationGroups::ButtonSizeUp);
		MEP::AnimationPosition& local1 = manager.get<MEP::AnimationPosition>(ButtonSize::X_pos, AnimationGroups::ButtonSizeUp);
		MEP::AnimationPosition& local2 = manager.get<MEP::AnimationPosition>(ButtonSize::Y_pos, AnimationGroups::ButtonSizeUp);
		MEP::AnimationPosition& local3 = manager.get<MEP::AnimationPosition>(ButtonSize::Scale, AnimationGroups::ButtonSizeDown);
		MEP::AnimationPosition& local4 = manager.get<MEP::AnimationPosition>(ButtonSize::X_pos, AnimationGroups::ButtonSizeDown);
		MEP::AnimationPosition& local5 = manager.get<MEP::AnimationPosition>(ButtonSize::Y_pos, AnimationGroups::ButtonSizeDown);
		for (int i = 0; i < NUMBER_OF_DS; ++i) {
			x[i].button->setFollow(local, MEP::Following::FollowType::X_Scale, AnimationGroups::ButtonSizeUp);
			x[i].button->setFollow(local, MEP::Following::FollowType::Y_Scale, AnimationGroups::ButtonSizeUp);
			x[i].button->setFollow(local1, MEP::Following::FollowType::X_Pos, AnimationGroups::ButtonSizeUp);
			x[i].button->setFollow(local2, MEP::Following::FollowType::Y_Pos, AnimationGroups::ButtonSizeUp);
			x[i].button->setFollow(local3, MEP::Following::FollowType::X_Scale, AnimationGroups::ButtonSizeDown);
			x[i].button->setFollow(local3, MEP::Following::FollowType::Y_Scale, AnimationGroups::ButtonSizeDown);
			x[i].button->setFollow(local4, MEP::Following::FollowType::X_Pos, AnimationGroups::ButtonSizeDown);
			x[i].button->setFollow(local5, MEP::Following::FollowType::Y_Pos, AnimationGroups::ButtonSizeDown);
			x[i].button->muteFollowGroup(AnimationGroups::ButtonSizeDown);
			if (i != 0) {
				x[i].button->muteFollowGroup(AnimationGroups::ButtonSizeUp);
			}
			else {
				current_pos = i;
			}
				
		}
	}
	/*
	* New pick.
	*/
	void buttonEvents(TextButton button[NUMBER_OF_DS], int pos, bool findPick = false) {
		if (pos != current_pos) {
			std::cout << "Right click";
			//Reset the events.
			manager.executeGroup([](auto& x) { x.second->setDirection(MEP::Direction::Backwards); x.second->reset(); }, AnimationGroups::ButtonSizeUp);
			manager.executeGroup([](auto& x) { x.second->setDirection(MEP::Direction::Backwards); x.second->reset(); }, AnimationGroups::ButtonSizeDown);
			//Active buttons.
			button[pos].button->unMuteFollowGroup(AnimationGroups::ButtonSizeUp);
			button[pos].button->muteFollowGroup(AnimationGroups::ButtonSizeDown);
			button[current_pos].button->unMuteFollowGroup(AnimationGroups::ButtonSizeDown);
			button[current_pos].button->muteFollowGroup(AnimationGroups::ButtonSizeUp);
			//Active animations
			manager.runGroup(MEP::Direction::Forward, AnimationGroups::ButtonSizeUp);
			manager.runGroup(MEP::Direction::Forward, AnimationGroups::ButtonSizeDown);
			current_pos = pos;
		}
		else {
			std::cout << "Wrong pick";
			if(findPick)
				for (int i = 0; i < NUMBER_OF_DS; ++i) {
					if (i != current_pos and button[i]._picked) {
						std::cout << "Click";
						buttonEvents(button, i);
					}
				}
		}
	}
public:
	MenuAnimations(MEP::BaseWindow* base_window): manager(*base_window)
	{
	}
};

#endif