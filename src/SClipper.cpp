#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "SClipper.hpp"
#include "BombThread.hpp"
#include "BoardMap.hpp"
#include "Board.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Player.hpp"

#include <stdlib.h>

#define SGN( x ) ( ( ( x ) == 0 ) ? 0 : ( ( ( x ) > 0 ) ? +1 : -1 ) )

SClipper::SClipper( Game *aGame ) :
  GameObject( aGame )
{
  reset();
}

SClipper::~SClipper()
{
  FDONE( cycles );
}

QR::HandlerResult SClipper::fire( QR::Condition *, 
							  QR::ObservationContext * )
{
  if( getGame() -> getState() == Game::clipped )
	{
	  OGuard _game( getGame(), this );
	  OGuard _board( getGame() -> getBoard(), this );
	  OGuard _player( getGame() -> getPlayer(), this );

	  for( int i = 0 ; i < getGame() -> getPlayer() -> pathLength ; ++i )
		{
		  getGame() -> getBoard() -> setState
			( getGame() -> getPlayer() -> pathX[ i ],
			  getGame() -> getPlayer() -> pathY[ i ], Cell::border );
		  getGame() -> drawCell
			( getGame() -> getPlayer() -> pathX[ i ],
			  getGame() -> getPlayer() -> pathY[ i ] );
		}
 	  addPath();
	  clip();
	  getGame() -> getBoard() -> setState
		( getGame() -> getPlayer() -> getX(), 
		  getGame() -> getPlayer() -> getY(),
		  Cell::head );
	  getGame() -> drawCell
		( getGame() -> getPlayer() -> getX(), 
		  getGame() -> getPlayer() -> getY() );
	}
  return 0;
}

void SClipper::addPath()
{
  //  Debug::getLogger() -> log( "SClipper::addPath()" );

  QR::List *segs;
  Segment *launchPad;
  Cycle *cycle;
  CoordX takeOffX;
  CoordX takeOffY;

  segs = getGame() -> getPlayer() -> getPath();
  OGuard _segs( segs, this );
//   Debug::getLogger() -> log( "SClipper::addPath(): path: %p", segs );
//   Debug::getLogger() -> logObject( "SClipper::addPath(): path: %s", segs );

  segs -> toFirst();
  takeOffX = TCAST( segs -> getCurrentValue(), Segment ) -> x0;
  takeOffY = TCAST( segs -> getCurrentValue(), Segment ) -> y0;

//   Debug::getLogger() -> log
// 	( "SClipper::addPath(): tx: %i: ty: %i", takeOffX, takeOffY );

  launchPad = NULL;
  cycle = NULL;
  for( cycles -> toFirst() ; !cycles -> atEnd() ; cycles -> next() )
	{
	  cycle = TCAST( cycles -> getCurrentValue(), Cycle );
	  launchPad = cycle -> contains( takeOffX, takeOffY );
// Debug::getLogger() -> logObject( "SClipper::addPath(): cycle: %s", cycle );
	  if( launchPad != NULL )
		{
// 		  Debug::getLogger() -> 
// 			logObject( "SClipper::addPath(): launchPad: %s", launchPad );
		  if( cycle -> split( this, segs ) )
			{
			  cycles -> remove(); /* no way back */
			  break;
			}
		}
	}
  test_( launchPad != NULL );
  test_( cycle != NULL );
}

void SClipper::clip()
{
  OGuard _self( this, this );

  //  Debug::getLogger() -> log( "SClipper::clip()" );
  for( cycles -> toFirst() ; ! cycles -> atEnd() ; cycles -> next() )
	{
	  TCAST( cycles -> getCurrentValue(), Cycle ) -> setMark( false );
	}
  for( getGame() -> getBombs() -> toFirst() ; 
	   ! getGame() -> getBombs() -> atEnd() ;
	   getGame() -> getBombs() -> next() )
	{
	  BombThread *bomb;

	  bomb = TCAST( getGame() -> getBombs() -> getCurrentValue(), BombThread );
	  OGuard _bomb( bomb, this );

	  if( bomb -> affectsClipping() )
		{
		  boolean found;

		  found = false;
		  for( cycles -> toFirst() ; !found && !cycles -> atEnd() ; 
			   cycles -> next() )
			{
			  Cycle *cycle;

			  cycle = TCAST( cycles -> getCurrentValue(), Cycle );
			  OGuard _cycle( cycle, this );
			  
			  test_( cycles != NULL );
			  if( cycle -> round( bomb -> getX(), bomb -> getY() ) )
				{
				  cycle -> setMark( true );
// 				  Debug::getLogger() -> logObject
// 					( "SClipper::clip(): cycle %s marked", cycle );
				  found = true;
				  getGame() -> incScore( 100 );
				  break;
				}
			}
		  test( found, "Bomb not within cycle!" );
		}
	}
  for( cycles -> toFirst() ; ! cycles -> atEnd() ; )
	{
	  Cycle *cycle;

	  cycle = TCAST( cycles -> getCurrentValue(), Cycle );

	  OGuard _cycle( cycle, this );
	  if( !cycle -> isMarked() )
		{
		  for( cycle -> getSegments() -> toFirst() ; 
			   !cycle -> getSegments() -> atEnd() ; 
			   cycle -> getSegments() -> next() )
			{
			  Segment *seg;

			  seg = TCAST( cycle -> getSegments() -> getCurrentValue(), 
						   Segment );
			  test_( seg != NULL );

			  getGame() -> incScore
				( seg -> fill( getGame() -> getBoard(), getGame() ) );
			}
		  cycles -> remove();
		}
	  else
		{
		  cycles -> next();
		}
	}
}

