#if !defined( __GAMEMANAGER_HPP__ )
#define __GAMEMANAGER_HPP__

#include <quercus.hpp>

class Game;
class GameManager : public virtual QR::Object
{
public: GameManager();
public: virtual ~GameManager();

public: void manage( QR::List *anArgs );
};

/* __GAMEMANAGER_HPP__ */
#endif

