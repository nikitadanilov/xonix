#if !defined( __OPTION_HPP__ )
#define __OPTION_HPP__

#include <quercus.hpp>

#include "Game.hpp"

class Option : public virtual QR::Object
{
public: Option( QR::String aName, 
				String aCmdLineName,
				String anEnvName,
				String aDescription );
public: virtual ~Option();

public: boolean isPresent() const;
public: boolean isMatched( QR::String aString ) const;
public: QR::String getEnvName() const;

public: virtual void pullArgs( QR::List *anArgs ) = 0;
public: virtual void pullEnv( QR::String aValue ) = 0;

public: static boolean parse( QR::List *anOptions, QR::List *anArgs );

public: virtual QR::String getUsageString() const;

public: virtual QR::String getString() const;

protected: boolean found;

protected: QR::String name;
protected: QR::String cmd;
protected: QR::String env;
protected: QR::String desc;
};

class SingleOption : public virtual Option
{
public: SingleOption( QR::String aName, 
					  QR::String aCmdLineName,
					  QR::String anEnvName,
					  QR::String aDescription,
					  boolean *aStorage );
public: virtual void pullArgs( QR::List *anArgs );
public: virtual void pullEnv( QR::String aValue );

protected: boolean *store;
};

class IntOption : public virtual Option
{
public: IntOption( QR::String aName, 
				   QR::String aCmdLineName,
				   QR::String anEnvName,
				   QR::String aDescription,
				   Index aLowBound,
				   Index aHighBound,
				   boolean aBarf,
				   Index *aStorage,
				   int aRadix = 10 );
public: virtual void pullArgs( QR::List *anArgs );
public: virtual void pullEnv( QR::String aValue );

public: Index getIntArg() const;

public: virtual QR::String getUsageString() const;

protected: void process();

protected: Index value;
protected: Index low;
protected: Index high;
protected: boolean barf;
protected: Index *store;
protected: int radix;
};

class StringOption : public virtual Option
{
public: StringOption( QR::String aName, 
					  QR::String aCmdLineName,
					  QR::String anEnvName,
					  QR::String aDescription,
					  QR::String *aStorage );
public: virtual void pullArgs( QR::List *anArgs );
public: virtual void pullEnv( QR::String aValue );

public: QR::String getStringArg() const;

public: virtual QR::String getUsageString() const;

protected: void process();

protected: QR::String value;
protected: QR::String *store;
};


/* __OPTION_HPP__ */
#endif