QR::List *SClipper::getCycles() const
{
  return cycles;
}

String SClipper::toString() const
{
  return QR::Object::toString() +
	" cycles: " + cycles -> toString();
}

void SClipper::reset()
{
  Cycle *boot;

  LinkManager::move( this, cycles, NULL );
  FINIT( cycles, new QR::List() );

  boot = new Cycle( getGame() -> getBoard() -> getWidth() / 2, 
					getGame() -> getBoard() -> getHeight() / 2 );
  cycles -> append( boot );

  boot -> add( new Segment /* right border */
			   ( getGame() -> getBoard() -> getWidth() - 2, 
				 1,
				 getGame() -> getBoard() -> getWidth() - 2,
				 getGame() -> getBoard() -> getHeight() - 2 ) );
  boot -> add( new Segment /* upper border */
			   ( 1, 
				 getGame() -> getBoard() -> getHeight() - 2, 
				 getGame() -> getBoard() -> getWidth() - 2,
				 getGame() -> getBoard() -> getHeight() - 2 ) );
  boot -> add( new Segment /* left border */
			   ( 1,
				 getGame() -> getBoard() -> getHeight() - 2,
				 1, 1 ) );  
  boot -> add( new Segment /* down border */
			   ( getGame() -> getBoard() -> getWidth() - 2,
				 1,
				 1, 1 ) );  
}

Cycle::Cycle( double anInnerX, double anInnerY )
{
  FINIT( segments, new QR::List() );
  mark = false;
  innerX = anInnerX;
  innerY = anInnerY;
}

Cycle::Cycle( Segment *anInitialSeg )
{
  preC_( anInitialSeg != NULL );

  FINIT( segments, new QR::List() );
  mark = false;
  getSegments() -> append( anInitialSeg );
}

Cycle::~Cycle()
{
  FDONE( segments );
}

boolean Cycle::isMarked() const
{
  return mark;
}

void Cycle::setMark( boolean aMark )
{
  mark = aMark;
}

void Cycle::add( Segment *aSegment )
{
  preC_( aSegment != NULL );
  OGuard _self( this, this );

  //  Debug::getLogger() -> logObject( "Cycle::add(): %s", aSegment );

  if( aSegment -> isEmpty() )
	{
	  return;
	}

  if( getSegments() -> isEmpty() )
	{
	  CoordX rX;
	  CoordY rY;

	  rX = aSegment -> x1 - aSegment -> x0;
	  rY = aSegment -> y1 - aSegment -> y0;

	  if( ( ( ( double ) rX ) * ( innerY - ( double ) aSegment -> y0 ) - 
			( ( double ) rY ) * ( innerX - ( double ) aSegment -> x0 ) ) < 0 )
		{
		  aSegment -> swap();
		}
	}
  else
	{
	  Segment *last;

	  last = getLast();
	  if( ( aSegment -> x0 != last -> x1 ) ||
		  ( aSegment -> y0 != last -> y1 ) )
		{
		  test_( ( aSegment -> x1 == last -> x1 ) &&
				 ( aSegment -> y1 == last -> y1 ) );
		  aSegment -> swap();
		}
	  test_( ( aSegment -> x0 == last -> x1 ) &&
			 ( aSegment -> y0 == last -> y1 ) );

	}

  aSegment -> dy = SGN( aSegment -> x1 - aSegment -> x0 );
  aSegment -> dx = -SGN( aSegment -> y1 - aSegment -> y0 );

  getSegments() -> append( aSegment );
  /* normal is counterclockwise from vector */
  postC_( ( ( aSegment -> x1 - aSegment -> x0 ) * ( aSegment -> dy ) - 
			( aSegment -> y1 - aSegment -> y0 ) * ( aSegment -> dx ) ) > 0 );
}

