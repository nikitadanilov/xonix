#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "IBombThread.hpp"
#include "Cell.hpp"
#include "Game.hpp"


IBombThread::IBombThread( Game *aGame ) :
  BombThread( aGame )
{
  //  init();
}

IBombThread::~IBombThread()
{}

Cell::State IBombThread::getMyState() const
{
  return Cell::ibomb;
}

boolean IBombThread::isAllowed( Index, Index, Cell::State aState ) const
{
  return( ( aState == Cell::solid ) || ( aState == Cell::border ) ) ;
}

void IBombThread::process( Index , Index , Cell::State aState )
{
  if( aState == Cell::head )
	{
	  getGame() -> setState( Game::hit );
	  getGame() -> update();
	}
}

boolean IBombThread::affectsClipping() const
{
  return false;
}

