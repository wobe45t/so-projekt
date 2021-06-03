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
    BoardType priorityBoard;
    int priorityChangeTime = 100000;
    int priorityChangeProgress = 0;
    while(priorityChangeProgress < 100) {
      usleep(priorityChangeTime/100);
      priorityChangeProgress += 1;
    }
    priorityBoard = transport->getTopBoardPriority();
    for(int i=0; i<(int)sawmills.size(); i++) {
      sawmills[i]->requestBoard();
      if(sawmills[i]->getBoardType() == priorityBoard) {
        sawmills[i]->setPriority(true);
      }
      else {
        sawmills[i]->setPriority(false);
      }
    }
    // zamiast odwolywac mutexy itd trzeba pozwolic na to zeby wzial te resources ale sciagnac mu priority ze stanu
    // a wiec niech stan bedzie atomiciem i niech bedzie settowany osobno
    // a wiec flow bedzie taki ze tartak lockuje resources ale w trakcie okazuje sie ze juz prio nie potrzebuje
    // wiec sciagamy mu prio i dajemy innemu. Dopiero w tym momencie tartak rozpoczyna zlockowana prace bo dostal resources
    // ale robi to bez prio.
    // PYTANIE jest teraz czy ja powininienm to przerabiac bo aktulnie dziala to identycznie. Teraz bedzie tylko dodatek zwiazany z prio.
    //
    // sawmill manager moglby czekac na zmiane SUMY desek z TRANSPORTU. Jezeli deski sie zmienia to sawmill manager dostanie notify
    // i wywola funkcje ktora ustawi tartakom prio odpowiednie.
    // czyli w ten sposob zrobie tak ze watek
    // sawmill manager ma tylko 2 akcje w petli while :: -> wait for notification from TRANSPORT about change in boards count -> set prio on sawmills Accordingly
    //
    // dzialanie tartakow powinno byc odpowiednie -> maja cos zrequestowane ale nagle zmienia sie prio to spoko, robia to bez prio co mialy reqestowane
    // a ten co ma prio (nawet jak bedzie jeszcze czekal dalej) to zostaje z prio i czeka na swoje resources.
    // pytanie czy teraz jak ten co mial zrequestowane i zrzucil swoje prio nie wezmie tego spowrotem i nie bedzie to prio za szybko przechodzic
    // ... zeby ^ rozwiacac to mozna zamiast roznica w deskach 1 dac roznica w deskach 2 -> wtedy nie beda sie mieniac co jeden obrot co bylo by glupie.
    // nawet mozna by bylo dac tego jeszcze wiecej.
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
