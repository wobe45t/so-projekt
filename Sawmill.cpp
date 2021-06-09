#include "Sawmill.h"
#include <unistd.h>
Sawmill::Sawmill(Resources *resources) : resources(resources)
{
}
Sawmill::Sawmill(Resources *resources, BoardType boardType) : resources(resources), boardType(boardType), td(&Sawmill::cycle, this)
{
}
void Sawmill::cycle() {
  int random_delay;
  int sleep_time;
  while(running){
    state = SawmillState::WAIT;
    progress = 0.0f;
    random_delay = rand() % 10000;
    resources->requestWood(5);
    while(progress<=100.0f && running == true) {
    state = SawmillState::WORK;
      progress+=1;
      if(speedState == SawmillSpeedState::NO_ORDER) {
        sleep_time = 200000;
      }
      else if(speedState == SawmillSpeedState::ORDER) {
        sleep_time = 100000;
      }
      else {
        sleep_time = 50000;
      }
      usleep(sleep_time + random_delay);
    }
    //  FIXME no need for first parameter in this function
    if(running) {
      resources->addBoard(1, boardType);
    }

  }
  done = true;
}
bool Sawmill::getDone() {
  return done;
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
bool Sawmill::getRunning() {
  return running;
}
void Sawmill::join() {
  this->td.join();
}
float Sawmill::getProgress() {
  return progress;
}
