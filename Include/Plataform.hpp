#ifndef Screen_Key
#define Screen_Key

#include <SDL2/SDL.h>

class SDL_Window; 
class SDL_Renderer;
class SDL_Texture; 


class Plataform{
 public:

  Plataform(char const *title , int W_size , int H_size , int textureWitdh , int textureHeight);
  ~Plataform();
  void Update(void const *buffer , int pitch);
  bool ProcessInput(uint8_t *keys);
 
 private: 
 	 SDL_Window   *window{};
 	 SDL_Renderer *renderer{};
 	 SDL_Texture  *texture{};



};



#endif 