#if !defined( __BITMAPGLYPH_HPP__ )
#define __BITMAPGLYPH_HPP__

#include "Game.hpp"

#include "XGlyph.hpp"

class BitmapGlyph : public XGlyph
{
public: BitmapGlyph( Game *aGame, QR::Aq::Drawable *aDrawable,
					 QR::Aq::Size aWidth, QR::Aq::Size aHeight ); 
public: virtual ~BitmapGlyph();

public: virtual void draw( QR::Aq::Painter *anArtist, 
						   Index aX, Index aY, 
						   Index aWidth, Index aHeight );

public: int getWidth() const;
public: int getHeight() const;

protected: Game *game;
protected: QR::Aq::Drawable *bitmap;
protected: QR::Aq::Size width;
protected: QR::Aq::Size height;
};

/* __BITMAPGLYPH_HPP__ */
#endif
