#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "Option.hpp"

#include <stdlib.h>

Option::Option( QR::String aName, 
				String aCmdLineName,
				String anEnvName,
				String aDescription ) :
  name( aName ),
  cmd( aCmdLineName ),
  env( anEnvName ),
  desc( aDescription )
{
  found = false;
}

Option::~Option()
{}

boolean Option::isPresent() const
{
  return found;
}

boolean Option::isMatched( QR::String aString ) const
{
  return( cmd == aString );
}

String Option::getEnvName() const
{
  return env;
}

boolean Option::parse( QR::List *anOptions, QR::List *anArgs )
{
  preC_( anOptions != NULL );
  preC_( anArgs != NULL );

  boolean rest;

  for( anOptions -> toFirst() ; ! anOptions -> atEnd() ; 
	   anOptions -> next() )
	{
	  Option *opt;
	  char *envName;
	  char *envVar;
	  
	  opt = TCAST( anOptions -> getCurrentValue(), Option );
	  OGuard _opt( opt, opt );

	  envName = opt -> getEnvName().getChars();
	  envVar = getenv( envName );
	  // Debug::getLogger() -> log( "envName: %s envVar: %s", envName, envVar );
	  if( envVar != NULL )
		{
		  opt -> found = true;
		  opt -> pullEnv( envVar );
		}
	  delete []envName;
	}

  rest = false;
  for( anArgs -> toFirst() ; ! anArgs -> atEnd() ; anArgs -> next() )
	{
	  boolean optFound;
	  QR::String arg;

	  arg = TCAST( anArgs -> getCurrentValue(), QR::String ) -> getUNIChars();
	  if( arg.getUNIChars()[ 0 ] != ( unichar ) '-' )
		{
		  rest = true;
		  continue;
		}
	  optFound = false;
	  for( anOptions -> toFirst() ; ! anOptions -> atEnd() ; 
		   anOptions -> next() )
		{
		  Option *opt;

		  opt = TCAST( anOptions -> getCurrentValue(), Option );
		  OGuard _opt( opt, opt );
		  if( opt -> isMatched( arg.getUNIChars() + 1 ) )
			{
			  opt -> found = true;
			  opt -> pullArgs( anArgs );
			  optFound = true;
			  break;
			}
		}
	  if( ! optFound )
		{
		  rest = true;
		}
	}
  return rest;
}

String Option::getString() const
{
  return getUsageString() + "\t [ " + env + " ] " + desc + " (" + name + ")";
}

String Option::getUsageString() const
{
  return QR::String( "-" ) + cmd;
}

SingleOption::SingleOption( QR::String aName, 
							String aCmdLineName,
							String anEnvName,
							String aDescription,
							boolean *aStorage ) :
  Option( aName, aCmdLineName, anEnvName, aDescription )
{
  store = aStorage;
}

void SingleOption::pullArgs( QR::List *anArgs )
{
  if( store != NULL )
	{
	  *store = true;
	}
}

void SingleOption::pullEnv( QR::String )
{
  if( store != NULL )
	{
	  *store = true;
	}
}

IntOption::IntOption( QR::String aName, 
					  QR::String aCmdLineName,
					  QR::String anEnvName,
					  QR::String aDescription,
					  Index aLowBound,
					  Index aHighBound,
					  boolean aBarf,
					  Index *aStorage,
					  int aRadix ) :
  Option( aName, aCmdLineName, anEnvName, aDescription )
{
  low = aLowBound;
  high = aHighBound;
  barf = aBarf;
  store = aStorage;
  radix = aRadix;
}

void IntOption::pullArgs( QR::List *anArgs )
{
  preC_( anArgs != NULL );
  
  if( ( anArgs -> isLast() ) || ( anArgs -> atEnd() ) )
	{
	  if( barf )
		{
		  Debug::getLogger() -> logString
			( "Number is missing after option ``%s''. (Ignored.)", cmd );
		}
	}
  else
	{
	  anArgs -> next();
	  value = TCAST( anArgs -> getCurrentValue(), QR::String ) -> toNum( radix );
	  process();
	}
}

void IntOption::pullEnv( QR::String aValue )
{
  value = aValue.toNum( 10 );
  process();
}

Index IntOption::getIntArg() const
{
  return value;
}

String IntOption::getUsageString() const
{
  return QR::String( "-" ) + cmd + " <number>";
}

void IntOption::process()
{
  if( ( low <= value ) && ( value < high ) )
	{
	  if( store != NULL )
		{
		  *store = value;
		}
	}
  else
	{
	  if( barf )
		{
		  Debug::getLogger() -> logString
			( "Value is out of range for option ``%s''. (Ignored.)", cmd );
		}
	}
}



StringOption::StringOption( QR::String aName, 
							String aCmdLineName,
							String anEnvName,
							String aDescription,
							String *aStorage ) :
  Option( aName, aCmdLineName, anEnvName, aDescription )
{
  store = aStorage;
}

void StringOption::pullArgs( QR::List *anArgs )
{
  preC_( anArgs != NULL );

  if( ( anArgs -> isLast() ) || ( anArgs -> atEnd() ) )
	{
	  Debug::getLogger() -> logString
		( "Value is missing after option ``%s''. (Ignored.)", cmd );
	}
  else
	{
	  anArgs -> next();
	  value = *TCAST( anArgs -> getCurrentValue(), QR::String );
	  process();
	}
}

void StringOption::pullEnv( QR::String aValue )
{
  value = aValue;
  process();
}

String StringOption::getStringArg() const
{
  return value;
}

String StringOption::getUsageString() const
{
  return QR::String( "-" ) + cmd + " <string>";
}

void StringOption::process()
{
  if( store != NULL )
	{
	  *store = value;
	}
}

