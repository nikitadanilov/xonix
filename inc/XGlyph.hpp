#if !defined( __XGLYPH_HPP__ )
#define __XGLYPH_HPP__

#include "Board.hpp"
#include "BoardMap.hpp"
#include "Cell.hpp"

#include <quercus.hpp>

class Game;
abstract class XGlyph : public virtual QR::Object
{
public: virtual void draw( QR::Aq::Painter *anArtist, 
						   Index aX, Index aY, 
						   Index aWidth, Index aHeight ) = 0;
public: virtual int getWidth() const = 0;
public: virtual int getHeight() const = 0;
};

/* __XGLYPH_HPP__ */
#endif
