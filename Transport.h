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
  std::string getState();
  int getTransportCounter();
  int getShortBoards();
  int getNormalBoards();
  int getLongBoards();
  int getOrderedShortBoards();
  int getOrderedNormalBoards();
  int getOrderedLongBoards();
  void generateOrder();
  float getProgress();
  std::vector<int> getOrder();
  BoardType getTopBoardPriority();
private:
  TransportState transportState {TransportState::FROM_SHOP};
  Resources * resources;
  SawmillManager * sawmillManager;
  float progress = 0.0f;
  int transportCounter = 0;
  bool running = true;
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
