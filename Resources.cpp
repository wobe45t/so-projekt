#include "Resources.h"

Resources::Resources() {

}

void Resources::addWood(int wood) {
  std::lock_guard<std::mutex> lock(mtx);
  this->wood += wood;
  //! tutaj notify one powiadomi tylko tartak ktory chce jezeli tylko jeden z nich bedzie czeakl w wait na danym stanie.
  cv.notify_all();
}

int Resources::requestWood(int requestedWood) {
  std::unique_lock<std::mutex> ul(mtx);
  message = "waiting for wood";
  cv.wait(ul, [&] {return (wood >= requestedWood) ? true : false;});
  message = "after wait, received wood";
  this->wood -= requestedWood;
  return requestedWood;
}

void Resources::addBoard(int boards, BoardType boardType) {
  std::lock_guard<std::mutex> lock(mtx);
  message = "providing borard";
  switch(boardType) {
    case BoardType::LONG:
    longBoard+=boards;
    case BoardType::NORMAL:
    normalBoard+=boards;
    case BoardType::SHORT:
    shortBoard+=boards;
  }
  cv.notify_all();
}

int Resources::requestBoard(int boards, BoardType boardType) {
  switch(boardType) {
    case BoardType::LONG:
    return requestLongBoard(boards);
    case BoardType::NORMAL:
    return requestNormalBoard(boards);
    case BoardType::SHORT:
    return requestShortBoard(boards);
    default:
    return 0;
  }
}

int Resources::requestLongBoard(int requestedBoards) {
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return (longBoard >= requestedBoards) ? true : false;});
  longBoard -= requestedBoards;
  return requestedBoards;
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