#if !defined( __PIXMAPGLYPH_HPP__ )
#define __PIXMAPGLYPH_HPP__

#include "Game.hpp"

#if defined( XONIX_USE_XQ )

#include "XGlyph.hpp"

class PixmapGlyph : public XGlyph
{
public: typedef enum { XBM, XPM } FileType;

public: PixmapGlyph( Game *aGame, QR::String aFileName, FileType aType, 
					 QR::Xq::GC *aBase );
public: virtual ~PixmapGlyph();

public: virtual void draw( QR::Aq::Painter *anArtist, 
						   Index aX, Index aY, 
						   Index aWidth, Index aHeight );

public: int getWidth() const;
public: int getHeight() const;

protected: Game *game;
protected: QR::Xq::Pixmap *pixmap;
};

/* XONIX_USE_XQ */
#endif

/* __PIXMAPGLYPH_HPP__ */
#endif
