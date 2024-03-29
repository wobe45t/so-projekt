#pragma once
#include <thread>
#include <atomic>
#include "TransportState.h"
#include "BoardType.h"
#include "Resources.h"
#include "SawmillManager.h"
#include <random>
#include <vector>
#include <condition_variable>
#include <mutex>
class Transport {
public:
  Transport(SawmillManager * sawmillManager, Resources * resources);
  void cycle();
  std::string getStateStr();
  TransportState getState();
  int getTransportCounter();
  int getShortBoards();
  int getNormalBoards();
  int getLongBoards();
  int getOrderedShortBoards();
  int getOrderedNormalBoards();
  int getOrderedLongBoards();
  void generateOrder();
  float getProgress();
  BoardType getTopBoardPriority();
  void setRunning(bool running);
  bool getRunning();
  bool getDone();
  void join();
  void finish();

private:
  TransportState transportState {TransportState::FROM_SHOP};
  Resources * resources;
  SawmillManager * sawmillManager;
  float progress = 0.0f;
  std::atomic<int> transportCounter {0};
  // NOTE shouldnt running be atomic ? it is accessed from the main
  bool running = true;
  bool done = false;
  std::atomic<int> orderedShortBoards {0};
  std::atomic<int> orderedNormalBoards {0};
  std::atomic<int> orderedLongBoards {0};
  std::atomic<int> shortBoards {0};
  std::atomic<int> normalBoards {0};
  std::atomic<int> longBoards {0};
  std::atomic<bool> orderReady {false};
  std::thread td;
  std::condition_variable cv;
  std::mutex mtx;
};
