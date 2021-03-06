/*
	string.cc
	---------
	
	Some of the find-related member functions were written using 
	boost/shmem/containers/string.hpp as a guide.
*/

#include "plus/string.hh"

// Standard C++
#include <algorithm>
#include <stdexcept>

// Standard C
#include <stdlib.h>
#include <string.h>

// debug
#include "debug/assert.hh"

// plus
#include "plus/string_details.hh"


#define LENGTH_ERROR_MESSAGE  "plus::string size can't exceed 0x7fffffff"


namespace plus
{
	
	const string string::null;
	
	
	static int lexicographical_compare_3way( const char* a, const char* a_end,
	                                         const char* b, const char* b_end )
	{
		while ( a < a_end  &&  b < b_end )
		{
			if ( const int diff = *a++ - *b++ )
			{
				return diff;
			}
		}
		
		return (a != a_end) - (b != b_end);
	}
	
	static int string_compare( const char* a, size_t a_size,
	                           const char* b, size_t b_size )
	{
		return lexicographical_compare_3way( a, a + a_size,
		                                     b, b + b_size );
	}
	
	static int string_equal( const char* a, size_t a_size,
	                         const char* b, size_t b_size )
	{
		return a_size == b_size  &&  string_compare( a, a_size,
		                                             b, b_size ) == 0;
	}
	
	
	class matches_any
	{
		private:
			const char*  begin;
			const char*  end;
		
		public:
			matches_any( const char* a, const char* z ) : begin( a ), end( z )
			{
			}
			
			bool operator()( char c ) const
			{
				return std::find( begin, end, c ) != end;
			}
	};
	
	class matches_none
	{
		private:
			const char*  begin;
			const char*  end;
		
		public:
			matches_none( const char* a, const char* z ) : begin( a ), end( z )
			{
			}
			
			bool operator()( char c ) const
			{
				return std::find( begin, end, c ) == end;
			}
	};
	
	
	template < class Iter, class F >
	static Iter find_last_if( Iter begin, Iter end, F f )
	{
		ASSERT( begin <= end );
		
		Iter it = end;
		
		while ( --it >= begin )
		{
			if ( f( *it ) )
			{
				return it;
			}
		}
		
		return end;
	}
	
	
	static void dispose( const char* pointer, int _policy )
	{
		switch ( _policy )
		{
			case ~delete_shared:
			case ~delete_owned:
			{
				pointer -= sizeof (size_t);
				
				// This casts away const, but it's only the characters that are
				// const, not the refcount.
				
				size_t& refcount = *(size_t*) pointer;
				
				if ( --refcount > 0 )
				{
					break;
				}
			}
			
			// fall through
			
			case ~delete_basic:
				::operator delete( (void*) pointer );
				break;
			
			case ~delete_free:
				free( (void*) pointer );
				break;
			
			default:
				break;
		}
	}
	
	
	void string::check_size( size_type size )
	{
		// 2 GB limit on 32-bit platforms
		
		if ( size > max_size() )
		{
			const bool _32bit = sizeof size == 4;
			
			const char* message = _32bit ? LENGTH_ERROR_MESSAGE
			                             : LENGTH_ERROR_MESSAGE "ffffffff";
			
			throw std::length_error( message );
		}
	}
	
	bool string::movable() const
	{
		if ( _policy() >= ~delete_never )
		{
			// small strings and delete_never are always shallow-copied
			return true;
		}
		
		if ( _policy() >= ~delete_owned )
		{
			// covers delete_shared, also.
			// An owned buffer should always have 1 user, but it's easier
			// to check both here.
			
			const size_t use_count = ((size_t*) its_alloc.pointer)[ -1 ];
			
			return use_count == 1;
		}
		
		return true;  // externally-supplied, single-user buffer
	}
	
	void string::set_length( size_type length )
	{
		if ( is_small() )
		{
			ASSERT( length <= max_offset );
			
			its_small_name[ max_offset ] = max_offset - length;
		}
		else
		{
			its_alloc.length = length;
		}
		
		char* p = const_cast< char* >( data() );
		
		p[ length ] = '\0';
	}
	
