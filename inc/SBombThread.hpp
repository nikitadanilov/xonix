#if !defined( __SBOMBTHREAD_HPP__ )
#define __SBOMBTHREAD_HPP__

#include "Cell.hpp"
#include "Geom.hpp"
#include "GameThread.hpp"
#include "OBombThread.hpp"

#include <quercus.hpp>

class SBombThread : public OBombThread
{
public: SBombThread( Game *aBoard );
public: virtual ~SBombThread();

public: Cell::State getMyState() const;
public: virtual void initDXY();
public: virtual boolean isClinamenable() const;
};


/* __SBOMBTHREAD_HPP__ */
#endif

