#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "BitmapGlyph.hpp"

#include "Game.hpp"

BitmapGlyph::BitmapGlyph( Game *aGame, QR::Aq::Drawable *aDrawable,
						 QR::Aq::Size aWidth, QR::Aq::Size aHeight )
{
  preC_( aGame != NULL );
  preC_( aDrawable != NULL );

  FNNINIT( game, aGame );
  FNNINIT( bitmap, aDrawable );

  width = aWidth;
  height = aHeight;
}

BitmapGlyph::~BitmapGlyph()
{
  FDONE( bitmap );
  FDONE( game );
}

void BitmapGlyph::draw( QR::Aq::Painter *anArtist, 
						Index aX, Index aY, 
						Index aWidth, Index aHeight )
{
  QR::Aq::PainterGuard glyphArtist( anArtist, this );

  anArtist -> 
	copyFrom( 0, 0, width, height, bitmap, aX, aY );
}

int BitmapGlyph::getWidth() const
{
  return width + 1;
}

int BitmapGlyph::getHeight() const
{
  return height + 1;
}