boolean Cycle::split( SClipper *aClipper, QR::List *aPath )
{
  preC_( aClipper != NULL );
  preC_( aPath != NULL );
  preC_( getSegments() -> getCurrentValue() != NULL );

  //  Debug::getLogger() -> log( "Cycle::split()" );

  OGuard _self( this, this );

  Segment *start;
  Segment *end;

  CoordX startX;
  CoordY startY;

  CoordX endX;
  CoordY endY;

  Segment *source;
  Segment *target;

  Cycle *outer;
  Cycle *inner;

  aPath -> toFirst();
  start = TCAST( aPath -> getCurrentValue(), Segment );
  aPath -> toLast();
  end = TCAST( aPath -> getCurrentValue(), Segment );

//   Debug::getLogger() -> logObject( "Cycle::split(): start: %s", start );
//   Debug::getLogger() -> logObject( "Cycle::split(): end: %s", end );

  startX = start -> x0;
  startY = start -> y0;

  endX = end -> x1;
  endY = end -> y1;

  source = TCAST( getSegments() -> getCurrentValue(), Segment );
  OGuard _source( source, this );
//   Debug::getLogger() -> logObject( "Cycle::split(): source: %s", source );

  if( SGN( start -> x1 - startX ) != SGN( source -> dx ) ||
	  SGN( start -> y1 - startY ) != SGN( source -> dy ) )
	{
	  if( ( startX == source -> x0 ) && ( startY == source -> y0 ) )
		{
//  		  Debug::getLogger() -> logObject
//  			( "Cycle::split(): source: prev!: %s", source );
		  source = getPrev();
		}
	  else if( ( startX == source -> x1 ) && ( startY == source -> y1 ) )
		{
//  		  Debug::getLogger() -> logObject
//  			( "Cycle::split(): source: next!: %s", source );
		  source = getNext();
		}
	}
  if( !( SGN( start -> x1 - startX ) == SGN( source -> dx ) &&
		 SGN( start -> y1 - startY ) == SGN( source -> dy ) ) )
	{
	  test_( !round( start -> x1, start -> y1 ) );
	  return false;
	}
  test_( SGN( start -> x1 - startX ) == SGN( source -> dx ) &&
		 SGN( start -> y1 - startY ) == SGN( source -> dy ) );

//   Debug::getLogger() -> log
// 	( "Cycle::split(): start: %i %i end: %i %i", startX, startY, endX, endY );

  test_( source -> contains( startX, startY ) );
  target = contains( endX, endY );
  OGuard _target( target, this );
  test_( target != NULL );
  //  Debug::getLogger() -> logObject( "Cycle::split(): target: %s", target );

  if( ( SGN( end -> x0 - endX ) != SGN( target -> dx ) ) ||
	  ( SGN( end -> y0 - endY ) != SGN( target -> dy ) ) )
	{
	  if( ( endX == target -> x0 ) && ( endY == target -> y0 ) )
		{
		  target = getPrev();
		}
	  else if( ( endX == target -> x1 ) && ( endY == target -> y1 ) )
		{
		  target = getNext();
		}
	  else
		{
		  impossible_;
		}
	}
  test_( ( SGN( end -> x0 - endX ) == SGN( target -> dx ) ) &&
		 ( SGN( end -> y0 - endY ) == SGN( target -> dy ) ) );

  test_( getSegments() -> getCurrentValue() == target );
  outer = buildSplit
	( source, target, startX, startY, endX, endY, aPath, false );
  OGuard _outer( outer, this );
  //  Debug::getLogger() -> logObject( "Cycle::split(): outer: %s", outer );

  test_( getSegments() -> getCurrentValue() == source );
  inner = buildSplit
	( target, source, endX, endY, startX, startY, aPath, true );
  OGuard _inner( inner, this );
  //  Debug::getLogger() -> logObject( "Cycle::split(): inner: %s", inner );

  aClipper -> getCycles() -> append( outer );
  aClipper -> getCycles() -> append( inner );
  return true;
}

