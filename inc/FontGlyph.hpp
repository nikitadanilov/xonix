#if !defined( __FONTGLYPH_HPP__ )
#define __FONTGLYPH_HPP__

#include "XGlyph.hpp"

class FontGlyph : public XGlyph
{
public: FontGlyph( Game *aGame, char aChar, QR::Aq::Painter *aPicasso );
public: ~FontGlyph();

public: virtual void draw( QR::Aq::Painter *anArtist, 
						   Index aX, Index aY, 
						   Index aWidth, Index aHeight );

public: int getWidth() const;
public: int getHeight() const;

protected: Game *game;
protected: QR::Aq::Painter *picasso;
protected: char ch;
};

/* __FONTGLYPH_HPP__ */
#endif
