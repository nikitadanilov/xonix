#if !defined( __PLAYER_HPP__ )
#define __PLAYER_HPP__

#include "GameThread.hpp"
#include "Cell.hpp"

#include <quercus.hpp>

class Game;
class Keyboard;
class Player : virtual public QR::PassiveObserver,
			   public GameThread
{
public: typedef enum { inSolid, inSpace } State;
public: Player( Game *aGame );
public: virtual ~Player();

public: virtual QR::HandlerResult fire( QR::Condition *aCondition, 
									QR::ObservationContext *aContext );
public: virtual QR::HandlerResult firePost( QR::Condition *aCondition, 
										QR::ObservationContext *aContext );

protected: void safePlace();

public: void reset();
public: virtual boolean iteration();
public: virtual Index getDelay();

public: int getX() const;
public: int getY() const;

public: QR::List *getPath() const;

protected: void buildPath();
protected: Cell::State getState() const;
protected: void addToPath( int aX, int aY );
protected: int x;
protected: int y;
protected: Cell::State prevState;
protected: Index version;

public: int takeOffX;
public: int takeOffY;

protected: State state;

public: Index *pathX;
public: Index *pathY;
public: Index pathLength;

protected: QR::List *path;
protected: Keyboard *keyboard;
};

/* __PLAYER_HPP__ */
#endif
