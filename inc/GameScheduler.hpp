#if !defined( __GAMESCHEDULER_HPP__ )
#define __GAMESCHEDULER_HPP__

#include <quercus.hpp>

#define BIG_YEAR_SHIFT 8
#define BIG_YEAR       ( 1 << BIG_YEAR_SHIFT )
#define BILLION        1000000000

class Game;
class GameScheduler : public QR::Runnable,
					  public QR::Object
{
public: GameScheduler( Game *aGame, QR::List *aThreads );
public: virtual ~GameScheduler();

public: virtual void *run();

protected: Game *game;
protected: QR::List *threads;
};

/* __GAMESCHEDULER_HPP__ */
#endif

