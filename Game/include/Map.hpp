#pragma once

#include <SDL.h>

#include <vector>

#include "Core/Texture.hpp"

#define MAP_WIDTH 1000 // Tiles
#define MAP_HEIGHT 1000 // Tiles

class Map {
public:
	Map();
	~Map();

	void Draw(
		SDL_Renderer* renderer,
		const std::vector<std::vector<uint16_t>>& map,
		SDL_Texture* textureWall,
		const SDL_Point& textureWallSize,
		const float textureWallZoom,
		const SDL_Point& offsetMap,
		const SDL_Point& mousePosDiff
	);
};