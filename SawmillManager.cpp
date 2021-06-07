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
  BoardType receivedBoard;
  while (running)
  {
    // NOTE this usleep fixed issue with race condition
    usleep(100);
    if(orderRdy) {
      // NOTE THIS CHECK THROWS AN ERROR
      // if(orderedLongBoards == preparedLongBoards && preparedLongBoards != 0 && orderedNormalBoards == preparedNormalBoards && preparedNormalBoards != 0 && orderedShortBoards == preparedShortBoards && preparedShortBoards != 0) {
      //   throw std::logic_error("prepared " + std::to_string(preparedLongBoards) + " ordered " + std::to_string(orderedLongBoards));
      // }
      if(preparedShortBoards >= orderedShortBoards) {
        shortBoardsNeeded = false;
        sawmills[0]->setSpeedState(SawmillSpeedState::NO_ORDER);
      }
      else {
        shortBoardsNeeded = true;
        sawmills[0]->setSpeedState(SawmillSpeedState::ORDER);
      }
      if(preparedNormalBoards >= orderedNormalBoards) {
        normalBoardsNeeded = false;
        sawmills[1]->setSpeedState(SawmillSpeedState::NO_ORDER);
      }
      else {
        normalBoardsNeeded = true;
        sawmills[1]->setSpeedState(SawmillSpeedState::ORDER);
      }
      if(preparedLongBoards >= orderedLongBoards) {
        message = "";
        longBoardsNeeded = false;
        sawmills[2]->setSpeedState(SawmillSpeedState::NO_ORDER);
      }
      else {
        longBoardsNeeded = true;
        sawmills[2]->setSpeedState(SawmillSpeedState::ORDER);
      }
      // CHOOSE SAWMILL TO SET PRIORITY
      choosePrioritySawmill();

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
void SawmillManager::choosePrioritySawmill() {
  int shortBoardDiff = orderedShortBoards - preparedShortBoards;
  int normalBoardDiff = orderedNormalBoards - preparedNormalBoards;
  int longBoardDiff = orderedLongBoards - preparedLongBoards;
  // NOTE tutaj mozna by bylo dodac zmienne counter oraz previous prio board
  // i sprawdzac czy counter przynajmniej odbyl sie 2 razy albo juz nie potrzeba konkretnych desek(wtedy counter moze byc 1)
  // naprawilo by to problem ze skakaniem pomiedzy boardami co zmiane o 1 boarda w niektorych konfiguracjach orderu
  if(longBoardDiff >= normalBoardDiff) {
    if(normalBoardDiff >= shortBoardDiff && normalBoardsNeeded) {
      sawmills[2]->setSpeedState(SawmillSpeedState::PRIORITY);
    }
    else if(shortBoardDiff >= normalBoardDiff && shortBoardsNeeded){
      sawmills[0]->setSpeedState(SawmillSpeedState::PRIORITY);
    }
  }
  else if(normalBoardDiff >= longBoardDiff){
    if(normalBoardDiff >= shortBoardDiff && normalBoardsNeeded) {
      sawmills[1]->setSpeedState(SawmillSpeedState::PRIORITY);
    }
    else if(shortBoardDiff >= normalBoardDiff && shortBoardsNeeded){
      sawmills[0]->setSpeedState(SawmillSpeedState::PRIORITY);
    }
  }
  else {
    return;
  }
}
void SawmillManager::getPreparedOrder(int shortBoards, int normalBoards, int longBoards) {
  orderedShortBoards = shortBoards;
  orderedNormalBoards = normalBoards;
  orderedLongBoards = longBoards;
  preparedShortBoards = resources->requestShortBoard(shortBoards);
  preparedNormalBoards = resources->requestNormalBoard(normalBoards);
  preparedLongBoards = resources->requestLongBoard(longBoards);
  orderRdy = true;
  std::unique_lock<std::mutex> ul(mtx);
  // NOTE possible issue with >= signs in the return statement
  cv.wait(ul, [&] {
    if(preparedShortBoards >= shortBoards && preparedNormalBoards >= normalBoards && preparedLongBoards >= longBoards) {
      orderRdy = false;
      sawmills[0]->setSpeedState(SawmillSpeedState::NO_ORDER);
      sawmills[1]->setSpeedState(SawmillSpeedState::NO_ORDER);
      sawmills[2]->setSpeedState(SawmillSpeedState::NO_ORDER);
      shortBoardsNeeded = false;
      normalBoardsNeeded = false;
      longBoardsNeeded = false;
      preparedShortBoards = 0;
      preparedNormalBoards = 0;
      preparedLongBoards = 0;
      orderedShortBoards = 0;
      orderedNormalBoards = 0;
      orderedLongBoards = 0;
      return true;
    }
    return false;
  });
}
bool SawmillManager::getOrderRdy() {
  return orderRdy;
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

int SawmillManager::getPreparedSum() {
  return preparedShortBoards + preparedNormalBoards + preparedLongBoards;
}

std::string SawmillManager::getSawmillBoardTypeStr(int index) {
  return sawmills[index]->getBoardTypeStr();
}

int SawmillManager::getSawmillProgress(int index) {
  return sawmills[index]->getProgress();
}

std::string SawmillManager::getSawmillSpeedStateStr(int index) {
  return sawmills[index]->getSpeedStateStr();
}

std::string SawmillManager::getSawmillStateStr(int index) {
  return sawmills[index]->getStateStr();
}

std::string SawmillManager::getMessage() {
  return message;
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
