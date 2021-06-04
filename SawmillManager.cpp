#include "SawmillManager.h"
#include "BoardType.h"
#include "SawmillState.h"
SawmillManager::SawmillManager(Resources *resources, std::vector<Sawmill *> sawmills): sawmills(sawmills), resources(resources) ,td(&SawmillManager::cycle, this)
{
}

void SawmillManager::cycle()
{
  while (running)
  {
    message = "Waiting for order";
    // BoardType priorityBoard;
    BoardType receivedBoard;
    // int priorityChangeTime = 100000;
    // int priorityChangeProgress = 0;

    while(orderRdy) {
      // check if all boards are prepared - if so break the loop
      if(preparedShortBoards >= orderedShortBoards) {
        shortBoardsNeeded = false;
        // message = "shortBoardsNotNeeded";
        // sawmills[0]->setWork(false);
      }
      else {
        shortBoardsNeeded = true;
        // sawmills[0]->setWork(true);
      }
      // FIXME problem when this block is set so when samill starts work
      if(preparedNormalBoards >= orderedNormalBoards) {
        normalBoardsNeeded = false;
        // message = "normalBoardsNotNeeded";
        // sawmills[1]->setWork(false);
      }
      else {
        normalBoardsNeeded = true;
        // sawmills[1]->setWork(true);
      }
      if(preparedLongBoards >= orderedLongBoards) {
        longBoardsNeeded = false;
        // message = "longBoardsNotNeeded";
        // sawmills[2]->setWork(false);
      }
      else {
        longBoardsNeeded = true;
        // sawmills[2]->setWork(true);
      }
      // request single board -> when returned increment counter of given type and check if boards are still needed
      message = std::string(shortBoardsNeeded ? "TRUE" : "FALSE") + " " + std::string(normalBoardsNeeded ? "TRUE" : "FALSE") + " " +std::string(longBoardsNeeded ? "TRUE" : "FALSE");
      receivedBoard = resources->requestAnyBoard(shortBoardsNeeded, normalBoardsNeeded, longBoardsNeeded);
      if(receivedBoard == BoardType::SHORT) {
        preparedShortBoards ++;
      }
      else if(receivedBoard == BoardType::NORMAL) {
        preparedNormalBoards ++;
      }
      else if(receivedBoard == BoardType::LONG) {
        preparedLongBoards ++;
      }
      else {
        throw std::logic_error("BoardType other");
      }
      cv.notify_one();
    }
      // if any of the board type is needed continue to work, else break
    // if(!shortBoardsNeeded && !normalBoardsNeeded && !longBoardsNeeded) {
    //   orderRdy = false;
    //   message = "BREAKING THE LOOP";
    //   cv.notify_one();
    //   break;
    // }
    // set prepared*Boards to 0 and send notification to TRANSPORT thread that the boards are prepared.
    // while(priorityChangeProgress < 100) {
    //   usleep(priorityChangeTime/100);
    //   priorityChangeProgress += 1;
    // }
    // priorityBoard = transport->getTopBoardPriority();
    // for(int i=0; i<(int)sawmills.size(); i++) {
    //   sawmills[i]->requestBoard();
    //   if(sawmills[i]->getBoardType() == priorityBoard) {
    //     sawmills[i]->setPriority(true);
    //   }
    //   else {
    //     sawmills[i]->setPriority(false);
    //   }
    // }
  }
}
// ta funckja mogla by sie nazywac orderBoards(short, normal, long)
// i mogla by jako pierwsze ustawiac zmienne managera ordered na te ilosci
// a potem ustawiac stan na ORDER_PLACED zeby uruchomic while loopa. Tam bedzie sie dzialo to wszystko.
// na koncu tej funckji po wait na cv bylo by zmienienie stanu run na na ORDER_WAIT i tylko monza by to wypisac na ekranie
// i czkeac w while na ustawienie poparwnego stanu spowrotem
void SawmillManager::getPreparedOrder(int shortBoards, int normalBoards, int longBoards) {
  orderedShortBoards = shortBoards;
  orderedNormalBoards = normalBoards;
  orderedLongBoards = longBoards;
  shortBoardsNeeded = true;
  normalBoardsNeeded = true;
  longBoardsNeeded = true;
  // here i could ask for all possible resources needed (produced when transport was being delivered)
  orderRdy = true;
  std::unique_lock<std::mutex> ul(mtx);
  // ? possible issue with >= signs in the return statement
  cv.wait(ul, [&] {
    return (preparedShortBoards >= shortBoards && preparedNormalBoards >= normalBoards && preparedLongBoards >= longBoards) ? true : false;
  });
  shortBoardsNeeded = normalBoardsNeeded = longBoardsNeeded = true;
  preparedShortBoards = preparedNormalBoards = preparedLongBoards = 0;
  orderedShortBoards = orderedNormalBoards = orderedLongBoards = 0;
  orderRdy = false;
}

std::string SawmillManager::getMessage() {
  return message;
}

int SawmillManager::getLongBoards()
{
  return longBoards;
}

int SawmillManager::getShortBoards()
{
  return shortBoards;
}

int SawmillManager::getNormalBoards()
{
  return normalBoards;
}

int SawmillManager::getPreparedLongBoards()
{
  return preparedLongBoards;
}

int SawmillManager::getPreparedShortBoards()
{
  return preparedShortBoards;
}

int SawmillManager::getPreparedNormalBoards()
{
  return preparedNormalBoards;
}

int SawmillManager::getOrderedLongBoards()
{
  return orderedLongBoards;
}

int SawmillManager::getOrderedShortBoards()
{
  return orderedShortBoards;
}

int SawmillManager::getOrderedNormalBoards()
{
  return orderedNormalBoards;
}
