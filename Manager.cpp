#include "Manager.h"
#include "BoardType.h"

#define TREES 12
#define LUMBERJACKS 5
#define SAWMILLS 3

Manager::Manager()
{
  nature = new Nature();
  lumberjacks = std::vector<Lumberjack *>();
  trees = std::vector<Tree *>();
  resources = new Resources();
  sawmillManager = new SawmillManager(resources);
  transport = new Transport(sawmillManager, resources);

  for (int i = 0; i < TREES; i++)
  {
    trees.push_back(new Tree(i, nature, resources));
  }
  for (int i = 0; i < LUMBERJACKS; i++)
  {
    lumberjacks.push_back(new Lumberjack(i, trees));
  }
}
void Manager::exit() {
  resources->setRunning(false);
  sawmillManager->setRunning(false);
  for(int i=0; i<(int)trees.size(); i++) {
    trees[i]->setRunning(false);
  }
  for(int i=0; i<(int)lumberjacks.size(); i++) {
    lumberjacks[i]->setRunning(false);
  }

  nature->setRunning(false);
  transport->setRunning(false);

  // joining threads;

  for(int i=0; i<(int)lumberjacks.size(); i++) {
    lumberjacks[i]->join();
  }
  for(int i=0; i<(int)trees.size(); i++) {
    trees[i]->join();
  }
  transport->join();
  nature->join();
  sawmillManager->join();
}

const std::vector<Tree *> Manager::getTrees()
{
  return trees;
}

const std::vector<Lumberjack *> Manager::getLumberjacks()
{
  return lumberjacks;
}

Nature * Manager::getNature() {
  return nature;
}

Resources * Manager::getResources() {
  return resources;
}

Transport * Manager::getTransport() {
  return transport;
}

SawmillManager * Manager::getSawmillManager() {
  return sawmillManager;
}
