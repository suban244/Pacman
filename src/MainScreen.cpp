#include "MainScreen.h"
#include "Game.h"
#include "GameState.h"
#include "Pacman.h"

bool buttonPress(SDL_Rect &rect, int x, int y) {
  return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
}

MainScreen::~MainScreen() { Mix_FreeMusic(music); }

MainScreen::MainScreen(StateMachine *s)
    : GameState(s), dialogueBox("", 32, {0, 0, 0, 255}) {
  pacmanIconRect.x = 400;
  pacmanIconRect.y = 30;
  pacmanIconRect.w = 500;
  pacmanIconRect.h = 100;
  pacmanIcon.loadFromFile("assets/pacmanIcon.png");

  textRect.x = 300;
  textRect.y = 200;
  textRect.w = 800;
  textRect.h = 300;
  text.loadFromFile("assets/new.jpg");

  soundTextRect.x = 370;
  soundTextRect.y = 470;
  soundTextRect.w = 500;
  soundTextRect.h = 200;
  soundText.loadFromFile("assets/sound.png");

  music = Mix_LoadMUS("assets/pacman_beginning.wav");
  dialogueBox.addOption("Play!", &MainScreen::loadPacman);
  dialogueBox.addOption(Game::playMusic ? "Music: On" : "Music: Off",
                        &MainScreen::toggleMusic);
  dialogueBox.addOption(Game::playSoundEffect ? "Sound Effects: On"
                                              : "Sound Effects: Off",
                        &MainScreen::toggleSoundEffects);
  dialogueBox.addOption("Quit", &MainScreen::quit);
}

void MainScreen::loadPacman() {
  stateMachineRef->AddState(new Pacman(stateMachineRef));
}

void MainScreen::init() {
  // startButtonRect.h = startButtonRect.w = startButtonRect.x =
  //   startButtonRect.y = 50;
  Mix_VolumeMusic(4);
  Mix_PlayMusic(music, -1);
  if (!Game::playMusic)
    Mix_PauseMusic();
}
void MainScreen::render() {
  SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255);
  SDL_RenderFillRect(Game::renderer, &startButtonRect);
  pacmanIcon.render(&pacmanIconRect);
  // text.render(&textRect);
  // soundText.render(&soundTextRect);
  dialogueBox.render();
}

void MainScreen::handleInput(SDL_Event &e) {

  /*
  int x = e.button.x;
  int y = e.button.y;
  */
  dialogueBox.handleInput(e, this);
}
void MainScreen::update() {}
void MainScreen::toggleMusic() {
  Game::playMusic = !Game::playMusic;
  if (Game::playMusic) {
    dialogueBox.updateOptionMessage("Music: Off", "Music: On");
    Mix_ResumeMusic();
  } else {
    dialogueBox.updateOptionMessage("Music: On", "Music: Off");
    Mix_PauseMusic();
  }
}
void MainScreen::toggleSoundEffects() {
  Game::playSoundEffect = !Game::playSoundEffect;
  if (Game::playSoundEffect) {
    dialogueBox.updateOptionMessage("Sound Effects: Off", "Sound Effects: On");
  } else {
    dialogueBox.updateOptionMessage("Sound Effects: On", "Sound Effects: Off");
  }
}

void MainScreen::quit() { stateMachineRef->quit(); }
