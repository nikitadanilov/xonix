#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Keyboard.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"

#if defined( XONIX_USE_XQ )
#include <X11/keysym.h>
#endif

Keyboard::Keyboard( Game *aGame ) :
  GameObject( aGame )
{
  dx = 0;
  dy = 0;
  setProtector( THR_SYSTEM -> recMutex() );

  PGuard _self( this, this );
  getGame() -> addPassive( this );
}

Keyboard::~Keyboard()
{}

QR::HandlerResult Keyboard::fire( QR::Condition *aCondition, 
							  QR::ObservationContext * )
{
#if defined( XONIX_USE_XQ )
  if( DCAST( aCondition, QR::Xq::KeyEvent ) != NULL )
	{
	  QR::Xq::KeyEvent *ev;

	  ev = TCAST( aCondition, QR::Xq::KeyEvent );

	  if( ev -> getData() -> type != KeyPress )
		{
		  return 0;
		}
	  {
		KeySym symbol;

		symbol = ev -> getKeySym();

		OGuard _self( this, this );
		switch( symbol )
		  {
		  case XK_Right:
		  case XK_KP_6:
			dx = 1;
			dy = 0;
			break;
		  case XK_Left:
		  case XK_KP_4:
			dx = -1;
			dy = 0;
			break;
		  case XK_Up:
		  case XK_KP_8:
			dx = 0;
			dy = 1;
			break;
		  case XK_Down:
		  case XK_KP_2:
			dx = 0;
			dy = -1;
			break;
		  case XK_space:
		  case XK_KP_5:
			dx = 0;
			dy = 0;
			break;
		  case XK_F10:
			getGame() -> incLives( 10 ); /* cheat a bit */
			break;
		  case XK_p:
		  case XK_Escape:
		  case XK_F11: /* ``Stop'' on SUN keyboards */
			getGame() -> flipPause();
			break;
		  case XK_F16: /* ``Copy'' on SUN keyboards */
		  case XK_d:
			getGame() -> makeScreenShot();
			break;
		  case XK_s:
			DLOGSTRING
			  ( "%s", getGame() -> getStats() );
			break;
		  case XK_q:
			getGame() -> setState( Game::over );
			break;
		  case XK_h:
		  case XK_F1:
			DLOGSTRING( "%s", game -> config -> usage() );
			break;
		  case XK_a:
			QR::Debug::getLogger() -> monitor();
			break;
		  default:
			DLOGSTRING( "keysym: %s", QR::Xq::KeyEvent::keysymToString
						( ev -> getDisplay(), symbol ) );
			break;
		  }
	  }
	}
#endif
#if defined( XONIX_USE_WQ )
  if( DCAST( aCondition, QR::Wq::KeyEvent ) != NULL )
	{
	  QR::Wq::KeyEvent *event;

	  event = TCAST( aCondition, QR::Wq::KeyEvent );
	  OGuard _event( event , this );
	  int keyCode;

	  if( ( event -> event.message != WM_KEYDOWN ) &&
		  ( event -> event.message != WM_CHAR ) )
	  {
		  return 0;
	  }
	  switch( ( int ) ( event -> event.wParam ) )
	  {
	  case 37: /* left */
			dx = -1;
			dy = 0;
			break;
	  case 38: /* up */
			dx = 0;
			dy = 1;
			break;
	  case 39: /* right */
			dx = 1;
			dy = 0;
			break;
	  case 40: /* down */
			dx = 0;
			dy = -1;
			break;
	  case ' ':
			dx = 0;
			dy = 0;
			break;
	  case 'q':
			getGame() -> setState( Game::over );
			break;
	  case 's':
			DLOGSTRING( "%s", getGame() -> getStats() );
			break;
	  case 'h':
			DLOGSTRING( "%s", game -> config -> usage() );
			break;
	  case 'p':
			getGame() -> flipPause();
			break;
	  case 121: /* F10 */
			getGame() -> incLives( 10 ); /* cheat a bit */
			break;
	  default:
//      	    Debug::getLogger() -> 
//				logObject( "Keyboard::fire(): QR::Wq::KeyEvent: %s", 
//					aCondition );
			break;
	  }
	  if( ( event -> event.lParam & ( 1 << 30 ) ) == 1 )
	  {
		dx = 0;
		dy = 0;
	  }
	}
#endif
#if defined( XONIX_USE_CQ )
  if( DCAST( aCondition, QR::Cq::KeyEvent ) != NULL )
	{
	  QR::Cq::KeyEvent *event;

	  event = TCAST( aCondition, QR::Cq::KeyEvent );
	  OGuard _event( event, this );

	  switch( event -> getKey() )
		{
		case KEY_LEFT: /* left */
		case '4': /* left */
		  dx = -1;
		  dy = 0;
		  break;
		case KEY_UP: /* up */
		case '8': /* left */
		  dx = 0;
		  dy = 1;
		  break;
		case KEY_RIGHT: /* right */
		case '6': /* left */
		  dx = 1;
		  dy = 0;
		  break;
		case KEY_DOWN: /* down */
		case '2': /* left */
		  dx = 0;
		  dy = -1;
		  break;
		case ' ':
		  dx = 0;
		  dy = 0;
		  break;
		case 'q':
		  getGame() -> setState( Game::over );
		  break;
		case 's':
		  DLOGSTRING( "%s", getGame() -> getStats() );
		  break;
		case 'h':
		  DLOGSTRING( "%s", game -> config -> usage() );
		  break;
		case 'p':
		  getGame() -> flipPause();
		  break;
		case KEY_F(10): /* F10 */
		  getGame() -> incLives( 10 ); /* cheat a bit */
		  break;
		case 12: /* ctrl-l */
		  clearok( TCAST( getGame() -> window, QR::Cq::Window ) -> getCWin(), 
				   true );
		  getGame() -> window -> update();
		  break;
		default:
		  DLOG( "Keycode: %i", event -> getKey() );
		  break;
		}
	}
#endif
  else
	{
	  if( ( getGame() -> getState() == Game::hit ) ||
		  ( getGame() -> getState() == Game::clipped ) )
		{
		  dx = 0;
		  dy = 0;
		}
	}
  //  DLOGOBJECT( "Keyboard::fire(): %s", aCondition );
  return 0;
}
 
void Keyboard::onKeyEvent( QR::Aq::KeyEvent * )
{}

int Keyboard::getDx()
{
  return dx;
}

int Keyboard::getDy()
{
  return dy;
}

void Keyboard::reset()
{
  dx = 0;
  dy = 0;
}

