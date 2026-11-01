#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Game.hpp"
#include "Board.hpp"
#include "BoardMap.hpp"

#if defined( XONIX_USE_XQ ) || defined( XONIX_USE_CQ )
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "ACursor.hpp"
#endif

#include "Keyboard.hpp"
#include "FrameFeeder.hpp"
#include "IBombThread.hpp"
#include "OBombThread.hpp"
#include "Player.hpp"
#include "SClipper.hpp"
#include "StatusLine.hpp"
#include "FontGlyph.hpp"
#include "PixmapGlyph.hpp"
#include "BitmapGlyph.hpp"
#include "AnimGlyph.hpp"
#include "GameConfig.hpp"
#include "GameScheduler.hpp"

#if defined( XONIX_USE_CQ )
class Refresh : public QR::Runnable, public virtual QR::Object
{
public: Refresh( Game *aGame )
	{
	  game = aGame;
	}
public: virtual void *run()
	{
	  while( true )
		{
		  {
			OGuard _window( game -> window, this );
			clearok( TCAST( game -> window, QR::Cq::Window ) -> getCWin(), 
					 true );
			game -> window -> update();
			DLOG( "refreshed" );
		  }
		  THR_SYSTEM -> sleep( 3, 0 );
		}
	  return NULL;
	};
protected: Game *game;
};
#endif

Game::Game( GameConfig *aConfig )
{
  preC_( aConfig != NULL );

  int i;

#if defined( USE_THREADS ) && defined( XQ_USE_X11_MUTEX )
  test_( ::XInitThreads() != 0 );
#endif

  setProtector( THR_SYSTEM -> recMutex() );
  TCAST( getProtector(), QR::Object ) -> dontManage();

  FNNINIT( config, aConfig );

  lives = config -> lives;
  score = 0;
  paused = false;

  FINIT( objects, new QR::List() );
  FINIT( threads, new QR::List() );

  state = normal;
  level = 0;
  version = 0;

  clinamena = false;
  clinamenaPromille = config -> clinamenaPromille;

#if defined( XONIX_USE_CQ )
  redirectLog();
  QR::Cq::initClass();
  FINIT( board, new Board( this, 
						   QR::Cq::CursesUI::getScreenHeight() - 2, 
						   QR::Cq::CursesUI::getScreenWidth() ) );
#else
  FINIT( board, new Board( this, 
						   config -> boardHeight, 
						   config -> boardWidth ) );
#endif
  board -> dontManage();
  board -> init();
  board -> clean();

#if defined( XONIX_USE_CQ )
  initCursesGraphics();
  THR_FACTORY -> createFromRunner( new Refresh( this ) );
#endif
#if defined( XONIX_USE_XQ )
  initX11Graphics();
#endif

  FINIT( keyboard, new Keyboard( this ) );
  keyboard -> dontManage();
  keyboard -> init();

#if defined( XONIX_USE_WQ )
  initWindowsGraphics();
#endif

  FINIT( events, window -> getEventSource() );

#if defined( XONIX_USE_WQ )
  TCAST( events, QR::Wq::WindowEventSource ) -> selectMouseMotion( false );
#endif

  events -> addPassive( keyboard );

#if defined( XONIX_USE_XQ ) || defined( XONIX_USE_CQ )
  THR_FACTORY -> createFromRunner( events );
#endif

  FINIT( feeder, new FrameFeeder( this,
								  pix, winPainter, 0, 0, 
								  map -> getWidth(), 
								  map -> getHeight(),
								  config -> frameDelay ) );

  map -> draw( pixPainter );

#if !defined( XONIX_USE_CQ )
  feeder -> sync();
  events -> addPassive( feeder );
#endif

  movingObject = new QR::List *[ ( int ) MAXMO ];

  scheduler = new GameScheduler *[ ( int ) MAXMO ];
  schedulerThread = new QR::Thread *[ ( int ) MAXMO ];

  for( i = 0 ; i < MAXMO ; ++i )
	{
	  FINIT( movingObject[ i ], new QR::List() );
	  FINIT( scheduler[ i ], new GameScheduler
			 ( this, getMovingObjects( ( MovingObject ) i ) ) );
	}

  FINIT( bombs, new QR::List() );

  BombThread *bomb;

  for( i = 0 ; i < config -> oBombsNum ; ++i )
	{
	  bomb = new OBombThread( this );
	  getBombs() -> append( bomb );
	  getMovingObjects( obombs ) -> append( bomb );
	  bomb -> init();
	}
  for( i = 0 ; i < config -> iBombsNum ; ++i )
	{
	  bomb = new IBombThread( this );
	  getBombs() -> append( bomb );
	  getMovingObjects( ibombs ) -> append( bomb );
	  bomb -> init();
	}

  FINIT( player, new Player( this ) );
  player -> init();

  FINIT( clipper, new SClipper( this ) );
  addPassive( clipper );
  clipper -> init();

  FINIT( statusLine, new StatusLine( this ) );
  statusLine -> init();

  start();

  //   EchoObserver *echo;
  //   echo = new EchoObserver();
  //   OGuard _echo( echo, echo );
  //   addPassive( echo );
  //   events -> addPassive( echo );
}

