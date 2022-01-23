#include "Pacman.h"
#include "Game.h"

/*
 * Map of 19 x 19
 * So we need a 19 x 19 ko array of nodes
 *
 */

Pacman::Pacman(StateMachine *s) : GameState(s), pacmanLocation(1, 1) {}
int Pacman::BLOCK_SIZE = 30;
int Pacman::gridStartPosX = WINDOW_WIDTH / 10 * 3;
int Pacman::gridStartPosY = WINDOW_HEIGHT / 10;
int Pacman::pacmanSpriteSize = 80; // Becasue each sprite is 80 x 80

void Pacman::init() {
  pacmanSprite.loadFromFile("assets/pacman.png");
  pacmanSrcRect.h = pacmanSrcRect.w = pacmanSpriteSize;
  pacmanSrcRect.y = pacmanSrcRect.x = animationCount = 0;
  pacmanDestRect.x = 0;
  pacmanDestRect.y = gridStartPosY + BLOCK_SIZE + 5;
  pacmanDestRect.h = pacmanDestRect.w = 20;
  pacmanSrcRect.x = 160;
}
void Pacman::render() {
  SDL_Rect boxRect;
  boxRect.x = gridStartPosX;
  boxRect.y = gridStartPosY;
  boxRect.w = boxRect.h = BLOCK_SIZE;
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {

      switch (gameGrid.nodes[i][j].state) {
      case 0:
        SDL_SetRenderDrawColor(Game::renderer, 0, 200, 50, 255);
        break;
      case 1:
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
        break;
      default:
        break;
      }
      SDL_RenderFillRect(Game::renderer, &boxRect);
      boxRect.x += BLOCK_SIZE;
    }
    boxRect.x = gridStartPosX;
    boxRect.y += BLOCK_SIZE;
  }
  pacmanLocation.calculateCoordinateToRender(pacmanDestRect, gridStartPosX,
                                             gridStartPosY, BLOCK_SIZE);
  pacmanSprite.renderEX(&pacmanDestRect, &pacmanSrcRect, 270);
}
void Pacman::update() {
  animationCount++;
  pacmanSrcRect.x = 80 * ((animationCount / 10) % 3);
}
void Pacman::handleInput(SDL_Event &e) {}
