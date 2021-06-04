#include "SawmillManager.h"
#include "BoardType.h"
#include "SawmillState.h"
#include "SawmillSpeedState.h"
#define SAWMILLS 3
SawmillManager::SawmillManager(Resources *resources): td(&SawmillManager::cycle, this)
{
  this->resources = resources;
  // create sawmills locally
  for(int i=0; i<SAWMILLS; i++) {
    sawmills.push_back(new Sawmill(resources, (BoardType)i));
  }
}

void SawmillManager::cycle()
{
  while (running)
  {
    message = "Waiting for order";
    // BoardType priorityBoard;
    BoardType receivedBoard;
    // int priorityChangeTime = 100000;
    // int priorityChangeProgress = 0;

    while(orderRdy) {
      // check if all boards are prepared - if so break the loop
      if(preparedShortBoards == orderedShortBoards) {
        std::lock_guard<std::mutex> lock(mtx);
        shortBoardsNeeded = false;
        sawmills[0]->setSpeedState(SawmillSpeedState::NO_ORDER);
      }
      else {
        std::lock_guard<std::mutex> lock(mtx);
        shortBoardsNeeded = true;
        sawmills[0]->setSpeedState(SawmillSpeedState::ORDER);
      }
      if(preparedNormalBoards == orderedNormalBoards) {
        std::lock_guard<std::mutex> lock(mtx);
        normalBoardsNeeded = false;
        sawmills[1]->setSpeedState(SawmillSpeedState::NO_ORDER);
      }
      else {
        std::lock_guard<std::mutex> lock(mtx);
        normalBoardsNeeded = true;
        sawmills[1]->setSpeedState(SawmillSpeedState::ORDER);
      }
      if(preparedLongBoards == orderedLongBoards) {
        std::lock_guard<std::mutex> lock(mtx);
        longBoardsNeeded = false;
        sawmills[2]->setSpeedState(SawmillSpeedState::NO_ORDER);
      }
      else {
        std::lock_guard<std::mutex> lock(mtx);
        longBoardsNeeded = true;
        sawmills[2]->setSpeedState(SawmillSpeedState::ORDER);
      }
      // request single board -> when returned increment counter of given type and check if boards are still needed
      message = std::string(shortBoardsNeeded ? "TRUE" : "FALSE") + " " + std::string(normalBoardsNeeded ? "TRUE" : "FALSE") + " " +std::string(longBoardsNeeded ? "TRUE" : "FALSE");
      receivedBoard = resources->requestAnyBoard(shortBoardsNeeded, normalBoardsNeeded, longBoardsNeeded);
      if(receivedBoard == BoardType::SHORT) {
        preparedShortBoards ++;
      }
      else if(receivedBoard == BoardType::NORMAL) {
        preparedNormalBoards ++;
      }
      else if(receivedBoard == BoardType::LONG) {
        preparedLongBoards ++;
      }
      else {
        throw std::logic_error("BoardType other");
      }
      cv.notify_one();
    }
  }
}
// ta funckja mogla by sie nazywac orderBoards(short, normal, long)
// i mogla by jako pierwsze ustawiac zmienne managera ordered na te ilosci
// a potem ustawiac stan na ORDER_PLACED zeby uruchomic while loopa. Tam bedzie sie dzialo to wszystko.
// na koncu tej funckji po wait na cv bylo by zmienienie stanu run na na ORDER_WAIT i tylko monza by to wypisac na ekranie
// i czkeac w while na ustawienie poparwnego stanu spowrotem
void SawmillManager::getPreparedOrder(int shortBoards, int normalBoards, int longBoards) {
  orderedShortBoards = shortBoards;
  orderedNormalBoards = normalBoards;
  orderedLongBoards = longBoards;
  shortBoardsNeeded = normalBoardsNeeded = longBoardsNeeded = true;
  // here i could ask for all possible resources needed (produced when transport was being delivered)
  orderRdy = true;
  std::unique_lock<std::mutex> ul(mtx);
  // ? possible issue with >= signs in the return statement
  cv.wait(ul, [&] {
    return (preparedShortBoards >= shortBoards && preparedNormalBoards >= normalBoards && preparedLongBoards >= longBoards) ? true : false;
  });
  shortBoardsNeeded = normalBoardsNeeded = longBoardsNeeded = true;
  preparedShortBoards = preparedNormalBoards = preparedLongBoards = 0;
  orderedShortBoards = orderedNormalBoards = orderedLongBoards = 0;
  orderRdy = false;
}
int SawmillManager::getOrderProgress() {
  int preparedSum = preparedShortBoards + preparedNormalBoards + preparedLongBoards;
  int orderedSum = orderedShortBoards + orderedNormalBoards + orderedLongBoards;
  if(orderedSum == 0) {
    return 0;
  }
  return (int)((preparedSum/(float)orderedSum)*100);
}
int SawmillManager::getOrderSum() {
  return orderedShortBoards + orderedNormalBoards + orderedLongBoards;
}
//  NOTE with mutexes it doent work - stuck
std::string SawmillManager::getSawmillBoardTypeStr(int index) {
  // mtx2.lock();
  return sawmills[index]->getBoardTypeStr();
  // mtx2.unlock();
}
int SawmillManager::getSawmillProgress(int index) {
  // mtx2.lock();
  return sawmills[index]->getProgress();
  // mtx2.unlock();
}
std::string SawmillManager::getSawmillSpeedStateStr(int index) {
  // mtx2.lock();
  return sawmills[index]->getSpeedStateStr();
  // mtx2.unlock();
}
std::string SawmillManager::getSawmillStateStr(int index) {
  // mtx2.lock();
  return sawmills[index]->getStateStr();
  // mtx2.unlock();
}
std::string SawmillManager::getMessage() {
  return message;
}

int SawmillManager::getLongBoards()
{
  return longBoards;
}

int SawmillManager::getShortBoards()
{
  return shortBoards;
}

int SawmillManager::getNormalBoards()
{
  return normalBoards;
}

int SawmillManager::getPreparedLongBoards()
{
  return preparedLongBoards;
}

int SawmillManager::getPreparedShortBoards()
{
  return preparedShortBoards;
}

int SawmillManager::getPreparedNormalBoards()
{
  return preparedNormalBoards;
}

int SawmillManager::getOrderedLongBoards()
{
  return orderedLongBoards;
}

int SawmillManager::getOrderedShortBoards()
{
  return orderedShortBoards;
}

int SawmillManager::getOrderedNormalBoards()
{
  return orderedNormalBoards;
}

bool SawmillManager::getLongBoardsNeeded()
{
  return longBoardsNeeded;
}

bool SawmillManager::getShortBoardsNeeded()
{
  return shortBoardsNeeded;

}

bool SawmillManager::getNormalBoardsNeeded()
{
  return normalBoardsNeeded;
}
