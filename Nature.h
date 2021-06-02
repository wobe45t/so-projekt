#pragma once
#include <unistd.h>
#include <thread>
#include "NatureCondition.h"
#include <vector>
#include <atomic>
#include <random>
class Nature {
public:
  Nature();
  void cycle();
  int getGrownTrees();
  std::string getConditions();
  float getGrowthCoef();
  int getChangeProgress();
private:
  int grownTrees = 0;
  std::atomic<int> changeProgress {0};
  std::thread td;
  std::atomic<NatureCondition> condition;
  bool running = true;
  
};
