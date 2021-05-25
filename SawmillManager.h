#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include "Sawmill.h"
#include "Resources.h"
#include "BoardType.h"
#include "Transport.h"
#include <condition_variable>
class SawmillManager {
public:
  SawmillManager(Resources* resources, std::vector<Sawmill*> sawmills, Transport * transport);
  void cycle();
  void setRunning(bool running);
  void getResources(int shortBoard, int normalBoard, int longBoard);
  bool getBoards();
  int requestBoard(BoardType boardType);
  int getLongBoards();
  int getShortBoards();
  int getNormalBoards();
  std::string getMessage();
  //Sawmill * getSawmill(BoardType boardType);
  Transport * getTransport();
private:
  std::thread td;
  std::mutex mtx;
  int longBoard;
  int normalBoard;
  int shortBoard;
  std::string message;
  std::condition_variable cv;
  bool running = true;
  Transport * transport;
  Resources * resources;
  std::vector<Sawmill*> sawmills;
};