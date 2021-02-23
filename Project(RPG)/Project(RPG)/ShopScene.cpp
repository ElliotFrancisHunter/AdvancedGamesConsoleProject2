#include "ShopScene.h"
#include <random>
#include <chrono>

std::mt19937 random_number_engine(time(0));

ShopScene::ShopScene(Interface* rng) : Scene(rng)
{
	bg_Music = Mix_LoadMUS("Assets/Music/MedievalLoop.mp3");
	buySell_SFX = Mix_LoadWAV("Assets/SFX/coin.wav");
	pCantBuy_SFX = Mix_LoadWAV("Assets/SFX/ErrorSound.wav");
	leave_SFX = Mix_LoadWAV("Assets/SFX/DoorClose.wav");
	Init();
}

ShopScene::~ShopScene()
{
	delete bg_Music;
	bg_Music = nullptr;
	
	delete buySell_SFX;
	buySell_SFX = nullptr;
	
	delete button_Click_SFX;
	button_Click_SFX = nullptr;
	
	delete leave_SFX;
	leave_SFX = nullptr;
	
	delete pExitButton;
	pExitButton = nullptr;
	
	delete playerItemHovered;
	playerItemHovered = nullptr;
	
	delete shopItemHovered;
	shopItemHovered = nullptr;

	delete pCantBuy_SFX;
	pCantBuy_SFX = nullptr;
	
}

void ShopScene::Update(double dTime, Act act, std::pair<int, int> mousePos)
{
	if (act == Act::MouseUpdate)
	{
		if (pExitButton->InBounds(mousePos.first, mousePos.second))
		{
			pExitButton->Tint({ 0, 255, 0 });
		}
		else
		{
			pExitButton->Untint();
		}
	}

	if (act == Act::Click)
	{
		if (pExitButton->InBounds(mousePos.first, mousePos.second))
		{
			mgr->PlaySFX(leave_SFX, 0, 1);
			mgr->LoadPreviousScene();
			pExitButton->Untint();
		}
	}
	ManagePlayerInventory(mgr->GetPlayer()->inventory, act, mousePos);
	ManageShopInventory(mShop.inventory, act, mousePos);
}

void ShopScene::PlaceItems(std::vector<Item*> inv)
{
	for (auto i : mgr->GetPlayer()->inventory)
	{
		// Display item to screen and set its render object to the correct image
		playerInv.push_back(ItemObject(i, AddObject(i->GetObjName(), i->inventoryPos.pos.first, i->inventoryPos.pos.second, Game)));
		
	}

	for (auto i : mShop.inventory)
	{
		// Display item to screen and set its render object to the correct image
		shopInv.push_back(ItemObject(i, AddObject(i->GetObjName(), i->inventoryPos.pos.first, i->inventoryPos.pos.second, Game)));

	}
}

void ShopScene::Init()
{
	AddObject("ShopBGObj",640, 360, Background);
	
	AddObject("merchantPortraitObj", 725, 225, UI);
	pExitButton = AddObject("exitButtonObj", 120, 640, UI);


	UIText defaultText;
	defaultText.text = "This is default tooltip text";
	mTooltip = ToolTip(AddObject("defaultItemObj", 640, 650, UI),
		AddObject("toolTipBgObj", 640, 650, UI), defaultText, std::make_pair(600, 600));
	mTooltip.mDescription.scale = { 140, 100 };
	mTooltip.mDescription.bWrapped = true;

	playerToolTip = ToolTip(AddObject("defaultItemObj", 640, 650, UI),
		AddObject("toolTipBgObj", 640, 650, UI), defaultText, std::make_pair(600, 600));
	playerToolTip.mDescription.scale = { 140, 100 };
	playerToolTip.mDescription.bWrapped = true;


	GenerateGrids();
	
}

void ShopScene::Load()
{
	mHighestCharacter = mgr->GetPlayer()->GetParty()[0]->GetLevel();

	AddObject(mgr->GetPlayer()->GetParty().at(0)->GetPortraitName(), 505, 225, UI); // Now loads portrait of 1st in party. These need matching in scale to merchant - EH

	mSceneText.clear();

	mShop.SetGold(-mShop.GetGold());
	mShop.SetGold(RandomRange(300, 2000));

	mPlayerGoldText.text = "Gold: " + std::to_string(mgr->GetPlayer()->GetGold());
	mPlayerGoldText.pos = std::make_pair(550, 415);
	mPlayerGoldText.textColor = SDL_Color{ 255, 215, 0 }; // Gold

	mShopGoldText.text = "Gold: " + std::to_string(mShop.GetGold());
	mShopGoldText.pos = std::make_pair(680, 415);
	mShopGoldText.textColor = SDL_Color{ 255, 215, 0 }; // Gold
	
	mSceneText.push_back(&mPlayerGoldText);
	mSceneText.push_back(& mShopGoldText);
	mSceneText.push_back(&mTooltip.mDescription);
	mSceneText.push_back(&playerToolTip.mDescription);

	mgr->FadeInMusic(bg_Music, -1, mgr->fadeTime);

	mLayers[Game].clear();
	playerInv.clear();
	shopInv.clear();
	PlaceItems(mgr->GetPlayer()->inventory);
	std::cout << 't';
}

void ShopScene::SetupShopInv()
{
	std::vector<std::string> weaponStrings{"Dagger", "Short Sword", "Long Sword" };
	std::vector<std::string> ArmourStrings{"Cloth Armour", "Leather Armour", "Chainmail Armour" };
	for (int f = 0; f < 20; ++f)
	{
		int i = RandomRange(0, 99);
		int j = RandomRange(0, 2);
		if (i >= 0 && i <= 24)
		{
			mShop.AddItem(mgr->RequestItem(weaponStrings[RandomRange(0,2)]));
		}
		else if (i > 24 && i <= 49)
		{
			mShop.AddItem(mgr->RequestItem(ArmourStrings[j]));
		}
		else
		{
			mShop.AddItem(mgr->RequestItem("Health Potion"));
		}
	}


}


