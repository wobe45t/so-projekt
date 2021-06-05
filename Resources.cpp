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

BoardType Resources::requestAnyBoard(bool shortBoardsNeeded, bool normalBoardsNeeded, bool longBoardsNeeded) {
  // moge tutaj zapisac stany poprzednich desek i sprawdzac wszystko w tym jednym cv.wait i przypisac po prostu typ deski ktory dostalo ?
  int previousShortBoards = shortBoard;
  int previousNormalBoards = normalBoard;
  int previousLongBoards = longBoard;
  BoardType returnBoard;
  bool result = true;
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {
      if(previousShortBoards < shortBoard ) {
        returnBoard = BoardType::SHORT;
        if(shortBoardsNeeded) {
          shortBoard--;
          return true;
        }
      }
      if(previousNormalBoards < normalBoard){
        returnBoard = BoardType::NORMAL;
        if(normalBoardsNeeded) {
          normalBoard --;
          return true;
        }
      }
      if(previousLongBoards < longBoard) {
        returnBoard = BoardType::LONG;
        if(longBoardsNeeded) {
          longBoard--;
          return true;
        }
      }
      return false;
  });
  return returnBoard;
}

int Resources::requestAllShortBoards() {
  int boards;
  mtx.lock(); // FIXME change to lock_guard
  boards = shortBoard;
  shortBoard = 0;
  mtx.unlock();
  return boards;
}

int Resources::requestAllNormalBoards() {
  int boards;
  mtx.lock();
  boards = normalBoard;
  normalBoard = 0;
  mtx.unlock();
  return boards;
}

int Resources::requestAllLongBoards() {
  int boards;
  mtx.lock();
  boards = longBoard;
  longBoard = 0;
  mtx.unlock();
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
  cv.notify_all();
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
  int result = 0;
  cv.wait(ul, [&] {
    if(requestedBoards >= shortBoard) {
      result = shortBoard;
      shortBoard = 0;
      return true;
    }
    else {
      result = requestedBoards;
      shortBoard -= requestedBoards;
      return true;
    }
    return false;
  });
  return result;
}
int Resources::requestNormalBoard(int requestedBoards) {
  std::unique_lock<std::mutex> ul(mtx);
  int result = 0;
  cv.wait(ul, [&] {
    if(requestedBoards >= normalBoard) {
      result = normalBoard;
      normalBoard = 0;
      return true;
    }
    else {
      result = requestedBoards;
      normalBoard -= requestedBoards;
      return true;
    }
    return false;
  });
  return result;
}

int Resources::requestLongBoard(int requestedBoards) {
  std::unique_lock<std::mutex> ul(mtx);
  int result = 0;
  cv.wait(ul, [&] {
    if(requestedBoards >= longBoard) {
      result = longBoard;
      longBoard = 0;
      return true;
    }
    else {
      result = requestedBoards;
      longBoard -= requestedBoards;
      return true;
    }
    return false;
  });
  return result;
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