Game::~Game()
{
  events -> removePassive( feeder );
  events -> removePassive( keyboard );
  removePassive( clipper );

#if defined( XONIX_USE_XQ )
  if( config -> acursor )
	{
	  removePassive( cursorThread );
	}
#endif

  for( int i = 0 ; i < MAXMO ; ++i )
	{
	  FDONE( scheduler[ obombs ] );
	  FDONE( schedulerThread[ obombs ] );
	  FDONE( movingObject[ obombs ] );
	}
  FDONE( fgPainter );
  FDONE( bgPainter );
  FDONE( plPainter );
  FDONE( grayPainter );
  FDONE( statusLine );
  FDONE( clipper );
  FDONE( player );
  FDONE( bombs );
  FDONE( feeder );
  FDONE( winPainter );
  FDONE( pixPainter );
  FDONE( pix );
  FDONE( events );
  FDONE( keyboard );
  FDONE( window );
  FDONE( map );
  FDONE( board );
#if defined( XONIX_USE_XQ )
  FDONE( clear );
  FDONE( gc );
  FDONE( fg );
  FDONE( bg );
  FDONE( display );
  FDONE( cursorThread );
#endif
  FDONE( threads );
  FDONE( objects );
  FDONE( config );

  delete []schedulerThread;
  delete []scheduler;
  delete []movingObject;
}

#if defined( XONIX_USE_CQ )

/*
  Quercus traces go to stderr. When stderr is the terminal curses draws
  on, they corrupt the display, so send them to config -> logFile
  (or discard them if it cannot be opened).
*/
void Game::redirectLog()
{
  int fd;
  char *fileName;

  if( !isatty( STDERR_FILENO ) )
	{
	  return;
	}
  fileName = config -> logFile.getChars();
  fd = open( fileName, O_WRONLY | O_CREAT | O_APPEND, 0644 );
  delete []fileName;
  if( fd == -1 )
	{
	  fd = open( "/dev/null", O_WRONLY );
	}
  if( fd != -1 )
	{
	  dup2( fd, STDERR_FILENO );
	  close( fd );
	}
}

void Game::initCursesGraphics()
{
  QR::Cq::initClass();
  curs_set( 0 );
  FINIT( window, QR::Cq::Window::create
		 ( 0, 0, 
		   QR::Cq::CursesUI::getScreenWidth(), QR::Cq::CursesUI::getScreenHeight() ) );
  window -> dontManage();
  window -> setProtector( THR_SYSTEM -> recMutex() );
  /* keys are read from this window: decode arrows into KEY_* codes */
  keypad( window -> getCWin(), true );

  FINIT( map, new BoardMap( board, 1, 1, 0 ) );
  map -> dontManage();
  FINIT( pix, window );

  FINIT( winPainter, new QR::Cq::Painter( new QR::Cq::GC( 0 ), window ) );
  winPainter -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( pixPainter, new QR::Cq::Painter( new QR::Cq::GC( 0 ), window ) );
  pixPainter -> setProtector( THR_SYSTEM -> recMutex() );

  initCursesGlyphs();
}

void Game::initCursesGlyphs()
{
  QR::Cq::Painter *mono;
  QR::Cq::Painter *red;
  QR::Cq::Painter *inv;

  mono = new QR::Cq::Painter( new QR::Cq::GC( 0 ), window );
  OGuard _mono( mono, this );

  if( QR::Cq::CursesUI::hasColors() )
	{
	  test_( init_pair( 1, COLOR_RED, COLOR_BLACK ) == OK );
	  red = new QR::Cq::Painter( new QR::Cq::GC( COLOR_PAIR( 1 ) ), window );
	  test_( init_pair( 2, COLOR_BLACK, COLOR_WHITE ) == OK );
	  inv = new QR::Cq::Painter( new QR::Cq::GC( COLOR_PAIR( 2 ) ), window );
	}
  else
	{
	  red = new QR::Cq::Painter( new QR::Cq::GC( A_BOLD ), window );
	  inv = new QR::Cq::Painter( new QR::Cq::GC( A_REVERSE ), window );
	}
  OGuard _red( red, this );
  OGuard _inv( inv, this );

  map -> setGlyph( Cell::head,   new FontGlyph( this, '@', red ) );
  map -> setGlyph( Cell::obomb,  new FontGlyph( this, 'O', mono ) );
  map -> setGlyph( Cell::ibomb,  new FontGlyph( this, ' ', mono ) );
  map -> setGlyph( Cell::xbomb,  new FontGlyph( this, 'X', mono ) );
  map -> setGlyph( Cell::sbomb,  new FontGlyph( this, 'S', mono ) );
  map -> setGlyph( Cell::path,   new FontGlyph( this, '.', mono ) );
  map -> setGlyph( Cell::solid,  new FontGlyph( this, ACS_BLOCK, mono ) );
  map -> setGlyph( Cell::border, new FontGlyph( this, ACS_BLOCK, mono ) );
  map -> setGlyph( Cell::vacuum, new FontGlyph( this, ' ', mono ) );
}

