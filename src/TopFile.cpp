#if !defined(_INLINE)
static char cvsid[] = "@(#)$Id$";
static char debugFileId[] = __FILE__;
#endif

#include "TopFile.hpp"

TopFile::TopFile( FILE *aFile )
{
  preC_( aFile != NULL );

  file = aFile;
  if( fseek( file, 0, SEEK_SET ) == -1 )
	{
	  throw new QR::Errno( "TopFile::TopFile(): fseek()", nil );
	}
}

TopFile::~TopFile()
{
  fclose( file );
}

TopFile::TopEntry::TopEntry( char *aUser, Index aScore, GameConfig *aConfig )
{
  user = aUser;
  score = aScore;
  FNNINIT( config, aConfig );
}

TopFile::TopEntry::~TopEntry()
{
  FDONE( config );
}

TopFile::TopEntry *TopFile::nextEntry()
{
  GameConfig *cfg;
  char *user;
  Index score;
  
  cfg = new GameConfig();
  fscanf( file, format,
		  user, &score, &( cfg -> boardWidth ), &( cfg -> boardHeight ), 
		  &( cfg -> bombDelay ), &( cfg -> playerDelay ), 
		  &( cfg -> iBombsNum ), &( cfg -> oBombsNum ),
		  &( cfg -> iBombsNumStep ), 
		  &( cfg -> oBombsNumStep ), &( cfg -> xBombsNumStep ), 
		  &( cfg -> sBombsNumStep ),
		  &( cfg -> xBombsLevel ), &( cfg -> sBombsLevel ), 
		  &( cfg -> playerAccel ), &( cfg -> bombAccel ), &( cfg -> lives ),
		  &( cfg -> fillThreshold ), &( cfg -> fillThresholdStep ),
		  &( cfg -> clinamenaPromille ), &( cfg -> clinamenaLevel ) );
  return new TopEntry( user, score, cfg );
}

void TopFile::addEntry( TopFile::TopEntry *anEntry )
{
  preC_( anEntry != NULL );

  OGuard _entry( anEntry, this );
  if( fseek( file, 0, SEEK_END ) == -1 )
	{
	  throw new QR::Errno( "TopFile::addEntry(): fseek()", nil );
	}
  fprintf( file, format,
		   anEntry -> user, anEntry -> score, 
		   anEntry -> config -> boardWidth, anEntry -> config -> boardHeight, 
		   anEntry -> config -> bombDelay, anEntry -> config -> playerDelay, 
		   anEntry -> config -> iBombsNum, anEntry -> config -> oBombsNum,
		   anEntry -> config -> iBombsNumStep, 
		   anEntry -> config -> oBombsNumStep, anEntry -> config -> xBombsNumStep, 
		   anEntry -> config -> sBombsNumStep,
		   anEntry -> config -> xBombsLevel, anEntry -> config -> sBombsLevel, 
		   anEntry -> config -> playerAccel, anEntry -> config -> bombAccel, 
		   anEntry -> config -> lives,
		   anEntry -> config -> fillThreshold, anEntry -> config -> fillThresholdStep,
		   anEntry -> config -> clinamenaPromille, 
		   anEntry -> config -> clinamenaLevel );
  if( fflush( file ) == -1 )
	{
	  throw new QR::Errno( "TopFile::addEntry(): fflush()", nil );
	}
}

TopFile *TopFile::create( QR::String aFileName )
{
  char *fileName;
  FILE *handle;

  fileName = aFileName.getChars();
  handle = fopen( fileName, "a+" );
  delete []fileName;
  if( handle == NULL )
	{
	  throw new QR::Errno( QR::String( "TopFile::create( " ) + aFileName + " )", nil );
	}
  return new TopFile( handle );
}

const char *TopFile::format = 
"user: %s score: %li width: %li height: %li bombDelay: %li playerDelay: %li "
"iBombsNum: %li oBombsNum: %li "
"iBombsNumStep: %li oBombsNumStep: %li xBombsNumStep: %li sBombsNumStep: %li "
"xBombsLevel: %li sBombsLevel: %li playerAccel: %li bombAccel: %li lives: %li "
"fillThreshold: %li fillThresholdStep: %li "
"clinamenaPromille: %li clinamenaLevel: %li\n";


