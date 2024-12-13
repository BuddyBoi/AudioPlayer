#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

//standalone single-headers
#include "webrequest.hpp"

//standard library
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <istream>
#include <chrono>
#include <experimental/filesystem>

//WinAPI imports
#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>
#include <playsoundapi.h>
#include <fileapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ShObjIdl.h>
#include <ShObjIdl_core.h>
#include <ObjIdl.h>
#include <ShlObj.h>
#include <sysinfoapi.h>
#include <VersionHelpers.h>
#include <WinBase.h>

//external libraries
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_mixer.lib")

//external headers
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_mixer.h>