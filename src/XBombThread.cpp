#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "XBombThread.hpp"
#include "Cell.hpp"
#include "Game.hpp"


XBombThread::XBombThread( Game *aGame ) :
  IBombThread( aGame )
{
  //  init();
}

XBombThread::~XBombThread()
{}

Cell::State XBombThread::getMyState() const
{
  return Cell::xbomb;
}

boolean XBombThread::isAllowed( Index, Index, Cell::State aState ) const
{
  return( ( aState == Cell::border ) || 
		  ( aState == Cell::solid ) ||
		  ( aState == Cell::vacuum ) );
}

void XBombThread::process( Index , Index , Cell::State aState )
{
  if( ( aState == Cell::path ) || ( aState == Cell::head ) )
	{
	  getGame() -> setState( Game::hit );
	  getGame() -> update();
	}
}


