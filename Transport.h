#pragma once
#include <thread>
#include <atomic>
#include "TransportState.h"
#include "Resources.h"
#include <random>
#include <vector>
#include <condition_variable>
#include <mutex>
class Transport {
public:
  Transport(Resources * resources);
  void cycle();
  std::string getState();
  int getTransportCounter();
  void generateOrder();
  float getProgress();
  std::vector<int> getOrder();
private:
  TransportState transportState {TransportState::WAITING};
  Resources * resources;
  float progress = 0.0f;
  int transportCounter = 0;
  bool running = true;
  std::atomic<int> shortBoards {0};
  std::atomic<int> normalBoards {0};
  std::atomic<int> longBoards {0};
  std::atomic<bool> orderReady {false};
  std::thread td;
  std::condition_variable cv;
  std::mutex mtx;

};