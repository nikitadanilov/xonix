#if !defined( __GAMEOBJECT_HPP__ )
#define __GAMEOBJECT_HPP__

#include <quercus.hpp>

#include "Game.hpp"

class Game;
class GameObject : public virtual QR::Object
{
public: GameObject( Game *aGame );
public: virtual ~GameObject();

public: virtual void init();
public: virtual void reset();

public: Game *getGame() const;

protected: const Game *game;
};

/* __GAMEOBJECT_HPP__ */
#endif

