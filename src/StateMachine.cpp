#include "StateMachine.h"

StateMachine::StateMachine()
    : temp(nullptr), isNewState(false), isReplacing(false), popState(false) {}

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
  if (popState) {
    delete gameStateStack.top();
    gameStateStack.pop();
    popState = false;
    if (!gameStateStack.empty())
      gameStateStack.top()->init();
  }
}

void StateMachine::popTopState() { popState = true; }

GameState *StateMachine::getActiveState() { return gameStateStack.top(); }

bool StateMachine::isEmpty() { return gameStateStack.empty(); }

void StateMachine::quit() {
  while (!gameStateStack.empty()) {
    delete gameStateStack.top();
    gameStateStack.pop();
  }
}
