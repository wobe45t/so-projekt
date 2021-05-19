#include "Nature.h"

Nature::Nature() : td(&Nature::cycle, this)
{}

void Nature::cycle() {
  while(running) {
    usleep(10000000 + rand() % 5000000);
    int randCondition = rand() % 3;
    condition = (NatureCondition)randCondition;
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