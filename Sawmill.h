#pragma once
#include <unistd.h>
#include <random>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "Resources.h"
#include "SawmillState.h"
#include "SawmillSpeedState.h"
#include "BoardType.h"
class Sawmill {
public:
  Sawmill(Resources * resources);
  Sawmill(Resources * resources, BoardType boardType);
  void cycle();
  std::string getBoardTypeStr();
  std::string getWorkRequestStr();
  BoardType getBoardType();
  void setRunning(bool running);
  std::string getStateStr();
  float getProgress();
  int wait_counter = 0;
  bool requestBoard();
  SawmillState getState();
  void setPriority(bool priority);
  bool getPriority();
  bool getWorkRequested();
  void setWork(bool work);
  void setSpeedState(SawmillSpeedState speedState);
  std::string getSpeedStateStr();
private:
  BoardType boardType;
  std::atomic<SawmillState> state {SawmillState::WAIT};
  SawmillSpeedState speedState = {SawmillSpeedState::NO_ORDER};
  std::atomic<bool> workRequested {false};
  float progress = 0.0f;
  Resources * resources;
  bool running = true;
  std::atomic<bool> priority {false};
  std::condition_variable cv;
  std::mutex mtx;
  std::mutex mtx2; // test mutex
  std::thread td;
};
