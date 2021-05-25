#include "Sawmill.h"
#include <unistd.h>
Sawmill::Sawmill(Resources *resources) : resources(resources)
{
}
Sawmill::Sawmill(Resources *resources, BoardType boardType) : resources(resources), boardType(boardType), td(&Sawmill::cycle, this)
{
}

void Sawmill::cycle() {
  int random_delay = 0;
  while(running){
    state = SawmillState::WAITING;
    progress = 0.0f;
    random_delay = rand() % 10000;
    if(workRequested == true) {
      resources->requestWood(5);
      state = SawmillState::WORKING;
      workRequested = false;
      // - notify the manager that the work started
      // - manager can choose another sawmill to schedule the work
      cv.notify_one(); 
      while(progress<=100.0f) {
        progress+=1;
        usleep(50000 + random_delay);
      }
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
    case SawmillState::WORKING:
    return "WORKING";
    case SawmillState::WAITING:
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

void Sawmill::setRunning(bool running){
  this->running = running;
}

float Sawmill::getProgress() {
  return progress;
}

bool Sawmill::requestBoard() { 
  workRequested = true;
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return !workRequested ? true : false;});
  return true;
}