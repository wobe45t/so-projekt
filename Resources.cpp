#include "Resources.h"

Resources::Resources() {

}
void Resources::setRunning(bool running) {
  this->running = running;
  mtx.unlock();
  cv.notify_all();
}

bool Resources::getRunning() {
  return running;
}

void Resources::addWood(int wood) {
  std::lock_guard<std::mutex> lock(mtx);
  this->wood += wood;
  cv.notify_all();
}

void Resources::requestWood(int requestedWood) {
  std::unique_lock<std::mutex> ul(mtx);
  if(running == false) return;
  cv.wait(ul, [&] {
      if(running == false) {
        return true;
      }
      if (wood >= requestedWood) {
        return true;
      }
      return false;
  });
  this->wood -= requestedWood;
}

BoardType Resources::requestAnyBoard(bool shortBoardsNeeded, bool normalBoardsNeeded, bool longBoardsNeeded) {
  if(running) {
    int previousShortBoards = shortBoard;
    int previousNormalBoards = normalBoard;
    int previousLongBoards = longBoard;
    BoardType returnBoard;
    std::unique_lock<std::mutex> ul(mtx);
    cv.wait(ul, [&] {
        if(running == false) {
          return true;
        }
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
  return BoardType::UNDEFINED;
}

int Resources::requestAllShortBoards() {
  std::lock_guard<std::mutex> lock(mtx);
  int boards;
  boards = shortBoard;
  shortBoard = 0;
  return boards;
}

int Resources::requestAllNormalBoards() {
  std::lock_guard<std::mutex> lock(mtx);
  int boards;
  boards = normalBoard;
  normalBoard = 0;
  return boards;
}

int Resources::requestAllLongBoards() {
  std::lock_guard<std::mutex> lock(mtx);
  int boards;
  boards = longBoard;
  longBoard = 0;
  return boards;
}

void Resources::addBoard(int boards, BoardType boardType) {
  if(running == false) return;
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
  if(running)
    cv.notify_all();
}

int Resources::requestBoard(int boards, BoardType boardType) {
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
  std::unique_lock<std::mutex> ul(mtx);
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

