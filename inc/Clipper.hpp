#if !defined( __CLIPPER_HPP__ )
#define __CLIPPER_HPP__

#include <quercus.hpp>

class Game;
class Clipper : virtual public QR::PassiveObserver,
				virtual public QR::Object
{
public: Clipper( Game *aGame );
public: virtual ~Clipper();

public: virtual QR::HandlerResult fire( QR::Condition *aCondition, 
									QR::ObservationContext *aContext );

protected: Index fill( Index aX, Index aY, int aLevel );
protected: Index tryFill( Index aX, Index aY, int aLevel );
protected: Game *game;

protected: class BombFound : public virtual QR::Exception
{
public: BombFound();
};
protected: static int dirX[ 4 ];
protected: static int dirY[ 4 ];
};

/* __CLIPPER_HPP__ */
#endif
