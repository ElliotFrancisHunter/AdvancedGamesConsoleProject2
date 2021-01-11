#include "ShopScene.h"



ShopScene::ShopScene(Interface* rng) : Scene(rng)
{
	bg_Music = Mix_LoadMUS("Assets/Music/MedievalLoop.mp3");
	buySell_SFX = Mix_LoadWAV("Assets/SFX/coin.wav");

	Mix_Volume(1, 5);

	mShop.SetGold(2000);

	Init();
	partyViewer = AddObject("StartBtnObj", 400, 500, UI);
}

void ShopScene::Update(double dTime, Act act, std::pair<int, int> mousePos)
{
	// Uncomment to hear the shop music, currently disabled as it was getting annoying while testing  - JP
	/*if (!startOnce)
	{
		mgr->GetManagers()->AudioMgr->PlayMusic(bg_Music, -1);
		Mix_VolumeMusic(10);
		startOnce = true;
	}*/

	if (act == Act::Click && partyViewer->InBounds(mousePos.first, mousePos.second))
		mgr->LoadScene(Party);

	//ManagePlayerInventory(mPlayer.GetInventory(), act, mousePos);
	ManagePlayerInventory(mgr->GetPlayer()->GetInventory(), act, mousePos);
	ManageShopInventory(mShop.GetInventory(), act, mousePos);
}

void ShopScene::PlaceItems(Inventory& inv)
{
	for (auto i : inv.GetContents())
	{
		// Display item to screen and set its render object to the correct image
		i->SetRenderObject(AddObject(i->GetObjName(), i->inventoryPos.pos.first, i->inventoryPos.pos.second, Game)); 
	}
}

void ShopScene::Init()
{
	AddObject("ShopBGObj", 1280 / 2, 720 / 2, Background);
	AddObject("playerPortraitObj", 505, 225, UI); // This needs to load the player's portrait in the future - JP
	AddObject("merchantPortraitObj", 725, 225, UI);

	
	GenerateGrids();

	
	mPlayerGoldText.text = "Gold: " + std::to_string( mgr->GetPlayer()->GetGold());
	mPlayerGoldText.pos = std::make_pair(520, 385);
	mPlayerGoldText.textColor = SDL_Color{ 255, 215, 0 }; // Gold

	mShopGoldText.text = "Gold: " + std::to_string(mShop.GetGold());
	mShopGoldText.pos = std::make_pair(620, 385);
	mShopGoldText.textColor = SDL_Color{ 255, 215, 0 }; // Gold
	
	mSceneText.push_back(mPlayerGoldText);
	mSceneText.push_back( mShopGoldText);

}

void ShopScene::SetupShopInv()
{
	Weapon* longSword = new Weapon("Long Sword", 50);
	Weapon* shortSword = new Weapon("Short Sword", 100);
	Weapon* greatSword = new Weapon("Great Sword", 500);
	Consumable* healthPotion = new Consumable("health potion", 25);
	Consumable* healthPotion2 = new Consumable("health potion", 25);

	mShop.GetInventory().AddItem(longSword);
	mShop.GetInventory().AddItem(shortSword);
	mShop.GetInventory().AddItem(greatSword);
	mShop.GetInventory().AddItem(healthPotion2);

}

//void ShopScene::SetupPlayerInv()
//{
//	Weapon* bigSword = new Weapon("Big Sword", 50);
//	bigSword->SetAtkPower(10);
//
//	Weapon* twitchSword = new Weapon("Twitch Sword", 100);
//	twitchSword->SetAtkPower(100);
//
//	Weapon* massiveSword = new Weapon("Massive Sword", 500);
//	massiveSword->SetAtkPower(500);
//
//	Armour* plateArmour = new Armour("Plate Armour", 150);
//	Armour* platArmour = new Armour("Plat Armour", 150);
//	
//	mPlayer.GetInventory().AddItem(bigSword);
//	mPlayer.GetInventory().AddItem(twitchSword);
//	mPlayer.GetInventory().AddItem(massiveSword);
//	mPlayer.GetInventory().AddItem(plateArmour);
//	mPlayer.GetInventory().AddItem(platArmour);
//
//}


