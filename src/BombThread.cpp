#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "BombThread.hpp"
#include "Board.hpp"
#include "BoardMap.hpp"
#include "Cell.hpp"
#include "Game.hpp"
#include "GameConfig.hpp"

#include <stdlib.h>

BombThread::BombThread( Game *aGame ) :
  GameThread( aGame )
{}

BombThread::~BombThread()
{}

void BombThread::init()
{
  GameThread::init();
  findSafe();
}

void BombThread::findSafe()
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
	  if( isAllowed
		  ( getX(), getY(), 
			getGame() -> getBoard() -> getState( getX(), getY() ) ) )
		{
		  break;
		}
	}
  if( getX() == -1 )
	{
	  throw new QR::InternalError( "Cannot find vacuum in board", null );
	}
  initDXY();
  getGame() -> getBoard() -> 
	getCellInfo( getX(), getY(), &prevState, &version );
  getGame() -> getBoard() -> setState( getX(), getY(), getMyState() );
  getGame() -> drawCell( getX(), getY() );
}

void BombThread::initDXY()
{
  dx = ( ( rand() % 2 ) == 0 ) ? +1 : -1;
  dy = ( ( rand() % 2 ) == 0 ) ? +1 : -1;
}

boolean BombThread::isClinamenable() const
{
  return true;
}

boolean BombThread::iteration()
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

Index BombThread::getDelay()
{
  return getGame() -> config -> bombDelay;
}

CoordX BombThread::getX() const
{
  return x;
}

CoordY BombThread::getY() const
{
  return y;
}

void BombThread::reset()
{
  findSafe();
}

