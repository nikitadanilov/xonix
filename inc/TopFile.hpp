#if !defined( __TOPFILE_HPP__ )
#define __TOPFILE_HPP__

#include "GameConfig.hpp"
#include "Game.hpp"
#include <quercus.hpp>

#include <stdio.h>

class TopFile : public QR::Object
{
public: TopFile( FILE *aFile );
public: virtual ~TopFile();

public: class TopEntry : public virtual QR::Object
{
public: TopEntry( char *aUser, Index aScore, GameConfig *aConfig );
public: virtual ~TopEntry();

public: char *user;
public: Index score;
public: GameConfig *config;
} ;

public: virtual TopEntry *nextEntry();
public: virtual void addEntry( TopEntry *anEntry );

public: static TopFile *create( QR::String aFileName );

protected: static const char *format;
protected: FILE *file;
};

/* __TOPFILE_HPP__ */
#endif
