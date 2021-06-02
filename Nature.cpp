#include "Nature.h"

Nature::Nature() : td(&Nature::cycle, this)
{}

void Nature::cycle() {
  int random_value;
  while(running) {
    random_value = rand() % 50000;
    while(changeProgress < 100) {
      usleep(100000 + random_value);
      changeProgress += 1;
    }
    int randCondition = rand() % 3;
    condition = (NatureCondition)randCondition;
    changeProgress = 0;
  }
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
