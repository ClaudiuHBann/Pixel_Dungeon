#pragma once

#include <SDL_mixer.h>

#include <string>
#include <map>

#include "Utility/Miscellaneous.hpp"

#define MIX_INIT_EVERYTHING (MIX_INIT_FLAC | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_OPUS)

class Mixer {
public:
	Mixer();
	~Mixer();

	inline Mix_Chunk* LoadChunk(const std::string& path) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN_X(m_isInitialized, "SDL_mixer", nullptr);

		if(!Miscellaneous::FileExists(path)) {
			Miscellaneous::PrintError("The chunk's path '" + path + "' is invalid!");
			return nullptr;
		}

		if(m_chunks.find(path) != m_chunks.end()) {
			return m_chunks[path];
		}

		Mix_Chunk* newChunk = Mix_LoadWAV(path.c_str());
		if(!newChunk) {
			Miscellaneous::PrintError("The chunk '" + path + "' could not be loaded. Mix_GetError: " + std::string(Mix_GetError()));
		} else {
			m_chunks[path] = newChunk;
		}

		return newChunk;
	}

	inline void PlayChunk(Mix_Chunk* chunk) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");
		CHECK_AND_RETURN(chunk);

		Mix_PlayChannel(-1, chunk, 0);
	}

	inline void SetVolumeChunk(Mix_Chunk* chunk, const uint8_t volume) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");
		CHECK_AND_RETURN(chunk);

		Mix_VolumeChunk(chunk, volume);
	}

	inline Mix_Chunk* GetChunk(const std::string& path) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN_X(m_isInitialized, "SDL_mixer", nullptr);

		if(!Miscellaneous::FileExists(path)) {
			Miscellaneous::PrintError("The chunk's path '" + path + "' is invalid!");
			return nullptr;
		}

		return ((m_chunks.find(path) != m_chunks.end()) ? m_chunks[path] : nullptr);
	}

	inline void Remove(Mix_Chunk* chunk) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");
		CHECK_AND_RETURN(chunk);

		for(auto& item : m_chunks) {
			if(chunk == item.second) {
				m_chunks.erase(item.first);
				break;
			}
		}

		Mix_FreeChunk(chunk);
	}

	inline Mix_Music* LoadMusic(const std::string& path) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");

		if(!Miscellaneous::FileExists(path)) {
			Miscellaneous::PrintError("The music's path '" + path + "' is invalid!");
			return nullptr;
		}

		if(m_musics.find(path) != m_musics.end()) {
			return m_musics[path];
		}

		Mix_Music* newMusic = Mix_LoadMUS(path.c_str());
		if(!newMusic) {
			Miscellaneous::PrintError("The music '" + path + "' could not be loaded. Mix_GetError: " + std::string(Mix_GetError()));
		} else {
			m_musics[path] = newMusic;
		}

		return newMusic;
	}

	inline void PlayMusic(Mix_Music* music, const bool loop = false) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");
		CHECK_AND_RETURN(music);

		Mix_PlayMusic(music, (loop ? -1 : 0));
	}

	inline void SetVolumeMusic(const uint8_t volume) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");

		Mix_VolumeMusic(volume);
	}

	inline void PauseMusic() {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");

		Mix_PauseMusic();
	}

	inline Mix_Music* GetMusic(const std::string& path) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN_X(m_isInitialized, "SDL_mixer", nullptr);

		if(!Miscellaneous::FileExists(path)) {
			Miscellaneous::PrintError("The music's path '" + path + "' is invalid!");
			return nullptr;
		}

		return ((m_musics.find(path) != m_musics.end()) ? m_musics[path] : nullptr);
	}

	inline void Remove(Mix_Music* music) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_mixer");
		CHECK_AND_RETURN(music);

		for(auto& item : m_musics) {
			if(music == item.second) {
				m_musics.erase(item.first);
				break;
			}
		}

		Mix_FreeMusic(music);
	}

private:
	static bool m_isInitialized;
	static uint8_t m_objectCounter;

	std::map<std::string, Mix_Chunk*> m_chunks;
	std::map<std::string, Mix_Music*> m_musics;
};