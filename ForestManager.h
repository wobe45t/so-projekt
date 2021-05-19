#pragma once
#include <mutex>
#include "Tree.h"
#include "Lumberjack.h"
#include "Forester.h"
#include "Sawmill.h"
#include "Nature.h"
#include <vector>
class ForestManager
{
public:

  ForestManager();
  ~ForestManager();
  const std::vector<Tree *> getTrees();
	const std::vector<Lumberjack *> getLumberjacks();

  Tree * getAvailableTree();


private:
  std::vector<Tree *> trees;
  std::vector<Lumberjack *> lumberjacks;

};
