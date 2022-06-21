#include "Minimap.hpp"

Minimap::Minimap() {

}

Minimap::~Minimap() {

}

void Minimap::Draw(
	SDL_Renderer* renderer,
	const std::vector<std::vector<uint16_t>>& map,
	SDL_Texture* textureWall,
	const SDL_Point& textureWallSize,
	const SDL_Point& windowSize
) {
	SDL_FPoint textureWallSizeDiff {
		textureWallSize.x / (MINIMAP_RADIUS * 2 + 1) / 20.f,
		textureWallSize.y / (MINIMAP_RADIUS * 2 + 1) / 20.f
	};

	for(size_t height = 0; height <= MINIMAP_RADIUS * 2; height++) {
		for(size_t width = 0; width <= MINIMAP_RADIUS * 2; width++) {
			if(map[height][width]) {
				Texture::Render(
					renderer,
					textureWall,
					{ windowSize.x - MINIMAP_WIDTH + (int)width * tileSize.x,
					windowSize.y - MINIMAP_HEIGHT + (int)height * tileSize.y },
					{ textureWallSizeDiff.x, textureWallSizeDiff.y }
				);
			}
		}
	}
}