#if !defined( __CELL_HPP__ )
#define __CELL_HPP__

#include <quercus.hpp>

#include "Game.hpp"

class Cell : public virtual QR::Object
{
public: typedef enum 
{
  vacuum = 0,  
  solid,  
  border,  
  path,  
  head,  
  ibomb,  
  obomb, 
  xbomb, 
  sbomb, 
  INVALID
} State;

public: static char getChar( State aState );
};

/* __CELL_HPP__ */
#endif

