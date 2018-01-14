#include "MainMenuScene.h"

#include "CreatorReader\CreatorReader.h"

#define FT_DATA_ERROR(...) CCLOGWARN("[DATA ERROR] %s", cocos2d::StringUtils::format("%s", ##__VA_ARGS__).c_str()); 

template<typename TClass>
using CWidgetCallback = std::_Mem_fn<void (TClass::*)(cocos2d::Ref*)>;

template<typename TClass, typename TWidget>
using CWidgetEventListener = std::_Mem_fn<void (TClass::*)(TWidget*, typename TWidget::EventType)>;

namespace
{
	namespace EScreen
	{
		std::string const& Main = "MainScreen";
		std::string const& Settings = "SettingsScreen";
		std::string const& Levels = "LevelsScreen";
		std::string const& About = "AboutScreen";
	} // namespace EScreen

	char const* const mainMenuCreatorScenePath = "creator/MainMenu/MainMenu.ccreator";

	std::string GetRecursivelySearching(std::string const& regularExp)
	{
		static std::string const recursively = "//";
		return recursively + regularExp;
	}

	template<typename TNode>
	TNode* GetChildNodeByType(cocos2d::Node const* node, std::string const& nodeName)
	{
		TNode* result = nullptr;
		node->enumerateChildren(GetRecursivelySearching(nodeName), [&result](cocos2d::Node* node)
		{
			if (auto tNode = dynamic_cast<TNode*>(node))
			{
				result = tNode;
				return true;
			}

			return false;
		});

		return result;
	}

	cocos2d::Node* GetChildNode(cocos2d::Node const* node, std::string const& nodeName)
	{
		cocos2d::Node* result = nullptr;
		node->enumerateChildren(GetRecursivelySearching(nodeName), [&result](cocos2d::Node* node)
		{
			result = node;
			return true;
		});

		return result;
	}

	template<typename TWidget>
	TWidget* GetScreenWidget(cocos2d::Node const* node, std::string const& screenName, std::string const& optionName)
	{
		auto screen = GetChildNode(node, screenName);
		return GetChildNodeByType<TWidget>(screen, optionName);
	}

	template<typename TClass>
	bool AddClickCallback(cocos2d::Node const* node, std::string const& screenName, std::string const& optionName, TClass& receiver, CWidgetCallback<TClass> const& callback)
	{
		if (auto widget = GetScreenWidget<cocos2d::ui::Widget>(node, screenName, optionName))
		{
			widget->addClickEventListener([&receiver, callback](cocos2d::Ref* sender)
			{
				callback(receiver, sender);
			});
			return true;
		}

		return false;
	}

	template<typename TWidget, typename TClass>
	bool AddWidgetEventListener(cocos2d::Node const* node, std::string const& screenName, std::string const& optionName, TClass& receiver, CWidgetEventListener<TClass, TWidget> const& callback)
	{
		if (auto widget = GetScreenWidget<TWidget>(node, screenName, optionName))
		{
			widget->addEventListener([&receiver, callback](cocos2d::Ref* sender, TWidget::EventType event)
			{
				callback(receiver, static_cast<TWidget*>(sender), event);
			});
			return true;
		}

		return false;
	}

} // namespace

bool CMainMenuScene::init()
{
	if (!Scene::init())
	{
		FT_DATA_ERROR("Supe::init error");
		return false;
	}

	if (!InitializeUIWidgets())
	{
		return false;
	}

	return true;
}

void CMainMenuScene::OnPlayButtonClick(cocos2d::Ref* sender)
{
	CCLOG("Play button click");
	ShowScreen(EScreen::Levels);
}

void CMainMenuScene::OnSettingsButtonClick(cocos2d::Ref* sender)
{
	CCLOG("Settings button click");
	ShowScreen(EScreen::Settings);
}

void CMainMenuScene::OnAboutButtonClick(cocos2d::Ref* sender)
{
	CCLOG("About button click");
	ShowScreen(EScreen::About);
}

