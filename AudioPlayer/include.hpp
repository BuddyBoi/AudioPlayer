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
#pragma comment(lib, "C:\\Users\\buddy\\Documents\\Visual Studio Libraries\\SDL2\\lib\\x64\\SDL2.lib")
#pragma comment(lib, "C:\\Users\\buddy\\Documents\\Visual Studio Libraries\\SDL2\\lib\\x64\\SDL2main.lib")
#pragma comment(lib, "C:\\Users\\buddy\\Documents\\Visual Studio Libraries\\SDL2_MIXER\\lib\\x64\\SDL2_mixer.lib")

//external headers
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_mixer.h>

std::string server_read( std::string page )
{
	std::string domain = "http://155.254.26.213";

	httplib::Client cli( domain.c_str() );

	auto res = cli.Get( page.c_str() );

	if ( res )
	{
		return res->body;
	}
	else
	{
		return "error";
	}
}

void lg( std::string s )
{
	std::cout << "[LOG] " << s << std::endl;
}

std::vector<uint8_t> string_to_buffer( std::string s_str )
{
	return std::vector<std::uint8_t>( std::begin( s_str ), std::end( s_str ) );
}