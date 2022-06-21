#include "Map.hpp"

Map::Map() {

}

Map::~Map() {

}

void Map::Draw(
	SDL_Renderer* renderer,
	const std::vector<std::vector<uint16_t>>& map,
	SDL_Texture* textureWall,
	const SDL_Point& textureWallSize,
	const float textureWallZoom,
	const SDL_Point& offsetMap,
	const SDL_Point& mousePosDiff
) {
	for(size_t height = 0; height < MAP_HEIGHT; height++) {
		for(size_t width = 0; width < MAP_WIDTH; width++) {
			if(map[height][width]) {
				Texture::Render(
					renderer,
					textureWall,
					{ (int)(width * textureWallSize.x * textureWallZoom) + offsetMap.x + mousePosDiff.x,
					(int)(height * textureWallSize.y * textureWallZoom) + offsetMap.y + mousePosDiff.y },
					{ textureWallZoom, textureWallZoom }
				);
			}
		}
	}
}