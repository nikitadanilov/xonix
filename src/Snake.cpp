#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Snake.hpp"
#include "Board.hpp"
#include "BoardMap.hpp"
#include "Cell.hpp"
#include "Game.hpp"
#include "GameConfig.hpp"

#include <stdlib.h>

Snake::Snake( Game *aGame ) :
  GameThread( aGame )
{
  cells = new List();
  LinkManager::reg( this, cells );
}

Snake::~Snake()
{
  LinkManager::free( this, cells );
}

void Snake::init()
{
  GameThread::init();
  findSafe();
}

void Snake::findSafe()
{
  x = -1;
  y = -1;
  for( int i = 0 ; 
	   i < getGame() -> getBoard() -> getWidth() * 
		 getGame() -> getBoard() -> getHeight() ; 
	   ++i )
	{
	  x = rand() % ( getGame() -> getBoard() -> getWidth() );
	  y = rand() % ( getGame() -> getBoard() -> getHeight() );
	  if( isAllowed( x, y, getGame() -> getBoard() -> getState( x, y ) ) )
		{
		  break;
		}
	}
  if( x == -1 )
	{
	  throw new InternalError( "Cannot find vacuum in board", null );
	}
  initDXY();
  getGame() -> getBoard() -> 
	getCellInfo( getX(), getY(), &prevState, &version );
  getGame() -> getBoard() -> setState( getX(), getY(), getMyState() );
  getGame() -> drawCell( getX(), getY() );
}

void Snake::initDXY()
{
  dx = ( ( rand() % 2 ) == 0 ) ? +1 : -1;
  dy = ( ( rand() % 2 ) == 0 ) ? +1 : -1;
}

boolean Snake::isClinamenable() const
{
  return true;
}

boolean Snake::iteration()
{
  Index nX;
  Index nY;

  Board *board;

  board = getGame() -> getBoard();

  board -> updateCellInfo( getX(), getY(), prevState, version + 1 );
  getGame() -> drawCell( getX(), getY() );

  if( getGame() -> clinamena && isClinamenable() )
	{
	  if( rand() % 1000 < getGame() -> clinamenaPromille )
		{
		  dx = ( rand() % 2 == 0 ) ? +1 : -1;
		}
	  if( rand() % 1000 < getGame() -> clinamenaPromille )
		{
		  dy = ( rand() % 2 == 0 ) ? +1 : -1;
		}
	}
  nX = getX() + dx;
  nY = getY() + dy;
  if( !board -> isValidRow( nY ) )
	{
	  dy = -dy;
	}
  else if( !isAllowed( getX(), nY, 
					   board -> getState( getX(), nY ) ) )
	{
	  process( getX(), nY, board -> getState( getX(), nY ) );
	  dy = -dy;
	}
  if( !board -> isValidCol( nX ) )
	{
	  dx = -dx;
	}
  else if( !isAllowed( nX, y, board -> getState( nX, getY() ) ) )
	{
	  process( nX, getY(), board -> getState( nX, getY() ) );
	  dx = -dx;
	}
  if( board -> isValidCol( getX() + dx ) &&
	  isAllowed( getX() + dx, getY(), 
				 board -> getState( getX() + dx, getY() ) ) )
	{
	  x += dx;
	}
  if( board -> isValidRow( getY() + dy ) &&
	  isAllowed( getX(), getY() + dy, 
				 board -> getState( getX(), getY() + dy ) ) )
	{
	  y += dy;
	}
  test_( board -> isValidCol( getX() ) );
  test_( board -> isValidRow( getY() ) );
  board -> getCellInfo( getX(), getY(), &prevState, &version );
  board -> setState( getX(), getY(), getMyState() );
  getGame() -> drawCell( getX(), getY() );
  return true;
}

Index Snake::getDelay()
{
  return getGame() -> config -> bombDelay;
}

CoordX Snake::getX() const
{
  return x;
}

CoordY Snake::getY() const
{
  return y;
}

void Snake::reset()
{
  findSafe();
}

