#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "BoardMap.hpp"
#include "Cell.hpp"
#include "XGlyph.hpp"

BoardMap::BoardMap( Board *aBoard, QR::Aq::Size aWidth, QR::Aq::Size aHeight,
					QR::Aq::Size anAscent )
{
  preC_( aBoard != NULL );

  FINIT( board, aBoard );

  for( int s = Cell::vacuum ; s <= Cell::INVALID ; ++s )
	{
	  glyph[ s ] = NULL;
	}

  ascent = anAscent;
  width = aWidth;
  height = aHeight;

  setProtector( THR_SYSTEM -> recMutex() );
  TCAST( getProtector(), QR::Object ) -> dontManage();
}

BoardMap::~BoardMap()
{
  for( int s = Cell::vacuum ; s <= Cell::INVALID ; ++s )
	{
	  FCDONE( glyph[ s ] );
	}
  FCDONE( board );
}

Index BoardMap::getCWidth() const
{
  OGuard _self( this, this );

  return width;
}

Index BoardMap::getCHeight() const
{
  OGuard _self( this, this );

  return height;
}

Index BoardMap::getWidth() const
{
  OGuard _self( this, this );

  return getCWidth() * getBoard() -> getWidth();
}

Index BoardMap::getHeight() const
{
  OGuard _self( this, this );

  return getCHeight() * getBoard() -> getHeight();
}

Board *BoardMap::getBoard() const
{
  OGuard _self( this, this );

  return board;
}

void BoardMap::drawCell( QR::Aq::Painter *aPainter, 
						 CoordX aCol, CoordY aRow ) const
{
  preC_( aPainter != NULL );
  preC_( getBoard() -> isValidRow( aRow ) );
  preC_( getBoard() -> isValidCol( aCol ) );

  Cell::State state;
  XGlyph *cellGlyph;

  state = getBoard() -> getState( aCol, aRow );
  cellGlyph = getGlyph( state );

  if( cellGlyph != NULL )
	{
	  cellGlyph -> draw
		( aPainter, aCol * getCWidth(), 
		  ( getBoard() -> getHeight() - aRow - 1 ) * getCHeight(), 
		  getCWidth(), getCHeight() );
	}
  else
	{
	  char string[ 2 ];
  
	  string[ 0 ] = Cell::getChar( state );
	  string[ 1 ] = ( char ) 0;

	  QR::Aq::PainterGuard cellArtist( aPainter, this );

	  cellArtist.getPainter() -> clearRectangle
		( aCol * getCWidth(), 
		  ( getBoard() -> getHeight() - aRow - 1 ) * getCHeight(), 
		  getCWidth(), getCHeight() );
	  cellArtist.getPainter() -> drawCString
		( aCol * getCWidth(), 
		  ( getBoard() -> getHeight() - aRow - 1 ) * getCHeight() + ascent, 
		  string );
	}
}

void BoardMap::draw( QR::Aq::Painter *aPainter ) const
{
  preC_( aPainter != NULL );

  OGuard _drawable( aPainter, this );

  FOR_ROWS( i, getBoard() )
	{
	  FOR_COLS( j, getBoard() )
		{
		  drawCell( aPainter, j, i );
		}
	}
}

Index BoardMap::getAscent() const
{
  return ascent;
}

void BoardMap::setGlyph( Cell::State aState, XGlyph *aGlyph )
{
  OGuard _self( this, this );

  preC_( ( 0 <= aState ) && ( aState <= Cell::INVALID ) );
  preC_( aGlyph != NULL );

  FNNINIT( glyph[ aState ], aGlyph );

  if( aGlyph -> getWidth() > width )
	{
	  width = aGlyph -> getWidth();
	}
  if( aGlyph -> getHeight() > height )
	{
	  height = aGlyph -> getHeight();
	}
  //Debug::getLogger() -> log( "BoardMap::setGlyph(): %ix%i", width, height );
}

XGlyph *BoardMap::getGlyph( Cell::State aState ) const
{
  OGuard _self( this, this );

  preC_( ( 0 <= aState ) && ( aState <= Cell::INVALID ) );

  return glyph[ aState ];
}