#endif

#if defined( XONIX_USE_XQ )

void Game::initX11Graphics()
{
  QR::Xq::Font *fixed;
  QR::Xq::Color *gray;
  int width;
  int height;

  if( config -> display.length() == 0 )
	{
	  display = QR::Xq::Display::open();
	}
  else
	{
	  char *displayName;

	  displayName = config -> display.getChars();
	  display = QR::Xq::Display::open( displayName );
	  delete []displayName;
	}
  for( int i = 0 ; i < 1000 ; ++i )
	{
	  display -> addRef();
	}
  display -> setProtector( THR_SYSTEM -> recMutex() );

  fixed = QR::Xq::Font::createFixed( display );
  OGuard _fixed( fixed, this );

  mapCellWidth = fixed -> getWidth();
  mapCellHeight = fixed -> getHeight();
  mapCellAscent = fixed -> getAscent();

  FINIT( map, 
		 new BoardMap( board, mapCellWidth, mapCellHeight, mapCellAscent ) );
  map -> dontManage();

  FINIT( bg, QR::Xq::Color::create( display, config -> bg ) );
  FINIT( fg, QR::Xq::Color::create( display, config -> fg ) );

  FINIT( window, QR::Xq::Window::createSimple
		 ( display, display -> getRootWindow(),
		   0, 0,
		   map -> getWidth(), map -> getHeight() + map -> getCHeight() + 1, 
		   0,
		   bg -> getData() -> pixel, bg -> getData() -> pixel ) );
  window -> dontManage();

  FINIT( gc, QR::Xq::GC::create( display, window ) );
  gc -> dontManage();

  gc -> setBackground( bg );
  gc -> setForeground( fg );
  gc -> setGraphicsExposures( false );

  FINIT( clear, QR::Xq::GC::create( display, window ) );
  clear -> dontManage();

  clear -> setBackground( bg );
  clear -> setForeground( bg );

  gray = QR::Xq::Color::create( display, config -> pg );
  OGuard _gray( gray, this );

  FINIT( fgPainter, QR::Xq::XPainter::create( gc, window, fg ) );
  fgPainter -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( bgPainter, QR::Xq::XPainter::create( gc, window, bg ) );
  bgPainter -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( plPainter, QR::Xq::XPainter::create
		 ( gc, window, 
		   QR::Xq::Color::create( display, config -> pl ) ) );
  plPainter -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( grayPainter, QR::Xq::XPainter::create( gc, window, gray ) );
  grayPainter -> setProtector( THR_SYSTEM -> recMutex() );

  initX11Glyphs();

  width = map -> getWidth();
  height = map -> getHeight() + map -> getCHeight() + 1;

  FINIT( pix, QR::Xq::Pixmap::create
		 ( window, map -> getWidth(), map -> getHeight(), 
		   display -> defaultDepth() ) );
  // pix = window;
  pix -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( winPainter, new QR::Xq::XPainter( gc, window ) );
  winPainter -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( pixPainter, new QR::Xq::XPainter( gc, pix ) );
  pixPainter -> setProtector( THR_SYSTEM -> recMutex() );

  window -> setSize( width, height);
  window -> setSizeHints( width, height, 
						  width, height,
						  width, height,
						  0, 0 );
  window -> setName( config -> windowTitle );
  window -> setClass( config -> appName, config -> resourceClass );

  window -> clear();
  window -> map();
  window -> selectInput
	( ExposureMask | 
	  KeyPressMask | 
	  // ButtonPressMask | StructureNotifyMask |
	  KeyReleaseMask 
	  // | ButtonReleaseMask 
	  //| EnterWindowMask | LeaveWindowMask |
	  //PointerMotionMask | /* PointerMotionHintMask | */ Button1MotionMask |
	  //Button2MotionMask |Button3MotionMask | Button4MotionMask | 
	  //Button5MotionMask | ButtonMotionMask | KeymapStateMask | ExposureMask |
	  //VisibilityChangeMask | ResizeRedirectMask | SubstructureNotifyMask |
	  //SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
	  //ColormapChangeMask | OwnerGrabButtonMask 
	  );

  FINIT( cursorThread, new ACursor
		 ( this, window, 
		   config -> normalCursorPattern, 
		   config -> normalCursorFrom, config -> normalCursorTo, 
		   config -> hitCursorPattern, 
		   config -> hitCursorFrom, config -> hitCursorTo, 
		   config -> clipCursorPattern, 
		   config -> clipCursorFrom, config -> clipCursorTo, 
		   32, 32 ) );
  if( config -> acursor )
	{
	  addPassive( cursorThread );
	}
  cursorThread -> init();
}

