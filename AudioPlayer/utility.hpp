#pragma once
#include "include.hpp"

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

namespace files
{
	std::vector<std::string> get_files_in_dir( std::string s_path )
	{
		std::vector<std::string> vec_files{};

		if ( !std::experimental::filesystem::is_directory( s_path ) )
		{
			lg( "files_get: failed to find dir path" );
			return std::vector<std::string>{};
		}

		for ( const auto& entry : std::experimental::filesystem::directory_iterator( s_path ) )
		{
			std::string s_path_name = entry.path( ).string( );
			auto path = entry.path( );
			vec_files.push_back( s_path_name );
		}

		return vec_files;
	}
}