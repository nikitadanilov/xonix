#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Clipper.hpp"
#include "BoardMap.hpp"
#include "Board.hpp"
#include "Game.hpp"
#include "Player.hpp"

#include <stdlib.h>

Clipper::Clipper( Game *aGame )
{
  preC_( aGame != NULL );
  game = aGame;
  LinkManager::reg( this, game );
}

Clipper::~Clipper()
{
  LinkManager::free( this, game );
}

HandlerResult Clipper::fire( Condition *, ObservationContext * )
{
  if( game -> getState() == Game::clipped )
	{
	  OGuard _game( game, this );
	  OGuard _board( game -> getBoard(), this );
	  OGuard _player( game -> getPlayer(), this );

	  Index filled;
	  Board *board;

	  board = game -> getBoard();

	  FOR_ROWS( i, board )
		{
		  FOR_COLS( j, board )
			{
			  board -> getAt( j, i ) -> clippingData = -1;
			}
		}
	  for( Index p = 0 ; p < game -> getPlayer() -> pathLength ; ++p )
		{
		  Cell *cell;
		  int x;
		  int y;

		  y = game -> getPlayer() -> pathY[ p ];
		  x = game -> getPlayer() -> pathX[ p ];

		  cell = board -> getAt( x, y );
		  cell -> clippingData = 0;

		  if( ( cell -> getState() == Cell::path ) ||
			  ( cell -> getState() == Cell::head ) )
			{
			  board -> setState( x, y, Cell::border );
			}
		  for( int dir = 0 ; dir < 4 ; ++dir )
			{
			  Cell *neibor;
			  Index unique;
					  
			  int dx;
			  int dy;
			  
			  int nX;
			  int nY;
			  
			  dx = dirX[ dir ];
			  dy = dirY[ dir ];

			  nX = x + dx;
			  nY = y + dy;

			  neibor = board -> getAt( nX, nY );
			  if( !board -> isValidRow( nY ) ||
				  !board -> isValidCol( nX ) )
				{ 
				  continue;
				}
			  neibor = board -> getAt( nX, nY );
			  if( ( neibor -> getState() != Cell::border ) &&
				  ( neibor -> getState() != Cell::solid ) &&
				  ( neibor -> getState() != Cell::path ) &&
				  ( neibor -> getState() != Cell::head ) &&
				  ( neibor -> clippingData == -1 ) )
				{
				  unique = ( p << 4 ) + dx + 1 + 3 * ( dy + 1 );
				  try
					{
					  filled = fill( nX, nY, unique );
					}
				  catch( BombFound *oops )
					{
					  OGuard _oops( oops, this );
					  continue;
					}
				  if( filled > 0 )
					{
					  FOR_ROWS( k, board )
						{
						  FOR_COLS( l, board )
							{
							  Cell *scan;
							  
							  scan = board -> getAt( l, k );
							  if( scan -> clippingData == unique )
								{
								  board -> setState( l, k, Cell::solid );
								  game -> drawCell( l, k );
								}
							}
						}
					}
				}
			}
		}
	  board -> setState
		( game -> getPlayer() -> getX(), game -> getPlayer() -> getY(),
		  Cell::head );
	  game -> getMap() -> draw( game -> getPixmap() );
	  game -> getMap() -> draw( game -> getWindow() );
	}
  return 0;
}

Index Clipper::tryFill( Index aX, Index aY, int aLevel )
{
  preC_( aLevel >= 0 );

  if( game -> getBoard() -> isValidCol( aX ) &&
	  game -> getBoard() -> isValidRow( aY ) )
	{
	  Cell *cell;

	  cell = game -> getBoard() -> getAt( aX, aY );
	  test_( cell != NULL );

	  if( ( ( cell -> getState() == Cell::vacuum ) ||
			( cell -> getState() == Cell::obomb ) ) &&
		  ( cell -> clippingData == -1 ) )
		{
		  return fill( aX, aY, aLevel );
		}
	}
  return 0L;
}

Index Clipper::fill( Index aX, Index aY, int aLevel )
{
  preC_( game -> getBoard() -> isValidCol( aX ) );
  preC_( game -> getBoard() -> isValidRow( aY ) );
  preC_( aLevel >= 0 );

  Cell *cell;
  Cell::State state;

  cell = game -> getBoard() -> getAt( aX, aY );
  test_( cell != NULL );

  state = cell -> getState();

  test_( ( state != Cell::solid ) && 
		 ( state != Cell::border ) &&
		 ( cell -> clippingData == -1 ) );

  cell -> clippingData = aLevel;

  if( state == Cell::obomb )
	{
	  throw new BombFound();
	}
  else
	{
	  boolean rethrow;
	  Index result;

	  rethrow = false;
	  result = 1;
	  for( int dir = 0 ; dir < 4 ; ++dir )
		{
		  try
			{
			  result += tryFill( aX + dirX[ dir ], aY + dirY[ dir ], aLevel );
			}
		  catch( BombFound *aDamn )
			{
			  OGuard _damn( aDamn, this );
			  rethrow = true;
			}
		}
	  if( rethrow )
		{
		  throw new BombFound();
		}
	  else
		{
		  return result;
		}
	}
}

Clipper::BombFound::BombFound() : 
  Exception( "", null )
{}

int Clipper::dirX[ 4 ] = { 1, 0, -1, 0 };
int Clipper::dirY[ 4 ] = { 0, -1, 0, 1 };

