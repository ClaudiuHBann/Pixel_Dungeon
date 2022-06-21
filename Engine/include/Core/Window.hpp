#pragma once

#include <SDL.h>

#include <string>

#include "Utility/Miscellaneous.hpp"

class Window {
public:
	Window();

	Window(
		const std::string& title,
		const uint32_t x, const uint32_t y,
		const uint16_t w, const uint16_t h,
		const uint32_t flagWindow, const uint32_t flagRenderer
	);

	~Window();

	inline void SetWindowAndRenderer(
		const std::string& title,
		const uint32_t x, const uint32_t y,
		const uint16_t w, const uint16_t h,
		const uint32_t flagWindow, const uint32_t flagRenderer
	) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL's VIDEO and EVENTS");

		if(m_isInitialized && _CreateWindow(title, x, y, w, h, flagWindow)) {
			CreateRenderer(-1, flagRenderer);
		}
	}

	inline SDL_Window* GetWindow() {
		return m_window;
	}

	inline SDL_Renderer* GetRenderer() {
		return m_renderer;
	}

private:
	inline bool _CreateWindow(
		const std::string& title,
		const uint32_t x, const uint32_t y,
		const uint16_t w, const uint16_t h,
		const uint32_t flags
	) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN_X(m_isInitialized, "SDL's VIDEO and EVENTS", false);

		m_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
		if(!m_window) {
			Miscellaneous::PrintError("Window '" + title + "' could not be created! SDL_GetError: " + std::string(SDL_GetError()) + ".");
			return false;
		}

		return true;
	}

	inline void CreateRenderer(int32_t index, const uint32_t flags) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL's VIDEO and EVENTS");

		m_renderer = SDL_CreateRenderer(m_window, index, flags);
		if(!m_renderer) {
			Miscellaneous::PrintError("Renderer for window '" + std::string(SDL_GetWindowTitle(m_window)) + "' could not be created! SDL_GetError: " + std::string(SDL_GetError()) + ".");
		}
	}

	inline bool InitializeVideoAndEvents() {
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
			Miscellaneous::PrintError("SDL's VIDEO and EVENTS could not be initialized! SDL_GetError: " + std::string(SDL_GetError()) + ".");
			return false;
		}

		return true;
	}

	static bool m_isInitialized;
	static uint8_t m_objectCounter;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
};