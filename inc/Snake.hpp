#if !defined( __SNAKE_HPP__ )
#define __SNAKE_HPP__

#include "Cell.hpp"
#include "Geom.hpp"
#include "GameThread.hpp"

#include <quercus.hpp>

class Board;
class BoardMap;
class Game;
class Snake : public GameThread
{
public: Snake( Game *aBoard );
public: virtual ~Snake();

public: virtual void init();

public: virtual void findSafe();
public: virtual void initDXY();
public: virtual boolean isClinamenable() const;

public: virtual Cell::State getMyState( Index aDistanceFromHead ) const = 0;
public: virtual boolean isAllowed( Index aX, Index aY, 
								   Cell::State aState ) const = 0;
public: virtual void process( Index aX, Index aY, Cell::State aState ) = 0;

public: virtual boolean affectsClipping() const = 0;
public: virtual boolean grow() const = 0;

public: CoordX getHeadX() const;
public: CoordY getHeadY() const;

public: virtual boolean iteration();
public: virtual void reset();
public: virtual Index getDelay();

protected: typedef struct
{
  CoordX x;
  CoordY y;
  Cell::State prevState;
  Index version;
} CellInfo;

protected: List *cells;

protected: int dx;
protected: int dy;
};


/* __SNAKE_HPP__ */
#endif