void CMainMenuScene::OnExitButtonClick(cocos2d::Ref* sender)
{
	CCLOG("Exit button click");
	cocos2d::Director::getInstance()->end();
}

void CMainMenuScene::OnSoundVolumeEvent(cocos2d::ui::Slider* sender, cocos2d::ui::Slider::EventType event)
{
}

void CMainMenuScene::OnGameSpeedEvent(cocos2d::ui::Slider* sender, cocos2d::ui::Slider::EventType event)
{
}

void CMainMenuScene::OnMouseSensitiveEvent(cocos2d::ui::Slider* sender, cocos2d::ui::Slider::EventType event)
{
}

void CMainMenuScene::OnLevelSelectionEvent(cocos2d::ui::PageView* sender, cocos2d::ui::PageView::EventType event)
{
	CCLOG("Level selection page is turned");
}

bool CMainMenuScene::InitializeUIWidgets()
{
	auto mainMenuReader = creator::CreatorReader::createWithFilename(mainMenuCreatorScenePath);
	if (mainMenuReader == nullptr)
	{
		FT_DATA_ERROR("Loading of the %s is failed", mainMenuCreatorScenePath);
		return false;
	}

	mainMenuReader->setup();
	m_uiRoot = mainMenuReader->getSceneGraph();
	CCASSERT(m_uiRoot != nullptr, "Main menu: Creator scene is not valid");
	addChild(m_uiRoot);

	if (!AddClickCallback(m_uiRoot, EScreen::Main, "ExitButton", *this, std::mem_fn(&CMainMenuScene::OnExitButtonClick)))
	{
		FT_DATA_ERROR("Exit button is not found");
	}

	if (!AddClickCallback(m_uiRoot, EScreen::Main, "AboutButton", *this,  std::mem_fn(&CMainMenuScene::OnAboutButtonClick)))
	{
		FT_DATA_ERROR("About button is not found");
	}

	if (!AddClickCallback(m_uiRoot, EScreen::Main, "SettingsButton", *this, std::mem_fn(&CMainMenuScene::OnSettingsButtonClick)))
	{
		FT_DATA_ERROR("Settings button is not found");
	}

	if (!AddClickCallback(m_uiRoot, EScreen::Main, "PlayButton", *this, std::mem_fn(&CMainMenuScene::OnPlayButtonClick)))
	{
		FT_DATA_ERROR("Play button is not found");
	}

	if (!AddWidgetEventListener(m_uiRoot, EScreen::Settings, "SoundVolumeSlider", *this, std::mem_fn(&CMainMenuScene::OnSoundVolumeEvent)))
	{
		FT_DATA_ERROR("Sound volume slider is not found");
	}

	if (!AddWidgetEventListener(m_uiRoot, EScreen::Settings, "GameSpeedSlider", *this, std::mem_fn(&CMainMenuScene::OnGameSpeedEvent)))
	{
		FT_DATA_ERROR("Game speed slider is not found");
	}

	if (!AddWidgetEventListener(m_uiRoot, EScreen::Settings, "MouseSensitive", *this, std::mem_fn(&CMainMenuScene::OnMouseSensitiveEvent)))
	{
		FT_DATA_ERROR("Mouse sensitive slider is not found");
	}

	if (!AddWidgetEventListener(m_uiRoot, EScreen::Levels, "LevelSelectionPageView", *this, std::mem_fn(&CMainMenuScene::OnLevelSelectionEvent)))
	{
		FT_DATA_ERROR("Level selection page view is not found");
	}

	return true;
}

void CMainMenuScene::ShowScreen(std::string const& screenName)
{
	if (m_currentScreen != nullptr)
	{
		m_currentScreen->setVisible(false);
	}

	auto nextScreen = GetChildNode(m_uiRoot, screenName);

	if (nextScreen != nullptr)
	{
		nextScreen->setVisible(true);
		m_currentScreen = nextScreen;
	}
}
