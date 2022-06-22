#include "Core/Mixer.hpp"

bool Mixer::m_isInitialized = false;
uint8_t Mixer::m_objectCounter = 0;

Mixer::Mixer() {
	m_objectCounter++;

	if(!m_isInitialized && !Mix_Init(MIX_INIT_EVERYTHING)) {
		Miscellaneous::PrintError("SDL_mixer could not be initialized. Mix_GetError: " + std::string(Mix_GetError()));
	} else {
		if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048)) {
			Miscellaneous::PrintError("The audio could not be opened. Mix_GetError: " + std::string(Mix_GetError()));
		} else {
			m_isInitialized = true;
		}
	}
}

Mixer::~Mixer() {
	for(auto& item : m_musics) {
		if(item.second) {
			Mix_FreeMusic(item.second);
		}
	}

	for(auto& item : m_chunks) {
		if(item.second) {
			Mix_FreeChunk(item.second);
		}
	}

	if(!--m_objectCounter && m_isInitialized) {
		Mix_CloseAudio();
		Mix_Quit();
	}
}