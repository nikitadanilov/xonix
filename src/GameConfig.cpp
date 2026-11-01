#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Game.hpp"
#include "GameConfig.hpp"
#include "Option.hpp"

GameConfig::GameConfig()
{

  boardWidth = 60;
  boardHeight = 25;

  bombDelay = 100000000;
  playerDelay = 100000000;
  frameDelay = 100000000;
  statusDelay = 100000000;
  cursorDelay = 100000000;
  managerDelay = 100000000;

  iBombsNum = 2;
  oBombsNum = 2;

  iBombsNumStep = 1;
  oBombsNumStep = 1;
  xBombsNumStep = 1;
  sBombsNumStep = 1;

  xBombsLevel = 6;
  sBombsLevel = 2;

  playerAccel = 17;
  bombAccel = 40;

  lives = 26;

  fillThreshold = 50;
  fillThresholdStep = 5;
  oneToMany = true;

  helpMessage = false;
  classic = false;
  mini = false;
  anim = false;
  acursor = false;

  appName = "XONIX";
  windowTitle = appName;
  resourceClass = appName;

  display = "";

  normalCursorPattern = "./img/cursor-%2.2i.xbm";
  normalCursorFrom = 0;
  normalCursorTo = 32;
  normalCursorColour = "green";

  hitCursorPattern = "./img/bomb.xbm";
  hitCursorFrom = 0;
  hitCursorTo = 1;
  hitCursorColour = "red";

  clipCursorPattern = "./img/cut.xbm";
  clipCursorFrom = 0;
  clipCursorTo = 1;
  clipCursorColour = "yellow";

  bg = "black";
  fg = "white";
  pg = "gray";
  pl = "red";

  logFile = "./xonix.log";

  clinamenaLevel = 4;
  clinamenaPromille = 2;
}

GameConfig::GameConfig( const GameConfig &aSource )
{
  boardWidth = aSource.boardWidth;
  boardHeight = aSource.boardHeight;

  bombDelay = aSource.bombDelay;
  playerDelay = aSource.playerDelay;
  frameDelay = aSource.frameDelay;
  statusDelay = aSource.statusDelay;
  cursorDelay = aSource.cursorDelay;
  managerDelay = aSource.managerDelay;

  iBombsNum = aSource.iBombsNum;
  oBombsNum = aSource.oBombsNum;

  iBombsNumStep = aSource.iBombsNumStep;
  oBombsNumStep = aSource.oBombsNumStep;
  xBombsNumStep = aSource.xBombsNumStep;
  sBombsNumStep = aSource.sBombsNumStep;

  xBombsLevel = aSource.xBombsLevel;
  sBombsLevel = aSource.sBombsLevel;

  playerAccel = aSource.playerAccel;
  bombAccel = aSource.bombAccel;

  lives = aSource.lives;

  fillThreshold = aSource.fillThreshold;
  fillThresholdStep = aSource.fillThresholdStep;
  oneToMany = aSource.oneToMany;

  helpMessage = aSource.helpMessage;
  classic = aSource.classic;
  mini = aSource.mini;
  anim = aSource.anim;
  acursor = aSource.acursor;

  appName = aSource.appName;
  windowTitle = aSource.windowTitle;
  resourceClass = aSource.resourceClass;

  display = aSource.display;

  normalCursorPattern = aSource.normalCursorPattern;
  normalCursorFrom = aSource.normalCursorFrom;
  normalCursorTo = aSource.normalCursorTo;
  normalCursorColour = aSource.normalCursorColour;

  hitCursorPattern = aSource.hitCursorPattern;
  hitCursorFrom = aSource.hitCursorFrom;
  hitCursorTo = aSource.hitCursorTo;
  hitCursorColour = aSource.hitCursorColour;

  clipCursorPattern = aSource.clipCursorPattern;
  clipCursorFrom = aSource.clipCursorFrom;
  clipCursorTo = aSource.clipCursorTo;
  clipCursorColour = aSource.clipCursorColour;

  bg = aSource.bg;
  fg = aSource.fg;
  pg = aSource.pg;
  pl = aSource.pl;

  logFile = aSource.logFile;

  clinamenaLevel = aSource.clinamenaLevel;
  clinamenaPromille = aSource.clinamenaPromille;
}