void Game::initX11Glyphs()
{
  if( config -> classic )
	{
	  map -> setGlyph( Cell::head, new FontGlyph( this, '@', plPainter ) );
	  map -> setGlyph( Cell::obomb, new FontGlyph( this, 'O', fgPainter ) );
	  map -> setGlyph( Cell::ibomb, new FontGlyph( this, 0, grayPainter ) );
	  map -> setGlyph( Cell::xbomb, new FontGlyph( this, 'X', grayPainter ) );
	  map -> setGlyph( Cell::sbomb, new FontGlyph( this, 'S', fgPainter ) );
	}
  else if( config -> mini )
	{
	  map -> setGlyph
		( Cell::head,
		  new PixmapGlyph( this, "img/mini-rball.xpm", PixmapGlyph::XPM, gc ) );
	  map -> setGlyph
		( Cell::obomb,
		  new PixmapGlyph( this, "img/mini-cd.xpm", PixmapGlyph::XPM, clear ) );
	  map -> setGlyph
		( Cell::ibomb,
		  new PixmapGlyph( this, "img/mini-ball.xpm", PixmapGlyph::XPM, gc ) );
	  map -> setGlyph
		( Cell::xbomb,
		  new PixmapGlyph( this, "img/mini-x2.xpm", PixmapGlyph::XPM, gc ) );
	  map -> setGlyph
		( Cell::sbomb,
		  new PixmapGlyph( this, "img/mini-bball.xpm", 
						   PixmapGlyph::XPM, clear ) );
	}
  else 
	{
	  if( config -> anim )
		{
		  map -> setGlyph
			( Cell::head, new AnimGlyph
			  ( this,
				new QR::List
				( 0, 
				  OW( new PixmapGlyph
					  ( this, "img/rball-a3.xpm", PixmapGlyph::XPM, gc ) ),
				  OW( new PixmapGlyph
					  ( this, "img/rball-a2.xpm", PixmapGlyph::XPM, gc ) ),
				  OW( new PixmapGlyph
					  ( this, "img/rball-a1.xpm", PixmapGlyph::XPM, gc ) ),
				  OW( new PixmapGlyph
					  ( this, "img/rball-a0.xpm", PixmapGlyph::XPM, gc ) ),
				  OW( new PixmapGlyph
					  ( this, "img/rball-a1.xpm", PixmapGlyph::XPM, gc ) ),
				  OW( new PixmapGlyph
					  ( this, "img/rball-a2.xpm", PixmapGlyph::XPM, gc ) ),
				  OW( new PixmapGlyph
					  ( this, "img/rball-a3.xpm", PixmapGlyph::XPM, gc ) ),
				  nil ),
				new QR::List
				( 0, 
				  OW( new QR::Integer( 70 ) ), 
				  OW( new QR::Integer( 70 ) ), 
				  OW( new QR::Integer( 70 ) ), 
				  OW( new QR::Integer( 70 ) ), 
				  OW( new QR::Integer( 70 ) ), 
				  OW( new QR::Integer( 70 ) ), 
				  OW( new QR::Integer( 2000 ) ), 
				  nil ) ) );
		}
	  else
		{
		  map -> setGlyph
			( Cell::head,
			  new PixmapGlyph( this, "img/rball.xpm", PixmapGlyph::XPM, gc ) );
		}
	  map -> setGlyph
		( Cell::obomb,
		  new PixmapGlyph( this, "img/cd.xpm", PixmapGlyph::XPM, clear ) );
	  map -> setGlyph
		( Cell::ibomb,
		  new PixmapGlyph( this, "img/ball.xpm", PixmapGlyph::XPM, gc ) );
	  map -> setGlyph
		( Cell::xbomb,
		  new PixmapGlyph( this, "img/x2.xpm", PixmapGlyph::XPM, gc ) );
	  map -> setGlyph
		( Cell::sbomb,
		  new PixmapGlyph( this, "img/bball.xpm", PixmapGlyph::XPM, clear ) );
	}

  map -> setGlyph( Cell::path, new FontGlyph( this, 0, grayPainter ) );
  map -> setGlyph( Cell::solid, new FontGlyph( this, 0, fgPainter ) );
  map -> setGlyph( Cell::border, new FontGlyph( this, 0, fgPainter ) );
  map -> setGlyph( Cell::vacuum, new FontGlyph( this, 0, bgPainter ) );

}

