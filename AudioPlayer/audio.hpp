#pragma once
#include "include.hpp"
#include "utility.hpp"

namespace audio
{
	class c_song
	{
	public:
		Mix_Music* mx_song = nullptr;
		std::string s_song_path = "";

		std::string s_song_artist = "";
		std::string s_song_title = "";
		std::string s_song_album = "";
		int i_song_duration = 0;

		c_song( std::string s_file_dir, Mix_Music* mx_song_input )
		{
			s_song_path = s_file_dir;
			mx_song = mx_song_input;

			s_song_title = Mix_GetMusicTitle( mx_song );
			s_song_artist = Mix_GetMusicArtistTag( mx_song );
			s_song_album = Mix_GetMusicAlbumTag( mx_song );
		}
	};

	std::vector<c_song> vec_songs{};

	std::vector<Mix_Chunk*> vec_sounds{};
	std::vector<Mix_Music*> vec_music{};
	std::vector<std::string> vec_music_names{};
	int i_volume = 100;
	int i_currently_playing = -1;

	//music to-be-played
	bool b_queued_songs = false;
	std::vector<int> vec_queue_music{};

	void volume_set( int v )
	{
		i_volume = ( MIX_MAX_VOLUME * v ) / 100;
		Mix_Volume( -1, i_volume );
		Mix_VolumeMusic( i_volume );
	}

	bool mixer_start( )
	{
		Mix_Init( MIX_INIT_FLAC );
		SDL_Init( SDL_INIT_AUDIO );
		if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		{
			printf( "SDL_Mixer couldnt init. Err: %s\n", Mix_GetError( ) );
			return false;
		}
		volume_set( i_volume );
		return true;
	}
	void mixer_stop( )
	{
		for ( int s = 0; s < vec_sounds.size( ); s++ )
		{
			Mix_FreeChunk( vec_sounds[s] );
			vec_sounds[s] = NULL;
		}
		for ( int s = 0; s < vec_music.size( ); s++ )
		{
			Mix_FreeMusic( vec_music[s] );
			vec_music[s] = NULL;
		}
		Mix_Quit( );
	}

#pragma region sound
	int sound_load( std::string s_sound_path )
	{
		if ( !std::experimental::filesystem::exists( s_sound_path ) )
		{
			lg( "Failed to find sound : " + s_sound_path );
			return -1;
		}

		Mix_Chunk* dec_sound_mem = Mix_LoadWAV( s_sound_path.c_str( ) );
		if ( !dec_sound_mem )
		{
			lg( "Failed to load sound : " + s_sound_path + " --- Error: " + Mix_GetError( ) );
			return -1;
		}

		vec_sounds.push_back( dec_sound_mem );
		return vec_sounds.size( ) - 1;
	}

	int sound_load_from_memory( std::vector<uint8_t> vec_buffer )
	{
		char* data = new char[vec_buffer.size( )];
		SDL_RWops* rw = SDL_RWFromMem( ( void* ) vec_buffer.data( ), vec_buffer.size( ) );
		if ( !rw )
		{
			lg( "Failed to load sound from memory 1" );
			return -1;
		}

		Mix_Chunk* audio = Mix_LoadWAV_RW( rw, 0 );
		if ( !audio )
		{
			lg( "Failed to load sound from memory 2" );
			return -1;
		}

		vec_sounds.push_back( audio );
		return vec_sounds.size( ) - 1;
	}

	bool sound_play( int i_index_sound, bool b_loop = false, int i_index_channel = -1, int i_volume_custom = i_volume )
	{
		Mix_Volume( i_index_channel, i_volume_custom );
		if ( Mix_PlayChannel( i_index_channel, vec_sounds[i_index_sound], ( b_loop ? -1 : 0 ) ) == -1 )
			return false;
		return true;
	}
#pragma endregion sound

#pragma region music
	int music_load_class( std::string s_music_path )
	{
		if ( !std::experimental::filesystem::exists( s_music_path ) )
		{
			lg( "Failed to find music : " + s_music_path );
			return -1;
		}

		Mix_Music* dec_music_mem = Mix_LoadMUS( s_music_path.c_str( ) );
		if ( !dec_music_mem )
		{
			lg( "Failed to load music : " + s_music_path + " --- Error: " + Mix_GetError( ) );
			return -1;
		}

		c_song cur_song( s_music_path, dec_music_mem );
		if ( !cur_song.mx_song )
			return -1;

		vec_songs.push_back( cur_song );
		return vec_songs.size( ) - 1;
	}

