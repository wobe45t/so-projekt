#include "UI.h"
#define SAWMILLS 3
#define BORDER_RIGHT 118
#define VERTICAL_SPLIT 80
#define HORIZONTAL_SPLIT (int)trees.size() + 3
#define BORDER_BOTTOM (int)trees.size() + 20

UI::UI(Manager *manager) : manager(manager)
{
  trees = manager->getTrees();
  lumberjacks = manager->getLumberjacks();
  nature = manager->getNature();
  resources = manager->getResources();
  transport = manager->getTransport();
  sawmillManager = manager->getSawmillManager();
}

UI::~UI()
{
  endwin();
}
void UI::update()
{
  while (running)
  {
    counter += 1;
    erase();
    //
    // TREES
    //
    mvprintw(0, 17, "TREES");
    attron(COLOR_PAIR(4));
    mvprintw(2, 2, "ID");
    mvprintw(2, 6, "CUT");
    mvprintw(2, 12, "STATE");
    mvprintw(2, 22, "LUMBER");
    mvprintw(2, 40, "PROGRESS");
    attroff(COLOR_PAIR(4));

    for (int i = 0; i < (int)trees.size(); i++)
    {
      std::stringstream stream;
      stream << std::fixed << std::setprecision(2) << trees[i]->getCutSize();
      std::string cutSize = stream.str();
      mvprintw(i + 3, 2, std::to_string(trees[i]->getId()).c_str());
      mvprintw(i + 3, 6, cutSize.c_str());
      mvprintw(i + 3, 22, std::to_string(trees[i]->getCuttingLumberjacks()).c_str());
      // mvprintw(i + 3, 27, trees[i]->getDone() ? "TRUE" : "FALSE");
      float progressBarValue = 0;
      if (trees[i]->state == TreeState::CUTTING)
      {
        progressBarValue = trees[i]->getCutProgress();
        attron(COLOR_PAIR(2));
      }
      else if (trees[i]->state == TreeState::GROWING)
      {
        progressBarValue = trees[i]->getGrowth();
        attron(COLOR_PAIR(3));
      }
      else
      {
        progressBarValue = 0.0f;
      }
      mvprintw(i + 3, 2, std::to_string(trees[i]->getId()).c_str());
      mvprintw(i + 3, 12, trees[i]->getState().c_str());
      mvprintw(i + 3, 30, progressBar(progressBarValue, VERTICAL_SPLIT - 42).c_str());
      if((int)progressBarValue == 100) {
        mvprintw(i + 3, VERTICAL_SPLIT - 11, "100");
      }
      else {
        mvprintw(i + 3, VERTICAL_SPLIT - 10, (std::to_string((int)progressBarValue)).c_str());
      }
      mvprintw(i + 3, VERTICAL_SPLIT - 8, " / 100");
      if (trees[i]->state == TreeState::CUTTING)
      {
        attroff(COLOR_PAIR(2));
      }
      else if (trees[i]->state == TreeState::GROWING)
      {
        attroff(COLOR_PAIR(3));
      }
      
    }
    // Print vertical spacer between TREES and LUMBERJACKS
    for (int i = 0; i < BORDER_BOTTOM; i++) {
      mvprintw(i, VERTICAL_SPLIT, "|");
    }

    // print vertical space between RESOURCES and right table
    for(int i=0; i < BORDER_BOTTOM; i++) {
      mvprintw(i, BORDER_RIGHT, "|");
    }
    // Print horizontal bottom border
    for(int i=0; i<BORDER_RIGHT; i++) {
      mvprintw(BORDER_BOTTOM, i, "=");
    }
    for(int i=0; i<BORDER_RIGHT; i++) {
      mvprintw(1, i, "=");
    }
    // Print horizontal spacer below TREES and above SAWMILLS
    for(int i=0; i<BORDER_RIGHT; i++) {
      mvprintw(HORIZONTAL_SPLIT, i, "=");
    }
    //
    //  LUMBERJACKS
    //
    mvprintw(0, VERTICAL_SPLIT + 8, "LUMBERJACKS");
    attron(COLOR_PAIR(1));
    mvprintw(2, VERTICAL_SPLIT + 2, "ID");
    mvprintw(2, VERTICAL_SPLIT + 7, "STATE");
    mvprintw(2,VERTICAL_SPLIT + 15, "TREE_ID");
    mvprintw(2,VERTICAL_SPLIT + 25, "CUT_TREES");
    mvprintw(2,VERTICAL_SPLIT + 32, "DONE");
    attroff(COLOR_PAIR(1));
    for (int i = 0; i < (int)lumberjacks.size(); i++)
    {
      mvprintw(i + 3, VERTICAL_SPLIT + 2, std::to_string(lumberjacks[i]->getId()).c_str());
      mvprintw(i + 3, VERTICAL_SPLIT + 7, lumberjacks[i]->getState().c_str());
      mvprintw(i + 3, VERTICAL_SPLIT + 17, std::to_string(lumberjacks[i]->getTreeId()).c_str());
      mvprintw(i + 3, VERTICAL_SPLIT + 27, std::to_string(lumberjacks[i]->getCutTreeCounter()).c_str());
      // mvprintw(i + 3, VERTICAL_SPLIT + 33, lumberjacks[i]->getDone() ? "TRUE" : "FALSE");
    }
    //
    // DONE
    //
    // mvprintw(HORIZONTAL_SPLIT + 9, VERTICAL_SPLIT + 15, ("SAWMILLMANAGER " + std::string(sawmillManager->getDone() ? "TRUE" : "FALSE")).c_str());
    // mvprintw(HORIZONTAL_SPLIT + 10, VERTICAL_SPLIT + 15, ("NATURE " + std::string(nature->getDone() ? "TRUE" : "FALSE")).c_str());
    // mvprintw(HORIZONTAL_SPLIT + 11, VERTICAL_SPLIT + 15, ("TRANSPORT " + std::string(transport->getDone() ? "TRUE" : "FALSE")).c_str());
    // for(int i=0;i<3; i++) {
    //   mvprintw(HORIZONTAL_SPLIT + 12 + i, VERTICAL_SPLIT + 15, ("SAWMILL "+std::to_string(i) + " " + std::string(sawmillManager->getSawmillDone(i) ? "TRUE" : "FALSE")).c_str());
    // }

    //
    // RESOURCES
    //
    mvprintw(HORIZONTAL_SPLIT + 1, VERTICAL_SPLIT + 2, "RESOURCES");
    attron(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 2, VERTICAL_SPLIT + 2, "NAME");
    mvprintw(HORIZONTAL_SPLIT + 2, VERTICAL_SPLIT + 10, "STORAGE");
    attroff(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 3, VERTICAL_SPLIT + 2, "WOOD");
    mvprintw(HORIZONTAL_SPLIT + 3, VERTICAL_SPLIT + 13, std::to_string(resources->getWood()).c_str());
    attron(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 5, VERTICAL_SPLIT + 2, "BOARDS");
    if(sawmillManager->getOrderRdy()) {
      mvprintw(HORIZONTAL_SPLIT + 5, VERTICAL_SPLIT + 10, "STORAGE");
      mvprintw(HORIZONTAL_SPLIT + 5, VERTICAL_SPLIT + 19, "PREP");
      mvprintw(HORIZONTAL_SPLIT + 5, VERTICAL_SPLIT + 24, "/");
      mvprintw(HORIZONTAL_SPLIT + 5, VERTICAL_SPLIT + 26, "ORDER");
      mvprintw(HORIZONTAL_SPLIT + 5, VERTICAL_SPLIT + 33, "NEED");
    }

    attroff(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 8, VERTICAL_SPLIT + 2, "-LONG");
    mvprintw(HORIZONTAL_SPLIT + 8, VERTICAL_SPLIT + 13, std::to_string(resources->getLongBoards()).c_str());
    if(sawmillManager->getOrderRdy()) {
      mvprintw(HORIZONTAL_SPLIT + 8, VERTICAL_SPLIT + 22, std::to_string(sawmillManager->getPreparedLongBoards()).c_str());
      mvprintw(HORIZONTAL_SPLIT + 8, VERTICAL_SPLIT + 23, " / ");
      mvprintw(HORIZONTAL_SPLIT + 8, VERTICAL_SPLIT + 26, std::to_string(sawmillManager->getOrderedLongBoards()).c_str());
      mvprintw(HORIZONTAL_SPLIT + 8, VERTICAL_SPLIT + 33, std::string(sawmillManager->getLongBoardsNeeded() ? "YES": "NO").c_str());
    }
    mvprintw(HORIZONTAL_SPLIT + 7, VERTICAL_SPLIT + 2, "-NORMAL");
    mvprintw(HORIZONTAL_SPLIT + 7, VERTICAL_SPLIT + 13, std::to_string(resources->getNormalBoards()).c_str());
    if(sawmillManager->getOrderRdy()) {
      mvprintw(HORIZONTAL_SPLIT + 7, VERTICAL_SPLIT + 22, std::to_string(sawmillManager->getPreparedNormalBoards()).c_str());
      mvprintw(HORIZONTAL_SPLIT + 7, VERTICAL_SPLIT + 23, " / ");
      mvprintw(HORIZONTAL_SPLIT + 7, VERTICAL_SPLIT + 26, std::to_string(sawmillManager->getOrderedNormalBoards()).c_str());
      mvprintw(HORIZONTAL_SPLIT + 7, VERTICAL_SPLIT + 33, std::string(sawmillManager->getNormalBoardsNeeded() ? "YES": "NO").c_str());
    }
    mvprintw(HORIZONTAL_SPLIT + 6, VERTICAL_SPLIT + 2, "-SHORT");
    mvprintw(HORIZONTAL_SPLIT + 6, VERTICAL_SPLIT + 13, std::to_string(resources->getShortBoards()).c_str());
    if(sawmillManager->getOrderRdy()) {
      mvprintw(HORIZONTAL_SPLIT + 6, VERTICAL_SPLIT + 22, std::to_string(sawmillManager->getPreparedShortBoards()).c_str());
      mvprintw(HORIZONTAL_SPLIT + 6, VERTICAL_SPLIT + 23, " / ");
      mvprintw(HORIZONTAL_SPLIT + 6, VERTICAL_SPLIT + 26, std::to_string(sawmillManager->getOrderedShortBoards()).c_str());
      mvprintw(HORIZONTAL_SPLIT + 6, VERTICAL_SPLIT + 33, std::string(sawmillManager->getShortBoardsNeeded() ? "YES": "NO").c_str());
    }

    //
    // SAWMILLS
    //
    mvprintw(HORIZONTAL_SPLIT+1, 6, "SAWMILLS");
    attron(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT+2, 2, "BOARD");
    mvprintw(HORIZONTAL_SPLIT+2, 10, "STATE");
    mvprintw(HORIZONTAL_SPLIT+2, 19, "SPEED");
    mvprintw(HORIZONTAL_SPLIT+2, 39, "BORDER PROGRESS");
    attroff(COLOR_PAIR(1));
    for(int i = 0; i < 3; i++) {
      mvprintw(HORIZONTAL_SPLIT+3+i, 2, (sawmillManager->getSawmillBoardTypeStr(i)).c_str());
      mvprintw(HORIZONTAL_SPLIT+3+i, 10, sawmillManager->getSawmillStateStr(i).c_str());
      mvprintw(HORIZONTAL_SPLIT+3+i, 19, sawmillManager->getSawmillSpeedStateStr(i).c_str());
      attron(COLOR_PAIR(2));
      mvprintw(HORIZONTAL_SPLIT+3+i, 30, progressBar(sawmillManager->getSawmillProgress(i), VERTICAL_SPLIT - 42).c_str());
      if((int)sawmillManager->getSawmillProgress(i) == 100) {
        mvprintw(HORIZONTAL_SPLIT+3+i, VERTICAL_SPLIT - 11, "100");
      }
      else {
        mvprintw(HORIZONTAL_SPLIT+3+i, VERTICAL_SPLIT - 10, (std::to_string((int)sawmillManager->getSawmillProgress(i))).c_str());
      }
      mvprintw(HORIZONTAL_SPLIT+3+i, VERTICAL_SPLIT - 8, " / 100");
      attroff(COLOR_PAIR(2));
    }
    //
    // WEATHER
    //
    attron(COLOR_PAIR(1));
    mvprintw(BORDER_BOTTOM - 2, 2, "WEATHER:");
    attroff(COLOR_PAIR(1));
    mvprintw(BORDER_BOTTOM - 2, 11, (nature->getConditions()).c_str());
    mvprintw(BORDER_BOTTOM - 2, 19, "CHANGE IN:");
    attron(COLOR_PAIR(2));
    mvprintw(BORDER_BOTTOM - 2, 30, progressBar((int)nature->getChangeProgress(), VERTICAL_SPLIT - 42).c_str());
    mvprintw(BORDER_BOTTOM - 2, VERTICAL_SPLIT - 10, std::to_string((int)nature->getChangeProgress()).c_str());
    mvprintw(BORDER_BOTTOM - 2, VERTICAL_SPLIT - 8, " / 100");
    attroff(COLOR_PAIR(2));
    //
    // TRANSPORT
    //
    mvprintw(HORIZONTAL_SPLIT + 7, 6, "TRANSPORT");
    attron(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 8, 2, "COUNT");
    mvprintw(HORIZONTAL_SPLIT + 8, 10, "STATE");
    if(transport->getState() == TransportState::WAITING) {
      mvprintw(HORIZONTAL_SPLIT + 8, 39, "ORDER PROGRESS");
    }
    attroff(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 9, 4, std::to_string(transport->getTransportCounter()).c_str());
    mvprintw(HORIZONTAL_SPLIT + 9, 10, transport->getStateStr().c_str());
    if(transport->getState() == TransportState::WAITING) {
      attron(COLOR_PAIR(2));
      mvprintw(HORIZONTAL_SPLIT + 10, 5 + (int)(transport->getProgress()*0.6), ".-------.___");
      mvprintw(HORIZONTAL_SPLIT + 11, 5 + (int)(transport->getProgress()*0.6), "| ||||| |[_o\\");
      mvprintw(HORIZONTAL_SPLIT + 12, 5 + (int)(transport->getProgress()*0.6), "| ^^^^^ |- ` )");
      mvprintw(HORIZONTAL_SPLIT + 13, 5 + (int)(transport->getProgress()*0.6), "'-()------()-");
      attroff(COLOR_PAIR(2));
      attron(COLOR_PAIR(1));
      mvprintw(HORIZONTAL_SPLIT + 9, 30, progressBar(sawmillManager->getOrderProgress(), VERTICAL_SPLIT - 42).c_str());
      mvprintw(HORIZONTAL_SPLIT + 9, VERTICAL_SPLIT - 10, (std::to_string((int)sawmillManager->getPreparedSum())).c_str());
      mvprintw(HORIZONTAL_SPLIT + 9, VERTICAL_SPLIT - 8, " / ");
      mvprintw(HORIZONTAL_SPLIT + 9, VERTICAL_SPLIT - 5, std::to_string(sawmillManager->getOrderSum()).c_str());
      attroff(COLOR_PAIR(1));
    }
    else {
      attron(COLOR_PAIR(2));
      if(transport->getState() == TransportState::TO_SHOP) {
        mvprintw(HORIZONTAL_SPLIT + 10, 5 + (int)(transport->getProgress()*0.6), ".-------.___");
        mvprintw(HORIZONTAL_SPLIT + 11, 5 + (int)(transport->getProgress()*0.6), "| ||||| |[_o\\");
        mvprintw(HORIZONTAL_SPLIT + 12, 5 + (int)(transport->getProgress()*0.6), "| ^^^^^ |- ` )");
        mvprintw(HORIZONTAL_SPLIT + 13, 5 + (int)(transport->getProgress()*0.6), "'-()------()-");
      }
      else {
        mvprintw(HORIZONTAL_SPLIT + 10, 65 - (int)(transport->getProgress()*0.6), "  ___.-------.");
        mvprintw(HORIZONTAL_SPLIT + 11, 65 - (int)(transport->getProgress()*0.6), " /o_]| ||||| |");
        mvprintw(HORIZONTAL_SPLIT + 12, 65 - (int)(transport->getProgress()*0.6),"( ' -| ^^^^^ |");
        mvprintw(HORIZONTAL_SPLIT + 13, 65 - (int)(transport->getProgress()*0.6)," -()------()-'");
      }
      mvprintw(HORIZONTAL_SPLIT + 9, VERTICAL_SPLIT - 10, (std::to_string((int)transport->getProgress())).c_str());
      mvprintw(HORIZONTAL_SPLIT + 9, VERTICAL_SPLIT - 8, " / 100");
      attroff(COLOR_PAIR(2));
    }
    mvprintw(HORIZONTAL_SPLIT + 12, VERTICAL_SPLIT + 2, ("FRAME="+std::to_string(counter)).c_str());

    attron(A_NORMAL);
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
}

void UI::init()
{
  initscr();
  start_color();
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
  raw();
  keypad(stdscr, TRUE);

  refresh();

  refreshThread = std::thread(&UI::update, this);

  int ch;
  while ((ch = getch()) != 'q')
  {
    if (ch == 'q')
    {
      break;
    }
  }
  mvprintw(BORDER_BOTTOM + 2, 2, "FINISHING THREADS ... ");
  refresh();
  this->running = false;
  manager->exit();
  refreshThread.join();
  endwin();
}

std::string UI::progressBar(float progress, int barLength)
{
  std::string finString = "";
  std::string progressInPercent = std::to_string((int)std::round(progress));
  int lpad = std::round(progress / 100 * barLength);
  int rpad = barLength - lpad;
  for (int i = 0; i < lpad; i++)
  {
    finString += "#";
  }
  for (int i = 0; i < rpad; i++)
  {
    finString += " ";
  }

  return finString;
}
