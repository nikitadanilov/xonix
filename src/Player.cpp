#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Keyboard.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "BoardMap.hpp"
#include "Cell.hpp"
#include "SClipper.hpp"
#include "GameConfig.hpp"

#include <stdlib.h>

Player::Player( Game *aGame ) :
  GameThread( aGame, false, true )
{
  setProtector( THR_SYSTEM -> recMutex() );
  TCAST( getProtector(), QR::Object ) -> dontManage();

  PGuard _self( this, this );

  pathX = new Index
	[ 
	 getGame() -> getBoard() -> getWidth() * 
	 getGame() -> getBoard() -> getHeight() ];
  pathY = new Index
	[ 
	 getGame() -> getBoard() -> getWidth() * 
	 getGame() -> getBoard() -> getHeight() ];

  path = NULL;

  x = getGame() -> getBoard() -> getWidth() / 2;
  y = getGame() -> getBoard() -> getHeight() - 2;

  FINIT( keyboard, getGame() -> getKeyboard() );

  safePlace();
  getGame() -> addPassive( this );

  test_( getState() == Cell::head );
}

Player::~Player()
{
  PGuard _self( this, this );

  FDONE( keyboard );

  getGame() -> removePassive( this );
  delete []pathX;
  delete []pathY;
  FCDONE( path );
}

boolean Player::iteration()
{
  int dx;
  int dy;

  {
	OGuard _lock( keyboard, this );

	dx = getGame() -> getKeyboard() -> getDx();
	dy = getGame() -> getKeyboard() -> getDy();
  }

  if( ( dx != 0 ) || ( dy != 0 ) )
	{
	  OGuard _game( game, this );
	  Board *board;
	  
	  Game::State newState;
	  Index ox;
	  Index oy;

	  board = getGame() -> getBoard();

	  ox = x;
	  oy = y;
		  
	  newState = Game::normal;

	  {
		OGuard _self( this, this );
		board -> updateCellInfo( x, y, prevState, version + 1 );
		getGame() -> drawCell( x, y );

		if( board -> isValidCol( x + dx ) )
		  {
			x += dx;
		  }
		if( board -> isValidRow( y + dy ) )
		  {
			y += dy;
		  }
		// Debug::getLogger() -> log( "x: %i y: %i", x, y );
		switch( getState() )
		  {
		  case Cell::vacuum:
			{
			  if( state == inSolid )
				{
				  test_( takeOffX == -1 );
				  test_( takeOffY == -1 );
					  
				  state = inSpace;
				  pathLength = 0;
				  newState = Game::path;
				  takeOffX = ox;
				  takeOffY = oy;
				}
			  prevState = Cell::path;
			  version = -2;
			  addToPath( x, y );
			  break;
			}
		  case Cell::solid:
		  case Cell::border:
			{
			  getGame() -> getBoard() -> getCellInfo
				( x, y, &prevState, &version );
			  if( state != inSolid )
				{
				  if( ( x == takeOffX ) && ( y == takeOffY ) &&
					  ( pathLength == 1 ) )
					{
					  x = ox;
					  y = oy;
					  newState = Game::hit;
					  break;
					}
				  else
					{
					  state = inSolid;
					  newState = Game::clipped;
					  addToPath( x, y );
					  buildPath();
					  takeOffX = -1;
					  takeOffY = -1;
					  version = -2;
					}
				}
			  break;
			}
		  case Cell::path:
		  case Cell::ibomb:
		  case Cell::obomb:
		  case Cell::xbomb:
		  case Cell::sbomb:
			{
			  x = ox;
			  y = oy;
			  newState = Game::hit;
			  break;
			}
		  case Cell::head:
		  default:
			{
			  impossible_;
			}
		  }
		board -> setState( x, y, Cell::head );
		getGame() -> drawCell( x, y );

		test_( getState() == Cell::head );

	  }
	  getGame() -> setState( newState );
	  getGame() -> update();
	}
  else
	{
	  getGame() -> drawCell( x, y );
	}
  return true;
}