	int music_load( std::string s_music_path )
	{
		if ( !std::experimental::filesystem::exists( s_music_path ) )
		{
			lg( "Failed to find music : " + s_music_path );
			return -1;
		}

		Mix_Music* dec_music_mem = Mix_LoadMUS( s_music_path.c_str( ) );
		if ( !dec_music_mem )
		{
			lg( "Failed to load music : " + s_music_path + " --- Error: " + Mix_GetError( ) );
			return -1;
		}

		std::string s_title = Mix_GetMusicTitle( dec_music_mem );
		std::string s_album = Mix_GetMusicAlbumTag( dec_music_mem );
		std::string s_artist = Mix_GetMusicArtistTag( dec_music_mem );

		std::string s_label = s_artist + " - " + s_album + " - " + s_title;

		vec_music.push_back( dec_music_mem );
		vec_music_names.push_back( s_label );
		return vec_music.size( ) - 1;
	}

	bool is_track_selected( )
	{
		return Mix_PlayingMusic( );
	}

	bool is_music_paused( )
	{
		return Mix_PausedMusic( );
	}

	bool music_pause( )
	{
		Mix_PauseMusic( );
		return true;
	}

	bool music_resume( )
	{
		Mix_ResumeMusic( );
		return true;
	}

	bool music_queue_add( int i_index_music )
	{
		vec_queue_music.push_back( i_index_music );
		b_queued_songs = true;
		return true;
	}

	bool music_play( int i_index_music, bool b_loop = false )
	{
		//track already playing, add song to queue
		if ( is_track_selected( ) )
		{
			music_queue_add( i_index_music );
			//temp
			lg( "Song already playing, added to queue" );
			Sleep( 250 );
			//temp
			return true;
		}

		Mix_Volume( 1, i_volume );

		if ( Mix_PlayMusic( vec_music[i_index_music], ( b_loop ? -1 : 0 ) ) == -1 )
			return false;

		i_currently_playing = i_index_music;
		return true;
	}

	std::string music_get_name( int i_index = i_currently_playing )
	{
		if ( i_index == i_currently_playing )
		{
			if ( i_currently_playing == -1 || !is_track_selected( ) )
				return "NULL";
		}

		if ( vec_music_names[i_index].size( ) > 2 )
			return vec_music_names[i_index];
	}

	int music_get_song_length( int i_index = i_currently_playing )
	{
		return ( int ) Mix_MusicDuration( vec_music[i_index] );
	}

	int music_cur_get_position( )
	{
		return ( int ) Mix_GetMusicPosition( vec_music[i_currently_playing] );
	}

	bool music_cur_set_position( int i_pos_new )
	{
		int i_pos_cur = music_cur_get_position( );

		if ( Mix_SetMusicPosition( i_pos_new ) == -1 )
			return false;

		return true;
	}

	bool music_cur_mv_forward( int i_time )
	{
		return music_cur_set_position( music_cur_get_position( ) + i_time );
	}

	bool music_cur_mv_rewind( int i_time )
	{
		return music_cur_set_position( music_cur_get_position( ) - i_time );
	}

	bool music_cur_end( bool b_fade_out = false )
	{
		return music_cur_set_position( music_get_song_length( i_currently_playing ) - ( b_fade_out ? 10 : 0 ) );
	}

	void music_toggle_play( )
	{
		//if no track is selected, do nothing
		if ( !is_track_selected( ) )
			return;

		if ( is_music_paused( ) )
		{
			music_resume( );
		}
		else
		{
			music_pause( );
		}
	}
#pragma endregion music
}