#include "Core/Texture.hpp"

bool Texture::m_isInitialized = false;
uint8_t Texture::m_objectCounter = 0;

Texture::Texture() {
	m_objectCounter++;

	if(!m_isInitialized) {
		if(!IMG_Init(IMG_INIT_EVERYTHING)) {
			Miscellaneous::PrintError("SDL_image could not be initialized! IMG_GetError: " + std::string(IMG_GetError()) + ".");
		} else {
			m_isInitialized = true;
		}
	}
}

Texture::~Texture() {
	for(auto& item : m_textures) {
		if(item.second) {
			SDL_DestroyTexture(item.second);
		}
	}

	if(!--m_objectCounter && m_isInitialized) {
		IMG_Quit();
	}
}