	string::string( const char*    p,
	                size_type      length,
	                delete_policy  policy,
	                size_type      capacity )
	{
		check_size( length   );
		check_size( capacity );
		
		its_alloc.pointer  = p;
		its_alloc.length   = length;
		its_alloc.capacity = capacity;  // may be zero
		
		_policy( ~policy );
	}
	
	string::string( const char* p, size_type length )
	{
		its_small_name[ 0          ] = '\0';
		its_small_name[ max_offset ] = max_offset;
		
		assign( p, length );
	}
	
	string::string( const char* p, const char* q )
	{
		its_small_name[ 0          ] = '\0';
		its_small_name[ max_offset ] = max_offset;
		
		assign( p, q );
	}
	
	string::string( const char* s )
	{
		its_small_name[ 0          ] = '\0';
		its_small_name[ max_offset ] = max_offset;
		
		assign( s );
	}
	
	string::string( size_type n, char c )
	{
		// An uninitialized 15-char string is acceptable because we're just
		// going to assign over it anyway, and empties aren't faster here.
		its_small_name[ max_offset ] = 0;
		
		assign( n, c );
	}
	
	string::~string()
	{
		dispose( its_alloc.pointer, _policy() );
	}
	
	string::string( const string& other, size_type pos, size_type n )
	{
		its_small_name[ 0          ] = '\0';
		its_small_name[ max_offset ] = max_offset;
		
		assign( other, pos, n );
	}
	
	string::string( const string& other )
	{
		its_small_name[ 0          ] = '\0';
		its_small_name[ max_offset ] = max_offset;
		
		assign( other );
	}
	
	string& string::operator=( const string& other )
	{
		assign( other );
		
		return *this;
	}
	
	string::size_type string::size() const
	{
		/*
			For small strings, the margin (stored in the last byte) is the
			number of bytes by which the string could increase while still
			fitting in the internal buffer.  Length plus margin equals the
			maximum length for a small string (either 15 for 32-bit longs,
			or 31 for 64-bit).
		*/
		
		const char margin = its_small_name[ max_offset ];
		
		return is_small() ? max_offset - margin
		                  : its_alloc.length;
	}
	
	string::size_type string::capacity() const
	{
		if ( is_small() )
		{
			return max_offset;
		}
		
		const long capacity = its_alloc.capacity;  // negative for substrings
		
		return capacity > 0 ? its_alloc.capacity
		                    : its_alloc.length;
	}
	
	string::size_type string::substr_offset() const
	{
		if ( is_small() )
		{
			return 0;
		}
		
		const long capacity = its_alloc.capacity;  // negative for substrings
		
		return capacity >= 0 ? 0
		                     : -capacity;
	}
	
	const char* string::data( bool zero_terminator_required ) const
	{
		if ( is_small() )
		{
			return its_small_name;  // always terminated
		}
		
		const char* begin = its_alloc.pointer + substr_offset();
		
		if ( !zero_terminator_required  ||  is_c_str() )
		{
			return begin;
		}
		
		string& non_const = const_cast< string& >( *this );
		
		plus::string temp( begin, its_alloc.length );
		
		non_const.swap( temp );
		
		return non_const.data();
	}
	
	const char* string::end() const
	{
		return begin() + size();
	}
	
	string& string::assign( const char* p, size_type length, delete_policy policy, size_type capacity )
	{
		check_size( length   );
		check_size( capacity );
		
		if ( length )
		{
			ASSERT( p != NULL );
			
			ASSERT( p + length >= p );
		}
		
		dispose( its_alloc.pointer, _policy() );
		
		its_alloc.pointer  = p;
		its_alloc.length   = length;
		its_alloc.capacity = capacity;  // may be zero
		
		_policy( ~policy );
		
		return *this;
	}
	
	char* string::reallocate( size_type length )
	{
		check_size( length );
		
		char const *const old_pointer = its_alloc.pointer;
		
		const char old_policy = _policy();
		
		char* new_pointer = NULL;
		
		if ( length >= sizeof its_small_name )
		{
			const size_type capacity = adjusted_capacity( length );
			
			const size_t buffer_length = sizeof (size_t) + capacity + 1;
			
			// may throw
			new_pointer = (char*) ::operator new( buffer_length );
			
			reinterpret_cast< size_t* >( new_pointer )[0] = 1;  // refcount
			
			new_pointer += sizeof (size_t);
			
			its_alloc.pointer  = new_pointer;
			its_alloc.length   = length;
			its_alloc.capacity = capacity;
			
			_policy( ~delete_shared );
		}
		else
		{
			new_pointer = its_small_name;
			
			its_small_name[ max_offset ] = max_offset - length;
		}
		
		new_pointer[ length ] = '\0';
		
		dispose( old_pointer, old_policy );
		
		return new_pointer;
	}
	
