#include "SawmillManager.h"

SawmillManager::SawmillManager(Resources *resources, std::vector<Sawmill *> sawmills) : resources(resources), sawmills(sawmills), td(&SawmillManager::cycle, this)
{
}

void SawmillManager::cycle()
{
  while (running)
  {

    
  }
}
void SawmillManager::getResources(int shortBoard, int normalBoard, int longBoard) {
  this->shortBoard = shortBoard;
  this->normalBoard = normalBoard;
  this->longBoard = longBoard;

  getBoards();

  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return (transportPrepared) ? true : false;});

  transportPrepared = false;

}

bool SawmillManager::getBoards()
{
  int sum = 0;
  do {
    sum = longBoard + shortBoard + normalBoard;
    int r = rand() % sum;
    if(r<shortBoard) {
      shortBoard -= 1;
      getSawmill(BoardType::SHORT)->startWork();
    }
    else if (r>=shortBoard && r < shortBoard + normalBoard) {
      normalBoard -= 1;
      getSawmill(BoardType::NORMAL)->startWork();
    } else {
      longBoard -= 1;
      getSawmill(BoardType::LONG)->startWork();
    }
  } while(sum > 0);
  transportPrepared = true;
  cv.notify_one();
}

Sawmill * SawmillManager::getSawmill(BoardType boardType) {
  for(int i=0; i<sawmills.size(); i++) {
    if(sawmills[i]->boardType == boardType) return sawmills[i];
  }
  return NULL;
}

std::string SawmillManager::getMessage() {
  return message;
}

int SawmillManager::getLongBoards()
{
  return longBoard;
}

int SawmillManager::getShortBoards()
{
  return shortBoard;
}

int SawmillManager::getNormalBoards()
{
  return normalBoard;
}