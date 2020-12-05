#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Animation.h"
#include "Globals.h"
#include <map>
class SpriteSheet
{
private:
	SDL_Surface* mTexture;
	const float mCellSize;
	const int mCellCount;
	std::map<std::string,Animation*> anims;

public:
	SpriteSheet(SDL_Surface* nTexture, float nCellSize, int nCellCount);
	SDL_Surface* GetTexture();
	float GetCellSize();
	Animation* GetAnim(std::string name);
	void AddAnim(std::string name, Animation* anim);
};

