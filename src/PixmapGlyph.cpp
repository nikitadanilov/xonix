#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "PixmapGlyph.hpp"

#if defined( XONIX_USE_XQ )

#include "Game.hpp"

PixmapGlyph::PixmapGlyph( Game *aGame, QR::String aFileName, FileType aType, 
						  QR::Xq::GC *aBase )
{
  preC_( aGame != NULL );
  preC_( aBase != NULL );

  FNNINIT( game, aGame );

  if( aType == XBM )
	{
	  FINIT( pixmap, QR::Xq::Pixmap::create( game -> getXWindow(), aFileName ) );
	}
  else if( aType == XPM )
	{
	  QR::Xq::Pixmap *xpm;
	  QR::Xq::Pixmap *clip;
	  QR::Xq::GC *gc;

	  gc = QR::Xq::GC::create( game -> getDisplay(), game -> getXWindow() );
	  OGuard _gc( gc, this );

	  game -> getGC() -> copyTo( gc );
  
	  xpm = QR::Xq::Pixmap::createFromXPM( game -> getXWindow(), aFileName, &clip );
	  OGuard _xpm( xpm, this );

	  if( clip != NULL )
		{
		  gc -> setClipMask( clip );
		}
	  
	  FINIT( pixmap, QR::Xq::Pixmap::create
			 ( game -> getXWindow(), 
			   xpm -> getWidth(), 
			   xpm -> getHeight(), 
			   game -> getDisplay() -> defaultDepth() ) );

	  OGuard _window( game -> getXWindow(), this );

	  pixmap -> fillRectangle
		( aBase, 0, 0, pixmap -> getWidth(), pixmap -> getHeight() );

	  xpm -> copyTo( pixmap, gc, 0, 0, 
					 pixmap -> getWidth(), pixmap -> getHeight(), 0, 0 );
	}
  else
	{
	  impossible_;
	}
}

PixmapGlyph::~PixmapGlyph()
{
  FDONE( pixmap );
  FDONE( game );
}

void PixmapGlyph::draw( QR::Aq::Painter *anArtist, 
						Index aX, Index aY, 
						Index aWidth, Index aHeight )
{
  QR::Aq::PainterGuard glyphArtist( anArtist, this );

  glyphArtist.getPainter() -> 
	copyFrom( 0, 0, pixmap -> getWidth(), pixmap -> getHeight(),
			  pixmap, aX, aY );
}

int PixmapGlyph::getWidth() const
{
  preC_( pixmap != NULL );

  return pixmap -> getWidth() + 1;
}

int PixmapGlyph::getHeight() const
{
  preC_( pixmap != NULL );

  return pixmap -> getHeight() + 1;
}

/* XONIX_USE_XQ */
#endif

