#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Game.hpp"
#include "Board.hpp"
#include "TopFile.hpp"
#include "GameConfig.hpp"
#include "GameManager.hpp"
#include "IBombThread.hpp"
#include "OBombThread.hpp"
#include "XBombThread.hpp"
#include "SBombThread.hpp"

#include <stdlib.h>

GameManager::GameManager()
{}

void GameManager::manage( QR::List *anArgs )
{
  Game *game;
  GameConfig *config;
  GameConfig *origConfig;

  FINIT( config, new GameConfig() );

  config -> init( anArgs );

  FINIT( origConfig, new GameConfig( *config ) );
  FINIT( game, new Game( config ) );

  while( true )
	{
// 	  Debug::getLogger() -> logObject( "Game: %s", game -> getProtector() );
// 	  Debug::getLogger() -> logObject( "Board: %s", 
// 									   game -> getBoard() -> getProtector() );
// 	  Debug::getLogger() -> logObject( "GameThreads: %s", 
// 									   game -> getThreads() );
	  if( game -> getState() == Game::finished )
		{
		  Index i;

		  game -> stop();
		  game -> setState( Game::normal );

		  ++( game -> level );

		  game -> config -> bombDelay = 
			game -> config -> bombDelay / 100 * 
			( 100 - game -> config -> bombAccel );
		  game -> config -> playerDelay = 
			game -> config -> playerDelay / 100 *
			( 100 - game -> config -> playerAccel );

		  game -> config -> iBombsNum += game -> config -> iBombsNumStep;
		  game -> config -> oBombsNum += game -> config -> oBombsNumStep;

		  game -> config -> fillThreshold += 
			game -> config -> fillThresholdStep;
		  /* it is quite possible, believe me */
		  if( game -> config -> fillThreshold > 100 )
			{
			  game -> config -> fillThreshold = 100;
			}

		  BombThread *bomb;

		  for( i = 0 ; i < config -> oBombsNumStep ; ++i )
			{
			  bomb = new OBombThread( game );

			  game -> getBombs() -> append( bomb );
			  game -> getMovingObjects( Game::obombs ) -> append( bomb );
			  bomb -> init();
			}
		  for( i = 0 ; i < config -> iBombsNumStep ; ++i )
			{
			  bomb = new IBombThread( game );

			  game -> getBombs() -> append( bomb );
			  game -> getMovingObjects( Game::ibombs ) -> append( bomb );
			  bomb -> init();
			}
		  if( game -> level >= config -> xBombsLevel )
			{
			  for( i = 0 ; i < config -> xBombsNumStep ; ++i )
				{
				  bomb = new XBombThread( game );
				  
				  game -> getBombs() -> append( bomb );
				  game -> getMovingObjects( Game::xbombs ) -> append( bomb );
				  bomb -> init();
				}
			}
		  if( game -> level >= config -> sBombsLevel )
			{
			  for( i = 0 ; i < config -> sBombsNumStep ; ++i )
				{
				  bomb = new SBombThread( game );
				  
				  game -> getBombs() -> append( bomb );
				  game -> getMovingObjects( Game::sbombs ) -> append( bomb );
				  bomb -> init();
				}
			}

		  game -> clinamena = ( game -> level >= config -> clinamenaLevel );
		  game -> incLives( 1 );

		  game -> reset();
		  game -> start();

		}
	  else if( game -> getState() == Game::over )
		{
		  TopFile *topTen;

		  game -> stop();

		  topTen = TopFile::create( "./.xonix.top" );
		  OGuard _top( topTen, this );

		  topTen -> addEntry
			( new TopFile::TopEntry( game -> getUserName(), 
									 game -> getScore(), origConfig ) );
		  Debug::getLogger() -> log( "Score: %li", game -> getScore() );
		  Debug::getLogger() -> log( "See you later.\n" );
		  FDONE( game );
		  FDONE( config );
		  FDONE( origConfig );
#if defined( XONIX_USE_CQ )
		  QR::Cq::restore();
#endif
		  return;
		}
	  THR_SYSTEM -> sleep( 0, game -> config -> managerDelay );
	}
}

GameManager::~GameManager()
{}

