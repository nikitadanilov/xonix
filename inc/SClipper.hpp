#if !defined( __SCLIPPER_HPP__ )
#define __SCLIPPER_HPP__

#include "Clipper.hpp"
#include "Geom.hpp"
#include "GameObject.hpp"

#include <quercus.hpp>

class Segment;
class Cycle;
class Game;
class SClipper : virtual public QR::PassiveObserver,
				 virtual public GameObject
{
public: SClipper( Game *aGame );
public: virtual ~SClipper();

public: virtual QR::HandlerResult fire( QR::Condition *aCondition, 
									QR::ObservationContext *aContext );

public: virtual void clip();
public: void addPath();

public: QR::List *getCycles() const;

public: virtual void reset();

public: virtual QR::String toString() const;

protected: QR::List *cycles;
};

class Cycle : public virtual QR::Object
{
public: Cycle( double anInnerX, double anInnerY );
public: Cycle( Segment *anInitialSeg );
public: ~Cycle();

public: boolean isMarked() const;
public: void setMark( boolean aMark );

public: void add( Segment *aSegment );
public: Segment *getLast() const;
public: Segment *getNext() const;
public: Segment *getPrev() const;

public: Segment *contains( CoordX aX, CoordY aY ) const;
public: QR::List *getSegments() const;

public: boolean split( SClipper *aClipper, QR::List *aPath );
public: Cycle *buildSplit( Segment *aSource, Segment *aTarget,
						   CoordX aStartX, CoordY aStartY,
						   CoordX anEndX, CoordY anEndY,
						   QR::List *aPath, boolean aReverse );
public: void normalize();
public: boolean round( CoordX aX, CoordY aY ) const;

public: virtual QR::String toString() const;

protected: QR::List *segments;
protected: boolean mark;
protected: double innerX;
protected: double innerY;
};

class Board;
class Segment : public virtual QR::Object
{
public: Segment( CoordX aStartX, CoordY aStartY, 
				 CoordX anEndX, CoordY anEndY );

public: boolean contains( CoordX aX, CoordY aY );
public: Index fill( Board *aBoard, Game *aGame );

public: void swap();

public: virtual QR::String toString() const;

public: virtual Top *clone() const;

public: boolean isEmpty() const;

public: CoordX getMinX() const;
public: CoordY getMinY() const;

public: CoordX getMaxX() const;
public: CoordY getMaxY() const;

public: CoordX x0;
public: CoordY y0;

public: CoordX x1;
public: CoordY y1;

public: int dx;
public: int dy;
};

/* __SCLIPPER_HPP__ */
#endif

