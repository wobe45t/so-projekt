#include "SawmillManager.h"
#include "BoardType.h"
#include "SawmillState.h"
SawmillManager::SawmillManager(Resources *resources, std::vector<Sawmill *> sawmills, Transport *transport) : resources(resources),
 sawmills(sawmills), transport(transport), td(&SawmillManager::cycle, this)
{
}

void SawmillManager::cycle()
{
  while (running)
  {
    message = "Waiting for order";
    std::vector<int> orderedBoards = transport->getOrder();
    while(resources->getShortBoards() <= orderedBoards[0] || resources->getNormalBoards() <= orderedBoards[1] || resources->getLongBoards() <= orderedBoards[2]){
      if(orderedBoards[2] >= orderedBoards[1]) {
        if(sawmills[2]->getState() == SawmillState::WAITING && orderedBoards[2]>0) {
          sawmills[2]->requestBoard();
        } else if(sawmills[1]->getState() == SawmillState::WAITING && orderedBoards[1] > 0) {
          sawmills[1]->requestBoard();
        } else if (sawmills[0]->getState()== SawmillState::WAITING && orderedBoards[0] > 0) {
          sawmills[0]->requestBoard();
        }
      } else if (orderedBoards[1] >= orderedBoards[0]){ // no need for long boards atm
        if(sawmills[1]->getState() == SawmillState::WAITING && orderedBoards[1]>0) {
          sawmills[1]->requestBoard();
        } else if(sawmills[0]->getState() == SawmillState::WAITING && orderedBoards[0] > 0) {
          sawmills[0]->requestBoard();
        } else if (sawmills[2]->getState() == SawmillState::WAITING && orderedBoards[2] > 0) {
          sawmills[2]->requestBoard();
        }
      } else { // no need for long or normal atm
        if(sawmills[0]->getState() == SawmillState::WAITING && orderedBoards[0]>0) {
          sawmills[0]->requestBoard();
        } else if(sawmills[1]->getState() == SawmillState::WAITING && orderedBoards[1] > 0) {
          sawmills[1]->requestBoard();
        } else if (sawmills[2]->getState() == SawmillState::WAITING && orderedBoards[2] > 0) {
          sawmills[2]->requestBoard();
        }
      }
      message = "Boards left: " + std::to_string(orderedBoards[0] - resources->getShortBoards()) + " " + std::to_string(orderedBoards[1] - resources->getNormalBoards()) + " " + std::to_string(orderedBoards[2] - resources->getLongBoards());
    }
  }
}

// Sawmill * SawmillManager::getSawmill(BoardType boardType) {
//   for(int i=0; i<sawmills.size(); i++) {
//     if(sawmills[i]->boardType == boardType) return sawmills[i];
//   }
//   return NULL;
// }

std::string SawmillManager::getMessage() {
  return message;
}

int SawmillManager::getLongBoards()
{
  return longBoard;
}

int SawmillManager::getShortBoards()
{
  return shortBoard;
}

int SawmillManager::getNormalBoards()
{
  return normalBoard;
}

Transport * SawmillManager::getTransport() {
  return transport;
}