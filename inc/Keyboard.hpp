#if !defined( __KEYBOARD_HPP__ )
#define __KEYBOARD_HPP__

#include "GameObject.hpp"

#include <quercus.hpp>

class Game;
class Keyboard : public virtual QR::Aq::EventDispatcher,
				 public virtual QR::PassiveObserver,
				 public virtual GameObject
{
public: Keyboard( Game *aGame );
public: virtual ~Keyboard();

public: int getDx();
public: int getDy();

public: virtual QR::HandlerResult fire( QR::Condition *aCondition, 
									QR::ObservationContext *aContext );
public: virtual void onKeyEvent( QR::Aq::KeyEvent *anEvent );

public: virtual void reset();

protected: int dx;
protected: int dy;
};

/* __KEYBOARD_HPP__ */
#endif