/* XONIX_USE_XQ */
#endif

#if defined( XONIX_USE_WQ )

#include "resource.h"

static COLORREF rgbFromString( QR::String aRGB, 
							   COLORREF aDefault )
{
	/* #rrggbb */
	/* 0123456 */
	if( ( aRGB.length() == 7 ) &&
			( aRGB.getUNIChars()[ 0 ] == ( unichar ) '#' ) )
		{
			return RGB( 
				aRGB.clip( 1, 3 ).toNum( 16 ),
				aRGB.clip( 3, 5 ).toNum( 16 ),
				aRGB.clip( 5, 7 ).toNum( 16 ) );
		}
	else
		{
			return aDefault;
		}
}

void Game::initWindowsGraphics()
{
  QR::Wq::Font  *fixed;
  QR::Wq::Pen   *pen;
  QR::Wq::Brush *bgBrush;
  QR::Wq::Brush *fgBrush;
  QR::Wq::GC    *gc;
  QR::Wq::GC    *clear;
  QR::Wq::DC    *dc;
  COLORREF   gray;

  int width;
  int height;

  fixed = QR::Wq::Font::create
		( 0, 0, 0, 0, FW_SEMIBOLD, 
			false, false, false, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, NULL );
  OGuard _fixed( fixed, this );

  if( config -> classic )
		{
			mapCellWidth = 12;
			mapCellHeight = 18;
			mapCellAscent = 0;
		}
	else
		{
			mapCellWidth = 0;
			mapCellHeight = 0;
			mapCellAscent = 0;
		}

  FINIT( map,
		 new BoardMap( board, mapCellWidth, mapCellHeight, mapCellAscent ) );
  map -> dontManage();

  bg = rgbFromString( config -> bg, RGB( 0, 0, 0 ) );
  fg = rgbFromString( config -> fg, RGB( 0xff, 0xff, 0xff ) );

  FINIT( window, QR::Wq::Window::create() );
  window -> dontManage();

  pen = QR::Wq::Pen::create( PS_SOLID, 0, RGB( 0, 0xff, 0 ) );
  OGuard _pen( pen, this );

  bgBrush = QR::Wq::Brush::createSolid( bg );
  OGuard _bgBrush( bgBrush, this );

  fgBrush = QR::Wq::Brush::createSolid( fg );
  OGuard _fgBrush( fgBrush, this );

  gc = new QR::Wq::GC( fg, bg, pen, fgBrush, fixed );
  OGuard _gc( gc, this );

  clear = new QR::Wq::GC( fg, bg, pen, bgBrush, fixed );
  OGuard _clear( clear, this );

  gray = rgbFromString( config -> pg, RGB( 200, 200, 200 ) );

  FINIT( fgPainter, new QR::Wq::WPainter( gc, window ) );
  fgPainter -> setProtector( THR_SYSTEM -> mutex() );

  FINIT( bgPainter, new QR::Wq::WPainter( clear, window ) );
  bgPainter -> setProtector( THR_SYSTEM -> mutex() );

  FINIT( plPainter, new QR::Wq::WPainter
		 ( new QR::Wq::GC( rgbFromString( config -> pl, RGB( 0xff, 0, 0 ) ), 
					   bg, pen, 
					   QR::Wq::Brush::createSolid
					   ( rgbFromString( config -> pl, RGB( 0xff, 0, 0 ) ) ), 
					   fixed ), 
		   window ) );
  plPainter -> setProtector( THR_SYSTEM -> mutex() );

  FINIT( grayPainter, new QR::Wq::WPainter
		 ( new QR::Wq::GC( gray, bg, pen, QR::Wq::Brush::createSolid( gray ), 
					   fixed ), window ) );
  grayPainter -> setProtector( THR_SYSTEM -> mutex() );

  initWindowsGlyphs();

  width = map -> getWidth() + 4;
  height = map -> getHeight() + 24 + 1;

  dc = window -> getDC();
  OGuard _dc( dc, this );

  QR::Wq::DC *screenDC;
  QR::Wq::Bitmap *bitmap;

  screenDC = new QR::Wq::DC( CreateDC( "DISPLAY", NULL, NULL, NULL ) );
  OGuard _screen( screenDC, this );

  bitmap = QR::Wq::Bitmap::create( screenDC, 
	  map -> getWidth(), map -> getHeight() );
  FINIT( pix, bitmap );

  bitmap -> setDC( screenDC ); 
  pix -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( winPainter, new QR::Wq::WPainter( gc, window ) );
  winPainter -> setProtector( THR_SYSTEM -> recMutex() );

  FINIT( pixPainter, new QR::Wq::WPainter
		 ( new QR::Wq::GC( fg, bg, pen, fgBrush, fixed ), 
		   pix ) );
  pixPainter -> setProtector( THR_SYSTEM -> recMutex() );

  window -> setSize( width, height );
  window -> setTitle( config -> windowTitle );

  window -> show();

  window -> setCursor( QR::Wq::Cursor::loadGlobal( NULL, IDC_CROSS ) );
//	for( int i = 0 ; i < ( sizeof cursor_00_xbm_bits ) / ( sizeof cursor_00_xbm_bits[ 0 ] ) ; ++i )
//		{
//			cursor_00_xbm_and[ i ] = 0xff;
//		}
//		QR::Wq::Cursor::create( cursor_00_xbm_width, cursor_00_xbm_height,
//			cursor_00_xbm_x_hot, cursor_00_xbm_y_hot, 
//			cursor_00_xbm_and, cursor_00_xbm_bits ) );
}

