#if !defined( __IBOMBTHREAD_HPP__ )
#define __IBOMBTHREAD_HPP__

#include "BombThread.hpp"

#include <quercus.hpp>

class IBombThread : public BombThread
{
public: IBombThread( Game *aBoard );
public: virtual ~IBombThread();

public: Cell::State getMyState() const;
public: boolean isAllowed( Index aX, Index aY, Cell::State aState ) const;
public: void process( Index aX, Index aY, Cell::State aState );
public: boolean affectsClipping() const;
};

/* __IBOMBTHREAD_HPP__ */
#endif
