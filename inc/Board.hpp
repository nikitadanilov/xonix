#if !defined( __BOARD_HPP__ )
#define __BOARD_HPP__

#include "Geom.hpp"
#include "Cell.hpp"
#include "GameObject.hpp"

#include <quercus.hpp>

#define FOR_ROWS( i, board )  \
    for( CoordY i = 0 ; i < board -> getHeight() ; ++i )

#define FOR_COLS( i, board )  \
    for( CoordX i = 0 ; i < board -> getWidth() ; ++i )

class Cell;
class Board : public virtual GameObject
{
public: Board( Game *aGame, Index aHeight, Index aWidth );
public: virtual ~Board();

public: virtual void clean();

public: Index getHeight() const;
public: Index getWidth() const;

public: Cell::State getState( CoordX aCol, CoordY aRow ) const;
public: void setState( CoordX aCol, CoordY aRow, Cell::State aState );

public: void getCellInfo( CoordX aCol, CoordY aRow, 
						  Cell::State *aState,
						  Index *aVersion );
public: void updateCellInfo( CoordX aCol, CoordY aRow, 
							 Cell::State aState,
							 Index aVersion );

public: Index getFilled() const;

boolean isValidRow( CoordY aRow ) const;
boolean isValidCol( CoordX aRow ) const;

public: virtual void reset();

public: virtual QR::String toString() const;

protected: const Index height;
protected: const Index width;
protected: Cell::State *cell;
protected: Index *version;
protected: Index filled;
};

/* __BOARD_HPP__ */
#endif

