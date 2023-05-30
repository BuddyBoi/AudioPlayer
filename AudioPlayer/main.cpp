#include "include.hpp"
#include "audio.hpp"
#include "utility.hpp"

void music_processing_thread( )
{
	while ( true )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

		//handle queue
		{
			if ( !audio::is_track_selected( ) )
			{
				//handle song queue
				if ( audio::b_queued_songs )
				{
					if ( !audio::vec_queue_music.size( ) )
					{
						audio::i_currently_playing = -1;
						audio::b_queued_songs = false;
					}

					else if ( audio::vec_queue_music.size( ) == 1 )
					{
						audio::music_play( audio::vec_queue_music[0], false );
						audio::vec_queue_music.clear( );
					}

					else if ( audio::vec_queue_music.size( ) >= 2 )
					{
						audio::music_play( audio::vec_queue_music[0], false );

						//remove song from queue
						std::vector<int> new_vec_queue_music{};
						for ( int i = 1; i != audio::vec_queue_music.size( ); i++ )
						{
							//re-shuffle song queue
							new_vec_queue_music.push_back( audio::vec_queue_music[i] );
						}
						audio::vec_queue_music = new_vec_queue_music;
					}
				}
			}
		}
	}
}

void music_menu_thread( )
{
	if ( HANDLE h = CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE ) music_processing_thread, 0, 0, 0 ) )
		CloseHandle( h );

	while ( true )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
		system( "CLS" );

		std::cout << "---Buddy's Music Player---" << std::endl;
		std::cout << "PAUSED: " << audio::is_music_paused( ) << std::endl;
		std::cout << "TRACK_SELECTED: " << audio::is_track_selected( ) << std::endl;
		std::cout << "SELECTED SONG: " << audio::music_get_name( ) << std::endl;
		std::cout << "QUEUE SIZE: " << audio::vec_queue_music.size( ) << std::endl;
		if ( audio::vec_queue_music.size( ) != 0 )
			std::cout << "NEXT QUEUE SONG: " << audio::music_get_name( audio::vec_queue_music[0] ) << std::endl;
		if ( audio::is_track_selected( ) )
		{
			std::cout << "CURRENT SONG POS: " + std::to_string( audio::music_cur_get_position( ) ) + " / " + std::to_string( audio::music_get_song_length( ) ) << std::endl;
		}

		if ( GetAsyncKeyState( VK_SPACE ) )
		{
			std::cout << "1) Play Song\n2)Stop Song\n3)Change Volume\n4)Skip Song\n5)Pause/Resume\n6)Forward\n7)Rewind";
			int i_menu_option = -1;
			std::cin >> i_menu_option;

			switch ( i_menu_option )
			{
				case 1:
				{
					int i_selected_song = -1;
					std::cout << "Select song from list: " << std::endl;

					for ( int i = 0; i != audio::vec_music_names.size( ); i++ )
						std::cout << i << " - " << audio::music_get_name( i ) << " Duration: " << std::to_string( Mix_MusicDuration( audio::vec_music[i] ) ) << std::endl;

					std::cout << "Selection number: ";
					std::cin >> i_selected_song;

					audio::music_play( i_selected_song, false );
					break;
				}
				case 2:
					audio::music_cur_end( );
					break;
				case 3:
				{
					std::cout << "Current volume: " << std::to_string( audio::i_volume ) << std::endl;
					std::cout << "Set volume to: ";
					int i_new_volume = audio::i_volume;
					std::cin >> i_new_volume;
					if ( i_new_volume != audio::i_volume )
						audio::volume_set( i_new_volume );
					break;
				}
				case 4:
					audio::music_cur_end( true );
					break;
				case 5:
					audio::music_toggle_play( );
					break;
				case 6:
					audio::music_cur_mv_forward( 10 );
					break;
				case 7:
					audio::music_cur_mv_rewind( 10 );
					break;
			}
		}
	}
}

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

		if ( std::experimental::filesystem::is_directory( s_path_name ) )
			continue;

		auto path = entry.path( );
		vec_files.push_back( s_path_name );
	}

	return vec_files;
}

void setup_music_files( )
{
	std::vector<std::string> vec_files = get_files_in_dir( "c:\\test\\music" );
	for ( std::string s : vec_files )
	{
		if ( s.substr( ( s.size( ) - 1 ) - 4, ( s.size( ) - 1 ) ) == ".flac" )
		{
			int i_index = audio::music_load( s );
			std::string s_title = Mix_GetMusicTitle( audio::vec_music[i_index] );
			std::string s_album = Mix_GetMusicAlbumTag( audio::vec_music[i_index] );
			std::string s_artist = Mix_GetMusicArtistTag( audio::vec_music[i_index] );
			std::cout << i_index << "- Title: " << s_title << "- Album: " << s_album << "- Artist: " << s_artist << std::endl;
		}
	}
}

void music_start( )
{
	audio::mixer_start( );
	lg( "Mixer started" );

	setup_music_files( );
	Sleep( 3000 );

	music_menu_thread( );

	lg( "Mixer stopping" );
	audio::mixer_stop( );
	lg( "Mixer stopped" );
}

int main( int argc, char* argv[] )
{	
	music_start( );

	while ( true )
	{
		Sleep( 250 );
	}
}