#include "Tree.h"
#include <atomic>
#include <iostream>
#include <random>
#include <unistd.h>
Tree::Tree(int id, Nature * nature, Resources * resources) : id(id), nature(nature), resources(resources), td(&Tree::cycle, this)
{}

void Tree::addLumberjack() {
  cuttingLumberjacks++;
  if(cuttingLumberjacks.load()>1) {
    float value = 1/float(cuttingLumberjacks.load());
    cutSize.store(cutSize.load() - value + 1);
  }
}

void Tree::cycle() {
  while(running) {
    if(state == TreeState::GROWING) {
      float newValue = growth.load() + 1.0f * nature->getGrowthCoef();
      growth.store(newValue);
      if(newValue >= 100.0f) {
        state = TreeState::LIVING;
      } 
      usleep(100000 + rand() % 100000);
    }
    // usleep(500000);
  }
}

bool Tree::cut() {
  if(state == TreeState::GROWING) return false;
  cutProgress.store(cutProgress.load() + cutSize.load());
  state = TreeState::CUTTING;
  if(cutProgress.load()>=100) {
    cuttingLumberjacks = 0;
    cutProgress = 0;
    growth = 0;
    cutSize = 1.0f;
    resources->addWood(2);
    state = TreeState::GROWING;
    return false;
  }
  return true;
}


int Tree::getId() {
  return id;
}
float Tree::getGrowth() {
  return growth.load();
}
float Tree::getCutProgress() {
  return cutProgress.load();
}
float Tree::getCutSize() {
  return cutSize.load();
}
int Tree::getCuttingLumberjacks() {
  return cuttingLumberjacks.load();
}

void Tree::setGrowth(float growth) {
  this->growth = growth;
}
void Tree::setCutProgress(float cutProgress) {
  this->cutProgress = cutProgress;
}
void Tree::setCutSize(float cutSize) {
  this->cutSize = cutSize;
}
void Tree::setCuttingLumberjacks(int cuttingLumberjacks) {
  this->cuttingLumberjacks = cuttingLumberjacks;
}


std::string Tree::getState() {
  switch(state) {
    case TreeState::LIVING:
      return "LIVING";
    case TreeState::GROWING:
      return "GROWING";
    case TreeState::CUTTING:
      return "CUTTING";
    default:
      return "UNKNOWN";
  }
}
