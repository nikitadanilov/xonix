#if !defined( __ACURSOR_HPP__ )
#define __ACURSOR_HPP__

#include "Game.hpp"
#include "GameThread.hpp"

#include <quercus.hpp>

class Game;
class ACursor : public GameThread,
				public virtual QR::PassiveObserver
{
public: ACursor( Game *aGame, QR::Xq::Window *aWindow, 
				 QR::String aNormalPattern, Index aNormalStart, Index aNormalEnd,
				 QR::String aHitPattern, Index aHitStart, Index aHitEnd,
				 QR::String aClipPattern, Index aClipStart, Index aClipEnd,
				 Index aWidth, Index aHeight );
public: virtual ~ACursor();

public: virtual QR::HandlerResult fire( QR::Condition *aCondition, 
									QR::ObservationContext *aContext );

public: virtual void sync();

public: virtual void setCursor();
public: virtual boolean iteration();
public: virtual void reset();
public: virtual Index getDelay();

protected: QR::Xq::Window *window;

protected: QR::Xq::Cursor **normal;
protected: QR::Xq::Cursor **hit;
protected: QR::Xq::Cursor **clip;

protected: Index normalLength;
protected: Index hitLength;
protected: Index clipLength;

protected: Index i;
protected: Game::State state;

#define COLORS_IN_SPECTRUM 7
protected: QR::Xq::Color **spectrum;
};

/* __ACURSOR_HPP__ */
#endif

