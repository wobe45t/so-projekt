#include "ForestManager.h"

#define LUMBERJACKS 10
#define TREES 50

ForestManager::ForestManager()
{
  trees = std::vector<Tree *>();
  lumberjacks = std::vector<Lumberjack *>();

  for (int i = 0; i < TREES; i++)
  {
    trees.push_back(new Tree(i));
  }

  for (int i = 0; i < LUMBERJACKS; i++)
  {
    lumberjacks.push_back(new Lumberjack(i, trees));
  }
}

Tree *ForestManager::getAvailableTree()
{
  int counter= 0;
  while(counter < LUMBERJACKS) {
    for (int i = 0; i < trees.size(); i++)
    {
      if(trees[i]->getGrowth() == 100.0f && trees[i]->getCuttingLumberjacks() == counter) {
        return trees[i];
      }
    }
    counter += 1;
  }
}