void ShopScene::ManageShopInventory(std::vector<Item*> inv, Act act, std::pair<int, int> mousePos)
{
	shopItemHovered = nullptr;
	// check if mouse is hovering over it
	if (act == Act::MouseUpdate)
	{
		for (auto& i : shopInv)
		{
			if (i.obj->InBounds(mousePos.first, mousePos.second))
			{
				shopItemHovered = &i;
				i.obj->tint = SDL_Color{ 0,255,0 };
			}
			else
				i.obj->tint = SDL_Color{ 255,255,255 };
		
		}
		if (shopItemHovered != nullptr)
		{
			//current->OnHover();
			mTooltip.pItemImage->SetTexture(shopItemHovered->obj->GetSheet());
			mTooltip.mDescription.text = shopItemHovered->_item->GetDescription();

			mTooltip.Show();
		}
		else
		{
			mTooltip.Hide();
			shopItemHovered = nullptr;
		}
	}
	
	for (ItemObject i : shopInv)
	{
		
		
		if (act == Act::RClick && i.obj->InBounds(mousePos.first, mousePos.second))
		{
			if (!(mgr->GetPlayer()->GetGold() < i._item->GetCost()) && i._item->GetLvlRequirement() <= mHighestCharacter) // If player can't afford item they can't buy it
			{
				mShop.SellItem(i._item);
				playerInv.push_back(i);
				shopInv.erase(std::remove_if(shopInv.begin(), shopInv.end(), [&i](ItemObject item_) {return item_._item == i._item; }));
				mgr->GetPlayer()->SetGold(-i._item->GetCost());	// Remove cost of item from player's gold
				mgr->GetPlayer()->AddItem(i._item);// Add bought item to player's inventory
				mgr->PlaySFX(buySell_SFX, 0, 1);
				i.obj->SetPos(i._item->inventoryPos.pos);
				mSceneText[0]->text = "Gold: " + std::to_string(mgr->GetPlayer()->GetGold());; // Display player gold
				mSceneText[1]->text = "Gold: " + std::to_string(mShop.GetGold());;	// Display shop gold
			}
			else
			{
				mgr->PlaySFX(pCantBuy_SFX, 0, 0);
			}

		}
	}
	
}

void ShopScene::ManagePlayerInventory(std::vector<Item*> inv, Act act, std::pair<int, int> mousePos)
{
	playerItemHovered = nullptr;
	if (act == Act::MouseUpdate)
	{
		for (auto& i : playerInv)
		{
			if (i.obj->InBounds(mousePos.first, mousePos.second))
			{
				playerItemHovered = &i;
				i.obj->tint = SDL_Color{ 0,255,0 };
			}
			else
				i.obj->tint = SDL_Color{ 255,255,255 };

		}
		if (playerItemHovered != nullptr)
		{
			playerToolTip.pItemImage->SetTexture(playerItemHovered->obj->GetSheet());
			playerToolTip.mDescription.text = playerItemHovered->_item->GetDescription();

			playerToolTip.Show();
		}
		else
		{
			playerToolTip.Hide();
		}
	}

	

	// for each item in player's inventory on screen
	for (ItemObject i : playerInv)
	{
		// check if mouse is hovering over it
		
		
		if (act == Act::RClick && i.obj->InBounds(mousePos.first, mousePos.second))
		{
			if (!(mShop.GetGold() < i._item->GetCost())) // Can only sell to the shop if the shop can give you the moeny for the item
			{
				mgr->GetPlayer()->SellItem(i._item);	 // Remove item from player's inventory and add to its gold
				shopInv.push_back(i);
				playerInv.erase(std::remove_if(playerInv.begin(), playerInv.end(), [&i](ItemObject nI) { return nI._item == i._item; })); // remove from local container as well
				mShop.SetGold(-i._item->GetCost());	 // Remove cost of sold item from shop gold
				mShop.AddItem(i._item); // Add sold item to shop inventory
				mgr->PlaySFX(buySell_SFX, 0, 1); // Play buy sfx on channel 1 and don't loop
				i.obj->SetPos(i._item->inventoryPos.pos); // Update the render object position 
				// Update the gold text for both player and shop
				mSceneText[0]->text = "Gold: " + std::to_string(mgr->GetPlayer()->GetGold());;
				mSceneText[1]->text = "Gold: " + std::to_string(mShop.GetGold());;
			}
		}
	}
}

void ShopScene::GenerateGrids()
{
	mShop.SetInitialGridPos(880);

	mShop.GeneratePositions(); 
	//mgr->GetPlayer()->GeneratePositions();
	SetupShopInv();

	DrawGrid(4, 5, 80, 110); // Draw item frames for player inventory
	DrawGrid(4, 5, 880, 110); // Draw item frames for shop inventory

	PlaceItems(mgr->GetPlayer()->inventory);
	PlaceItems(mShop.inventory);
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

void ShopScene::HandleTooltip(ItemObject* hovered)
{
	if (hovered != nullptr)
	{
		//current->OnHover();
		mTooltip.pItemImage->SetTexture(hovered->obj->GetSheet());
		mTooltip.mDescription.text = hovered->_item->GetDescription();

		mTooltip.Show();
	}
	else
	{
		mTooltip.Hide();
	
	}
}



int ShopScene::RandomRange(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(random_number_engine);
}


