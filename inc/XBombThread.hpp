#if !defined( __XBOMBTHREAD_HPP__ )
#define __XBOMBTHREAD_HPP__

#include "IBombThread.hpp"

#include <quercus.hpp>

class XBombThread : public IBombThread
{
public: XBombThread( Game *aBoard );
public: virtual ~XBombThread();

public: Cell::State getMyState() const;
public: boolean isAllowed( Index aX, Index aY, Cell::State aState ) const;
public: void process( Index , Index , Cell::State aState );
};

/* __XBOMBTHREAD_HPP__ */
#endif
