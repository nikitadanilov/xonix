#if !defined( __OBOMBTHREAD_HPP__ )
#define __OBOMBTHREAD_HPP__

#include <BombThread.hpp>

#include <quercus.hpp>

class OBombThread : public BombThread
{
public: OBombThread( Game *aBoard );
public: virtual ~OBombThread();

public: Cell::State getMyState() const;
public: boolean isAllowed( Index aX, Index aY, Cell::State aState ) const;
public: void process( Index aX, Index aY, Cell::State aState );
public: boolean affectsClipping() const;
};

/* __OBOMBTHREAD_HPP__ */
#endif