	char* string::copy_on_write( bool tainting )
	{
		if ( is_small() )
		{
			return its_small_name;
		}
		
		if ( _policy() == ~delete_shared )
		{
			const size_t refcount = ((size_t*) its_alloc.pointer)[ -1 ];
			
			ASSERT( refcount != 0 );
			
			if ( refcount == 1 )
			{
				// Shared with no others
				
				if ( tainting )
				{
					// ...and now never will be
					_policy( ~delete_owned );
				}
				
				return const_cast< char* >( data() );
			}
		}
		else if ( long( its_alloc.capacity ) > 0 )
		{
			// delete_owned or read/write external buffer
			return const_cast< char* >( its_alloc.pointer );
		}
		
		assign( data(), its_alloc.length, capacity() );
		
		if ( tainting  &&  _policy() == ~delete_shared )
		{
			_policy( ~delete_owned );
		}
		
		return const_cast< char* >( data() );
	}
	
	string& string::assign( const char* p, size_type length, size_type capacity )
	{
		// reallocate() will throw if length or capacity exceeds max_size()
		
		if ( length )
		{
			ASSERT( p != NULL );
			
			ASSERT( p + length >= p );
		}
		
		if ( empty() )
		{
			capacity = std::max( length, capacity );
			
			char* new_pointer = reallocate( capacity );
			
			memcpy( new_pointer, p, length );
			
			set_length( length );
		}
		else
		{
			// Always assign to a temporary and then swap.
			// This handles the cases of input occupying either our small buffer
			// or an allocated one.
			
			string temp;
			
			temp.assign( p, length, capacity );
			
			swap( temp );
		}
		
		return *this;
	}
	
	string& string::assign( const char* s )
	{
		ASSERT( s != NULL );
		
		const size_type length = strlen( s );
		
		return assign( s, length );
	}
	
	string& string::assign( size_type n, char c )
	{
		// reallocate() will throw if n exceeds max_size()
		
		char* pointer = reallocate( n );
		
		memset( pointer, c, n );
		
		return *this;
	}
	
	string& string::assign( const move_t& m )
	{
		if ( &m.source != this )
		{
			reset();
			
			memcpy( its_longs, m.source.its_longs, buffer_size );
			
			if ( m.source._policy() < 0 )
			{
				// Empty any allocated strings; leave small strings alone
				m.source._policy( max_offset );
			}
			
			if ( _policy() == ~delete_owned )
			{
				_policy( ~delete_shared );
			}
		}
		
		return *this;
	}
	
	string& string::assign( const string& other, size_type pos, size_type n )
	{
		const size_type other_size = other.size();
		
		if ( pos > other_size )
		{
			throw std::out_of_range( "plus::string" );
		}
		
		n = std::min( n, other_size - pos );
		
		const bool small = n < sizeof its_small_name;
		
		const bool shallow = !small  &&  other._policy() >= ~delete_shared;
		
		/*
			A shallow copy is made for delete_shared and delete_never when
			the resulting substring is not a small string.
		*/
		
		if ( shallow )
		{
			if ( other._policy() == ~delete_shared )
			{
				size_t& refcount = ((size_t*) other.its_alloc.pointer)[ -1 ];
				
				// Should never happen, since address space would be exhausted
				// by size( -1 ) / sizeof (string) copies of a string, which is
				// necessarily less than size( -1 ).  This is to catch errors in
				// maintaining the refcount.
				
				ASSERT( refcount <= size_t( -1 ) );
				
				++refcount;
			}
			
			// Either it's shared or it occupies static storage.
			// Either way, we perform a shallow copy.
			
			// If this is a self-assignment, then *we* are either static
			// or shared with non-minimal refcount, and dispose() does nothing.
			
			dispose( its_alloc.pointer, _policy() );
			
			std::copy( other.its_longs,
			           other.its_longs + buffer_size_in_longs,
			           its_longs );
			
			its_alloc.length = n;
			
			if ( pos != 0 )
			{
				const long new_offset = substr_offset() + pos;
				
				its_alloc.capacity = size_type( -new_offset );
			}
		}
		else
		{
			assign( other.data() + pos, n );
		}
		
		return *this;
	}
	