Cycle *Cycle::buildSplit( Segment *aSource, Segment *aTarget,
						  CoordX aStartX, CoordY aStartY,
						  CoordX anEndX, CoordY anEndY,
						  QR::List *aPath, boolean aReverse )
{
  Segment *boot;
  Cycle *result;

//   Debug::getLogger() -> logObject( "Cycle::buildSplit(): %s", this );
//   Debug::getLogger() -> log( "Cycle::buildSplit()0: segs: %i path: %i", 
// 							 getSegments() -> atEnd(), aPath -> atEnd() );

  boot = new Segment( aSource -> x0, aSource -> y0, aStartX, aStartY );
  OGuard _boot( boot, this );

  boot -> dx = aSource -> dx;
  boot -> dy = aSource -> dy;
  if( ( aTarget == aSource ) && boot -> contains( anEndX, anEndY ) )
	{
	  boot -> x0 = anEndX;
	  boot -> y0 = anEndY;
	}
  result = new Cycle( boot );
  PGuard _result( result, this );
  if( ! aReverse )
	{
// 	  Debug::getLogger() -> log( "Cycle::buildSplit()1: segs: %i path: %i", 
// 								 getSegments() -> atEnd(), aPath -> atEnd() );
	  for( aPath -> toFirst() ; ! aPath -> atEnd() ; aPath -> next() )
		{
		  result -> add
			( TCAST( aPath -> getCurrentValue() -> clone(), Segment ) );
		}
	}
  else
	{
	  for( aPath -> toLast() ; ! aPath -> isFirst() ; aPath -> prev() )
		{
		  result -> add
			( TCAST( aPath -> getCurrentValue() -> clone(), Segment ) );
		}
	  result -> add
		( TCAST( aPath -> getCurrentValue() -> clone(), Segment ) );
	}

//   Debug::getLogger() -> log( "Cycle::buildSplit()2: segs: %i path: %i", 
// 							 getSegments() -> atEnd(), aPath -> atEnd() );
  if( ( aTarget != aSource ) || !boot -> contains( anEndX, anEndY ) )
	{
	  result -> add( new Segment( anEndX, anEndY, 
								  aTarget -> x1, aTarget -> y1 ) );
	  test_( getSegments() -> getCurrentValue() == aTarget );
	  for( getNext() ; 
		   getSegments() -> getCurrentValue() != aSource ;
		   getNext() )
		{
		  result -> add( TCAST( getSegments() -> getCurrentValue() -> clone(),
								Segment ) );
		}
	}
  result -> normalize();
  return result;
}

void Cycle::normalize()
{
  Index len;
  Segment *current;
  Segment *next;

  for( getSegments() -> toFirst() ; ! getSegments() -> isLast() ; )
	{
	  Segment *seg;

	  seg = TCAST( getSegments() -> getCurrentValue(), Segment );
	  if( seg -> isEmpty() )
		{
		  getSegments() -> remove();
		}
	  else
		{
		  getSegments() -> next();
		}
	  //	  Debug::getLogger() -> log( "Cycle::normalize(): inner loop-0" );
	}
  len = getSegments() -> length();
  for( Index i = 0 ; i < len + 2 ; ++i )
	{
	  current = TCAST( getSegments() -> getCurrentValue(), Segment );
	  next = getNext();
	  
	  test_( ( current -> x1 == next -> x0 ) &&
			 ( current -> y1 == next -> y0 ) );
	  if( Segment( current -> x0, current -> y0,
				   next -> x1, next -> y1 ).
		  contains( current -> x1, current -> y1 ) )
		{
		  current -> x1 = next -> x1;
		  current -> y1 = next -> y1;
		  
		  getSegments() -> remove();
		  getSegments() -> toFirst();
		}
	}
}

Segment *Cycle::getLast() const
{
  OGuard _segments( getSegments(), this );
  getSegments() -> toLast();
  return TCAST( getSegments() -> getCurrentValue(), Segment );
}

Segment *Cycle::getNext() const
{
  OGuard _segments( getSegments(), this );
  if( ( getSegments() -> atEnd() ) || ( getSegments() -> isLast() ) )
	{
	  getSegments() -> toFirst();
	}
  else
	{
	  getSegments() -> next();
	}
  test_( !getSegments() -> atEnd() );
  return TCAST( getSegments() -> getCurrentValue(), Segment );
}

Segment *Cycle::getPrev() const
{
  OGuard _segments( getSegments(), this );
  if( getSegments() -> isFirst() )
	{
	  getSegments() -> toLast();
	}
  else
	{
	  getSegments() -> prev();
	}
  return TCAST( getSegments() -> getCurrentValue(), Segment );
}

