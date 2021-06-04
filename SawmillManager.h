#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include "Sawmill.h"
#include "Resources.h"
#include "BoardType.h"
#include <condition_variable>
class SawmillManager {
public:
  SawmillManager(Resources* resources, std::vector<Sawmill*> sawmills);
  void cycle();
  void setRunning(bool running);
  void getResources(int shortBoard, int normalBoard, int longBoard);
  bool getBoards();
  int requestBoard(BoardType boardType);
  int getLongBoards();
  int getShortBoards();
  int getNormalBoards();
  int getPreparedLongBoards();
  int getPreparedShortBoards();
  int getPreparedNormalBoards();
  int getOrderedLongBoards();
  int getOrderedShortBoards();
  int getOrderedNormalBoards();
  void getPreparedOrder(int shortBoards, int normalBoards, int longBoards);
  std::string getMessage();
private:
  std::thread td;
  std::mutex mtx;
  bool orderRdy = {false};
  int longBoards;
  int normalBoards;
  int shortBoards;
  int preparedLongBoards;
  int preparedNormalBoards;
  int preparedShortBoards;
  int orderedLongBoards;
  int orderedNormalBoards;
  int orderedShortBoards;
  bool shortBoardsNeeded;
  bool normalBoardsNeeded;
  bool longBoardsNeeded;
  std::string message;
  std::condition_variable cv;
  bool running = true;
  Resources * resources;
  std::vector<Sawmill*> sawmills;
};
