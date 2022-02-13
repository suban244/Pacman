#include "Structures.h"
#include "Texture.h"

struct Entity {
  Texture texture;
  EntityLocation location;
  SDL_Rect destRect;
  Direction direction;
};
