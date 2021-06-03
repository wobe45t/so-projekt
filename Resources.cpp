#include "Resources.h"
#include <stdexcept>

Resources::Resources() {

}

void Resources::addWood(int wood) {
  std::lock_guard<std::mutex> lock(mtx);
  this->wood += wood;
  cv.notify_all();
}

void Resources::requestWood(int requestedWood) {
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return (wood >= requestedWood) ? true : false;});
  this->wood -= requestedWood;
}

void Resources::addNormalBoard() {
  normalBoard++;
  cv.notify_one();
}
BoardType Resources::requestAnyBoard(int shortBoards, int normalBoards, int longBoards) { // parameters could be changed to neededShortBoards etc.
  int previousBoards = shortBoard + normalBoard + longBoard;
  // moge tutaj zapisac stany poprzednich desek i sprawdzac wszystko w tym jednym cv.wait i przypisac po prostu typ deski ktory dostalo ?
  int previousShortBoards = shortBoard;
  int previousNormalBoards = normalBoard;
  int previousLongBoards = longBoard;
  BoardType returnBoard;
  std::unique_lock<std::mutex> ul(board_mutex);
  int DEBUG_COUNTER = 0;
  board_cv.wait(ul, [&] {
      if(previousShortBoards != shortBoard && shortBoards > 0) {
        returnBoard = (BoardType)0;
        DEBUG_COUNTER ++;
      } else if(previousNormalBoards != normalBoard && normalBoards > 0){
        returnBoard = (BoardType)1;
        DEBUG_COUNTER ++;
      } else if(previousLongBoards != longBoard && longBoards > 0) {
        returnBoard = (BoardType)2;
        DEBUG_COUNTER ++;
      }
      if(DEBUG_COUNTER == 2) {
        throw std::logic_error("DEBUG_COUNTER="+ std::to_string(DEBUG_COUNTER) + " -> SHOULD BE 0|1");
      }
      return (previousBoards + 1 == shortBoard + normalBoard + longBoard) ? true : false;
      // ^ tutaj sprawdzam czy boardy sie roznia. Jezeli w wczesniejszym ifie odejme to wtedy ilosc bedzie sie zgadzac.
  });
  if(returnBoard == BoardType::SHORT) {
    shortBoard -= 1;
  } else if(returnBoard == BoardType::NORMAL) {
    normalBoard -= 1;
  } else if(returnBoard == BoardType::LONG) {
    longBoard -= 1;
  } else {
    throw std::logic_error("NO SUCH BOARD ERROR");
  }
  return returnBoard;
}

int Resources::requestAllShortBoards() {
  int boards;
  board_mutex.lock(); // FIXME change to lock_guard
  boards = shortBoard;
  shortBoard = 0;
  board_mutex.unlock();
  return boards;
}

int Resources::requestAllNormalBoards() {
  int boards;
  board_mutex.lock();
  boards = normalBoard;
  normalBoard = 0;
  board_mutex.unlock();
  return boards;
}

int Resources::requestAllLongBoards() {
  int boards;
  board_mutex.lock();
  boards = longBoard;
  longBoard = 0;
  board_mutex.unlock();
  return boards;
}

void Resources::addBoard(int boards, BoardType boardType) {
  std::lock_guard<std::mutex> lock(mtx);
  switch(boardType) {
    case BoardType::LONG:
    longBoard+=boards;
    break;
    case BoardType::NORMAL:
    normalBoard+=boards;
    break;
    case BoardType::SHORT:
    shortBoard+=boards;
    break;
  }
  boardSum += boards;
  board_cv.notify_all();
}

int Resources::requestBoard(int boards, BoardType boardType) {
  // FIXME add lock_guard ...
  boardSum -= boards;
  switch(boardType) {
    case BoardType::LONG:
    return requestLongBoard(boards);
    break;
    case BoardType::NORMAL:
    return requestNormalBoard(boards);
    break;
    case BoardType::SHORT:
    return requestShortBoard(boards);
    break;
    default:
    return 0;
  }
}

void Resources::requestBoards(int requestedShortBoards, int requestedNormalBoards, int requestedLongBoards) {
  std::unique_lock<std::mutex> ul(mtx); // FIXME change mutex to separate mutex (check other functions)
  cv.wait(ul, [&] {return (shortBoard >= requestedShortBoards && normalBoard >= requestedNormalBoards && longBoard >= requestedLongBoards) ? true : false;});
  shortBoard -= requestedShortBoards;
  normalBoard -= requestedNormalBoards;
  longBoard -= requestedLongBoards;
}


int Resources::requestShortBoard(int requestedBoards) {
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return (shortBoard >= requestedBoards) ? true : false;});
  shortBoard -= requestedBoards;
  return requestedBoards;
}


int Resources::requestNormalBoard(int requestedBoards) {
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return (normalBoard >= requestedBoards) ? true : false;});
  normalBoard -= requestedBoards;
  return requestedBoards;
}
int Resources::requestLongBoard(int requestedBoards) {
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return (longBoard >= requestedBoards) ? true : false;});
  longBoard -= requestedBoards;
  return requestedBoards;
}
int Resources::getWood() {
  return wood;
}

int Resources::getLongBoards() {
  return longBoard;
}
int Resources::getShortBoards() {
  return shortBoard;
}
int Resources::getNormalBoards() {
  return normalBoard;
}


std::string Resources::getMessage() {
  return this->message;
}
