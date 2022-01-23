#include "StateMachine.h"

StateMachine::StateMachine()
    : temp(nullptr), isNewState(false), isReplacing(false) {}

void StateMachine::AddState(GameState *newState, bool toReplace) {
  temp = newState;
  isReplacing = toReplace;
  isNewState = true;
}

void StateMachine::processStateChanges() {
  if (isNewState) {
    if (isReplacing)
      gameStateStack.pop();
    temp->init();
    gameStateStack.push(temp);
    temp = nullptr;
    isNewState = false;
    isReplacing = false;
  }
}

GameState *StateMachine::getActiveState() { return gameStateStack.top(); }
