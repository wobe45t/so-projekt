#pragma once
#include <array>
#include <mutex>
#include <atomic>
#include <iostream>
#include "Tree.h"
#include "Lumberjack.h"
#include "Forester.h"
#include "Resources.h"
#include "Nature.h"
#include "Sawmill.h"
#include "SawmillManager.h"
#include "Transport.h"
#include <vector>
class Manager
{
public:

  Manager();
  ~Manager();
  const std::vector<Tree *> getTrees();
	const std::vector<Lumberjack *> getLumberjacks();
  bool getRunning();
  void setRunning(bool running);
  Nature* getNature();
  Resources* getResources();
  Sawmill* getSawmill();
  Transport * getTransport();
  std::vector<Sawmill*> getSawmills();
private:
  std::vector<Tree *> trees;
  std::vector<Lumberjack *> lumberjacks;
  std::vector<Sawmill *> sawmills;
  Nature * nature;
  Resources * resources;
  Sawmill * sawmill;
  SawmillManager * sawmillManager;
  Transport * transport;

  bool running = true;
};
