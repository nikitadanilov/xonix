#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Cell.hpp"

char Cell::getChar( State aState )
{
  static char *dChars = NULL;

  if( dChars == NULL )
	{
	   dChars = new char[ INVALID + 1 ];
	   dChars[ vacuum ] = ' ';
	   dChars[ solid ] = '#';
	   dChars[ border ] = '+';
	   dChars[ path ] = '!';
	   dChars[ head ] = '@';
	   dChars[ ibomb ] = 'o';
	   dChars[ obomb ] = 'O';
	   dChars[ xbomb ] = 'X';
	   dChars[ sbomb ] = 'S';
	   dChars[ INVALID ] = '?';
	}
  test_( dChars != NULL );
  return dChars[ aState ];
}

