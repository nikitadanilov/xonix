#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Geom.hpp"

Point::Point( CoordX aX, CoordY aY )
{
  setX( aX );
  setY( aY );
}

Point::Point( const Point &aPoint )
{
  setX( aPoint.getX() );
  setY( aPoint.getY() );
}

Point::~Point()
{}

CoordX Point::getX() const
{
  return x;
}

CoordY Point::getY() const
{
  return y;
}

Coord  Point::get( Axe anAxe ) const
{
  switch( anAxe )
	{
	case X:
	  return getX();
	case Y:
	  return getY();
	default:
	  impossible_;
	  return 0;
	}
}

void Point::setX( CoordX aX )
{
  x = aX;
}

void Point::setY( CoordY aY )
{
  y = aY;
}

void Point::set( Axe anAxe, Coord aCoord )
{
  switch( anAxe )
	{
	case X:
	  setX( ( CoordX ) aCoord );
	  break;
	case Y:
	  setY( ( CoordY ) aCoord );
	  break;
	default:
	  impossible_;
	}
}

XVector::XVector( Point anEndPoint ) :
  end( anEndPoint )
{}
  
XVector::XVector( Point aStartPoint, Point anEndPoint ) :
  end( anEndPoint.getX() - aStartPoint.getX(), 
	   anEndPoint.getY() - aStartPoint.getY() )
{}
  
XVector::XVector( XVector &aVector ) :
  end( aVector.end )
{}

XVector::~XVector()
{}

Point &XVector::getEndPoint()
{
  return end;
}

int XVector::scalarProduct( XVector aVector )
{
  return 
	getEndPoint().getX() * aVector.getEndPoint().getX() + 
	getEndPoint().getY() * aVector.getEndPoint().getY();
}

int XVector::vectorProduct( XVector aVector )
{
  return 
	getEndPoint().getX() * aVector.getEndPoint().getY() - 
	getEndPoint().getY() * aVector.getEndPoint().getX();
}

