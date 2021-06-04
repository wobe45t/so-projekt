#include "Sawmill.h"
#include <unistd.h>
Sawmill::Sawmill(Resources *resources) : resources(resources)
{
}
Sawmill::Sawmill(Resources *resources, BoardType boardType) : resources(resources), boardType(boardType), td(&Sawmill::cycle, this)
{
}
// NOTE it works for now (04 14.06)
void Sawmill::cycle() {
  workRequested = true;
  int random_delay;
  int sleep_time; // depending on SawmillSpeedState
  while(running){
    state = SawmillState::WAIT;
    progress = 0.0f;
    random_delay = rand() % 10000;
    resources->requestWood(5);
    while(progress<=100.0f) {
    state = SawmillState::WORK;
      progress+=1;
      mtx.lock();
      if(speedState == SawmillSpeedState::NO_ORDER) {
        sleep_time = 200000;
      }
      else if(speedState == SawmillSpeedState::ORDER) {
        sleep_time = 100000;
      }
      else {
        sleep_time = 60000;
      }
      mtx.unlock();
      usleep(sleep_time + random_delay);
    }
    resources->addBoard(1, boardType);
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
std::string Sawmill::getSpeedStateStr() {
  std::lock_guard<std::mutex> lock(mtx);
  switch(speedState) {
    case SawmillSpeedState::NO_ORDER:
      return "NO_ORDER";
    case SawmillSpeedState::ORDER:
      return "NORMAL";
    case SawmillSpeedState::PRIORITY:
      return "PRIORITY";
    default:
      return "UNKNOWN";
  }
}
BoardType Sawmill::getBoardType() {
  return boardType;
}
void Sawmill::setSpeedState(SawmillSpeedState speedState) {
  std::lock_guard<std::mutex> lock(mtx);
  this->speedState = speedState;
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
