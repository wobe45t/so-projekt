#include "Transport.h"
#include <unistd.h>

#define LONG 20
#define NORMAL 15
#define SHORT 18

Transport::Transport(SawmillManager * sawmillManager) : sawmillManager(sawmillManager), td(&Transport::cycle, this)
{

}

void Transport::cycle() {
  while(running) {
    if(transportState == TransportState::WAITING) {
      sawmillManager->getResources(SHORT, NORMAL, LONG);
      transportState = TransportState::TO_SHOP;
    }
    if(transportState == TransportState::TO_SHOP) {
      usleep(40000000); //20sek
      transportState = TransportState::FROM_SHOP;
    }
    if(transportState == TransportState::FROM_SHOP){
      usleep(40000000); //20sek
      transportState = TransportState::WAITING;
    }
  }
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