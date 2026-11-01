#if !defined( __GAME_HPP__ )
#define __GAME_HPP__

#include "Geom.hpp"

#include <quercus.hpp>

#if defined( USE_CQ )
//#define XONIX_USE_CQ
#endif

#if defined( USE_WQ )
//#define XONIX_USE_WQ
#endif

#if defined( USE_XQ )
//#define XONIX_USE_XQ
#endif

/* Back-end is selected by the Makefile (-DXONIX_USE_XQ or -DXONIX_USE_CQ);
   X11 is the default. */
#if !defined( XONIX_USE_XQ ) && !defined( XONIX_USE_CQ ) && \
    !defined( XONIX_USE_WQ )
#define XONIX_USE_XQ
#endif

using QR::Debug;
using QR::ThreadFactory;
using QR::LinkManager;
using QR::OGuard;
using QR::PGuard;
using QR::String;
using QR::ObjectWrapper;
using QR::Top;

class Board;
class BoardMap;
class FrameFeeder;
class Player;
class SClipper;
class StatusLine;
class GameConfig;
class GameScheduler;
class Keyboard;

#if defined( XONIX_USE_XQ )
class ACursor;
#endif

class Game : public virtual QR::Object,
			 public virtual QR::ObservableAdapter
{
public: typedef enum { normal, 
					   path, 
					   hit, 
					   clipped, 
					   finished, 
					   over } State;
public: typedef enum { ibombs = 0, obombs, xbombs, sbombs, MAXMO } MovingObject;

public: Game( GameConfig *aConfig );
public: virtual ~Game();

public: Board *getBoard() const;
public: Keyboard *getKeyboard() const;
public: BoardMap *getMap() const;
public: Player *getPlayer() const;
public: QR::List *getBombs() const;
public: QR::List *getObjects() const;
public: QR::List *getThreads() const;
public: QR::List *getMovingObjects( MovingObject aType );

public: Index getMilliseconds() const;
public: char *getUserName() const;

public: QR::Aq::Drawable *getPixmap() const;
public: QR::Aq::Window *getWindow() const;

public: QR::Aq::Size mapCellWidth;
public: QR::Aq::Size mapCellHeight;
public: QR::Aq::Size mapCellAscent;

#if defined( XONIX_USE_XQ )

protected: void initX11Graphics();
protected: void initX11Glyphs();

public: QR::Xq::Display *getDisplay() const;
public: QR::Xq::GC *getGC() const;
public: QR::Xq::GC *getClearGC() const;
public: QR::Xq::Color *getBg() const;
public: QR::Xq::Color *getFg() const;
public: QR::Xq::Drawable *getXPixmap() const;
public: QR::Xq::Window *getXWindow() const;

public: void makeScreenShot() const;

protected: QR::Xq::Display *display;
protected: QR::Xq::Window *window;
protected: QR::Xq::Drawable *pix;
protected: QR::Xq::Color *bg;
protected: QR::Xq::Color *fg;
protected: QR::Xq::GC *gc;
protected: QR::Xq::GC *clear;

protected: ACursor *cursorThread;

/* XONIX_USE_XQ */
#endif

#if defined( XONIX_USE_WQ )

protected: void initWindowsGraphics();
protected: void initWindowsGlyphs();

protected: QR::Wq::Window   *window;
protected: QR::Wq::Drawable *pix;
protected: COLORREF bg;
protected: COLORREF fg;


/* XONIX_USE_WQ */
#endif

#if defined( XONIX_USE_CQ )

protected: void initCursesGraphics();
protected: void initCursesGlyphs();

public: QR::Cq::Window   *window;
public: QR::Cq::Window   *pix;

/* XONIX_USE_CQ */
#endif

public: int getLives() const;
public: void incLives( Index aShift );

public: Index getScore() const;
public: void incScore( Index aShift );

public: void drawCell( CoordX aCol, CoordY aRow ) const;

public: void setState( State aNewState );
public: State getStateFast() const;
public: State getState() const;

public: boolean isPaused() const;
public: void flipPause();

public: void update();

public: void stop();
public: void start();
public: void reset();

public: QR::String getStats() const;

public: QR::String save() const;
public: static Game *load( QR::String aSource, Index *anOffset );
protected: Board *board;
protected: BoardMap *map;

public: QR::Aq::Painter *winPainter;
public: QR::Aq::Painter *pixPainter;

public: QR::Aq::Painter *fgPainter;
public: QR::Aq::Painter *bgPainter;
public: QR::Aq::Painter *grayPainter;
public: QR::Aq::Painter *plPainter;


protected: QR::Aq::EventSource *events;
 
protected: FrameFeeder *feeder;
protected: Keyboard *keyboard;
protected: Player *player;
protected: SClipper *clipper;
protected: StatusLine *statusLine;
protected: State state;

protected: QR::CondVar *status;
protected: int lives;
protected: Index score;

protected: QR::List *bombs;

protected: QR::List **movingObject;

protected: QR::List *objects;
protected: QR::List *threads;

public: GameConfig *config;
public: int level;
public: Index version;

public: boolean clinamena;
public: int clinamenaPromille;

protected: GameScheduler **scheduler;
protected: QR::Thread **schedulerThread;
protected: boolean paused;
};

/* __GAME_HPP__ */
#endif

