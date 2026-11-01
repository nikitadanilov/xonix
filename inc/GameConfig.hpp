#if !defined( __GAMECONFIG_HPP__ )
#define __GAMECONFIG_HPP__

#include <quercus.hpp>

class GameConfig : public virtual QR::Object
{
public: GameConfig();
public: GameConfig( const GameConfig &aSource );
public: virtual ~GameConfig();

public: void init( QR::List *anArgs );
public: QR::String usage() const;

public: Index boardWidth;
public: Index boardHeight;

public: Index bombDelay;
public: Index playerDelay;
public: Index frameDelay;
public: Index statusDelay;
public: Index cursorDelay;
public: Index managerDelay;

public: Index iBombsNum;
public: Index oBombsNum;

public: Index iBombsNumStep;
public: Index oBombsNumStep;
public: Index xBombsNumStep;
public: Index sBombsNumStep;

public: Index xBombsLevel;
public: Index sBombsLevel;

public: Index playerAccel;
public: Index bombAccel;

public: Index lives;

public: Index fillThreshold;
public: Index fillThresholdStep;

public: boolean oneToMany;
public: boolean classic;
public: boolean mini;
public: boolean anim;
public: boolean acursor;

public: QR::String windowTitle;
public: QR::String resourceClass;
public: QR::String appName;
public: QR::String display;

public: QR::String normalCursorPattern;
public: Index normalCursorFrom;
public: Index normalCursorTo;
public: QR::String normalCursorColour;

public: QR::String hitCursorPattern;
public: Index hitCursorFrom;
public: Index hitCursorTo;
public: QR::String hitCursorColour;

public: QR::String clipCursorPattern;
public: Index clipCursorFrom;
public: Index clipCursorTo;
public: QR::String clipCursorColour;

public: QR::String bg;
public: QR::String fg;
public: QR::String pg;
public: QR::String pl;

public: QR::String logFile;

public: Index clinamenaLevel;
public: Index clinamenaPromille;

public: boolean helpMessage;
public: QR::List *options;
};

/* __GAMECONFIG_HPP__ */
#endif

