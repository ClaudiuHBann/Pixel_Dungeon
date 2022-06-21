#include "Core/Window.hpp"

bool Window::m_isInitialized = false;
uint8_t Window::m_objectCounter = 0;

Window::Window() {
	m_objectCounter++;

	if(!m_isInitialized) {
		m_isInitialized = InitializeVideoAndEvents();
	}
}

Window::Window(
	const std::string& title,
	const uint32_t x, const uint32_t y,
	const uint16_t w, const uint16_t h,
	const uint32_t flagWindow, const uint32_t flagRenderer
) {
	m_objectCounter++;

	if(!m_isInitialized) {
		m_isInitialized = InitializeVideoAndEvents();
	}

	SetWindowAndRenderer(title, x, y, w, h, flagWindow, flagRenderer);
}

Window::~Window() {
	if(m_renderer) {
		SDL_DestroyRenderer(m_renderer);
	}

	if(m_window) {
		SDL_DestroyWindow(m_window);
	}

	if(!--m_objectCounter && m_isInitialized) {
		SDL_Quit();
	}
}