void Game::initWindowsGlyphs()
{
  if( config -> classic )
		{
			map -> setGlyph( Cell::head,  new FontGlyph( this, '@', plPainter ) );
			map -> setGlyph( Cell::obomb, new FontGlyph( this, 'O', fgPainter ) );
			map -> setGlyph( Cell::ibomb, new FontGlyph( this, 0, grayPainter ) );
			map -> setGlyph( Cell::xbomb, new FontGlyph( this, 'X', grayPainter ) );
			map -> setGlyph( Cell::sbomb, new FontGlyph( this, 'S', fgPainter ) );
		}
  else if( config -> mini )
		{
			map -> setGlyph( Cell::head,   new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_MINI_RBALL ), 8, 8 ) );
			map -> setGlyph( Cell::obomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_MINI_CD ), 8, 8 ) );
			map -> setGlyph( Cell::ibomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_MINI_BALL ), 8, 8 ) );
			map -> setGlyph( Cell::xbomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_MINI_X2 ), 8, 8 ) );
			map -> setGlyph( Cell::sbomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_MINI_BBALL ), 8, 8 ) );
		}
	else
		{
			map -> setGlyph( Cell::head,   new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_RBALL ), 16, 16 ) );
			map -> setGlyph( Cell::obomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_CD ), 16, 16 ) );
			map -> setGlyph( Cell::ibomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_BALL ), 16, 16 ) );
			map -> setGlyph( Cell::xbomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_X2 ), 16, 16 ) );
			map -> setGlyph( Cell::sbomb,  new BitmapGlyph( this, QR::Wq::Bitmap::load( IDB_BBALL ), 16, 16 ) );
	  }
  map -> setGlyph( Cell::path,   new FontGlyph( this, 0, grayPainter ) );
  map -> setGlyph( Cell::solid,  new FontGlyph( this, 0, fgPainter ) );
  map -> setGlyph( Cell::border, new FontGlyph( this, 0, fgPainter ) );
  map -> setGlyph( Cell::vacuum, new FontGlyph( this, 0, bgPainter ) );
}

/* XONIX_USE_WQ */
#endif

Board *Game::getBoard() const
{
  return board;
}

BoardMap *Game::getMap() const
{
  return map;
}

QR::Aq::Drawable *Game::getPixmap() const
{
  return pix;
}

QR::Aq::Window *Game::getWindow() const
{
  return window;
}

Game::State Game::getState() const
{
  OGuard _self( this, this );
  return getStateFast();
}

Game::State Game::getStateFast() const
{
  return state;
}

void Game::setState( State aNewState )
{
  OGuard _self( this, this );
  state = aNewState;
}

boolean Game::isPaused() const
{
  return paused;
}

void Game::flipPause()
{
  paused = !paused;
}

void Game::drawCell( CoordX aCol, CoordY aRow ) const
{
  //   OGuard _self( this, this );
  //   OGuard _map( getMap(), this );
  //   OGuard _pix( getPixmap(), this );
  //   OGuard _window( window, this );

  preC_( getBoard() -> isValidRow( aRow ) );
  preC_( getBoard() -> isValidCol( aCol ) );

  getMap() -> drawCell( pixPainter, aCol, aRow );
  getMap() -> drawCell( winPainter, aCol, aRow );
}

void Game::update()
{
  OGuard _self( this, this );
  fire( new QR::JustCondition(), new QR::ObservationContext() );
}

Player *Game::getPlayer() const
{
  return player;
}

Keyboard *Game::getKeyboard() const
{
  return keyboard;
}

#if defined( XONIX_USE_XQ )

QR::Xq::GC *Game::getGC() const
{
  return gc;
}

QR::Xq::GC *Game::getClearGC() const
{
  return clear;
}

