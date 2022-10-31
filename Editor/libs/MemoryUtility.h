#pragma once
#include <intsafe.h>
#include <cstring>
#include <type_traits>

/*-----------------------------------------------------------------------------
	FMemory.
-----------------------------------------------------------------------------*/
typedef unsigned char 		uint8;

struct FMemory
{
	/** @name Memory functions (wrapper for FPlatformMemory) */

	inline static void* Memmove( void* Dest, const void* Src, SIZE_T Count )
	{
		return std::memmove( Dest, Src, Count );
	}

	inline static int Memcmp( const void* Buf1, const void* Buf2, SIZE_T Count )
	{
		return std::memcmp( Buf1, Buf2, Count );
	}

	inline static void* Memset(void* Dest, uint8 Char, SIZE_T Count)
	{
		return std::memset( Dest, Char, Count );
	}

	template< class T > 
	inline static void Memset( T& Src, uint8 ValueToSet )
	{
		static_assert( !std::is_pointer_v<T>, "For pointers use the three parameters function");
		Memset( &Src, ValueToSet, sizeof( T ) );
	}

	inline static void* Memzero(void* Dest, SIZE_T Count)
	{
		return  memset( Dest, 0, Count );
	}

	template< class T > 
	inline static void Memzero( T& Src )
	{
		static_assert( !std::is_pointer_v<T>, "For pointers use the two parameters function");
		Memzero( &Src, sizeof( T ) );
	}

	inline static void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest,Src,Count);
	}

	template< class T > 
	inline static void Memcpy( T& Dest, const T& Src )
	{
		static_assert( !std::is_pointer_v<T>, "For pointers use the three parameters function");
		Memcpy( &Dest, &Src, sizeof( T ) );
	}
};
