#include <vector>

#include "Core/Window.hpp"
#include "Core/Texture.hpp"

#include "Utility/Random.hpp"



#define WINDOW_WIDTH_START (405) // Pixels
#define WINDOW_HEIGHT_START (570) // Pixels
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL)

#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

#define MAP_WIDTH 1000 // Tiles
#define MAP_HEIGHT 1000 // Tiles

#define ZOOM_MIN 5
#define ZOOM_MAX 145

#define MINIMAP_RADIUS 5 // Tiles
#define MINIMAP_WIDTH 100 // Pixels
#define MINIMAP_HEIGHT 100 // Pixels



void GenerateRoomSquare(std::vector<std::vector<uint8_t>>& room, const uint8_t maxSquareSize = 11) {
	Random random;
	const uint8_t squareSide = random.GetUInt32(3, 11);
	room.resize(squareSide, std::vector<uint8_t>(squareSide));

	for(uint8_t height = 0; height < squareSide; height++) {
		for(uint8_t width = 0; width < squareSide; width++) {
			room[height][width] = (height == 0 || width == 0 || height == squareSide - 1 || width == squareSide - 1);
		}
	}
}

void DrawRoomOnMap(std::vector<std::vector<uint16_t>>& map, std::vector<std::vector<uint8_t>>& room, const SDL_Point& offset = { 0, 0 }) {
	for(size_t height = 0; height < room.size(); height++) {
		for(size_t width = 0; width < room[height].size(); width++) {
			map[offset.x + height][offset.y + width] = room[height][width];
		}
	}
}



int main(int argc, char** argv) {
	Window window(
		"Pixel Dungeon",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH_START, WINDOW_HEIGHT_START,
		WINDOW_FLAGS, RENDERER_FLAGS
	);



	std::string pathBase(SDL_GetBasePath());
	pathBase += "assets\\";

	Texture texture;
	float textureWallZoom = 1.f;
	SDL_Point textureWallSize { 0, 0 };
	auto textureWall = texture.Load(window.GetRenderer(), pathBase + "wall.jpg");

	SDL_QueryTexture(textureWall, nullptr, nullptr, &textureWallSize.x, &textureWallSize.y);

	SDL_Point offsetMap { 0, 0 };

	bool isMouseButtonDown = false;
	SDL_Point mousePosOnButtonDown { 0, 0 };
	SDL_Point mousePosDiff { 0, 0 };

	Random random;
	std::vector<std::vector<uint16_t>> map(MAP_HEIGHT, std::vector<uint16_t>(MAP_WIDTH, 0));
	/////////////////////////////////////////////////////////////////////
	// Generate rooms
	const uint8_t roomsCount = 5;
	std::vector<std::vector<std::vector<uint8_t>>> rooms(roomsCount, std::vector<std::vector<uint8_t>>());
	for(size_t i = 0; i < roomsCount; i++) {
		GenerateRoomSquare(rooms[i]);
	}

	// Draw the rooms to the map
	uint16_t index = 0;
	for(size_t i = 0; i < roomsCount; i++) {
		DrawRoomOnMap(map, rooms[0], { index, 0 });
		index += (uint16_t)rooms[i].size() + 1;
	}
	/////////////////////////////////////////////////////////////////////

	SDL_Point windowSize { WINDOW_WIDTH_START, WINDOW_HEIGHT_START };
	SDL_Point playerPos { 5, 5 };



	bool isRunning = true;
	SDL_Event event {};

	while(isRunning) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					isRunning = false;

					break;
				case SDL_WINDOWEVENT:
					switch(event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
							windowSize = { event.window.data1, event.window.data2 };
							break;
					}

					break;
				case SDL_MOUSEBUTTONDOWN:
					isMouseButtonDown = true;
					SDL_GetMouseState(&mousePosOnButtonDown.x, &mousePosOnButtonDown.y);

					break;
				case SDL_MOUSEMOTION:
					if(isMouseButtonDown) {
						SDL_Point mousePosCurrent { 0, 0 };
						SDL_GetMouseState(&mousePosCurrent.x, &mousePosCurrent.y);

						mousePosDiff.x = -(mousePosOnButtonDown.x - mousePosCurrent.x);
						mousePosDiff.y = -(mousePosOnButtonDown.y - mousePosCurrent.y);
					}

					break;
				case SDL_MOUSEBUTTONUP:
					isMouseButtonDown = false;

					offsetMap.x += mousePosDiff.x;
					offsetMap.y += mousePosDiff.y;
					mousePosOnButtonDown = { 0, 0 };
					mousePosDiff = { 0, 0 };

					break;

				case SDL_MOUSEWHEEL:
					if(event.wheel.y > 0 && (int)(textureWallZoom * 100.f) <= ZOOM_MAX) {
						textureWallZoom += 0.05f;
					} else if(event.wheel.y < 0 && (int)(textureWallZoom * 10.f) >= ZOOM_MIN) {
						textureWallZoom -= 0.05f;
					}

					break;
			}
		}

		SDL_RenderClear(window.GetRenderer());
		SDL_SetRenderDrawColor(window.GetRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);

		//Draw map
		for(size_t height = 0; height < MAP_HEIGHT; height++) {
			for(size_t width = 0; width < MAP_WIDTH; width++) {
				if(map[height][width]) {
					texture.Render(
						window.GetRenderer(),
						textureWall,
						{ (int)(width * textureWallSize.x * textureWallZoom) + offsetMap.x + mousePosDiff.x,
						(int)(height * textureWallSize.y * textureWallZoom) + offsetMap.y + mousePosDiff.y },
						{ textureWallZoom, textureWallZoom }
					);
				}
			}
		}

		//Draw minimap
		SDL_Point tileSize { MINIMAP_WIDTH / (MINIMAP_RADIUS * 2 + 1), MINIMAP_HEIGHT / (MINIMAP_RADIUS * 2 + 1) };
		SDL_FPoint textureWallSizeDiff { textureWallSize.x / (MINIMAP_RADIUS * 2 + 1) / 20.f, textureWallSize.y / (MINIMAP_RADIUS * 2 + 1) / 20.f };
		for(size_t height = 0; height <= MINIMAP_RADIUS * 2; height++) {
			for(size_t width = 0; width <= MINIMAP_RADIUS * 2; width++) {
				if(map[height][width]) {
					texture.Render(
						window.GetRenderer(),
						textureWall,
						{ windowSize.x - MINIMAP_WIDTH + (int)width * tileSize.x,
						windowSize.y - MINIMAP_HEIGHT + (int)height * tileSize.y },
						{ textureWallSizeDiff.x, textureWallSizeDiff.y }
					);
				}
			}
		}

		SDL_RenderPresent(window.GetRenderer());
		SDL_SetRenderDrawColor(window.GetRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
	}



	texture.Remove(textureWall);
	IMG_Quit();

	return EXIT_SUCCESS;
}

/*
	TO DO:
		- the zoom should be centered on the player
*/