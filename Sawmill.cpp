#include "Sawmill.h"
#include <unistd.h>
Sawmill::Sawmill(Resources *resources) : resources(resources)
{
}
Sawmill::Sawmill(Resources *resources, BoardType boardType) : resources(resources), boardType(boardType)
{
}

int Sawmill::startWork() {
  state = SawmillState::WORKING;
  return resources->requestBoard(1, boardType);
}

std::string Sawmill::getBoardType() {
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

std::string Sawmill::getState() {
  switch(state) {
    case SawmillState::WORKING:
    return "WORKING";
    case SawmillState::WAITING:
    return "WAITING";
    default:
    return "UNKNOWN";
  }
}

void Sawmill::setRunning(bool running){
  this->running = running;
}