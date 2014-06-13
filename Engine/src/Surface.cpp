#include "stdafx.h"
#include "Surface.h"

CSurface::CSurface() {
}

CSurface::CSurface( const glm::vec3* const verts ) {
	this->Set( verts );
}

const CPlane&	CSurface::GetPlane() const {
	return this->m_SurfacePlane;
}

const CPlane&	CSurface::GetEdgePlane( const unsigned edge ) const {
	return this->m_EdgePlane[ edge ];
}

const glm::vec3&	CSurface::GetVert( const unsigned vert ) const {
	return this->m_Verts[ vert ];
}

const bool	CSurface::Intersects( const glm::vec3& rayOrigin, const glm::vec3& rayVector ) const {
	glm::vec3 pos;
	return this->Intersects( rayOrigin, rayVector, pos ); 
}

const bool	CSurface::Intersects( const glm::vec3& rayOrigin, const glm::vec3& rayVector, glm::vec3& outPosition ) const {
	if( !this->m_SurfacePlane.Intersects( rayOrigin, rayVector, outPosition ) )
		return false;

	bool ok = true;
	for( unsigned i = 0; i < 4; i++ ){
		if( this->m_EdgePlane[i].Distance( outPosition ) < 0.0f )
			ok = false;
	}

	return ok;
}

void	CSurface::Set( const glm::vec3* const verts ) {
	for( unsigned i = 0; i < 4; i++ ) {
		this->m_Verts[i] = verts[i];
	}

	this->m_SurfacePlane.Set( this->m_Verts[0], this->m_Verts[1], this->m_Verts[2] );

	for( unsigned i = 0; i < 4; i++ ) {
		const glm::vec3& v1 = this->m_Verts[ ( i + 0 ) % 4 ];
		const glm::vec3& v2 = this->m_Verts[ ( i + 1 ) % 4 ];

		const glm::vec3 normal = glm::normalize( glm::cross( this->m_SurfacePlane.N, v2 - v1 ) );
		this->m_EdgePlane[ i ].Set( normal, v1 );
	}
}

void	CSurface::Set( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 ){
	std::vector<glm::vec3> v;
	v.push_back(v0);
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);

	this->Set( &v[0] );
}

const bool	CSurface::IsEqual( const CSurface& surface, const bool sameSide ) const {
	if( !this->m_SurfacePlane.IsEqual( surface.GetPlane(), sameSide ) )
		return false;

	unsigned same = 0;
	for( unsigned i = 0; i < 4; i++ ) {
		for( unsigned j = 0; j < 4; j++ ) {
			if( this->m_Verts[i] == surface.GetVert( j ) )
				same ++;
		}
	}

	return same == 4;
}

const bool	CSurface::operator==( const CSurface& surface ) const {
	return IsEqual( surface, true );
}

const bool	CSurface::operator!=( const CSurface& surface ) const {
	return !IsEqual( surface, true );
}

const bool	CSurface::operator<( const CSurface& surface ) const { 
	return this->m_SurfacePlane < surface.m_SurfacePlane &&
		VectorLess( this->m_Verts[0], surface.m_Verts[0] ) &&
		VectorLess( this->m_Verts[1], surface.m_Verts[1] ) &&
		VectorLess( this->m_Verts[2], surface.m_Verts[2] ) &&
		VectorLess( this->m_Verts[3], surface.m_Verts[3] );
}