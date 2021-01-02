#include "StartButton.h"
#include "ManagerStruct.h"
#include "SceneManager.h"
StartButton::StartButton(std::string sprSheet) : Button(sprSheet)
{
	
}


bool StartButton::Update(double dTime, Act act, std::pair<int, int> mouse)
{
	if(act == Act::Click && InBounds(mouse.first, mouse.second))
		mgrs->SceneMgr->SetScene(1);

	if (act == Act::MouseUpdate && InBounds(mouse.first, mouse.second))
	{
		SetAnim("Hover");
	}
	else
		SetAnim("default");

	return true;
}