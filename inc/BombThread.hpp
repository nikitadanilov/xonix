#if !defined( __BOMB_THREAD_HPP__ )
#define __BOMB_THREAD_HPP__

#include "Cell.hpp"
#include "Geom.hpp"
#include "GameThread.hpp"

#include <quercus.hpp>

class Board;
class BoardMap;
class Game;
class BombThread : public GameThread
{
public: BombThread( Game *aBoard );
public: virtual ~BombThread();

public: virtual void init();

public: virtual void findSafe();
public: virtual void initDXY();
public: virtual boolean isClinamenable() const;

public: virtual Cell::State getMyState() const = 0;
public: virtual boolean isAllowed( Index aX, Index aY, 
								   Cell::State aState ) const = 0;
public: virtual void process( Index aX, Index aY, Cell::State aState ) = 0;

public: virtual boolean affectsClipping() const = 0;

public: CoordX getX() const;
public: CoordY getY() const;

public: virtual boolean iteration();
public: virtual void reset();
public: virtual Index getDelay();

protected: CoordX x;
protected: CoordY y;
protected: int dx;
protected: int dy;

protected: Cell::State prevState;
protected: Index version;
};


/* __BOMB_THREAD_HPP__ */
#endif

