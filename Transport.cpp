#include "Transport.h"
#include <unistd.h>

#define RAND_RANGE 10

Transport::Transport(Resources * resources) : resources(resources), td(&Transport::cycle, this)
{

}

void Transport::cycle() {
  while(running) {
    if(transportState == TransportState::WAITING) {
      if(orderReady == false) {
        generateOrder();
        orderReady = true;
        cv.notify_one();    // powiadomienie SawmillManager o gotowym zamowieniu
      }
      resources->requestBoards(shortBoards, normalBoards, longBoards);
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

std::vector<int> Transport::getOrder() {
  std::unique_lock<std::mutex> ul(mtx);
  cv.wait(ul, [&] {return orderReady ? true : false;});
  std::vector<int> order = {shortBoards, normalBoards, longBoards};
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
  longBoards = rand() % RAND_RANGE + 1;
  normalBoards = rand() % RAND_RANGE + 1;
  shortBoards = rand() % RAND_RANGE + 1;
  orderReady = true;
}

int Transport::getTransportCounter() {
  return transportCounter;
}

float Transport::getProgress() {
  return progress;
}