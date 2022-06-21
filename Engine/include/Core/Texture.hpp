#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <map>

#include "Utility/Miscellaneous.hpp"

#define IMG_INIT_EVERYTHING (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP)

class Texture {
public:
	Texture();
	~Texture();

	static inline void Render(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Point& position, const SDL_FPoint& size = { 1.f, 1.f }, const SDL_Rect& textureRect = { -1, -1, -1, -1 }) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_image");
		CHECK_AND_RETURN(renderer);
		CHECK_AND_RETURN(texture);

		SDL_Rect textureRectFinal = { position.x, position.y, 0, 0 };
		SDL_QueryTexture(texture, nullptr, nullptr, &textureRectFinal.w, &textureRectFinal.h);

		textureRectFinal.w = (int)(textureRectFinal.w * size.x);
		textureRectFinal.h = (int)(textureRectFinal.h * size.y);

		if(textureRect.x == -1 && textureRect.y == -1 &&
		   textureRect.w == -1 && textureRect.h == -1) {
			SDL_RenderCopy(renderer, texture, nullptr, &textureRectFinal);
		} else {
			SDL_RenderCopy(renderer, texture, &textureRect, &textureRectFinal);
		}
	}

	inline SDL_Texture* Load(SDL_Renderer* renderer, const std::string& path) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN_X(m_isInitialized, "SDL_image", nullptr);
		CHECK_AND_RETURN_X(renderer, nullptr);
		if(!Miscellaneous::FileExists(path)) {
			Miscellaneous::PrintError("The texture's path '" + path + "' is invalid!");
			return nullptr;
		}

		if(m_textures.find(path) != m_textures.end()) {
			return m_textures[path];
		}

		SDL_Texture* newTexture = nullptr;

		SDL_Surface* surface = IMG_Load(path.c_str());
		if(!surface) {
			Miscellaneous::PrintError("The surface for the texture '" + path + "' could not be loaded! IMG_GetError: " + IMG_GetError() + ".");
		} else {
			newTexture = SDL_CreateTextureFromSurface(renderer, surface);
			if(!newTexture) {
				Miscellaneous::PrintError("The texture '" + path + "' could not be created! IMG_GetError: " + IMG_GetError() + ".");
			} else {
				m_textures[path] = newTexture;
			}

			SDL_FreeSurface(surface);
		}

		return newTexture;
	}

	inline void Remove(SDL_Texture* texture) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_image");
		CHECK_AND_RETURN(texture);

		for(auto& item : m_textures) {
			if(texture == item.second) {
				m_textures.erase(item.first);
				break;
			}
		}

		SDL_DestroyTexture(texture);
	}

	inline SDL_Texture* GetTexture(const std::string& path) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN_X(m_isInitialized, "SDL_image", nullptr);

		if(!Miscellaneous::FileExists(path)) {
			Miscellaneous::PrintError("The texture's path '" + path + "' is invalid!");
			return nullptr;
		}

		return ((m_textures.find(path) != m_textures.end()) ? m_textures[path] : nullptr);
	}

private:
	static bool m_isInitialized;
	static uint8_t m_objectCounter;

	std::map<std::string, SDL_Texture*> m_textures;
};