GameConfig::~GameConfig()
{
  FCDONE( options );
}

void GameConfig::init( QR::List *anArgs )
{
  preC_( anArgs != NULL );
  
  boolean oneToOne;

  FINIT( options, new QR::List() );

  options -> append( new IntOption( "width", "w", "XONIX_WIDTH", 
									"Width of the playing board", 
									11, 10000000, true, &boardWidth ) );

  options -> append( new IntOption( "height", "h", "XONIX_HEIGHT", 
									"Height of the playing board",
									11, 10000000, true, &boardHeight ) );
  options -> append( new IntOption( "ibombs", "i", "XONIX_IBOMBS", 
									"Number of internal bombs", 
									0, 10000, true, &iBombsNum ) );
  options -> append( new IntOption( "obombs", "o", "XONIX_OBOMBS", 
									"Number of outer bombs", 
									0, 10000, true, &oBombsNum ) );
  options -> append( new IntOption( "lives", "l", "XONIX_LIVES", 
									"Initial number of lives", 
									1, 10000, true, &lives ) );
  options -> append( new IntOption( "fill", "f", "XONIX_FILL", 
									"Fill percentage", 
									1, 100, true, &fillThreshold ) );
  options -> append( new IntOption( "fillStep", "fs", "XONIX_FILL_STEP", 
									"Fill percentage step", 
									0, 100000000, true, &fillThreshold ) );
  options -> append( new IntOption
					 ( "ibombsStep", "is", "XONIX_IBOMBS_STEP", 
					   "Increment of the number of internal bombs", 
					   0, 10000, true, &iBombsNumStep ) );
  options -> append( new IntOption( "obombsStep", "os", "XONIX_OBOMBS_STEP", 
									"Increment of the number of outer bombs", 
									0, 10000, true, &oBombsNumStep ) );
  options -> append( new IntOption( "xbombsStep", "xs", "XONIX_XBOMBS_STEP", 
									"Increment of the number of X-bombs", 
									0, 10000, true, &xBombsNumStep ) );
  options -> append( new IntOption( "sbombsStep", "ss", "XONIX_SBOMBS_STEP", 
									"Increment of the number of S-bombs", 
									0, 10000, true, &sBombsNumStep ) );
  options -> append( new IntOption( "xbombsLevel", "xl", "XONIX_XBOMBS_LEVEL", 
									"Level where X-bombs appear", 
									0, 10000, true, &xBombsLevel ) );
  options -> append( new IntOption( "sbombsLevel", "sl", "XONIX_SBOMBS_LEVEL", 
									"Level where S-bombs appear", 
									0, 10000, true, &sBombsLevel ) );
  options -> append( new IntOption( "bombsDelay", "bd", "XONIX_BOMBS_DELAY", 
									"Initial delay of bombs cycle (nanosecs.)", 
									0, 1000000000, true, &bombDelay ) );
  options -> append( new IntOption
					 ( "playerDelay", "pd", "XONIX_PLAYER_DELAY", 
					   "Initial delay of player cycle (nanosecs.)", 
					   0, 1000000000, true, &playerDelay ) );
  options -> append( new IntOption
					 ( "frameDelay", "fd", "XONIX_FRAME_DELAY", 
					   "Delay of the frame refresh cycle (nanosecs.)", 
					   0, 1000000000, true, &frameDelay ) );
  options -> append( new IntOption
					 ( "statusDelay", "sd", "XONIX_STATUS_DELAY", 
					   "Delay of the status line refresh cycle (nanosecs.)", 
					   0, 1000000000, true, &statusDelay ) );
  options -> append( new IntOption
					 ( "cursorDelay", "cd", "XONIX_CURSOR_DELAY", 
					   "Delay of the cursor refresh cycle (nanosecs.)", 
					   0, 1000000000, true, &cursorDelay ) );
  options -> append( new IntOption
					 ( "managerDelay", "md", "XONIX_MANAGER_DELAY", 
					   "Delay of the watch cycle (nanosecs.)", 
					   0, 1000000000, true, &managerDelay ) );
  options -> append( new IntOption
					 ( "bombAccel", "ba", "XONIX_BOMB_ACCEL", 
					   "Percentage of bomb acceleration", 
					   0, 101, true, &bombAccel ) );
  options -> append( new IntOption
					 ( "playerAccel", "pa", "XONIX_PLAYER_ACCEL", 
					   "Percentage of player acceleration", 
					   0, 101, true, &playerAccel ) );
  options -> append( new SingleOption
					 ( "oneToOne", "-one-to-one", "XONIX_ONE_TO_ONE", 
					   "Use standard thread scheduler", &oneToOne ) );
  options -> append( new SingleOption
					 ( "oneToMany", "-one-to-many", "XONIX_ONE_TO_MANY", 
					   "Use internal thread scheduler", &oneToMany ) );
  options -> append( new SingleOption
					 ( "classic", "-classic", "XONIX_CLASSIC", 
					   "Use good ol' look and feel", &classic ) );
  options -> append( new SingleOption
					 ( "mini", "-mini", "XONIX_MINI", 
					   "Use mini-icons", &mini ) );
  options -> append( new SingleOption
					 ( "anim", "-anim", "XONIX_ANIM", 
					   "Use animation", &anim ) );
  options -> append( new SingleOption
					 ( "acursor", "-acursor", "XONIX_ACURSOR", 
					   "Use animated cursors", &acursor ) );
  options -> append( new StringOption
					 ( "title", "-title", "XONIX_TITLE", 
					   "Top-level window title", &windowTitle ) );
  options -> append( new StringOption
					 ( "name", "-name", "XONIX_NAME", 
					   "Application name", &appName ) );
  options -> append( new StringOption
					 ( "class", "-class", "XONIX_CLASS", 
					   "Application resource class", &resourceClass ) );
  options -> append( new StringOption
					 ( "display", "display", "XONIX_DISPLAY", 
					   "Display to draw on", &display ) );
  options -> append( new StringOption
					 ( "log", "-log", "XONIX_LOG", 
					   "Trace file used when curses owns the terminal", 
					   &logFile ) );

  options -> append( new StringOption
					 ( "normalCursorPattern", "-normal-cursor-pattern", 
					   "XONIX_NORMAL_CURSOR_PATTERN", 
					   "Pattern of filenames of normal cursor bitmaps", 
					   &normalCursorPattern ) );
  options -> append( new IntOption
					 ( "normalCursorFrom", "-normal-cursor-from", 
					   "XONIX_NORMAL_CURSOR_FROM", 
					   "Start index of filenames of normal cursor bitmaps", 
					   0, 1000000000, true, 
					   &normalCursorFrom ) );
  options -> append( new IntOption
					 ( "normalCursorTo", "-normal-cursor-to", 
					   "XONIX_NORMAL_CURSOR_TO", 
					   "End index of filenames of normal cursor bitmaps", 
					   0, 1000000000, true, 
					   &normalCursorTo ) );
  options -> append( new StringOption
					 ( "normalCursorColour", "-normal-cursor-colour", 
					   "XONIX_NORMAL_CURSOR_COLOUR", 
					   "Colour of normal cursors", 
					   &normalCursorColour ) );

  options -> append( new StringOption
					 ( "hitCursorPattern", "-hit-cursor-pattern", 
					   "XONIX_HIT_CURSOR_PATTERN", 
					   "Pattern of filenames of hit cursor bitmaps", 
					   &hitCursorPattern ) );
  options -> append( new IntOption
					 ( "hitCursorFrom", "-hit-cursor-from", 
					   "XONIX_HIT_CURSOR_FROM", 
					   "Start index of filenames of hit cursor bitmaps", 
					   0, 1000000000, true, 
					   &hitCursorFrom ) );
  options -> append( new IntOption
					 ( "hitCursorTo", "-hit-cursor-to", 
					   "XONIX_HIT_CURSOR_TO", 
					   "End index of filenames of hit cursor bitmaps", 
					   0, 1000000000, true, 
					   &hitCursorTo ) );
  options -> append( new StringOption
					 ( "hitCursorColour", "-hit-cursor-colour", 
					   "XONIX_HIT_CURSOR_COLOUR", 
					   "Colour of hit cursors", 
					   &hitCursorColour ) );

  options -> append( new StringOption
					 ( "clipCursorPattern", "-clip-cursor-pattern", 
					   "XONIX_CLIP_CURSOR_PATTERN", 
					   "Pattern of filenames of clip cursor bitmaps", 
					   &clipCursorPattern ) );
  options -> append( new IntOption
					 ( "clipCursorFrom", "-clip-cursor-from", 
					   "XONIX_CLIP_CURSOR_FROM", 
					   "Start index of filenames of clip cursor bitmaps", 
					   0, 1000000000, true, 
					   &clipCursorFrom ) );
  options -> append( new IntOption
					 ( "clipCursorTo", "-clip-cursor-to", 
					   "XONIX_CLIP_CURSOR_TO", 
					   "End index of filenames of clip cursor bitmaps", 
					   0, 1000000000, true, 
					   &clipCursorTo ) );
  options -> append( new StringOption
					 ( "clipCursorColour", "-clip-cursor-colour", 
					   "XONIX_CLIP_CURSOR_COLOUR", 
					   "Colour of clip cursors", 
					   &clipCursorColour ) );


  options -> append( new StringOption
					 ( "bg", "bg", "XONIX_BG", "Background colour",  &bg ) );
  options -> append( new StringOption
					 ( "fg", "fg", "XONIX_FG", "Foreground colour",  &fg ) );
  options -> append( new StringOption
					 ( "pg", "pg", "XONIX_PG", "Pathground colour",  &pg ) );
  options -> append( new StringOption
					 ( "pl", "pl", 
					   "XONIX_PL", "Player colour (in classic mode)", &pl ) );

  options -> append( new IntOption
					 ( "clinamenaLevel", "-clinamena", 
					   "XONIX_CLINAMENA_LEVEL", 
					   "First level where clinamena began", 
					   0, 1000, true, 
					   &clinamenaLevel ) );
  options -> append( new IntOption
					 ( "clinamenaPromille", "-clinamena-promille", 
					   "XONIX_CLINAMENA_PROMILLE", 
					   "Intencity of clinamena", 
					   0, 1001, true, 
					   &clinamenaPromille ) );

  options -> append( new SingleOption( "help", "-help", "XONIX_HELP", 
									   "Usage message", &helpMessage ) );

  oneToOne = false;

  Option::parse( options, anArgs );

  if( oneToOne )
	{
	  oneToMany = false;
	}
  if( helpMessage )
	{
	  Debug::getLogger() -> logString( "%s", usage() );
	}
}

