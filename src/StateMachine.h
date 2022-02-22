#include "GameState.h"
#include <stack>
/*
 * Every Cycle Some event happens
 * It queus up a update
 * And on update does update
 *
 */
class StateMachine {
private:
  std::stack<GameState *> gameStateStack;
  GameState *temp;
  bool isNewState;
  bool isReplacing;
  bool popState;

public:
  StateMachine();
  void AddState(GameState *newState, bool isReplacing = false);
  void popTopState();
  void quit();
  bool isEmpty();
  void processStateChanges();
  GameState *getActiveState();
};
