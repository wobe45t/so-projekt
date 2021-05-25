#pragma once
#include <mutex>
#include <vector>
#include <atomic>
#include <string>
#include <thread>
#include "TreeState.h"
#include "Nature.h"
#include "Resources.h"
using namespace std;
class Tree {

public:
  Tree(int id, Nature * nature, Resources * resources);
  int getId();
  float getGrowth();
  float getCutProgress();
  float getCutSize();
  int getCuttingLumberjacks();

  void setGrowth(float growth);
  void setCutProgress(float cutProgress);
  void setCutSize(float cutSize);
  void setCuttingLumberjacks(int cuttingLumberjacks);
 
  void addLumberjack();
  bool cut();
  void grow();

  std::string getState();

  std::mutex mtx;
  std::atomic<TreeState> state {TreeState::LIVING};
  
private:
  std::thread td;
  bool running = true;
  Nature * nature;
  Resources * resources;
  int id;
  std::atomic<float> growth {100.0f};
  std::atomic<float> cutProgress {0.0f};
  std::atomic<float> cutSize {1.0f}; 
  std::atomic<int> cuttingLumberjacks {0};

  void cycle();
};
