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
  sawmills = std::vector<Sawmill * >();
  resources = new Resources();
  sawmillManager = new SawmillManager(resources, sawmills);
  transport = new Transport(sawmillManager, resources);

  for (int i = 0; i < TREES; i++)
  {
    trees.push_back(new Tree(i, nature, resources));
  }

  for (int i = 0; i < LUMBERJACKS; i++)
  {
    lumberjacks.push_back(new Lumberjack(i, trees));
  }
  for (int i = 0; i< SAWMILLS; i++) {
    sawmills.push_back(new Sawmill(resources, (BoardType)i));
  }
}

void Manager::setRunning(bool running)
{
  this->running = running;
}

bool Manager::getRunning()
{
  return running;
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

Sawmill * Manager::getSawmill() {
  return sawmill;
}

std::vector<Sawmill *> Manager::getSawmills() {
  return sawmills;
}

Transport * Manager::getTransport() {
  return transport;
}

SawmillManager * Manager::getSawmillManager() {
  return sawmillManager;
}
