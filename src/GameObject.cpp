#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "GameObject.hpp"
#include "Game.hpp"

GameObject::GameObject( Game *aGame ) :
  game( aGame )
{
  preC_( aGame != NULL );

  OREG( game );
}

GameObject::~GameObject()
{
  FDONE( game );
}

void GameObject::init()
{
  getGame() -> getObjects() -> append( this );
}

void GameObject::reset()
{}

Game *GameObject::getGame() const
{
  return ( Game * ) game;
}

