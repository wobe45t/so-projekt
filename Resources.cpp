#include "Resources.h"

Resources::Resources() {

}

void Resources::addWood(int wood) {
  std::lock_guard<std::mutex> lock(mtx);
  this->wood += wood;
  cv.notify_all();
}

void Resources::requestWood(int requestedWood) {
  std::unique_lock<std::mutex> ul(mtx);
  message = "waiting for wood";
  cv.wait(ul, [&] {return (wood >= requestedWood) ? true : false;});
  message = "after wait, received wood";
  this->wood -= requestedWood;
}

void Resources::addNormalBoard() {
  normalBoard++;
  cv.notify_one();
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
  cv.notify_all();
}

int Resources::requestBoard(int boards, BoardType boardType) {
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