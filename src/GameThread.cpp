#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "GameThread.hpp"
#include "Game.hpp"

GameThread::GameThread( Game *aGame, 
						boolean attach,
						boolean isPausable ) :
  GameObject( aGame )
{
  thread = NULL;
  if( attach )
	{
	  getGame() -> getThreads() -> append( this );
	}
  pausable = isPausable;
}

GameThread::~GameThread()
{
  FCDONE( thread );
}

void GameThread::start()
{
  THR_FACTORY -> createFromRunner( this );
}

void GameThread::stop()
{
  if( getThread() != NULL )
	{
	  getThread() -> join();
	}
}

void *GameThread::run()
{
  FMOVE( thread, THR_SYSTEM -> getSelf() );

  while( ( getGame() -> getState() != Game::finished ) && 
		 ( getGame() -> getState() != Game::over ) )
	{
	  if( ! getGame() -> isPaused() || ! pausable )
		{
		  if( !iteration() )
			{
			  break;
			}
		}
	  THR_SYSTEM -> sleep( 0, getDelay() );
	}
  return NULL;
}

QR::Thread *GameThread::getThread() const
{
  return thread;
}

