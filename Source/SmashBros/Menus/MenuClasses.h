
#include "CharSelectScreen.h"
#include "StageSelectScreen.h"
#include "MenuPieces/Arrows.h"
#include "MenuPieces/MenuBarSmallButton.h"
#include "MenuPieces/MenuBarToggle.h"
#include "MenuPieces/MenuBarValue.h"
#include "MenuPieces/ToggleButtons.h"
#include "MenuPieces/TogglePane.h"

#pragma once

namespace SmashBros
{
	class TitleScreen;
	class MainMenu;
	class GroupMenu;
	class SoloMenu;
	class BrawlCharSelect;
	class BrawlStageSelect;
	class OptionsMenu;
	class ControlOptions;
	class AudioOptions;
	class DisplayOptions;
	class OtherOptions;
	class TrainingCharSelect;
	class TrainingStageSelect;
	class RulesMenu;
	class ItemsMenu;
	class BluetoothMenu;
	
	class TitleScreen : public Screen
	{
	private:
		class TitleScreenActor : public Actor
		{
		public:
			TitleScreenActor(TitleScreen*screen, float x1, float y1);
			virtual ~TitleScreenActor();
			virtual void onRelease();

		private:
			TitleScreen*screen;
		};

		TitleScreenActor*titleScreen;

		long changeTime;
		boolean changing;

	public:
		TitleScreen(const String&name);
		virtual ~TitleScreen();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};

	class MainMenu : public MenuScreen
	{
	public:
		MainMenu(const String&name);
		virtual ~MainMenu();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};

	class GroupMenu : public MenuScreen
	{
	public:
		GroupMenu(const String&name);
		virtual ~GroupMenu();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};

	class SoloMenu : public MenuScreen
	{
	public:
		SoloMenu(const String&name);
		virtual ~SoloMenu();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};

	class BrawlCharSelect : public CharSelectScreen
	{
	private:
		class RulesBar : public Actor
		{
		public:
			RulesBar(float x1, float y1);
			virtual ~RulesBar();

			virtual void onMouseEnter();
			virtual void onMouseLeave();
			virtual void onRelease();
		};

		class ReadyToFightBar : public Actor
		{
		public:
			ReadyToFightBar(float x1, float y1);
			virtual ~ReadyToFightBar();

			virtual void onMouseEnter();
			virtual void onMouseLeave();
			virtual void onRelease();
		};

		class TeamsButton : public Actor
		{
		public:
			TeamsButton(float x1, float y1);
			virtual ~TeamsButton();

			virtual void onMouseEnter();
			virtual void onMouseLeave();
			virtual void onRelease();
		};

		RulesBar*rules_bar;
		TextActor*rules_bar_text;
		TextActor*rules_bar_value;
		Arrows*rules_arrows;

		ReadyToFightBar*readyToFight;
		TeamsButton*teams_button;
		
		boolean wasReadyToFight;

	public:
		BrawlCharSelect(const String&name);
		virtual ~BrawlCharSelect();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
		
		virtual void onSelectCharacter(int player, int character);
	};

	class BrawlStageSelect : public StageSelectScreen
	{
	public:
		BrawlStageSelect(const String&name);
		virtual ~BrawlStageSelect();
		
		virtual void LoadContent();
		virtual void Update(long gameTime);
		
		virtual void onSelect(int stageNo);
		virtual void onLoad();
	};

	class OptionsMenu : public MenuScreen
	{
	public:
		OptionsMenu(const String&name);
		virtual ~OptionsMenu();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};
	
	class ControlOptions : public Screen
	{
	private:
		MenuBarSmall*joystickLabel;
		MenuBarSmall*dpadLabel;
		ToggleButtons*toggles;
		
		static const float farDist;
		static const float centerDist;
		
		Actor*button_a;
		Actor*button_b;
		Actor*button_x;
		
		Actor*arrow_up;
		Actor*arrow_down;
		Actor*arrow_left;
		Actor*arrow_right;
		
		WireframeActor*joystickArea;
		Actor*joystick;
		
		byte joystickDir;
		bool joystickFar;
		bool joystickDown;
		
		bool joystickEnabled;
		
	public:
		ControlOptions(const String&name);
		virtual ~ControlOptions();
		
		virtual void Initialize();
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};
	
	class AudioOptions : public Screen
	{
	private:
		MenuBarToggle*menumusic;
		boolean prevMenumusic;
		MenuBarToggle*menusoundfx;
		boolean prevMenusoundfx;
		MenuBarToggle*ingamemusic;
		boolean prevIngamemusic;

		void onSettingToggle(String setting, boolean toggle);
	public:
		AudioOptions(const String&name);
		virtual ~AudioOptions();
		
		virtual void Initialize();
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};

