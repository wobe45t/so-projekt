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
    // message = "WAITING FOR ORDER";
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
      // NOTE check if ordered board are not 0
      message = std::to_string(preparedShortBoards) + "/" + std::to_string(orderedShortBoards) + " " +
 std::to_string(preparedNormalBoards) + "/" + std::to_string(orderedNormalBoards) + " " + std::to_string(preparedLongBoards) + "/" + std::to_string(orderedLongBoards);
      // to robic po checku czy ktoras z tych w ogole jest true
      receivedBoard = resources->requestAnyBoard(shortBoardsNeeded, normalBoardsNeeded, longBoardsNeeded);
      // FIXME remove from .h/.coo
      counter++;
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
std::string SawmillManager::getCounterStr() {
  return std::to_string(counter);
}

void SawmillManager::getPreparedOrder(int shortBoards, int normalBoards, int longBoards) {
  // FIXME this-> is redundant
  this->orderedShortBoards = shortBoards;
  this->orderedNormalBoards = normalBoards;
  this->orderedLongBoards = longBoards;
  // get all possible boards from the resources
  this->preparedShortBoards = resources->requestShortBoard(shortBoards);
  this->preparedNormalBoards = resources->requestNormalBoard(normalBoards);
  this->preparedLongBoards = resources->requestLongBoard(longBoards);
  //FIXME fresh is redundant
  fresh = true;
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
      resetOrder();
      counter = 0;
      return true;
    }
    return false;
  });
}
void SawmillManager::resetOrder() {
  preparedShortBoards = 0;
  preparedNormalBoards = 0;
  preparedLongBoards = 0;
  orderedShortBoards = 0;
  orderedNormalBoards = 0;
  orderedLongBoards = 0;
}
std::string SawmillManager::getOrderRdyStr() {
  if(orderRdy) {
    return "TRUE";
  }
  else {
    return "FALSE";
  }
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
// NOTE  THIS FUNCTIONS DIDNT HELP
// FIXME remove those functions from .cpp/.h
void SawmillManager::setPreparedLongBoards(int preparedLongBoards) {
  mtx2.lock();
  this->preparedLongBoards = preparedLongBoards;
  mtx2.unlock();
}
void SawmillManager::setPreparedNormalBoards(int preparedNormalBoards) {
  mtx2.lock();
  this->preparedNormalBoards = preparedNormalBoards;
  mtx2.unlock();
}
void SawmillManager::setPreparedShortBoards(int preparedShortBoards) {
  mtx2.lock();
  this->preparedShortBoards = preparedShortBoards;
  mtx2.unlock();
}
void SawmillManager::setOrderedLongBoards(int orderedLongBoards) {
  mtx2.lock();
  this->orderedLongBoards = orderedLongBoards;
  mtx2.unlock();
}
void SawmillManager::setOrderedNormalBoards(int orderedNormalBoards) {
  mtx2.lock();
  this->orderedNormalBoards = orderedNormalBoards;
  mtx2.unlock();
}
void SawmillManager::setOrderedShortBoards(int orderedShortBoards) {
  mtx2.lock();
  this->orderedShortBoards = orderedShortBoards;
  mtx2.unlock();
}

//  NOTE with mutexes it doent work - stuck
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
