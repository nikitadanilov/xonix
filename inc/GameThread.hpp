#if !defined( __GAMETHREAD_HPP__ )
#define __GAMETHREAD_HPP__

#include "GameObject.hpp"

class GameThread : public QR::Runnable,
				   public GameObject
{
public: GameThread( Game *aGame, 
					boolean attach = true,
					boolean isPausable = true );
public: virtual ~GameThread();

public: virtual void start();
public: virtual void stop();

public: virtual void *run();
public: virtual boolean iteration() = 0;
public: virtual Index getDelay() = 0;

public: boolean pausable;

public: QR::Thread *getThread() const;

protected: QR::Thread *thread;
};

/* __GAMETHREAD_HPP__ */
#endif