String GameConfig::usage() const
{
  preC_( options != NULL );

  QR::StringBuffer buffer( QR::StringBuffer::defaultLength, 
					   QR::StringBuffer::defaultDelta );
  QR::StringBuffer desc( QR::StringBuffer::defaultLength, 
					 QR::StringBuffer::defaultDelta );

  buffer.add( QR::String( "\nUsage: [ " ).getUNIChars() );
  for( options -> toFirst() ; ! options -> atEnd() ; options -> next() )
	{
	  Option *opt;

	  opt = TCAST( options -> getCurrentValue(), Option );
	  OGuard _opt( opt, this );

	  buffer.add( opt -> getUsageString().getUNIChars() );
	  buffer.add( QR::String( " | " ).getUNIChars() );

	  desc.add( opt -> getString().getUNIChars() );
	  desc.add( QR::String( "\n" ).getUNIChars() );
	}
  buffer.add( QR::String( "... ]\n\n" ).getUNIChars() );
  buffer.add( desc.asString().getUNIChars() );
  buffer.add( QR::String( "\n\nKeyboard controls:" ) );
  buffer.add( QR::String( "\nArrows and keypad digits : motion" ) );
  buffer.add( QR::String( "\nSpace and keypad digit 5 : stop" ) );
  buffer.add( QR::String( "\nP, Esc and SUN Stop key  : pause" ) );
  buffer.add( QR::String( "\nD and SUN Copy key       : dump screenshot" ) );
  buffer.add( QR::String( "\nS                        : dump statistics" ) );
  buffer.add( QR::String( "\nQ                        : quit" ) );
  buffer.add( QR::String( "\nF1,H                     : help" ) );
  return buffer.asString();
}

