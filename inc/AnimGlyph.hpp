#if !defined( __ANIMPGLYPH_HPP__ )
#define __ANIMPGLYPH_HPP__

#include "Game.hpp"

#include "XGlyph.hpp"

class AnimGlyph : public XGlyph
{
public: AnimGlyph( Game *aGame, 
				   QR::Container *aGlyphs,
				   QR::Container *aDelays ); 
public: virtual ~AnimGlyph();

public: virtual void draw( QR::Aq::Painter *anArtist, 
						   Index aX, Index aY, 
						   Index aWidth, Index aHeight );

public: int getWidth() const;
public: int getHeight() const;

protected: Game *game;
protected: QR::CArray *glyphs;
protected: QR::CArray *delays;
protected: Index lastShot;
protected: Index width;
protected: Index height;
protected: Index position;
protected: Index length;
};

/* __ANIMPGLYPH_HPP__ */
#endif
