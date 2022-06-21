#pragma once

#define GET_LOCATION "file '" << __FILE__ << "' in function '" << __func__ << "' at line '" << __LINE__ << '\''
#define PRINT_ERROR_LOCATION std::cerr << std::endl << std::endl << " An error occured in " << GET_LOCATION << '.' << std::endl << std::endl;

#define PARAMETER_FROM_IS_NULL(p) std::string(std::string(typeid(p).name()) + " '" + #p + "' parameter from method '" + __func__ + "' call is null!")
#define CHECK_AND_RETURN(p) \
if(!p) { \
	Miscellaneous::PrintError(PARAMETER_FROM_IS_NULL(p)); \
	return; \
}
#define CHECK_AND_RETURN_X(p, x) \
if(!p) { \
	Miscellaneous::PrintError(PARAMETER_FROM_IS_NULL(p)); \
	return x; \
}

#define CHECK_IF_IS_INITIALIZED_AND_RETURN(isInitialized, subsystem) \
if (!isInitialized) { \
	Miscellaneous::PrintError("The subsystem(s) '" + std::string(subsystem) + "' is(are) not initialized!"); \
	return; \
}
#define CHECK_IF_IS_INITIALIZED_AND_RETURN_X(isInitialized, subsystem, x) \
if (!isInitialized) { \
	Miscellaneous::PrintError("The subsystem(s) '" + std::string(subsystem) + "' is(are) not initialized!"); \
	return x; \
}

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <iostream>
#include <string>

typedef enum {
#ifdef _WIN32
	FG_BLUE = 1,
	FG_GREEN = 2,
	FG_RED = 4,
	FG_INTENSITY = 8,
	BG_BLUE = 16,
	BG_GREEN = 32,
	BG_RED = 64,
	BG_INTENSITY = 128,
#elif defined(__linux__)
	FG_BLACK = 30,
	FG_RED,
	FG_GREEN,
	FG_YELLOW,
	FG_BLUE,
	FG_MAGENTA,
	FG_CYAN,
	FG_WHITE,
	FG_BRIGHT_BLACK = 90,
	FG_BRIGHT_RED,
	FG_BRIGHT_GREEN,
	FG_BRIGHT_YELLOW,
	FG_BRIGHT_BLUE,
	FG_BRIGHT_MAGENTA,
	FG_BRIGHT_CYAN,
	FG_BRIGHT_WHITE,
	BG_BLACK = 40,
	BG_RED,
	BG_GREEN,
	BG_YELLOW,
	BG_BLUE,
	BG_MAGENTA,
	BG_CYAN,
	BG_WHITE,
	BG_BRIGHT_BLACK = 100,
	BG_BRIGHT_RED,
	BG_BRIGHT_GREEN,
	BG_BRIGHT_YELLOW,
	BG_BRIGHT_BLUE,
	BG_BRIGHT_MAGENTA,
	BG_BRIGHT_CYAN,
	BG_BRIGHT_WHITE,
#endif
} ConsoleColor;

class Miscellaneous {
public:
	Miscellaneous();
	~Miscellaneous();

	static inline bool FileExists(const std::string& file) {
		struct stat info;
		return !stat(file.c_str(), &info);
	}

	static inline void PrintError(const std::string& error, const uint16_t color = ConsoleColor::FG_RED
#ifdef _WIN32
								  , HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE)
#endif // _WIN32
	) {
#if _DEBUG
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(consoleHandle, &csbi);

		SetConsoleTextAttribute(consoleHandle, color);
		std::cerr << std::endl << " " << error << std::endl;
		SetConsoleTextAttribute(consoleHandle, csbi.wAttributes);
#elif defined(__linux__)
		std::cerr << std::endl << " \033[" << color << "m" << error << "\033[0m" << std::endl;
#endif // OS
#else
		SDL_SetError(error.c_str());
#endif // _DEBUG
	}
};