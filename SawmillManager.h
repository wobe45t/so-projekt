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
  bool getLongBoardsNeeded();
  bool getShortBoardsNeeded();
  bool getNormalBoardsNeeded();
  void getPreparedOrder(int shortBoards, int normalBoards, int longBoards);
  std::string getMessage();
private:
  std::thread td;
  std::mutex mtx;
  std::mutex mtx2;
  bool orderRdy = {false};
  std::atomic<int> longBoards {0};
  std::atomic<int> normalBoards {0};
  std::atomic<int> shortBoards {0};
  std::atomic<int> preparedLongBoards {0};
  std::atomic<int> preparedNormalBoards {0};
  std::atomic<int> preparedShortBoards {0};
  std::atomic<int> orderedLongBoards {0};
  std::atomic<int> orderedNormalBoards {0};
  std::atomic<int> orderedShortBoards {0};
  std::atomic<bool> shortBoardsNeeded {false};
  std::atomic<bool> normalBoardsNeeded {false};
  std::atomic<bool> longBoardsNeeded {false};
  std::string message;
  std::condition_variable cv;
  bool running = true;
  Resources * resources;
  std::vector<Sawmill*> sawmills;
};