QR::Xq::Display *Game::getDisplay() const
{
  return display;
}

QR::Xq::Color *Game::getBg() const
{
  return bg;
}

QR::Xq::Color *Game::getFg() const
{
  return fg;
}

QR::Xq::Drawable *Game::getXPixmap() const
{
  return pix;
}

QR::Xq::Window *Game::getXWindow() const
{
  return window;
}

#endif


int Game::getLives() const
{
  return lives;
}

void Game::incLives( Index aShift )
{
  lives += aShift;
}

QR::List *Game::getBombs() const
{
  return bombs;
}

QR::List *Game::getObjects() const
{
  return objects;
}

QR::List *Game::getThreads() const
{
  return threads;
}

void Game::stop()
{
  if( config -> oneToMany )
	{
	  for( int i = 0 ; i < MAXMO ; ++i )
		{
		  test_( schedulerThread[ i ] != NULL );
		  schedulerThread[ i ] -> join();
		}
	}
  else
	{
	  for( getThreads() -> toFirst() ; ! getThreads() -> atEnd() ;
		   getThreads() -> next() )
		{
		  TCAST( getThreads() -> getCurrentValue(), GameThread ) -> stop();
		}
	}
  player -> stop();
  //  feeder -> stop();

#if defined( XONIX_USE_XQ )
  if( config -> acursor )
	{
	  cursorThread -> stop();
	}
#endif

  statusLine -> stop();
}

void Game::start()
{
  OGuard _self( this, this );
  OGuard _board( getBoard(), this );
  OGuard _map( getMap(), this );
  OGuard _player( getPlayer(), this );

  //  feeder -> start();
  player -> start();
#if defined( XONIX_USE_XQ )
  if( config -> acursor )
	{
	  cursorThread -> start();
	}
#endif
  statusLine -> start();

  if( config -> oneToMany )
	{
	  for( int i = 0 ; i < MAXMO ; ++i )
		{
		  schedulerThread[ i ] = 
			THR_FACTORY -> createFromRunner( scheduler[ i ] );
		}
	}
  else
	{
	  for( getThreads() -> toFirst() ; ! getThreads() -> atEnd() ;
		   getThreads() -> next() )
		{
		  TCAST( getThreads() -> getCurrentValue(), GameThread ) -> start();
		}
	}
}

void Game::reset()
{
  for( getObjects() -> toFirst() ; ! getObjects() -> atEnd() ;
	   getObjects() -> next() )
	{
	  TCAST( getObjects() -> getCurrentValue(), GameObject ) -> reset();
	}
  //  feeder -> reset();
  // player -> reset();
#if defined( XONIX_USE_XQ )
  if( config -> acursor )
	{
	  cursorThread -> reset();
	}
#endif
  statusLine -> reset();

  map -> draw( pixPainter );
#if !defined( XONIX_USE_CQ )
  feeder -> sync();
#endif
}

Index Game::getScore() const
{
  return score;
}

void Game::incScore( Index aShift )
{
  OGuard _self( this, this );
  score += ( aShift * ( level / 2 + 1 ) );
  if( score < 0 )
	{
	  score = 0;
	}
}

