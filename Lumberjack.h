#pragma once
#include <thread>
#include <random>
#include <mutex>
#include <atomic>
#include <vector>
#include "Tree.h"
#include "LumberjackState.h"
using namespace std;
class Lumberjack
{
public:
  static const int NumberOfLumberjacks = 10;
  Lumberjack(int id, vector<Tree *> trees);
  bool getRunning();
  void setRunning(bool running);
  int getId();
  int getTreeId();
  int getCutTreeCounter();
  std::string getState();
private:
  std::thread td;
  std::mutex mtx;
  int id;
  int treeId;
  int cutTreeCounter = {0};
  bool running = true; 
  std::atomic<LumberjackState> state {LumberjackState::WAITING};
  vector<Tree*> trees;

  void cycle();
  Tree* findTree();
};