void ShopScene::ManageShopInventory(Inventory& inv, Act act, std::pair<int, int> mousePos)
{

	for (Item* i : inv.GetContents())
	{
		// check if mouse is hovering over it
		if (act == Act::MouseUpdate )
		{
			if (i->GetRenderObject()->InBounds(mousePos.first, mousePos.second))
				i->OnHover();
			else
				i->OnLeave();

		}
		
		if (act == Act::RClick && i->GetRenderObject()->InBounds(mousePos.first, mousePos.second))
		{
			if (!(mgr->GetPlayer()->GetGold() < i->GetCost())) // If player can't afford item they can't buy it
			{
				mShop.BuyItem(i);
				mgr->GetPlayer()->SetGold(-i->GetCost());	// Remove cost of item from player's gold
				mgr->GetPlayer()->GetInventory().AddItem(i);// Add bought item to player's inventory
				mgr->PlaySFX(buySell_SFX, 0, 1);
				i->GetRenderObject()->SetPos(i->inventoryPos.pos);
				mSceneText[0].text = "Gold: " + std::to_string(mgr->GetPlayer()->GetGold());; // Display player gold
				mSceneText[1].text = "Gold: " + std::to_string(mShop.GetGold());;	// Display shop gold
			}

		}
	}
	
}

void ShopScene::ManagePlayerInventory(Inventory& inv, Act act, std::pair<int, int> mousePos)
{
	// for each item in player's inventory on screen
	for (Item* i : inv.GetContents())
	{
		// check if mouse is hovering over it
		if (act == Act::MouseUpdate)
		{
			if (i->GetRenderObject()->InBounds(mousePos.first, mousePos.second))
				i->OnHover();
			else
				i->OnLeave();
			
		}
		
		if (act == Act::RClick && i->GetRenderObject()->InBounds(mousePos.first, mousePos.second))
		{
			if (!(mShop.GetGold() < i->GetCost())) // Can only sell to the shop if the shop can give you the moeny for the item
			{
				mgr->GetPlayer()->SellItem(i);	 // Remove item from player's inventory and add to its gold
				mShop.SetGold(-i->GetCost());	 // Remove cost of sold item from shop gold
				mShop.GetInventory().AddItem(i); // Add sold item to shop inventory
				mgr->PlaySFX(buySell_SFX, 0, 1); // Play buy sfx on channel 1 and don't loop
				i->GetRenderObject()->SetPos(i->inventoryPos.pos); // Update the render object position 
				// Update the gold text for both player and shop
				mSceneText[0].text = "Gold: " + std::to_string(mgr->GetPlayer()->GetGold());;
				mSceneText[1].text = "Gold: " + std::to_string(mShop.GetGold());;
			}
		}
	}

}

void ShopScene::GenerateGrids()
{
	mgr->GetPlayer()->GetInventory().SetInitialGridPos(80);
	mShop.GetInventory().SetInitialGridPos(880);

	mgr->GetPlayer()->GetInventory().GeneratePositions();
	mShop.GetInventory().GeneratePositions();

	//SetupPlayerInv();
	SetupShopInv();

	DrawGrid(4, 5, 80, 110); // Draw item frames for player inventory
	DrawGrid(4, 5, 880, 110); // Draw item frames for shop inventory

	PlaceItems(mgr->GetPlayer()->GetInventory());
	PlaceItems(mShop.GetInventory());
}


void ShopScene::DrawGrid(int gridWidth, int gridHeight, int offsetX, int offsetY)
{
	int cellAmount = gridWidth * gridHeight;
	int initialX = offsetX;
	for (int i = 0; i < cellAmount; ++i)
	{
		if (offsetX >= 360 + initialX)
		{
			offsetX = initialX;
			offsetY += 110;
		}
		AddObject("itemFrameObj", offsetX, offsetY, Background);
		offsetX += 90;
	}
}