QR::HandlerResult Player::fire( QR::Condition *aCondition, 
							QR::ObservationContext *aContext )
{
  preC_( aCondition != NULL );
  preC_( aContext != NULL );

  OGuard _self( this, this );
  
  OGuard _condition( aCondition, this );
  OGuard _context( aContext, this );

  test_( getState() == Cell::head );
  if( getGame() -> getState() == Game::hit )
	{
	  //	  Debug::getLogger() -> log( "Damn!" );
	  // 	  {
	  // 		OGuard _lock( getGame() -> getDisplay(), this );
	  // 		::XBell( getGame() -> getDisplay() -> getData(), 1 );
	  // 	  }
	  
	  OGuard _board( getGame() -> getBoard(), this );
	  Board *board;
	  
	  board = getGame() -> getBoard();

	  if( state == inSpace )
		{
		  for( Index i = 0 ; i < pathLength ; ++i )
			{
			  Cell::State cellState;

			  cellState = board -> getState( pathX[ i ], pathY[ i ] );
			  test_( ( cellState == Cell::path ) || 
					 ( cellState == Cell::head ) );
			  board -> setState( pathX[ i ], pathY[ i ], Cell::vacuum );
			  getGame() -> drawCell( pathX[ i ], pathY[ i ] );
			}
		  test_( getState() == Cell::vacuum );
		  test_( takeOffX >= 0 );
		  test_( takeOffY >= 0 );
		  x = takeOffX;
		  y = takeOffY;
		}
	  else
		{
		  test_( takeOffX == -1 );
		  test_( takeOffY == -1 );
		  test_( getState() == Cell::head );
		  board -> updateCellInfo( x, y, prevState, -1 );
		  getGame() -> drawCell( x, y );
		  x = board -> getWidth() / 2;
		  y = board -> getHeight() - 2;
		}
	  safePlace();
	  getGame() -> incLives( -1 );
	  if( getGame() -> getLives() < 0 )
		{
		  getGame() -> setState( Game::over );
		}
	}
  test_( getState() == Cell::head );
  return 0;
}

QR::HandlerResult Player::firePost( QR::Condition *aCondition, 
								QR::ObservationContext *aContext )
{
  preC_( aCondition != NULL );
  preC_( aContext != NULL );

  OGuard _self( this, this );
  
  OGuard _condition( aCondition, this );
  OGuard _context( aContext, this );

  if( ( getGame() -> getState() == Game::hit ) ||
	  ( getGame() -> getState() == Game::clipped ) )
	{
	  getGame() -> setState( Game::normal );
	}
  return 0;
}

void Player::reset()
{
  x = getGame() -> getBoard() -> getWidth() / 2;
  y = getGame() -> getBoard() -> getHeight() - 2;

  safePlace();
}

void Player::safePlace()
{
  OGuard _self( this, this );
  
  while( ( getState() != Cell::solid ) &&
		 ( getState() != Cell::border ) )
	{
	  x = rand() % ( getGame() -> getBoard() -> getWidth() );
	  y = rand() % ( getGame() -> getBoard() -> getHeight() );
	}
	  
  takeOffX = -1;
  takeOffY = -1;

  state = inSolid;
  pathLength = 0;

  getGame() -> getBoard() -> getCellInfo( x, y, &prevState, &version );
  getGame() -> getBoard() -> setState( x, y, Cell::head );
  getGame() -> drawCell( x, y );
}

Index Player::getDelay()
{
  return getGame() -> config -> playerDelay;
}

int Player::getX() const
{
  return x;
}

int Player::getY() const
{
  return y;
}

QR::List *Player::getPath() const
{
  preC_( path != NULL );
  return path;
}

void Player::buildPath()
{
  preC_( takeOffX >= 0 );
  preC_( takeOffY >= 0 );
  preC_( pathLength > 0 );

  QR::List *result;

  int xStart;
  int yStart;

  int xLast;
  int yLast;

  int dx;
  int dy;

  result = new QR::List();
  PGuard _result( result, this );

  xLast = pathX[ 0 ];
  yLast = pathY[ 0 ];

  xStart = takeOffX;
  yStart = takeOffY;

  dx = xLast - xStart;
  dy = yLast - yStart;

  for( int i = 1 ; i < pathLength ; ++i )
	{
	  int pX;
	  int pY;

	  pX = pathX[ i ];
	  pY = pathY[ i ];

	  if( ( ( pX - xLast ) != dx ) || ( ( pY - yLast ) != dy ) )
		{
		  result -> append( new Segment( xStart, yStart, xLast, yLast ) );
		  dx = pX - xLast;
		  dy = pY - yLast;
		  xStart = xLast;
		  yStart = yLast;
		}
	  xLast = pX;
	  yLast = pY;
	}
  result -> append( new Segment( xStart, yStart, xLast, yLast ) );
  FMOVE( path, result );
  //Debug::getLogger() -> logObject( "Player::buildPath(): result: %s", result );
}

Cell::State Player::getState() const
{
  return getGame() -> getBoard() -> getState( x, y );
}

void Player::addToPath( int aX, int aY )
{
  preC_( pathLength < 
		 getGame() -> getBoard() -> getWidth() * 
		 getGame() -> getBoard() -> getHeight() );

  pathX[ pathLength ] = aX;
  pathY[ pathLength ] = aY;
  ++pathLength;
}

