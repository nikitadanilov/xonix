#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "FrameFeeder.hpp"

FrameFeeder::FrameFeeder( Game *, 
						  QR::Aq::Drawable *aSource,
						  QR::Aq::Painter *aTarget,
						  Index aX, Index aY,
						  Index aWidth, Index aHeight,
						  Index aDelay )
{
  preC_( aSource != NULL );
  preC_( aTarget != NULL );
  preC_( aDelay > 0 );
  preC_( aX >= 0 );
  preC_( aY >= 0 );
  preC_( aWidth > 0 );
  preC_( aHeight > 0 );
	  
  FNNINIT( source, aSource );
  FNNINIT( target, aTarget );

  delay = aDelay;

  x = aX;
  y = aY;
  width = aWidth;
  height = aHeight;
}

FrameFeeder::~FrameFeeder()
{
  FDONE( target );
  FDONE( source );
}

QR::HandlerResult FrameFeeder::fire( QR::Condition *aCondition, 
								 QR::ObservationContext * )
{
#if defined( XONIX_USE_XQ )
  if( DCAST( aCondition, QR::Xq::WindowEvent ) != NULL )
#endif
#if defined( XONIX_USE_WQ )
  if( ( DCAST( aCondition, QR::Wq::WqEvent ) != NULL ) ||
	  ( DCAST( aCondition, QR::Wq::WindowEvent ) != NULL )
      /* &&
	  ( TCAST( aCondition, QR::Wq::WindowEvent ) -> event.message == WM_PAINT ) */ )
#endif
	{
		sync();
	}
  return 0;
}

void FrameFeeder::sync()
{
  OGuard _lock( source, this );
  QR::Aq::PainterGuard bitBlitter( target, this );

  target -> copyFrom( 0, 0, width, height, source, x, y );
}
