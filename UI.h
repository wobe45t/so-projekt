#pragma once
#include <array>
#include <thread>
#include <ncurses.h>
#include <mutex>
#include "Lumberjack.h"
#include "Manager.h"
#include "Tree.h"
#include "Forester.h"
#include "Nature.h"
#include "Resources.h"
#include "Sawmill.h"
#include "SawmillManager.h"
#include "Transport.h"
#include <vector>
#include <sstream>
#include <iomanip>

class UI
{
public:
    UI(Manager * manager);
    ~UI();
    void update();
    void init();
    void finish();
    std::string progressBar(float progress, int barLength);
private:

    std::thread refreshThread;
    std::thread keyboardThread;
    std::mutex mtx;
    std::vector<Tree*> trees;
    std::vector<Lumberjack*> lumberjacks;
    bool running = true;
    Nature * nature;
    Manager * manager;
    Sawmill * sawmill;
    SawmillManager * sawmillManager;
    Resources * resources;
    Transport * transport;
    int counter = {0};

};
