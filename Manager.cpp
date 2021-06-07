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
//FIXME change this function to exit or something and close all the child threads
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

Transport * Manager::getTransport() {
  return transport;
}

SawmillManager * Manager::getSawmillManager() {
  return sawmillManager;
}
