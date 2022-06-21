#pragma once

#include <SDL_ttf.h>

#include <string>
#include <map>

#include "Utility/Miscellaneous.hpp"

class Text {
public:
	Text();
	~Text();

	inline TTF_Font* Load(const std::string& path, const uint8_t fontSize) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN_X(m_isInitialized, "SDL_TTF", nullptr);
		if(!Miscellaneous::FileExists(path)) {
			Miscellaneous::PrintError("The font's path '" + path + "' is invalid!");
			return nullptr;
		}

		if(m_fonts.find(path) != m_fonts.end()) {
			return m_fonts[path];
		}

		TTF_Font* newFont = TTF_OpenFont(path.c_str(), fontSize);
		if(!newFont) {
			Miscellaneous::PrintError("The font '" + path + "' could not be opened! TTF_GetError: " + TTF_GetError() + ".");
		} else {
			m_fonts[path] = newFont;
		}

		return newFont;
	}

	inline void Render(
		SDL_Renderer* renderer,
		TTF_Font* font,
		const std::string& text,
		const SDL_Point& position,
		const float size = 1.f,
		const SDL_Color& color = { 255, 255, 255, 255 },
		const SDL_Rect& textureRect = { -1, -1, -1, -1 }
	) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_TTF");
		CHECK_AND_RETURN(renderer);
		CHECK_AND_RETURN(font);

		std::string fontName("");
		for(auto& item : m_fonts) {
			if(item.second == font) {
				fontName = item.first;
				break;
			}
		}

		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		if(!surface) {
			Miscellaneous::PrintError("The surface with the font '" + fontName + "' could not be created! TTF_GetError: " + TTF_GetError() + ".");
		} else {
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			if(!texture) {
				Miscellaneous::PrintError("The texture of the surface with the font '" + fontName + "' could not be created! TTF_GetError: " + TTF_GetError() + ".");
			} else {
				SDL_Rect textureRectFinal = {
					position.x, position.y,
					(int)(surface->w * size), (int)(surface->h * size)
				};

				if(textureRect.x == -1 && textureRect.y == -1 &&
				   textureRect.w == -1 && textureRect.h == -1) {
					SDL_RenderCopy(renderer, texture, nullptr, &textureRectFinal);
				} else {
					SDL_RenderCopy(renderer, texture, &textureRect, &textureRectFinal);
				}

				SDL_DestroyTexture(texture);
			}

			SDL_FreeSurface(surface);
		}
	}

	inline void Remove(TTF_Font* font) {
		CHECK_IF_IS_INITIALIZED_AND_RETURN(m_isInitialized, "SDL_TTF");
		CHECK_AND_RETURN(font);

		for(auto& item : m_fonts) {
			if(item.second == font) {
				m_fonts.erase(item.first);
				break;
			}
		}

		TTF_CloseFont(font);
	}

private:
	static uint8_t m_objectCounter;
	static bool m_isInitialized;

	std::map<std::string, TTF_Font*> m_fonts;
};