#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "OBombThread.hpp"
#include "Cell.hpp"
#include "Game.hpp"


OBombThread::OBombThread( Game *aGame ) :
  BombThread( aGame )
{
  //  init();
}

OBombThread::~OBombThread()
{}

Cell::State OBombThread::getMyState() const
{
  return Cell::obomb;
}

boolean OBombThread::isAllowed( Index, Index, Cell::State aState ) const
{
  return( aState == Cell::vacuum );
}

void OBombThread::process( Index , Index , Cell::State aState )
{
  if( ( aState == Cell::path ) || ( aState == Cell::head ) )
	{
	  getGame() -> setState( Game::hit );
	  getGame() -> update();
	}
}

boolean OBombThread::affectsClipping() const
{
  return true;
}


