#pragma once

#include <vector>

template< typename _Key, typename _Value >
class CDictionary :
	public std::vector<std::pair<_Key, _Value>>
{
public:
	void		push_back( const _Key& key, const _Value& val );
	const bool	contains( const _Key& key ) const;
	const _Value&	get_value( const _Key& key ) const;
	_Value&			get_value( const _Key& key );
};


template< typename _Key, typename _Value >
void	CDictionary<_Key, _Value>::push_back( const _Key& key, const _Value& val ) {
	std::vector<std::pair<_Key, _Value>>::push_back( std::pair<_Key, _Value>( key, val ) );
}

template< typename _Key, typename _Value >
const bool	CDictionary<_Key, _Value>::contains( const _Key& key ) const {
	for( auto it = this->cbegin(); it != this->cend(); it++ ) {
		if( it->first == key )
			return true;
	}
	return false;
}

template< typename _Key, typename _Value >
const _Value&	CDictionary<_Key, _Value>::get_value( const _Key& key ) const {
	for( auto it = this->cbegin(); it != this->cend(); it++ ) {
		if( it->first == key )
			return it->second;
	}
	throw std::exception("key not found");
}

template< typename _Key, typename _Value >
_Value&			CDictionary<_Key, _Value>::get_value( const _Key& key ) {
	for( auto it = this->begin(); it != this->end(); it++ ) {
		if( it->first == key )
			return it->second;
	}
	throw std::exception("key not found");
}
