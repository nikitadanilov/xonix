#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Board.hpp"
#include "Cell.hpp"
#include "Game.hpp"

Board::Board( Game *aGame, Index aHeight, Index aWidth ) :
  GameObject( aGame ),
  height( aHeight ),  width( aWidth )
{
  preC_( aHeight > 0 );
  preC_( aWidth > 0 );

  cell = new Cell::State [ getHeight() * getWidth() ];
  version = new Index [ getHeight() * getWidth() ];
  
  setProtector( THR_SYSTEM -> recMutex() );
  TCAST( getProtector(), QR::Object ) -> dontManage();
}

Board::~Board()
{
  delete []cell;
}

Index Board::getHeight() const
{
  return height;
}

Index Board::getWidth() const
{
  return width;
}

boolean Board::isValidRow( CoordY aRow ) const
{
  return( ( 0 <= aRow ) && ( aRow < getHeight() ) );
}

boolean Board::isValidCol( CoordX aCol ) const
{
  return( ( 0 <= aCol ) && ( aCol < getWidth() ) );
}

void Board::clean()
{
  OGuard _self( this, this );

  preC_( getHeight() > 1 );
  preC_( getWidth() > 1 );

  FOR_ROWS( i0, this )
	{
	  FOR_COLS( k0, this )
		{
		  setState( k0, i0, Cell::vacuum );
		  version[ i0 * getWidth() + k0 ] = 0;
		}
	}

  FOR_COLS( i, this )
	{
	  setState( i, 0, Cell::solid );
	  setState( i, getHeight() - 1, Cell::solid );

	  if( ( i > 0 ) && ( i < getWidth() - 1 ) )
		{
		  setState( i, 1, Cell::border );
		  setState( i, getHeight() - 2, Cell::border );
		}
	}
  FOR_ROWS( j, this )
	{
	  setState( 0, j, Cell::solid );
	  setState( getWidth() - 1, j, Cell::solid );

	  if( ( j > 0 ) && ( j < getHeight() - 1 ) )
		{
		  setState( 1, j, Cell::border );
		  setState( getWidth() - 2, j, Cell::border );
		}
	}
}

Cell::State Board::getState( CoordX aCol, CoordY aRow ) const
{
  preC_( isValidRow( aRow ) );
  preC_( isValidCol( aCol ) );

  return cell[ aRow * getWidth() + aCol ];
}

void Board::setState( CoordX aCol, CoordY aRow, Cell::State aState )
{
  preC_( isValidRow( aRow ) );
  preC_( isValidCol( aCol ) );

  Cell::State oldState;

  oldState = getState( aCol, aRow );
  if( ( aState == Cell::border ) || ( aState == Cell::solid ) )
	{
	  ++filled;
	}
  else if( ( oldState == Cell::border ) || ( oldState == Cell::solid ) )
	{
	  --filled;
	}
  cell[ aRow * getWidth() + aCol ] = aState;
  version[ aRow * getWidth() + aCol ]++;
  getGame() -> version++;
}


void Board::getCellInfo( CoordX aCol, CoordY aRow, 
						  Cell::State *aState, Index *aVersion )
{
  preC_( isValidRow( aRow ) );
  preC_( isValidCol( aCol ) );

  preC_( aState != NULL );
  preC_( aVersion != NULL );

  OGuard _self( this, this );

  *aState = getState( aCol, aRow );
  *aVersion = version[ aRow * getWidth() + aCol ];
}

void Board::updateCellInfo( CoordX aCol, CoordY aRow, 
							Cell::State aState, Index aVersion )
{
  preC_( isValidRow( aRow ) );
  preC_( isValidCol( aCol ) );

  OGuard _self( this, this );

  test_( aVersion <= version[ aRow * getWidth() + aCol ] );
  if( ( aVersion != version[ aRow * getWidth() + aCol ] ) && ( aVersion != -1 ) )
	{
	  Debug::getLogger() -> log
		( "Clash detected: (%li, %li): %li != %li", 
		  aRow, aCol,
		  aVersion, version[ aRow * getWidth() + aCol ] );
	}
  setState( aCol, aRow, aState );
}

String Board::toString() const
{
  OGuard _self( this, this );

  QR::StringBuffer buffer( ( getWidth() + 1 ) * getHeight(), 
					   QR::StringBuffer::defaultDelta );

  FOR_ROWS( i, this )
	{
	  FOR_COLS( j, this )
		{
		  buffer.add( ( unichar ) Cell::getChar( getState( j, i ) ) );
		}
	  buffer.add( ( unichar ) '\n' );
	}
  return buffer.asString();
}

Index Board::getFilled() const
{
  return filled;
}

void Board::reset()
{
  clean();
}

