#include "Tree.h"
#include "Lumberjack.h"
#include "Manager.h"
#include "UI.h"
#include <vector>
#include <mutex>
#include <iostream>
using namespace std;
int main() {
  
  mutex global_mutex;
  Manager *manager = new Manager();
  // cout<<"Manager running "<< manager->getRunning()<<endl;
  // manager->test();
  UI *ui = new UI(manager);
  ui->init();
  return 0;
}