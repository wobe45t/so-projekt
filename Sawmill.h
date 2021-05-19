#pragma once
#include <unistd.h>
#include <random>
#include <thread>
#include <atomic>
#include <mutex>
#include "Resources.h"
#include "SawmillState.h"
#include "BoardType.h"
class Sawmill {
public:
  Sawmill(Resources * resources);
  Sawmill(Resources * resources, BoardType boardType);
  void cycle();
  std::string getBoardType();
  void setRunning(bool running);
  int startWork();
  BoardType boardType;
  SawmillState state;
  std::string getState();
private:
  Resources * resources;
  bool running = true;
  std::mutex mtx;
  std::thread td;
};