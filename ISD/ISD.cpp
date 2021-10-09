
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <locale>      

#include "ISD.h"
#include "ISD_MemorySteam.h"

using namespace ISD;
using std::pair;
using std::make_pair;

Status EntityLoader::Initialize( const std::string &path )
	{
	if( !this->Path.empty() )
		{
		return Status::EAlreadyInitialized;
		}

	std::wstring wpath = widen( path );

	// make path absolute
	wpath = full_path( wpath );

	// make sure it is a directory 
	DWORD file_attributes = GetFileAttributesW( wpath.c_str() );
	if( file_attributes != FILE_ATTRIBUTE_DIRECTORY )
		{
		return Status::EParam; // invalid path
		}

	this->Path = wpath;

	return Status::Ok;
	}

struct LoadThreadParams
	{
	UUID uuid; // id of entity to load
	std::wstring path; // root path of the entities on disk
	};

static DWORD WINAPI LoadThreadProcedure( _In_ LPVOID lpParameter )
	{
	// get the params
	LoadThreadParams *params = (LoadThreadParams *)lpParameter;
	UUID uuid = params->uuid;
	std::wstring path = params->path;
	free( params );

	// create the file name from the uuid
	uint8 top_byte = (uuid.Data1 >> 24) & 0xff;
	std::wstring dir_name = uint8_to_hex_wstring( top_byte );
	std::wstring file_name = uuid_to_hex_wstring( uuid ) + L".dat";
	std::wstring file_path = path + L"\\" + file_name;
	
	// open the file
	HANDLE file_handle = ::CreateFileW( file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr );
	if( file_handle == INVALID_HANDLE_VALUE )
		{
		// failed to open the file
		return (DWORD)Status::ECantOpen;
		}

	// get the size
	LARGE_INTEGER dfilesize = {};
	if( !::GetFileSizeEx( file_handle, &dfilesize ) )
		{
		// failed to get the size
		return (DWORD)Status::ECantOpen;
		}
	uint64 total_bytes_to_read = dfilesize.QuadPart;

	// read in all of the file
	uint8 *buffer = (uint8*) malloc(total_bytes_to_read);
	if( !buffer )
		{
		// failed to allocate the memory
		return (DWORD)Status::ECantAllocate;
		}

	uint64 bytes_read = 0;
	while( bytes_read < total_bytes_to_read )
		{
		// check how much to read and cap at 32 bits
		uint64 bytes_left = total_bytes_to_read - bytes_read;
		uint32 bytes_to_read_this_time = 0xffffffff;
		if( bytes_left < 0xffffffff )
			bytes_to_read_this_time = (uint32)bytes_left;

		DWORD bytes_that_were_read = 0;
		if( !::ReadFile( file_handle, &buffer[bytes_read], bytes_to_read_this_time, &bytes_that_were_read, nullptr ) )
			{
			// failed to read
			return (DWORD)Status::ECantRead;
			}

		// update number of bytes that were read
		bytes_read += bytes_that_were_read;
		}
	
	::CloseHandle( file_handle );

//	// read the header
//	MemoryStream *pstream = new MemoryStream( buffer , total_bytes_to_read , false );
//
//	UUID uuid = pstream->Read<UUID>();
//
	return (DWORD)Status::Ok;
	}

Status EntityLoader::AsyncLoadEntity( const UUID &uuid )
	{
	LoadThreadParams *params = (LoadThreadParams *)malloc( sizeof( LoadThreadParams ) );
	if( !params )
		{
		return Status::ECantAllocate;
		}
	params->uuid = uuid;
	params->path = this->Path;


	DWORD thread_id = {};
	HANDLE thread_handle = CreateThread( nullptr, 0, LoadThreadProcedure, (void *)params, 0, &thread_id );
	if( !thread_handle )
		{
		return Status::ECantAllocate;
		}

	return Status::Ok;
	}

pair<bool, Status> EntityLoader::IsEntityLoaded( const UUID &uuid )
	{
	return make_pair(false, Status::Ok);
	}

pair<const Entity *, Status> EntityLoader::GetLoadedEntity( const UUID &uuid )
	{

	return make_pair( nullptr, Status::Ok);
	}