	void string::swap( string& other )
	{
		long temp_longs[ buffer_size_in_longs ];
		
		memcpy( temp_longs,      other.its_longs, buffer_size );
		memcpy( other.its_longs, its_longs,       buffer_size );
		memcpy( its_longs,       temp_longs,      buffer_size );
	}
	
	
	string::size_type string::copy( char* buffer, size_type n, size_type pos ) const
	{
		const size_t size = length();
		
		if ( pos > size )
		{
			throw std::out_of_range( "plus::string" );
		}
		
		n = std::min( n, size - pos );
		
		memcpy( buffer, data() + pos, n );
		
		return n;
	}
	
	
	string::size_type string::find( const string& s, size_type pos ) const
	{
		return find( s.data(), pos, s.size() );
	}
	
	string::size_type string::find( const char* s, size_type pos ) const
	{
		return find( s, pos, strlen( s ) );
	}
	
	string::size_type string::find( const char* s, size_type pos, size_type n ) const
	{
		const size_t size = length();
		
		if ( pos > size  ||  pos + n > size )
		{
			return npos;
		}
		
		if ( n == 0 )
		{
			return pos;
		}
		
		const char* begin = data();
		const char* end   = begin + size;
		
		const char* it = std::search( begin + pos, end, s, s + n );
		
		return it != end ? it - begin : npos;
	}
	
	string::size_type string::find( char c, size_type pos ) const
	{
		return find( &c, pos, 1 );
	}
	
	
	string::size_type string::rfind( const string& s, size_type pos ) const
	{
		return rfind( s.data(), pos, s.size() );
	}
	
	string::size_type string::rfind( const char* s, size_type pos ) const
	{
		return rfind( s, pos, strlen( s ) );
	}
	
	string::size_type string::rfind( const char* s, size_type pos, size_type n ) const
	{
		const size_type size = length();
		
		if ( n > size )
		{
			return npos;
		}
		
		pos = std::min( pos, size - n );
		
		if ( n == 0 )
		{
			return pos;
		}
		
		const char* begin = data();
		const char* end   = begin + pos + n;
		
		const char* it = std::find_end( begin, end, s, s + n );
		
		return it != end ? it - begin : npos;
	}
	
	string::size_type string::rfind( char c, size_type pos ) const
	{
		return rfind( &c, pos, 1 );
	}
	
	
	string::size_type string::find_first_of( const string& s, size_type pos ) const
	{
		return find_first_of( s.data(), pos, s.size() );
	}
	
	string::size_type string::find_first_of( const char* s, size_type pos ) const
	{
		return find_first_of( s, pos, strlen( s ) );
	}
	
	string::size_type string::find_first_of( const char* s, size_type pos, size_type n ) const
	{
		const size_t size = length();
		
		if ( pos >= size )
		{
			return npos;
		}
		
		const char* begin = data();
		const char* end   = begin + size;
		
		const char* it = std::find_first_of( begin + pos, end, s, s + n );
		
		return it != end ? it - begin : npos;
	}
	
	string::size_type string::find_first_of( char c, size_type pos ) const
	{
		return find( c, pos );
	}
	
	
	string::size_type string::find_last_of( const string& s, size_type pos ) const
	{
		return find_last_of( s.data(), pos, s.size() );
	}
	
	string::size_type string::find_last_of( const char* s, size_type pos ) const
	{
		return find_last_of( s, pos, strlen( s ) );
	}
	
	string::size_type string::find_last_of( const char* s, size_type pos, size_type n ) const
	{
		const size_type size = length();
		
		if ( 1 > size )
		{
			return npos;
		}
		
		pos = std::min( pos, size - 1 );
		
		const char* begin = data();
		const char* end   = begin + pos + 1;
		
		const char* it = find_last_if( begin, end, matches_any( s, s + n ) );
		
		return it != end ? it - begin : npos;
	}
	
	string::size_type string::find_last_of( char c, size_type pos ) const
	{
		return find_last_of( &c, pos, 1 );
	}
	
	
	string::size_type string::find_first_not_of( const string& s, size_type pos ) const
	{
		return find_first_not_of( s.data(), pos, s.size() );
	}
	
