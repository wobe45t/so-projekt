#pragma once
#include <unistd.h>
#include <random>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "Resources.h"
#include "SawmillState.h"
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
private:
  BoardType boardType;
  std::atomic<SawmillState> state {SawmillState::WAITING};
  std::atomic<bool> workRequested {false};
  float progress = 0.0f;
  Resources * resources;
  bool running = true;
  std::atomic<bool> priority {false};
  std::condition_variable cv;
  std::mutex mtx;
  std::thread td;
};
