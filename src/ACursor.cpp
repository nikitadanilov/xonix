#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "ACursor.hpp"
#include "Game.hpp"
#include "GameConfig.hpp"

ACursor::ACursor( Game *aGame, QR::Xq::Window *aWindow, 
				  QR::String aNormalPattern, Index aNormalStart, Index aNormalEnd,
				  QR::String aHitPattern, Index aHitStart, Index aHitEnd,
				  QR::String aClipPattern, Index aClipStart, Index aClipEnd,
				  Index aWidth, Index aHeight ) :
  GameThread( aGame, false, false )
{
  preC_( aWindow != NULL );
  preC_( aNormalStart < aNormalEnd );
  preC_( aHitStart < aHitEnd );

  int r;
  QR::Xq::Color *normalC;
  QR::Xq::Color *hitC;
  QR::Xq::Color *clipC;

  normalC = QR::Xq::Color::create
	( aWindow -> getDisplay(), game -> config -> normalCursorColour );
  OGuard _normalC( normalC, this );

  hitC = QR::Xq::Color::create
	( aWindow -> getDisplay(), game -> config -> hitCursorColour );
  OGuard _hitC( hitC, this );

  clipC = QR::Xq::Color::create
	( aWindow -> getDisplay(), game -> config -> clipCursorColour );
  OGuard _clipC( clipC, this );

  spectrum = new QR::Xq::Color *[ COLORS_IN_SPECTRUM ];
  spectrum[ 0 ] = QR::Xq::Color::create( aWindow -> getDisplay(), "red" );
  spectrum[ 1 ] = QR::Xq::Color::create( aWindow -> getDisplay(), "orange" );
  spectrum[ 2 ] = QR::Xq::Color::create( aWindow -> getDisplay(), "yellow" );
  spectrum[ 3 ] = QR::Xq::Color::create( aWindow -> getDisplay(), "green" );
  spectrum[ 4 ] = QR::Xq::Color::create( aWindow -> getDisplay(), "blue" );
  spectrum[ 5 ] = QR::Xq::Color::create( aWindow -> getDisplay(), "navy" );
  spectrum[ 6 ] = QR::Xq::Color::create( aWindow -> getDisplay(), "violet" );

  for( r = 0 ; r < COLORS_IN_SPECTRUM ; ++r )
	{
	  OREG( spectrum[ r ] );
	}

  FNNINIT( window, aWindow );

  normalLength = aNormalEnd - aNormalStart;
  normal = new QR::Xq::Cursor *[ normalLength ];

  for( r = 0 ; r < normalLength ; ++r )
	{
	  QR::Xq::Pixmap *pix;
	  
	  pix = QR::Xq::Pixmap::create
		( window, aNormalPattern.sprintf( 1, r + aNormalStart ) );
	  OGuard _pix( pix, this );
	  
	  FINIT( normal[ r ], QR::Xq::Cursor::create
			 ( window -> getDisplay(), pix, pix, normalC , normalC, 
			   aWidth, aHeight ) );
	}

  hitLength = aHitEnd - aHitStart;
  hit = new QR::Xq::Cursor *[ hitLength ];

  for( r = 0 ; r < hitLength ; ++r )
	{
	  QR::Xq::Pixmap *pix;
	  
	  pix = QR::Xq::Pixmap::create
		( window, aHitPattern.sprintf( 1, r + aHitStart ) );
	  OGuard _pix( pix, this );
	  
	  FINIT( hit[ r ], QR::Xq::Cursor::create
			 ( window -> getDisplay(), pix, pix, hitC, hitC, 
			   aWidth, aHeight ) );
	}

  clipLength = aClipEnd - aClipStart;
  clip = new QR::Xq::Cursor *[ clipLength ];

  for( r = 0 ; r < clipLength ; ++r )
	{
	  QR::Xq::Pixmap *pix;
	  
	  pix = QR::Xq::Pixmap::create
		( window, aClipPattern.sprintf( 1, r + aClipStart ) );
	  OGuard _pix( pix, this );
	  
	  FINIT( clip[ r ], QR::Xq::Cursor::create
			 ( window -> getDisplay(), pix, pix, clipC, clipC, 
			   aWidth, aHeight ) );
	}
}

ACursor::~ACursor()
{
  int r;

  for( r = 0 ; r < clipLength ; ++r )
	{
	  FDONE( clip[ r ] );
	}
  delete []clip;
  for( r = 0 ; r < hitLength ; ++r )
	{
	  FDONE( hit[ r ] );
	}
  delete []hit;
  for( r = 0 ; r < normalLength ; ++r )
	{
	  FDONE( normal[ r ] );
	}
  delete []normal;
  LinkManager::free( this, window );
  
  for( r = 0 ; r < COLORS_IN_SPECTRUM ; ++r )
	{
	  FDONE( spectrum[ r ] );
	}
  delete []spectrum;

}

QR::HandlerResult ACursor::fire( QR::Condition *, QR::ObservationContext * )
{
  if( ( getGame() -> getStateFast() != Game::normal ) &&
	  ( getGame() -> getStateFast() != Game::path ) )
	{
	  sync();
	}
  return 0;
}

void ACursor::sync()
{
  state = getGame() -> getStateFast();
  setCursor();
}

void ACursor::setCursor()
{
  int r;
  QR::Xq::Cursor ***cursor;
  Index length;

  switch( state )
	{
	case Game::hit:
	  length = hitLength;
	  cursor = &hit;
	  break;
	case Game::clipped:
	  length = clipLength;
	  cursor = &clip;
	  break;
	default:
	  length = normalLength;
	  cursor = &normal;
	  break;
	}
  if( ( i / length ) % 2 == 0 )
	{
	  r = i % length;
	}
  else
	{
	  r = ( ( 2 * length - 1 ) - ( i % ( 2 * length ) ) ) % length;
	}

  if( state == Game::normal )
	{
	  ( *cursor )[ r ] -> recolor
		( spectrum[ ( i >> 2 ) % COLORS_IN_SPECTRUM ],
		  spectrum[ ( i >> 2 ) % COLORS_IN_SPECTRUM ] );
						
	}
  window -> setCursor( ( *cursor )[ r ] );
  ++i;
  state = getGame() -> getStateFast();
}

boolean ACursor::iteration()
{
  setCursor();
  return true;
}

void ACursor::reset()
{
  i = 0;
}

Index ACursor::getDelay()
{
  return getGame() -> config -> cursorDelay;
}