	string::size_type string::find_first_not_of( const char* s, size_type pos ) const
	{
		return find_first_not_of( s, pos, strlen( s ) );
	}
	
	string::size_type string::find_first_not_of( const char* s, size_type pos, size_type n ) const
	{
		const size_t size = length();
		
		if ( pos >= size )
		{
			return npos;
		}
		
		const char* begin = data();
		const char* end   = begin + size;
		
		const char* it = std::find_if( begin + pos, end, matches_none( s, s + n ) );
		
		return it != end ? it - begin : npos;
	}
	
	string::size_type string::find_first_not_of( char c, size_type pos ) const
	{
		return find_first_not_of( &c, pos, 1 );
	}
	
	
	string::size_type string::find_last_not_of( const string& s, size_type pos ) const
	{
		return find_last_not_of( s.data(), pos, s.size() );
	}
	
	string::size_type string::find_last_not_of( const char* s, size_type pos ) const
	{
		return find_last_not_of( s, pos, strlen( s ) );
	}
	
	string::size_type string::find_last_not_of( const char* s, size_type pos, size_type n ) const
	{
		const size_type size = length();
		
		if ( 1 > size )
		{
			return npos;
		}
		
		pos = std::min( pos, size - 1 );
		
		const char* begin = data();
		const char* end   = begin + pos + 1;
		
		const char* it = find_last_if( begin, end, matches_none( s, s + n ) );
		
		return it != end ? it - begin : npos;
	}
	
	string::size_type string::find_last_not_of( char c, size_type pos ) const
	{
		return find_last_not_of( &c, pos, 1 );
	}
	
	
	string string::substr( size_type pos, size_type n ) const
	{
		return string( *this, pos, n );
	}
	
	
	int string::compare( const string& b ) const
	{
		return string_compare( data(), size(), b.data(), b.size() );
	}
	
	int string::compare( size_type a_pos, size_type a_n, const string& b ) const
	{
		const size_type a_size = size();
		
		if ( a_pos > a_size )
		{
			throw std::out_of_range( "plus::string" );
		}
		
		a_n = std::min( a_n, a_size - a_pos );
		
		return string_compare( data() + a_pos, a_n, b.data(), b.size() );
	}
	
	int string::compare( size_type a_pos, size_type a_n, const string& b, size_type b_pos, size_type b_n ) const
	{
		const size_type a_size = size();
		
		if ( a_pos > a_size )
		{
			throw std::out_of_range( "plus::string" );
		}
		
		a_n = std::min( a_n, a_size - a_pos );
		
		const size_type b_size = b.size();
		
		if ( b_pos > b_size )
		{
			throw std::out_of_range( "plus::string" );
		}
		
		b_n = std::min( b_n, b_size - b_pos );
		
		return string_compare( data() + a_pos, a_n, b.data() + b_pos, b_n );
	}
	
	int string::compare( const char* b ) const
	{
		return string_compare( data(), size(), b, strlen( b ) );
	}
	
	int string::compare( size_type a_pos, size_type a_n, const char* b, size_type b_n ) const
	{
		const size_type a_size = size();
		
		if ( a_pos > a_size )
		{
			throw std::out_of_range( "plus::string" );
		}
		
		a_n = std::min( a_n, a_size - a_pos );
		
		b_n = std::min< size_type >( b_n, strlen( b ) );
		
		return string_compare( data(), size(), b, b_n );
	}
	
	
	bool operator==( const string& a, const string& b )
	{
		return string_equal( a.data(), a.size(), b.data(), b.size() );
	}
	
	bool operator==( const string& a, const char* b )
	{
		return string_equal( a.data(), a.size(), b, strlen( b ) );
	}
	
	
	bool operator<( const string& a, const string& b )
	{
		return string_compare( a.data(), a.size(), b.data(), b.size() ) < 0;
	}
	
	bool operator<( const string& a, const char* b )
	{
		return string_compare( a.data(), a.size(), b, strlen( b ) ) < 0;
	}
	
	bool operator<( const char* a, const string& b )
	{
		return string_compare( a, strlen( a ), b.data(), b.size() ) < 0;
	}
	
}

