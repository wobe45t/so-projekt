#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <string>
#include "BoardType.h"
class Resources {
public:
  Resources();
  int requestWood(int wood);
  void addWood(int wood);
  int requestLongBoard(int boards);
  int requestNormalBoard(int boards);
  int requestShortBoard(int boards);
  int requestBoard(int boards, BoardType boardType);
  void addBoard(int boards, BoardType boardType);
  int getWood();
  int getLongBoards();
  int getShortBoards();
  int getNormalBoards();
  std::string getMessage();
private:
  int wood = 0;
  int shortBoard = 0;
  int longBoard = 0;
  int normalBoard = 0;
  int furniture = 0;
  std::string message;
  std::mutex mtx;
  std::condition_variable cv;
};