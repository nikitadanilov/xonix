#if !defined( __FRAMEFEEDER_HPP__ )
#define __FRAMEFEEDER_HPP__

#include "GameThread.hpp"

#include <quercus.hpp>

class FrameFeeder : public QR::PassiveObserver,
	public virtual QR::Object
{
public: FrameFeeder( Game *aGame, 
					 QR::Aq::Drawable *aSource,
					 QR::Aq::Painter *aTarget,
					 Index aX, Index aY,
					 Index aWidth, Index aHeight,
					 Index aDelay );

public: virtual ~FrameFeeder();

public: virtual QR::HandlerResult fire( QR::Condition *aCondition, 
									QR::ObservationContext *aContext );

public: void sync();

protected: QR::Aq::Drawable *source;
protected: QR::Aq::Painter *target;
protected: Index delay;

protected: Index x;
protected: Index y;
protected: Index width;
protected: Index height;
};

/* __FRAMEFEEDER_HPP__ */
#endif

