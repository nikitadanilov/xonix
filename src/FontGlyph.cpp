#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "FontGlyph.hpp"
#include "Game.hpp"

FontGlyph::FontGlyph( Game *aGame, char aChar, QR::Aq::Painter *aPicasso )
{
  preC_( aGame != NULL );
  preC_( aPicasso != NULL );

  FNNINIT( game, aGame );
  FNNINIT( picasso, aPicasso );

  ch = aChar;
}

FontGlyph::~FontGlyph()
{
  FDONE( picasso );
  FDONE( game );
}

void FontGlyph::draw( QR::Aq::Painter *anArtist, 
					  Index aX, Index aY, 
					  Index aWidth, Index aHeight )
{
  OGuard _art( anArtist, this );

  QR::Aq::PainterGuard glyphArtist( anArtist, this );

  char string[ 2 ];

  string[ 0 ] = ch;
  string[ 1 ] = ( char ) 0;

  anArtist -> clearRectangle( aX, aY, aWidth, aHeight );

  picasso -> setDrawable( anArtist -> getDrawable() );
#if defined( XONIX_USE_WQ ) && 0
  TCAST( picasso, QR::Wq::WPainter ) -> 
	  setWDrawable( 
		TCAST( anArtist -> getDrawable(), QR::Wq::Drawable ) );
#endif
  QR::Aq::PainterGuard leonardo( picasso, this );

  if( ch != ( char ) 0 )
	{
	  picasso -> drawCString
		( aX, aY + game -> getMap() -> getAscent(), string );
	}
  else
	{
	  picasso -> fillRectangle( aX, aY, aWidth - 1, aHeight - 1 );
	}
}

int FontGlyph::getWidth() const
{
  return -1;
}

int FontGlyph::getHeight() const
{
  return -1;
}

