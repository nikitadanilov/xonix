#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "StatusLine.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "BoardMap.hpp"
#include "GameConfig.hpp"

StatusLine::StatusLine( Game *aGame ) :
  GameThread( aGame, false, true )
{}

StatusLine::~StatusLine()
{}

boolean StatusLine::iteration()
{
  QR::Aq::Painter *arena;
  int y;
  int fill;
  
  arena = getGame() -> winPainter;
  y = getGame() -> getMap() -> getHeight() + 
	getGame() -> getMap() -> getAscent() + 1;

  {
	String statusLine;

	OGuard _player( getGame() -> getPlayer(), this );

	fill = getGame() -> getBoard() -> getFilled() * 100 / 
	  ( getGame() -> getBoard() -> getWidth() * 
		getGame() -> getBoard() -> getHeight() );

	statusLine = QR::String
	  ( "Lives: %2.2i Level: %2.2i Filled: %3.3i%% (req: %3.3i%%)"
		" Score: %li X: %3.3i Y: %3.3i T: %3.3f" ).
	  sprintf( 1, getGame() -> getLives(),
			   getGame() -> level,
			   fill, getGame() -> config -> fillThreshold,
			   getGame() -> getScore(),
			   getGame() -> getPlayer() -> getX(),
			   getGame() -> getPlayer() -> getY(),
			   ( double ) ( ( ( double ) getGame() -> getMilliseconds() ) / 1000 ) );

	//	Debug::getLogger() -> log( "Status: %s", statusLine );

	QR::Aq::PainterGuard _arenaLock( arena, this );

	arena -> clearRectangle
	  ( 0, getGame() -> getMap() -> getHeight(),
		getGame() -> getMap() -> getWidth(),
		getGame() -> getMap() -> getCHeight() );

	arena -> drawString( 0, y, statusLine );

  }
  if( fill >= getGame() -> config -> fillThreshold )
	{
	  getGame() -> setState( Game::finished );
	}
  return true;
}

Index StatusLine::getDelay()
{
  return getGame() -> config -> statusDelay;
}

