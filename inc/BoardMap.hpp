#if !defined( __BOARDMAP_HPP__ )
#define __BOARDMAP_HPP__

#include <quercus.hpp>

#include "Board.hpp"
#include "Cell.hpp"

class XGlyph;
class BoardMap : public virtual QR::Object
{
public: BoardMap( Board *aBoard, QR::Aq::Size aWidth, QR::Aq::Size aHeight,
				  QR::Aq::Size anAscent );
public: virtual ~BoardMap();

public: void setGlyph( Cell::State aState, XGlyph *aGlyph );
public: XGlyph *getGlyph( Cell::State aState ) const;

Board *getBoard() const;

public: Index getCWidth() const;
public: Index getCHeight() const;

public: Index getWidth() const;
public: Index getHeight() const;

public: Index getAscent() const;

public: void drawCell( QR::Aq::Painter *aPainter, CoordX aCol, CoordY aRow ) const;
public: void draw( QR::Aq::Painter *aPainter ) const;

protected: Board *board;
protected: Index ascent;

protected: XGlyph *glyph[ Cell::INVALID + 1 ];
protected: int width;
protected: int height;
};

/* __BOARDMAP_HPP__ */
#endif
