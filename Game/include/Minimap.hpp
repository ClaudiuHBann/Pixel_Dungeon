#pragma once

#include <SDL.h>

#include <vector>

#include "Core/Texture.hpp"

#define MINIMAP_RADIUS 5 // Tiles
#define MINIMAP_WIDTH 100 // Pixels
#define MINIMAP_HEIGHT 100 // Pixels

class Minimap {
public:
	Minimap();
	~Minimap();

	void Draw(
		SDL_Renderer* renderer,
		const std::vector<std::vector<uint16_t>>& map,
		SDL_Texture* textureWall,
		const SDL_Point& textureWallSize,
		const SDL_Point& windowSize
	);

private:
	SDL_Point tileSize {
			MINIMAP_WIDTH / (MINIMAP_RADIUS * 2 + 1),
			MINIMAP_HEIGHT / (MINIMAP_RADIUS * 2 + 1)
	};
};