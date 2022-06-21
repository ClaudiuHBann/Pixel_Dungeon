#include <vector>

#include "Core/Window.hpp"
#include "Core/Texture.hpp"

#include "Utility/Random.hpp"

#include "Map.hpp"
#include "Minimap.hpp"



#define WINDOW_WIDTH_START (405) // Pixels
#define WINDOW_HEIGHT_START (570) // Pixels
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL)

#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

#define ZOOM_MIN 5
#define ZOOM_MAX 145


// Allocates space for 'room' and creates a random sized square
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

// Draws a 'room' on the 'mapLow' the the specified 'offset'
void DrawRoomOnMap(std::vector<std::vector<uint16_t>>& mapLow, std::vector<std::vector<uint8_t>>& room, const SDL_Point& offset = { 0, 0 }) {
	for(size_t height = 0; height < room.size(); height++) {
		for(size_t width = 0; width < room[height].size(); width++) {
			mapLow[offset.x + height][offset.y + width] = room[height][width];
		}
	}
}



int main(int argc, char** argv) {
	// Window
	Window window(
		"Pixel Dungeon",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH_START, WINDOW_HEIGHT_START,
		WINDOW_FLAGS, RENDERER_FLAGS
	);
	SDL_Point windowSize { WINDOW_WIDTH_START, WINDOW_HEIGHT_START };



	// Create the path to the assets folder
	std::string pathBase(SDL_GetBasePath());
	pathBase += "assets\\";



	// Wall texture
	float textureWallZoom = 1.f;
	SDL_Point textureWallSize { 0, 0 };

	Texture texture;
	auto textureWall = texture.Load(window.GetRenderer(), pathBase + "wall.jpg");
	SDL_QueryTexture(textureWall, nullptr, nullptr, &textureWallSize.x, &textureWallSize.y);



	// Map
	Map map;
	SDL_Point offsetMap { 0, 0 };
	std::vector<std::vector<uint16_t>> mapLow(MAP_HEIGHT, std::vector<uint16_t>(MAP_WIDTH, 0));



	// Minimap
	Minimap minimap;



	// Mouse
	bool isMouseButtonDown = false;
	SDL_Point mousePosOnButtonDown { 0, 0 };
	SDL_Point mousePosDiff { 0, 0 };



	// Create some rooms and draw them on the mapLow
	const uint8_t roomsCount = 10;
	std::vector<std::vector<std::vector<uint8_t>>> rooms(roomsCount, std::vector<std::vector<uint8_t>>());
	for(size_t i = 0; i < roomsCount; i++) {
		GenerateRoomSquare(rooms[i]);
	}

	uint16_t index = 0;
	for(size_t i = 0; i < roomsCount; i++) {
		DrawRoomOnMap(mapLow, rooms[i], { index, 0 });
		index += (uint16_t)rooms[i].size() + 1;
	}



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

		// Clear the renderer and set the drawing color to white
		SDL_RenderClear(window.GetRenderer());
		SDL_SetRenderDrawColor(window.GetRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);

		// Draw the map and minimap
		map.Draw(window.GetRenderer(), mapLow, textureWall, textureWallSize, textureWallZoom, offsetMap, mousePosDiff);
		minimap.Draw(window.GetRenderer(), mapLow, textureWall, textureWallSize, windowSize);

		// Present the renderer and set the drawing color to black
		SDL_RenderPresent(window.GetRenderer());
		SDL_SetRenderDrawColor(window.GetRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
	}

	return EXIT_SUCCESS;
}

/*
	TO DO:
		- the zoom should be centered on the player
*/