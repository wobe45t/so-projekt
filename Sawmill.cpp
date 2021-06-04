#include "Sawmill.h"
#include <unistd.h>
Sawmill::Sawmill(Resources *resources) : resources(resources)
{
}
Sawmill::Sawmill(Resources *resources, BoardType boardType) : resources(resources), boardType(boardType), td(&Sawmill::cycle, this)
{
}

void Sawmill::cycle() {
  workRequested = true;
  int random_delay = 0;
  while(running){
    state = SawmillState::WAIT;
    progress = 0.0f;
    random_delay = rand() % 10000;
    if(workRequested) {
      // add work speed in separate state
      // THIS IS CORRECT
      state = SawmillState::WORK;
      resources->requestWood(5);
      while(progress<=100.0f) {
        progress+=1;
        usleep(50000 + random_delay);
      }
      // FIXME a moze po zmianie tych mutexow tutaj sie sypie w addBoard
      resources->addBoard(1, boardType);
    }
  }
}

std::string Sawmill::getBoardTypeStr() {
  switch(boardType) {
    case BoardType::SHORT:
    return "SHORT";
    case BoardType::NORMAL:
    return "NORMAL";
    case BoardType::LONG:
    return "LONG";
    default:
    return "UNKNOWN";
  }
}

BoardType Sawmill::getBoardType() {
  return boardType;
}


std::string Sawmill::getStateStr() {
  switch(state) {
    case SawmillState::WORK:
    return "WORKING";
    case SawmillState::WAIT:
    return "WAITING";
    default:
    return "UNKNOWN";
  }
}
std::string Sawmill::getWorkRequestStr() {
  if(workRequested == true) {
    return "REQUESTED";
  } else {
    return "";
  }
}
SawmillState Sawmill::getState()  {
  return state;
}
void Sawmill::setPriority(bool priority) {
  this->priority = priority;
}

bool Sawmill::getPriority() {
  return priority;
}

void Sawmill::setRunning(bool running){
  this->running = running;
}

float Sawmill::getProgress() {
  return progress;
}
void Sawmill::setWork(bool work) {
  std::lock_guard<std::mutex> lock(mtx);
  // workRequested = work;
}
// FIXME not used
bool Sawmill::requestBoard() { 
  workRequested = true;
  // std::unique_lock<std::mutex> ul(mtx);
  // cv.wait(ul, [&] {return !workRequested ? true : false;});
  return true;
}

bool Sawmill::getWorkRequested() {
  return workRequested;
}
