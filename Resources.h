#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <string>
#include "BoardType.h"
class Resources {
public:
  Resources();
  void requestWood(int wood);
  void addWood(int wood);
  int requestLongBoard(int boards);
  int requestNormalBoard(int boards);
  int requestShortBoard(int boards);
  void requestBoards(int shortBoards, int normalBoards, int longBoards);
  int requestBoard(int boards, BoardType boardType);
  void addBoard(int boards, BoardType boardType);
  int getWood();
  int getLongBoards();
  int getShortBoards();
  int getNormalBoards();
  BoardType requestAnyBoard(bool shortBoards, bool normalBoards, bool longBoards);
  int requestAllShortBoards();
  int requestAllNormalBoards();
  int requestAllLongBoards();
  bool getRunning();
  void setRunning(bool running);
private:
  std::atomic<int> wood {0};
  std::atomic<int> shortBoard {0};
  std::atomic<int> longBoard {0};
  std::atomic<int> normalBoard {0};
  int boardSum = 0;
  std::atomic<bool> running {true};
  std::string message;
  std::mutex board_mutex;
  std::mutex mtx;
  std::condition_variable board_cv;
  std::condition_variable cv;
};
