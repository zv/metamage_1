/*
	var_string.cc
	-------------
*/

#include "plus/var_string.hh"

// Standard C++
#include <algorithm>
#include <stdexcept>

// Standard C
#include <string.h>

// debug
#include "debug/assert.hh"

// plus
#include "plus/string_details.hh"


namespace plus
{
	
	var_string& var_string::assign( const move_t& m )
	{
		if ( &m.source != this )
		{
			/*
				A buffer that can't be moved (because it's shared) is still
				handled by copy_on_write(), but to be exception-safe, we'll
				need to reallocate *before* resetting the source.  For this
				case, move a copy of the source string.
			*/
			
			string::assign( m.source.movable() ? m
			                                   : string( m.source ).move() );
			
			copy_on_write( true );
			
			m.source.reset();
		}
		
		return *this;
	}
	
	char* var_string::erase_unchecked( char* p, size_type n )
	{
		const size_type old_size = size();
		
		char* begin = const_cast< char* >( data() );
		char* end   = begin + old_size;
		
		char* q = p + n;
		
		std::copy( q, end, p );
		
		const size_type new_size = old_size - n;
		
		set_length( new_size );
		
		return p;
	}
	
	var_string& var_string::erase( size_type pos, size_type n )
	{
		const size_type old_size = size();
		
		if ( pos > old_size )
		{
			throw std::out_of_range( __func__ );
		}
		
		n = std::min( n, old_size - pos );
		
		char* data = mutable_data();
		
		char* p = data + pos;
		
		erase_unchecked( p, n );
		
		return *this;
	}
	
	char* var_string::erase( char* p, char* q )
	{
		char* begin = const_cast< char* >( data() );
		char* end   = begin + size();
		
		ASSERT( begin <= p   );
		ASSERT( p     <= q   );
		ASSERT( q     <= end );
		
		const size_type n = q - p;
		
		return erase_unchecked( p, n );
	}
	
	char* var_string::embiggen( size_type new_length, size_type new_capacity )
	{
		// reallocate() will throw if either parameter exceeds max_size()
		
		const size_type capacity_ = capacity();
		const size_type size_     = size();
		
		if ( new_capacity <= new_length )
		{
			const bool growing = new_length > capacity_;
			
			new_capacity = new_capacity ? new_length
			             : growing      ? std::max( size_ * 2, new_length )
			             :                capacity_;
		}
		
		new_capacity = adjusted_capacity( new_capacity );
		
		if ( new_capacity != capacity_ )
		{
			try
			{
				var_string temp;
				
				char* new_pointer = temp.reallocate( new_capacity );
				
				memcpy( new_pointer, data(), size() );
				
				swap( temp );
				
				copy_on_write( true );  // taint the string as unshareable
			}
			catch ( ... )
			{
				const bool increasing = new_capacity > capacity_;
				
				if ( increasing )
				{
					throw;
				}
				
				// Failure to decrease capacity is not an error
			}
		}
		
		char* data = mutable_data();
		
		set_length( new_length );
		
		return data;
	}
	
	void var_string::reserve( size_type new_capacity )
	{
		// embiggen() will throw if new_capacity exceeds max_size()
		
		if ( new_capacity < max_offset )
		{
			new_capacity = max_offset;
		}
		
		embiggen( size(), new_capacity );
	}
	
	void var_string::resize( size_type new_size, char c )
	{
		// embiggen() will throw if new_size exceeds max_size()
		
		const size_type old_size = size();
		
		char* new_pointer = embiggen( new_size );
		
		if ( new_size > old_size )
		{
			std::fill( new_pointer + old_size, new_pointer + new_size, c );
		}
	}
	
	char* var_string::insert_uninitialized( char* p, size_type n )
	{
		check_size( n );
		
		const size_type old_size = size();
		const size_type new_size = old_size + n;
		
		// embiggen() will throw if new_size exceeds max_size()
		
		char* begin = const_cast< char* >( data() );
		char* end   = begin + old_size;
		
		ASSERT( begin <= p   );
		ASSERT( p     <= end );
		
		ASSERT( new_size >= old_size );  // catch integer overflow
		
		const size_type offset = p - begin;
		
		begin = embiggen( new_size );
		
		p    = begin + offset;
		end  = begin + old_size;
		
		std::copy_backward( p, end, begin + new_size );
		
		return p;
	}
	
	var_string& var_string::insert( size_type pos, const string& s )
	{
		if ( pos > size() )
		{
			throw std::out_of_range( __func__ );
		}
		
		return insert( pos, s.data(), s.size() );
	}
	
	var_string& var_string::insert( size_type pos, const string& s, size_type offset, size_type n )
	{
		const size_type s_size = s.size();
		
		if ( pos > size()  ||  offset > s_size )
		{
			throw std::out_of_range( __func__ );
		}
		
		n = std::min( n, s_size - offset );
		
		return insert( pos, s.data() + offset, n );
	}
	
	var_string& var_string::insert( size_type pos, const char* s, size_type n )
	{
		// insert_uninitialized() will throw if n exceeds max_size()
		
		if ( pos > size() )
		{
			throw std::out_of_range( __func__ );
		}
		
		char* data = mutable_data();
		
		memcpy( insert_uninitialized( data + pos, n ), s, n );
		
		return *this;
	}
	
	var_string& var_string::insert( size_type pos, const char* s )
	{
		return insert( pos, s, strlen( s ) );
	}
	
