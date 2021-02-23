#pragma once
#include "Scene.h"

struct characterInventory
{
	characterInventory(Character* character, RenderObject* portrait, RenderObject* weaponSlot, RenderObject* armourSlot) :portrait(portrait), weaponSlot(weaponSlot), armourSlot(armourSlot), character(character)
	{

	}
	Character* character;
	RenderObject* portrait;
	RenderObject* weaponSlot;
	RenderObject* armourSlot;

};

class InventoryScene :
	public Scene
{
public:
	InventoryScene(Interface* mgr);
	~InventoryScene();
	void Load();
protected:
	void Update(double dTime, Act act, std::pair<int, int> mousePos) override;
private:
	void Init();
	void DrawGrid(int offsetX, int offsetY, int gridBoundsX);
private:
	RenderObject* pCloseBtn;
	Mix_Chunk* button_SFX;
	Mix_Chunk* pDrink_SFX;
	ToolTip mToolTip;
	std::vector< std::pair<RenderObject*, ItemObject*>> playerInvGrid;
	std::vector<ItemObject> itemObjects;
	std::vector<characterInventory> characters;
	std::vector<Character*> mParty;
	// temp
	int it = 1;
	int pos;
	bool bHasitemPickedUp = false;
	std::vector<RenderObject*>::iterator freePos;
};

