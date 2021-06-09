#include "Tree.h"
#include <atomic>
#include <iostream>
#include <random>
#include <unistd.h>
Tree::Tree(int id, Nature * nature, Resources * resources) : id(id), nature(nature), resources(resources), td(&Tree::cycle, this)
{}

void Tree::addLumberjack() {
  cuttingLumberjacks++;
  if(cuttingLumberjacks>1) {
    float value = 1/float(cuttingLumberjacks);
    cutSize = cutSize - value + 1;
  }
}

void Tree::cycle() {
  while(running) {
    if(state == TreeState::GROWING) {
      float newValue = growth + 1.0f * nature->getGrowthCoef();
      growth = newValue;
      if(newValue >= 100.0f) {
        state = TreeState::LIVING;
      } 
      usleep(100000 + rand() % 100000);
    }
  }
  done = true;
}

void Tree::setRunning(bool running) {
  this->running = running;
}
bool Tree::getDone() {
  return done;
}
void Tree::join() {
  this->td.join();
}

bool Tree::cut() {
  if(state == TreeState::GROWING) return false;
  cutProgress.store(cutProgress + cutSize);
  state = TreeState::CUTTING;
  if(cutProgress>=100 && running == true) {
    cuttingLumberjacks = 0;
    cutProgress = 0;
    growth = 0;
    cutSize = 1.0f;
    resources->addWood(4);
    state = TreeState::GROWING;
    return false;
  }
  return true;
}


int Tree::getId() {
  return id;
}
float Tree::getGrowth() {
  return growth;
}
float Tree::getCutProgress() {
  return cutProgress;
}
float Tree::getCutSize() {
  return cutSize;
}
int Tree::getCuttingLumberjacks() {
  return cuttingLumberjacks;
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
