#if !defined( __STATUSLINE_HPP__ )
#define __STATUSLINE_HPP__

#include "Game.hpp"
#include "GameThread.hpp"

#include <quercus.hpp>

class StatusLine : public GameThread
{
public: StatusLine( Game *aGame );
public: virtual ~StatusLine();

public: boolean iteration();
public: virtual Index getDelay();
};

/* __STATUSLINE_HPP__ */
#endif
