#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "SBombThread.hpp"
#include "Board.hpp"
#include "BoardMap.hpp"
#include "Cell.hpp"
#include "Game.hpp"
#include "GameConfig.hpp"

#include <stdlib.h>

SBombThread::SBombThread( Game *aGame ) :
  OBombThread( aGame )
{}

SBombThread::~SBombThread()
{}

Cell::State SBombThread::getMyState() const
{
  return Cell::sbomb;
}

void SBombThread::initDXY()
{
  OBombThread::initDXY();

  if( ( rand() % 2 ) == 0 )
	{
	  dx = 0;
	}
  else
	{
	  dy = 0;
	}
}

boolean SBombThread::isClinamenable() const
{
  return false;
}


