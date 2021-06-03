#include "Transport.h"
#include <unistd.h>

#define RAND_RANGE 10

Transport::Transport(Resources * resources) : resources(resources), td(&Transport::cycle, this)
{

}

void Transport::cycle() {
  int orderSum = 0;
  BoardType receivedBoard;
  while(running) {
    if(transportState == TransportState::WAITING) {
      if(orderReady == false) {
        generateOrder();
        orderSum = longBoards + normalBoards + shortBoards;
        orderReady = true;
        cv.notify_one();
      }
      shortBoards = resources->requestAllShortBoards();
      normalBoards = resources->requestAllNormalBoards();
      longBoards = resources->requestAllLongBoards();
      // FIXME AKTUALNIE ^ TE FUNKCJE NIE PATRZA ILE JEST W ORDERZE TYLKO WSZYSTKO DAJA .
      while(orderedLongBoards > longBoards || orderedNormalBoards > normalBoards || orderedShortBoards > shortBoards) {
        receivedBoard = resources->requestAnyBoard(orderedLongBoards - longBoards, orderedNormalBoards - normalBoards, orderedShortBoards - shortBoards);
        if(receivedBoard == BoardType::SHORT) {
          shortBoards += 1;
        }
        else if(receivedBoard == BoardType::NORMAL) {
          normalBoards += 1;
        }
        else if(receivedBoard == BoardType::LONG) {
          longBoards += 1;
        }
      }
      shortBoards = longBoards = normalBoards = 0;
      transportCounter++;
      transportState = TransportState::TO_SHOP;
      orderReady=false;
    }
    if(transportState == TransportState::TO_SHOP) {
      while(progress <= 100.0f) {
        progress+=1;
        usleep(200000); //20sek
      }
      transportState = TransportState::FROM_SHOP;
    }
    progress=0.0f;
    if(transportState == TransportState::FROM_SHOP){
      while(progress <= 100.0f) {
        progress+=1;
        usleep(200000); //20sek
      }
      transportState = TransportState::WAITING;
    }
    progress=0.0f;
  }
}
BoardType Transport::getTopBoardPriority() {
  BoardType priorityBoard;

  if(orderedLongBoards >= orderedNormalBoards) {
    if(longBoards>0) {
      priorityBoard = (BoardType)2;
    } else if(normalBoards > 0) {
      priorityBoard = (BoardType)1;
    } else if (shortBoards > 0) {
      priorityBoard = (BoardType)0;
    }
  } else if (orderedNormalBoards >= orderedShortBoards){ // no need for long boards atm
    if(normalBoards>0) {
      priorityBoard = (BoardType)1;
    } else if(shortBoards > 0) {
      priorityBoard = (BoardType)0;
    } else if (longBoards > 0) {
      priorityBoard = (BoardType)2;
    }
  } else { // no need for long or normal atm
    if(shortBoards>0) {
      priorityBoard = (BoardType)0;
    } else if(normalBoards > 0) {
      priorityBoard = (BoardType)1;
    } else if (longBoards > 0) {
      priorityBoard = (BoardType)2;
    }
  }
  // FIXME dodac tutaj else statement ze stanem NO-PRIORITY~ zeby pokazac to w UI
  return priorityBoard;
}

int Transport::getShortBoards() {
  return shortBoards;
}

int Transport::getNormalBoards(){
  return normalBoards;
}

int Transport::getLongBoards() {
  return longBoards;
}

int Transport::getOrderedShortBoards() {
  return orderedShortBoards;
}

int Transport::getOrderedNormalBoards(){
  return orderedNormalBoards;
}

int Transport::getOrderedLongBoards() {
  return orderedLongBoards;
}
std::vector<int> Transport::getOrder() {
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return orderReady ? true : false;});
  std::vector<int> order = {orderedShortBoards, orderedNormalBoards, orderedLongBoards};
  return order;
}

std::string Transport::getState() {
  switch(transportState) {
    case TransportState::WAITING:
    return "WAITING";
    case TransportState::FROM_SHOP:
    return "FROM_SHOP";
    case TransportState::TO_SHOP:
    return "TO_SHOP";
    default:
    return "UNKNOWN";
  }
}

void Transport::generateOrder() {
  orderedLongBoards = rand() % RAND_RANGE + 1;
  orderedNormalBoards = rand() % RAND_RANGE + 1;
  orderedShortBoards = rand() % RAND_RANGE + 1;
  orderReady = true;
}

int Transport::getTransportCounter() {
  return transportCounter;
}

float Transport::getProgress() {
  return progress;
}
