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
Lumberjack::Lumberjack(int id, vector<Tree *> trees) : id(id), trees(trees), td(&Lumberjack::cycle, this)
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
      
    }
    state = LumberjackState::RESTING;
    usleep(1000000);
  }
}

void Lumberjack::setRunning(bool running)
{
  this->running = running;
}

bool Lumberjack::getRunning() { return running; }

int Lumberjack::getId()
{
  return id;
}

int Lumberjack::getTreeId()
{
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