String Game::getStats() const
{
  QR::StringBuffer buffer( QR::StringBuffer::defaultLength,
					   QR::StringBuffer::defaultDelta );

  buffer.add( QR::String( "\nStatistics:" ) );
  buffer.add( QR::String( "\n\tboardWidth: %li" ).sprintf
			  ( 1, config -> boardWidth ) );
  buffer.add( QR::String( "\n\tboardHeight: %li" ).sprintf
			  ( 1, config -> boardHeight ) );
  buffer.add( QR::String( "\n\tbombDelay: %li" ).sprintf
			  ( 1, config -> bombDelay ) );
  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\tplayerDelay: %li" ).sprintf
			  ( 1, config -> playerDelay ) );
  buffer.add( QR::String( "\n\tframeDelay: %li" ).sprintf
			  ( 1, config -> frameDelay ) );
  buffer.add( QR::String( "\n\tstatusDelay: %li" ).sprintf
			  ( 1, config -> statusDelay ) );
  buffer.add( QR::String( "\n\tcursorDelay: %li" ).sprintf
			  ( 1, config -> cursorDelay ) );
  buffer.add( QR::String( "\n\tmanagerDelay: %li" ).sprintf
			  ( 1, config -> managerDelay ) );
  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\tiBombsNum: %li" ).sprintf
			  ( 1, config -> iBombsNum ) );
  buffer.add( QR::String( "\n\toBombsNum: %li" ).sprintf
			  ( 1, config -> oBombsNum ) );
  buffer.add( QR::String( "\n\tiBombsNumStep: %li" ).sprintf
			  ( 1, config -> iBombsNumStep ) );
  buffer.add( QR::String( "\n\toBombsNumStep: %li" ).sprintf
			  ( 1, config -> oBombsNumStep ) );
  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\tplayerAccel: %li" ).sprintf
			  ( 1, config -> playerAccel ) );
  buffer.add( QR::String( "\n\tbombAccel: %li" ).sprintf
			  ( 1, config -> bombAccel ) );
  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\tlives: %li" ).sprintf( 1, lives ) );
  buffer.add( QR::String( "\n\tfillThreshold: %li" ).sprintf
			  ( 1, config -> fillThreshold ) );
  buffer.add
	( QR::String( "\n\tfillThresholdStep: %li" ).sprintf
	  ( 1, config -> fillThresholdStep ) );

  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\toneToMany: %i" ).sprintf( 1, config -> oneToMany ) );
  buffer.add( QR::String( "\n\tclassic: %i" ).sprintf( 1, config -> classic ) );
  buffer.add( QR::String( "\n\tmini: %i" ).sprintf( 1, config -> mini ) );
  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\tclinamena: %i" ).sprintf( 1, clinamena ) );
  buffer.add( QR::String( "\n\tclinamenaLevel: %li" ).sprintf
			  ( 1, config -> clinamenaLevel ) );
  buffer.add( QR::String( "\n\tclinamenaPromille: %li" ).sprintf
			  ( 1, clinamenaPromille ) );
  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\twindowTitle: " ) );  
  buffer.add( config -> windowTitle );
  buffer.add( QR::String( "\n\tresourceClass: " ) );  
  buffer.add( config -> resourceClass );
  buffer.add( QR::String( "\n\tappName: " ) );  
  buffer.add( config -> appName );
  buffer.add( QR::String( "\n" ) );
  buffer.add( QR::String( "\n\tobjects: %lO" ).format( 1, OW( getObjects() ) ) );
  if( player != NULL )
	{
	  buffer.add( QR::String( "\n\tplayer: x: %i y: %i" ).sprintf
				  ( 1, player -> getX(), player -> getY() ) );
	}
  else
	{
	  buffer.add( QR::String( "\n\tno player?!" ) );
	}
  return buffer.asString();
}

QR::List *Game::getMovingObjects( MovingObject aType )
{
  preC_( ( 0 <= aType ) && ( aType < MAXMO ) );

  return movingObject[ aType ];
}

Index Game::getMilliseconds() const
{
#if defined( XONIX_USE_XQ ) || defined( XONIX_USE_CQ )
  struct itimerval newCharge;
  struct itimerval value;

  static time_t startupTime = time( NULL );

  test_( getitimer( ITIMER_REAL, &value ) == 0 );
  if( value.it_value.tv_sec < 1000000 )
	{
	  newCharge.it_interval.tv_sec = 100000000;
	  newCharge.it_interval.tv_usec = 0;
	  newCharge.it_value.tv_sec = 100000000;
	  newCharge.it_value.tv_usec = 0;
	  
	  test_( setitimer( ITIMER_REAL, &newCharge, NULL ) == 0 );
	  if( value.it_value.tv_sec != 0 )
		{
		  DLOG( "Microsecond timer was adjusted!" );
		}
	}
  return 1000 * ( unsigned long ) ( time( NULL ) - startupTime ) + 
	1000 - value.it_value.tv_usec / 1000;
#endif
#if defined( XONIX_USE_WQ )
  SYSTEMTIME instant;
  static long startupTime = -1; 
  
  if( startupTime == -1 )
	{ 
	  startupTime = 0;
	  startupTime = getMilliseconds();
	}

  GetLocalTime( &instant );
  return 1000 * ( 3600 * instant.wHour + 
				  60 * instant.wMinute + 
				  instant.wSecond ) + 
	instant.wMilliseconds - startupTime;
#endif
}

char *Game::getUserName() const
{
#if defined( XONIX_USE_XQ ) || defined( XONIX_USE_CQ )
  return getenv( "LOGNAME" );
#endif
#if defined( XONIX_USE_WQ )
  static char retBuffer[ 1024 ];
  unsigned long bufSize;

  bufSize = sizeof retBuffer;

  if( GetUserName( retBuffer, &bufSize ) )
	{
	  return retBuffer;
	}
  else
	{
	  return "user with incredibly long name";
	}
#endif
}

#if defined( XONIX_USE_XQ )
void Game::makeScreenShot() const
{
  static Index screenShotCount = 0;

  Debug::getLogger() -> logString( "\n%s", getBoard() -> toString() );
  TCAST( getPixmap(), QR::Xq::Pixmap ) -> 
	writeToXPMFile( QR::String( "./screenshot-%li.xpm" ).
					sprintf( 1, ++screenShotCount ) );
}
#endif
