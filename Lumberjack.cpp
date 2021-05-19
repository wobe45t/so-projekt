#include "Lumberjack.h"
#include "LumberjackState.h"
#include "TreeState.h"
#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <mutex>
#include <thread>
#include <random>

using namespace std;
Lumberjack::Lumberjack(int id, vector<Tree *> trees, Resources *resources) : id(id), trees(trees), resources(resources), td(&Lumberjack::cycle, this)
{
}

Tree *Lumberjack::findTree()
{
  Tree *minTree;
  int minLumberjacks = Lumberjack::NumberOfLumberjacks;
  for (int i = 0; i < trees.size(); i++)
  {
    if ((trees[i]->state == TreeState::LIVING || trees[i]->state == TreeState::CUTTING) && trees[i]->getCuttingLumberjacks() < minLumberjacks && trees[i]->getCuttingLumberjacks() < 3)
    {
      minLumberjacks = trees[i]->getCuttingLumberjacks();
      minTree = trees[i];
    }
  }
  return minTree;
}
void Lumberjack::cycle()
{
  Tree *tree;
  while (running)
  {
    tree = findTree();

    if (tree != NULL)
    {
      state = LumberjackState::CUTTING;
      tree->addLumberjack();
      treeId = tree->getId();

      bool cutTree = true;

      int sleepTime = 100000 + rand() % 100000;

      while (cutTree)
      {
        tree->mtx.lock();
        cutTree = tree->cut();
        tree->mtx.unlock();
        usleep(sleepTime);
      }
      treeId = 0;
      resources->addWood(1);
    }
    state = LumberjackState::RESTING;
    usleep(1000000);
  }
}

void Lumberjack::setRunning(bool running)
{
  std::lock_guard<std::mutex> lock(mtx);
  this->running = running;
}

bool Lumberjack::getRunning() { return running; }

int Lumberjack::getId()
{
  std::lock_guard<std::mutex> lock(mtx);
  return id;
}

int Lumberjack::getTreeId()
{
  std::lock_guard<std::mutex> lock(mtx);
  return treeId;
}

std::string Lumberjack::getState()
{
  switch (state)
  {
  case LumberjackState::WAITING:
    return "WAITING";
  case LumberjackState::RESTING:
    return "RESTING";
  case LumberjackState::CUTTING:
    return "CUTTING";
  default:
    return "UNKNOWN";
  }
}