Segment *Cycle::contains( CoordX aX, CoordY aY ) const
{
  OGuard _self( this, this );
  for( getSegments() -> toFirst() ; ! getSegments() -> atEnd() ; 
	   getSegments() -> next() )
	{
	  Segment *seg;

	  seg = TCAST( getSegments() -> getCurrentValue(), Segment );
	  PGuard _seg( seg, this );

	  if( seg -> contains( aX, aY ) )
		{
		  return seg;
		}
	}
  return NULL;
}

QR::List *Cycle::getSegments() const
{
  return segments;
}

boolean Cycle::round( CoordX aX, CoordY aY ) const
{
  for( CoordY y = aY ; y >= 0 ; --y )
	{
	  Segment *hit;

	  hit = contains( aX, y );
	  if( hit != NULL )
		{
		  if( hit -> dy == 0 )
			{
			  while( !hit -> contains( aX, y ) || ( hit -> dy == 0 ) )
				{
				  hit = getNext();
				}
			}
		  test_( hit -> contains( aX, y ) && ( hit -> dy != 0 ) );
		  return( SGN( aY - y ) == SGN( hit -> dy ) );
		}
	}
  return false;
}

String Cycle::toString() const
{
  return QR::Object::toString() +
	" segments: " + getSegments() -> toString();
}

Segment::Segment( CoordX aStartX, CoordY aStartY, 
				  CoordX anEndX, CoordY anEndY )
{
  x0 = aStartX;
  y0 = aStartY;
  x1 = anEndX;
  y1 = anEndY;
}

boolean Segment::contains( CoordX aX, CoordY aY )
{
  if( x0 == x1 ) /* vertical */
	{
	  return( ( aX == x0 ) && 
			  ( abs( aY - y0 ) + abs( y1 - aY ) == abs( y1 - y0 ) ) );
	}
  else if( y0 == y1 ) /* horizontal */
	{
	  return( ( aY == y0 ) && 
			  ( abs( aX - x0 ) + abs( x1 - aX ) == abs( x1 - x0 ) ) );
	}
  else /* not possible in classical game */
	{
	  return( ( ( ( double ) ( aY - y0 ) ) / ( ( double ) ( aX - x0 ) ) ) == 
			  ( ( ( double ) ( y1 - y0 ) ) / ( ( double ) ( x1 - x0 ) ) ) );
	}

}

Index Segment::fill( Board *aBoard, Game *aGame )
{
  preC_( aBoard != NULL );
  preC_( aGame != NULL );

  Index filled;

  filled = 0;
  for( CoordX x = getMinX() ; x <= getMaxX() ; ++x )
	{
	  for( CoordY y = getMinY() ; y <= getMaxY() ; ++y )
		{
		  CoordX xT;
		  CoordY yT;

		  for( xT = x + dx, yT = y + dy ;
			   aBoard -> isValidRow( yT ) && aBoard -> isValidCol( xT ) &&
				 ( ( aBoard -> getState( xT, yT ) == Cell::vacuum ) ||
				   ( aBoard -> getState( xT, yT ) == Cell::xbomb ) );
			   ( xT += dx, yT += dy ) )
			{
			  aBoard -> setState( xT, yT, Cell::solid );
			  aGame -> drawCell( xT, yT );
			  ++filled;
			}
		}
	}
  return filled;
}

String Segment::toString() const
{
  return QR::Object::toString() + 
	String( " (%i, %i)-(%i, %i)[%i, %i]" ).sprintf( 1, x0, y0, x1, y1, dx, dy );
}

CoordX Segment::getMinX() const
{
  return ( x0 < x1 ) ? x0 : x1;
}

CoordY Segment::getMinY() const
{
  return ( y0 < y1 ) ? y0 : y1;
}

CoordX Segment::getMaxX() const
{
  return ( x0 < x1 ) ? x1 : x0;
}

CoordY Segment::getMaxY() const
{
  return ( y0 < y1 ) ? y1 : y0;
}

void Segment::swap()
{
  CoordX tX;
  CoordY tY;
		  
  tX = x0;
  tY = y0;

  x0 = x1;
  y0 = y1;

  x1 = tX;
  y1 = tY;
}

Top *Segment::clone() const
{
  Segment *result;

  result = new Segment( x0, y0, x1, y1 );
  result -> dx = dx;
  result -> dy = dy;
  return result;
}

boolean Segment::isEmpty() const
{
  return( ( x0 == x1 ) && ( y0 == y1 ) );
}