	class DisplayOptions : public Screen
	{
	private:
		MenuBarToggle*fullScreen;
		boolean prevFullscreen;

		MenuBarValue*fps;
		void onSettingToggle(String setting, boolean toggle);

	public:
		DisplayOptions(const String&name);
		virtual ~DisplayOptions();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};
	
	class OtherOptions : public Screen
	{
	private:
		class FacebookButton : public MenuBarSmallButton
		{
		public:
			FacebookButton(float x1, float y1, const String&label);
			virtual ~FacebookButton();
			
			virtual void onRelease();
		};
		
		class TwitterButton : public MenuBarSmallButton
		{
		public:
			TwitterButton(float x1, float y1, const String&label);
			virtual ~TwitterButton();
			
			virtual void onRelease();
		};
        
        class Twitter2Button : public MenuBarSmallButton
        {
        public:
            Twitter2Button(float x1, float y1, const String&label);
            virtual ~Twitter2Button();
            
            virtual void onRelease();
        };
		
		class ContactDeveloperButton : public MenuBarSmallButton
		{
		public:
			ContactDeveloperButton(float x1, float y1, const String&label);
			virtual ~ContactDeveloperButton();
			
			virtual void onRelease();
		};
		
		class DonateButton : public MenuBarSmallButton
		{
		public:
			DonateButton(float x1, float y1, const String&label);
			virtual ~DonateButton();
			
			virtual void onRelease();
		};
		
		class DonateBitcoinButton : public MenuBarSmallButton
		{
		public:
			DonateBitcoinButton(float x1, float y1, const String&label);
			virtual ~DonateBitcoinButton();
			
			virtual void onRelease();
		};
		
		FacebookButton*facebook;
		TwitterButton*twitter;
        Twitter2Button*twitter2;
		ContactDeveloperButton*contactDev;
		DonateButton*donate;
		DonateBitcoinButton*donateBitcoin;
	public:
		OtherOptions(const String&name);
		virtual ~OtherOptions();
		
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};
	
	class TrainingCharSelect : public CharSelectScreen
	{
	private:
		class ReadyToFightBar : public Actor
		{
		public:
			ReadyToFightBar(float x1, float y1);
			virtual ~ReadyToFightBar();

			virtual void onMouseEnter();
			virtual void onMouseLeave();
			virtual void onRelease();
		};

		ReadyToFightBar*readyToFight;
		Actor*trainingBanner;
		
		boolean wasReadyToFight;

	public:
		TrainingCharSelect(String name);
		virtual ~TrainingCharSelect();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};

	class TrainingStageSelect : public StageSelectScreen
	{
	public:
		TrainingStageSelect(const String&name);
		virtual ~TrainingStageSelect();

		virtual void LoadContent();
	};

	class RulesMenu : public Screen
	{
	private:
		MenuBarSmallButton*button_items;
		MenuBarValue*stocks;
		MenuBarValue*time;
		ToggleButtons*gameMode;

	public:
		RulesMenu(const String&name);
		virtual ~RulesMenu();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};

	class ItemsMenu : public Screen
	{
	private:
		ActorGrid*paneGrid;
		ActorGrid*itemGrid;
		
		static boolean updatingGrid;

		class ItemPane : public TogglePane
		{
		private:
			int id;
			ItemsMenu*menu;

		public:
			ItemPane(ItemsMenu*menu, int id, float x1, float y1, boolean toggle);
			virtual ~ItemPane();

			virtual void onToggle(boolean toggle);
		};
		
		void addActorsToGrid();
		
	public:
		ItemsMenu(const String&name);
		virtual ~ItemsMenu();

		virtual void Initialize();
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};
	
	class BluetoothMenu : public Screen
	{
	private:
		class PeerCard
		{
		private:
			Actor*card;
			TextActor*textDisplay;
			String displayName;
			String peerID;
			String device;
			
		public:
			float x, y;
			
			PeerCard(const String&name, const String&id);
			~PeerCard();
			
			virtual void Update(long gameTime);
			virtual void Draw(Graphics2D&g, long gameTime);
			
			boolean isClicked();
			boolean wasClicked();
			boolean mouseOver();
			
			void setDisplayName(const String&name);
			void setPeerID(const String&id);
			void setDevice(const String&dev);
			String getDisplayName();
			String getPeerID();
			String getDevice();
		};
		
		PeerCard*selfCard;
		PeerCard*peerCard;
		
		Actor*brawl;
		
		boolean wasConnected;
		
	public:
		BluetoothMenu(const String&name);
		virtual ~BluetoothMenu();
		
		virtual void Initialize();
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};
	
	class P2PWaitScreen : public Screen
	{
	private:
		TextActor*message;
	public:
		P2PWaitScreen(const String&name);
		virtual ~P2PWaitScreen();
		
		virtual void LoadContent();
		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);
	};
}