#include "Nature.h"

Nature::Nature() : td(&Nature::cycle, this)
{}

void Nature::cycle() {
  int random_value;
  while(running) {
    random_value = rand() % 50000;
    while(changeProgress < 100 && running == true) {
      usleep(100000 + random_value);
      changeProgress += 1;
    }
    int randCondition = rand() % 3;
    condition = (NatureCondition)randCondition;
    changeProgress = 0;
  }
  done = true;
}

void Nature::setRunning(bool running) {
  this->running = running;
}
bool Nature::getRunning() {
  return running;
}
bool Nature::getDone() {
  return done;
}
void Nature::join() {
  this->td.join();
}
void Nature::finish() {
  this->running = false;
  this->td.join();
}
std::string Nature::getConditions() {
  switch(condition) {
    case NatureCondition::BAD:
    return "BAD";
    case NatureCondition::NORMAL:
    return "NORMAL";
    case NatureCondition::GOOD:
    return "GOOD";
    default:
    return "UNKNOWN";
  }
}
int Nature::getChangeProgress() {
  return changeProgress;
}
float Nature::getGrowthCoef() {
  switch(condition) {
    case NatureCondition::BAD:
    return 0.5f;
    case NatureCondition::NORMAL:
    return 1.0f;
    case NatureCondition::GOOD:
    return 1.5f;
    default:
    return 1.0f;
  }
}

int Nature::getGrownTrees() {
  return grownTrees;
}
