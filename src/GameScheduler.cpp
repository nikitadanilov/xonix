#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Game.hpp"
#include "GameThread.hpp"
#include "GameScheduler.hpp"

GameScheduler::GameScheduler( Game *aGame, QR::List *aThreads )
{
  preC_( aGame != NULL );
  preC_( aThreads != NULL );

  FNNINIT( game, aGame );
  FNNINIT( threads, aThreads );
}

GameScheduler::~GameScheduler()
{
  FDONE( threads );
  FDONE( game );
}

void *GameScheduler::run()
{
  Index minDelay;
  
  minDelay = 1000000000;

  while( true )
	{
	  for( threads -> toFirst() ; ! threads -> atEnd() ; threads -> next() )
		{
		  GameThread *thread;
		  Index delay;

		  thread = TCAST( threads -> getCurrentValue(), GameThread );
		  delay = thread -> getDelay();
		  if( delay < minDelay )
			{
			  minDelay = delay;
			}
		  if( ! game -> isPaused() || ! thread -> pausable )
			{
			  thread -> iteration();
			}
		}
	  if( ( game -> getState() == Game::finished ) ||
		  ( game -> getState() == Game::over ) )
		{
		  return NULL;
		}
	  THR_SYSTEM -> sleep( minDelay / BILLION, minDelay % BILLION );
	}
}

