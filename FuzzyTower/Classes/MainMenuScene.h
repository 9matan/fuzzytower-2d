#pragma once

#include "cocos/2d/CCScene.h"

#include "cocos/ui/UIPageView.h"
#include "cocos/ui/UISlider.h"

class CMainMenuScene: public cocos2d::Scene
{
public:
	CREATE_FUNC(CMainMenuScene)

public:
	virtual bool init() override;

private:
	cocos2d::Node* m_uiRoot = nullptr;
	cocos2d::Node* m_currentScreen = nullptr;

// ui events
private:
	// main screen
	void OnPlayButtonClick(cocos2d::Ref* sender);
	void OnSettingsButtonClick(cocos2d::Ref* sender);
	void OnAboutButtonClick(cocos2d::Ref* sender);
	void OnExitButtonClick(cocos2d::Ref* sender);
	// settings screen
	void OnSoundVolumeEvent(cocos2d::ui::Slider* sender, cocos2d::ui::Slider::EventType event);
	void OnGameSpeedEvent(cocos2d::ui::Slider* sender, cocos2d::ui::Slider::EventType event);
	void OnMouseSensitiveEvent(cocos2d::ui::Slider* sender, cocos2d::ui::Slider::EventType event);
	// levels screen
	void OnLevelSelectionEvent(cocos2d::ui::PageView* sender, cocos2d::ui::PageView::EventType event);

private:
	bool InitializeUIWidgets();

	void ShowScreen(std::string const& screenName);
};