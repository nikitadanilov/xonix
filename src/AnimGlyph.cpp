#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "AnimGlyph.hpp"

#include "Game.hpp"

AnimGlyph::AnimGlyph( Game *aGame, 
					  QR::Container *aGlyphs,
					  QR::Container *aDelays )
{
  OGuard _g( aGlyphs, this );
  OGuard _d( aDelays, this );

  preC_( aGame != NULL );
  preC_( aGlyphs != NULL );
  preC_( aDelays != NULL );
  preC_( aGlyphs -> getCardinality() == aDelays -> getCardinality() );
  preC_( ! aGlyphs -> isEmpty() );

  QR::Enumeration *glyphStream;
  QR::Enumeration *delayStream;

  FNNINIT( game, aGame );

  length = aGlyphs -> getCardinality();
  position = 0;
  height = -1;
  width = -1;

  FINIT( glyphs, new QR::CArray( 0, length ) );
  FINIT( delays, new QR::CArray( 0, length ) );

  glyphStream = aGlyphs -> getValueEnumeration();
  OGuard _gs( glyphStream, this );

  delayStream = aDelays -> getValueEnumeration();
  OGuard _ds( delayStream, this );

  while( glyphStream -> hasMoreElements() )
	{
	  test_( delayStream -> hasMoreElements() );

	  XGlyph *frame;

	  frame = TCAST( glyphStream -> getNextElement(), XGlyph );
	  OGuard _frame( frame, this );

	  glyphs -> setAt( position, frame );
	  delays -> setAt( position, delayStream -> getNextElement() );

	  if( frame -> getWidth() > width )
		{
		  width = frame -> getWidth();
		}
	  if( frame -> getHeight() > height )
		{
		  height = frame -> getHeight();
		}
	  ++position;
	}
  test_( position == length );

  position = 0;
  lastShot = game -> getMilliseconds();
}

AnimGlyph::~AnimGlyph()
{
  FDONE( delays );
  FDONE( glyphs );
  FDONE( game );
}

void AnimGlyph::draw( QR::Aq::Painter *anArtist, 
					  Index aX, Index aY, 
					  Index aWidth, Index aHeight )
{
  Index instant;

  instant = game -> getMilliseconds();

  if( instant - lastShot > 
	  TCAST( delays -> getAt( position ), QR::Integer ) -> getInt() )
	{
	  lastShot = instant;
	  position = ( ++position ) % length;
	}
  TCAST( glyphs -> getAt( position ), XGlyph ) 
	-> draw( anArtist, aX, aY, aWidth, aHeight );
}

int AnimGlyph::getWidth() const
{
  return width;
}

int AnimGlyph::getHeight() const
{
  return height;
}

