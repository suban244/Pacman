#pragma once
#include "DialogueBox.h"
#include "Enemy.h"
#include "GameState.h"
#include "Structures.h"
#include <SDL2/SDL_mixer.h>
#define STARTING_STATE_TIME 120
#define JUST_DIED_PAUSE_TIME 30
#define VOLUME_PLAYING 40
#define VOLUME_PAUSED 10

class Pacman : public GameState {
private:
  State state;

  Texture pacmanSprite;
  EntityLocation pacmanLocation;
  int lives;
  SDL_Rect pacmanSrcRect, pacmanDestRect;
  Direction pacmanDirection, pacmanNextDirection;
  int animationCount;
  Texture enemyRunningSprite, enemyResetingSprite;

  std::vector<Enemy *> enemies;
  Grid gameGrid;
  static int pacmanSpriteSize;

  int startingStateTime;
  Texture numTextures[3], WonTexture;
  int pauseTime; // This for JUST_DIED_PAUSE_TIME

  State stateBeforePause; // Stores state before the pause
  Mix_Chunk *chunkChomp, *chunkDeath, *chunkPower, *chunkEatEnemy,
      *chunkGameover;
  Mix_Music *music;

  DialogueBox pause, gameover, win;

  int score, scoringWindow, scoreFactor, oldScore;
  Texture scoreTexture, scoreAmountTexture;
  SDL_Rect scoreRect, scoreAmountRect;

public:
  Pacman(StateMachine *s);
  ~Pacman();
  void init() override;
  void handleInput(SDL_Event &event) override;
  void update() override;
  void render() override;

  void resetBoard();
  void pacmanGoToDefaultLocation();

  void togglePause();
  void quit();
  void returnToMainScreen();
  void toggleMusic();
  void toggleSoundEffects();
};
