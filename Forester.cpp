#include "Forester.h"

Forester::Forester(std::vector<Tree*> trees) : trees(trees), td(&Forester::cycle, this)
{

}

void Forester::cycle() {
  while(running) {
    int counter=0;
    for(int i=0; i<trees.size(); i++) {
      if(trees[i]->getCutProgress() == 0.0f) {
        counter++;
      }
    }
    if(counter>=10) {
      continue;
    }
  }
}