
#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include <quercus.hpp>

#include "Game.hpp"
#include "GameManager.hpp"

#if !defined( USE_THREADS )
#error XONIX requires threads!
#endif

namespace QR {

int QR::QRMain( QR::List *anArgs )
{
  GameManager *manager;

  manager = new GameManager();
  OGuard _manager( manager, manager );
  
  manager -> manage( anArgs );
  return 0;
}

};
