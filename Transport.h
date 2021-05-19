#pragma once
#include <thread>
#include "TransportState.h"
#include "SawmillManager.h"
class Transport {
public:
  Transport(SawmillManager * sawmillManager);
  void cycle();
  std::string getState();
private:
  TransportState transportState {TransportState::WAITING};
  SawmillManager * sawmillManager;
  int longBoards = 0;
  int shortBoards = 0;
  int mediumBoards = 0;
  std::thread td;
  bool running = true;
};