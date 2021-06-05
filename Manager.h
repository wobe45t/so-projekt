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
  SawmillManager * getSawmillManager();
  Transport * getTransport();
private:
  std::vector<Tree *> trees;
  std::vector<Lumberjack *> lumberjacks;
  Nature * nature;
  Resources * resources;
  Transport * transport;
  SawmillManager * sawmillManager;
  bool running = true;
};
