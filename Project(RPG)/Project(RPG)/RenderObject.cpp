#include "RenderObject.h"
#include "ManagerStruct.h"
RenderObject::RenderObject(std::string sprSheet)
{
	path = sprSheet;
	mCurrentAnim = nullptr;
	mVisible = true;
}

bool RenderObject::Update(double dTime, Act act, std::pair<int,int> mouse)
{
	return true;
}

bool RenderObject::Init(Managers* mgrs)
{
	this->mgrs = mgrs;
	mVisible = true;
	return true;
}

std::pair<int, int> RenderObject::GetPos()
{
	return mPos;
}

void RenderObject::SetAnim(std::string id)
{
	mCurrentAnim = mSheet->GetAnim(id);
}

void RenderObject::SetPos(std::pair<int, int> nPos)
{
	mPos = nPos;
}

void RenderObject::SetTexture(SpriteSheet* nSheet)
{
	mSheet = nSheet;
}

void RenderObject::SetVisible(const bool vis)
{
	mVisible = vis;
}

SpriteSheet* RenderObject::GetSheet()
{
	return mSheet;
}

bool RenderObject::IsVisible()
{
	return mVisible;
}

void RenderObject::Select()
{
	//std::cout << "Render object clicked";
}

bool RenderObject::InBounds(int x, int y)
{
	// currenlty this allows render objects to be pressed slightly outside of the sprites bounds - JP

	//return false if cant be selected
	std::pair<float, float> bound = GetSheet()->GetCellSize();
	bound.first = bound.first / 2 * scale;
	bound.second = bound.second / 2 * scale;
	//float bound = GetSheet()->GetCellSize().first;
	if (x > mPos.first - bound.first  && x < mPos.first + bound.first  )
		return (y >= mPos.second  - bound.second   && y <= mPos.second + bound.second  );
	else
		return false;
}

Animation* RenderObject::GetAnim()
{
	return mCurrentAnim;	
}