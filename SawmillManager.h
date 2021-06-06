#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include "Sawmill.h"
#include "SawmillSpeedState.h"
#include "Resources.h"
#include "BoardType.h"
#include <condition_variable>
class SawmillManager {
public:
  SawmillManager(Resources* resources);
  void cycle();
  void setRunning(bool running);
  bool getBoards();
  int getPreparedLongBoards();
  int getPreparedShortBoards();
  int getPreparedNormalBoards();
  int getOrderedLongBoards();
  int getOrderedShortBoards();
  int getOrderedNormalBoards();
  bool getLongBoardsNeeded();
  bool getShortBoardsNeeded();
  bool getNormalBoardsNeeded();

  int getSawmillProgress(int index);
  std::string getSawmillSpeedStateStr(int index);
  std::string getSawmillStateStr(int index);
  std::string getSawmillBoardTypeStr(int index);
  int getOrderProgress();
  int getOrderSum();
  int getPreparedSum();
  std::string getOrderRdyStr();
  void getPreparedOrder(int shortBoards, int normalBoards, int longBoards);
  void choosePrioritySawmill();
  std::string getMessage();
private:
  std::atomic<bool> fresh = {false};
  std::thread td;
  std::mutex mtx;
  std::mutex mtx2;
  std::atomic<bool> orderRdy {false};
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
