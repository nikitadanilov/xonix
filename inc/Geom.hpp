#if !defined( __GEOM_HPP__ )
#define __GEOM_HPP__

#include <quercus.hpp>

typedef short int Coord;
typedef Coord CoordX;
typedef Coord CoordY;

#include "Game.hpp"

class Point : public virtual QR::Object
{
  typedef enum { X, Y } Axe;
public: Point( CoordX aX, CoordY aY );
public: Point( const Point &aPoint );
public: virtual ~Point();

public: CoordX getX() const;
public: CoordY getY() const;
public: Coord  get( Axe anAxe ) const;

public: void setX( CoordX aX );
public: void setY( CoordY aY );
public: void set( Axe anAxe, Coord aCoord );

protected: CoordX x;
protected: CoordY y;
};

class XVector : public virtual QR::Object
{
public: XVector( Point anEndPoint );
public: XVector( Point aStartPoint, Point anEndPoint );
public: XVector( XVector &aVector );
public: virtual ~XVector();

public: Point &getEndPoint();

public: int scalarProduct( XVector aVector );
public: int vectorProduct( XVector aVector );

protected: Point end;
};

/* __GEOM_HPP__ */
#endif

