#include "Core/Text.hpp"

uint8_t Text::m_objectCounter = 0;
bool Text::m_isInitialized = false;

Text::Text() {
	m_objectCounter++;

	if(!m_isInitialized) {
		if(TTF_Init()) {
			Miscellaneous::PrintError("SDL_TTF could not be initialized! TTF_GetError: " + std::string(TTF_GetError()) + ".");
		} else {
			m_isInitialized = true;
		}
	}
}

Text::~Text() {
	for(auto& font : m_fonts) {
		if(font.second) {
			TTF_CloseFont(font.second);
		}
	}

	if(!--m_objectCounter && m_isInitialized) {
		TTF_Quit();
	}
}