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
  std::string getMessage();
  Sawmill * getSawmill(BoardType boardType);
private:
  bool transportPrepared = false;
  std::mutex mtx;
  Resources * resources;
  int longBoard;
  int normalBoard;
  int shortBoard;
  std::string message;
  std::condition_variable cv;
  bool running = true;
  std::thread td;
  std::vector<Sawmill*> sawmills;
};