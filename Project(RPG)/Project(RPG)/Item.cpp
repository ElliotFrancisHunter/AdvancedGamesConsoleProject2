#include "Item.h"


Item::Item(std::string name, int cost) : mName(name), mCost(cost)
{
	
}

Item::~Item()
{
}

std::string Item::GetName() const
{
	return mName;
}

std::string Item::GetObjName() const
{
	return mObjName;
}

int Item::GetCost() const
{
	return mCost;
}

RenderObject* Item::GetRenderObject()
{
	return mRenderObject;
}

void Item::SetObjName(std::string name)
{
	mObjName = name;
}

void Item::SetRenderObject(RenderObject* obj)
{
	mRenderObject = obj;
}

void Item::OnHover()
{
	mRenderObject->tint = SDL_Color{ 0, 255 , 0 }; // Green
}

void Item::OnLeave()
{
	mRenderObject->tint = SDL_Color{ 255, 255 , 255 }; // White
}