	var_string& var_string::insert( size_type pos, size_type n, char c )
	{
		// insert_uninitialized() will throw if n exceeds max_size()
		
		if ( pos > size() )
		{
			throw std::out_of_range( __func__ );
		}
		
		char* data = mutable_data();
		
		memset( insert_uninitialized( data + pos, n ), c, n );
		
		return *this;
	}
	
	void var_string::insert( char* p, char* i, char* j )
	{
		ASSERT( i <= j );
		
		const size_type n = j - i;
		
		// insert_uninitialized() will throw if n exceeds max_size()
		
		std::copy( i, j, insert_uninitialized( p, n ) );
	}
	
	void var_string::insert( char* p, size_type n, char c )
	{
		// insert_uninitialized() will throw if n exceeds max_size()
		
		memset( insert_uninitialized( p, n ), c, n );
	}
	
	char* var_string::insert( char* p, char c )
	{
		p = insert_uninitialized( p, 1 );
		
		*p = c;
		
		return p;
	}
	
	var_string& var_string::append( const char* p, size_type length )
	{
		check_size( length );
		
		if ( length )
		{
			ASSERT( p != NULL );
			
			ASSERT( p + length >= p );
		}
		
		const size_type old_size = size();
		const size_type new_size = old_size + length;
		
		// embiggen() will throw if new_size exceeds max_size()
		
		char* new_pointer = embiggen( new_size );
		
		memcpy( new_pointer + old_size, p, length );
		
		return *this;
	}
	
	var_string& var_string::append( const char* s )
	{
		ASSERT( s != NULL );
		
		const size_type length = strlen( s );
		
		return append( s, length );
	}
	
	var_string& var_string::append( size_type n, char c )
	{
		resize( size() + n, c );
		
		return *this;
	}
	
	var_string& var_string::append( const string& other, size_type pos, size_type n )
	{
		const size_type other_size = other.size();
		
		if ( pos > other_size )
		{
			throw std::out_of_range( __func__ );
		}
		
		n = std::min( n, other_size - pos );
		
		return append( other.data() + pos, n );
	}
	
	
	char* var_string::replace_setup( char* p, size_type m, difference_type delta )
	{
		// delta is signed and therefore can't exceed max_size()
		
		// insert_uninitialized() will throw if new size exceeds max_size()
		
		if ( delta == 0 )
		{
			return p;
		}
		
		char* q = p + m;
		
		if ( delta > 0 )
		{
			p = insert_uninitialized( q, delta ) - m;
		}
		else
		{
			erase_unchecked( q + delta, -delta );
		}
		
		return p;
	}
	
	var_string& var_string::replace( size_type pos, size_type m, const string& s )
	{
		if ( pos > size() )
		{
			throw std::out_of_range( __func__ );
		}
		
		return replace( pos, m, s.data(), s.size() );
	}
	
	var_string& var_string::replace( size_type pos, size_type m, const string& s, size_type offset, size_type n )
	{
		const size_type s_size = s.size();
		
		if ( pos > size()  ||  offset > s_size )
		{
			throw std::out_of_range( __func__ );
		}
		
		n = std::min( n, s_size - offset );
		
		return replace( pos, m, s.data() + offset, n );
	}
	
	var_string& var_string::replace( size_type pos, size_type m, const char* s, size_type n )
	{
		check_size( n );
		
		const size_type old_size = size();
		
		if ( pos > old_size )
		{
			throw std::out_of_range( __func__ );
		}
		
		m = std::min( m, old_size - pos );
		
		const difference_type delta = n - m;
		
		char* p = mutable_data() + pos;
		
		p = replace_setup( p, m, delta );
		
		memcpy( p, s, n );
		
		return *this;
	}
	
	var_string& var_string::replace( size_type pos, size_type m, const char* s )
	{
		return replace( pos, m, s, strlen( s ) );
	}
	
	var_string& var_string::replace( size_type pos, size_type m, size_type n, char c )
	{
		check_size( n );
		
		const size_type old_size = size();
		
		if ( pos > old_size )
		{
			throw std::out_of_range( __func__ );
		}
		
		m = std::min( m, old_size - pos );
		
		const difference_type delta = n - m;
		
		char* p = mutable_data() + pos;
		
		p = replace_setup( p, m, delta );
		
		memset( p, c, n );
		
		return *this;
	}
	
	void var_string::replace( char* p, char* q, const string& s )
	{
		replace( p, q, s.data(), s.size() );
	}
	
	void var_string::replace( char* p, char* q, const char *i, size_type n )
	{
		check_size( n );
		
		ASSERT( begin() <= p );
		
		ASSERT( q <= end() );
		
		ASSERT( p <= q );
		
		const char* j = i + n;
		
		ASSERT( j <= begin()  ||  i >= end() );
		
		const size_type m = q - p;
		
		const difference_type delta = n - m;
		
		p = replace_setup( p, m, delta );
		
		memcpy( p, i, n );
	}
	
	void var_string::replace( char* p, char* q, const char *s )
	{
		replace( p, q, s, strlen( s ) );
	}
	
	void var_string::replace( char* p, char* q, size_type n, char c )
	{
		check_size( n );
		
		ASSERT( begin() <= p );
		
		ASSERT( q <= end() );
		
		ASSERT( p <= q );
		
		const size_type m = q - p;
		
		const difference_type delta = n - m;
		
		p = replace_setup( p, m, delta );
		
		memset( p, c, n );
	}
	
	void var_string::replace( char* p, char* q, const char *i, const char *j )
	{
		ASSERT( i <= j );
		
		const size_type n = j - i;
		
		replace( p, q, i, n );
	}
	
}

