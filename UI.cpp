#include "UI.h"
#define SAWMILLS 3
#define BORDER_RIGHT 104
#define VERTICAL_SPLIT 72
#define HORIZONTAL_SPLIT (int)trees.size() + 3
#define BORDER_BOTTOM (int)trees.size() + 16

UI::UI(Manager *manager) : manager(manager)
{
  trees = manager->getTrees();
  lumberjacks = manager->getLumberjacks();
  nature = manager->getNature();
  resources = manager->getResources();
  sawmills = manager->getSawmills();
  transport = manager->getTransport();
  sawmillManager = manager->getSawmillManager();
}

UI::~UI()
{
  endwin();
}
//hello test.
void UI::update()
{
  while (manager->getRunning())
  {
    counter += 1;
    erase();
    //! PRINTING TREES
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
      mvprintw(i + 3, 31, progressBar(progressBarValue, 30).c_str());
      mvprintw(i + 3, 62, (std::to_string((int)progressBarValue)).c_str());
      mvprintw(i + 3, 64, " / 100");
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
    //! PRINTING LUMBERJACKS
    mvprintw(0, VERTICAL_SPLIT + 8, "LUMBERJACKS");
    attron(COLOR_PAIR(1));
    mvprintw(2, VERTICAL_SPLIT + 2, "ID");
    mvprintw(2, VERTICAL_SPLIT + 7, "STATE");
    mvprintw(2,VERTICAL_SPLIT + 15, "TREE_ID"); // END OF CURRENT TREE PROGRESS BAR @@
    mvprintw(2,VERTICAL_SPLIT + 23, "CUT_TREES"); // END OF CURRENT TREE PROGRESS BAR @@
    attroff(COLOR_PAIR(1));
    for (int i = 0; i < (int)lumberjacks.size(); i++)
    {
      mvprintw(i + 3, VERTICAL_SPLIT + 2, std::to_string(lumberjacks[i]->getId()).c_str());
      mvprintw(i + 3, VERTICAL_SPLIT + 7, lumberjacks[i]->getState().c_str());
      mvprintw(i + 3, VERTICAL_SPLIT + 17, std::to_string(lumberjacks[i]->getTreeId()).c_str());
      mvprintw(i + 3, VERTICAL_SPLIT + 25, std::to_string(lumberjacks[i]->getCutTreeCounter()).c_str());
    }

    //
    // RESOURCES
    //
    mvprintw((int)trees.size()+ 4, VERTICAL_SPLIT + 2, "RESOURCES");
    attron(COLOR_PAIR(1));
    mvprintw((int)trees.size() + 6, VERTICAL_SPLIT + 2, "NAME");
    mvprintw((int)trees.size() + 6, VERTICAL_SPLIT + 11, "QUANTITY");
    attroff(COLOR_PAIR(1));
    mvprintw((int)trees.size() + 7, VERTICAL_SPLIT + 2, "WOOD");
    mvprintw((int)trees.size() + 7, VERTICAL_SPLIT + 14, std::to_string(resources->getWood()).c_str());
    attron(COLOR_PAIR(1));
    mvprintw((int)trees.size() + 8, VERTICAL_SPLIT + 2, "BOARDS");
    mvprintw((int)trees.size() + 8, VERTICAL_SPLIT + 11, "STORAGE");
    mvprintw((int)trees.size() + 8, VERTICAL_SPLIT + 20, "GOT");
    mvprintw((int)trees.size() + 8, VERTICAL_SPLIT + 24, "/");
    mvprintw((int)trees.size() + 8, VERTICAL_SPLIT + 26, "ORDER");
    attroff(COLOR_PAIR(1));
    mvprintw((int)trees.size() + 11, VERTICAL_SPLIT + 2, "-LONG");
    mvprintw((int)trees.size() + 11, VERTICAL_SPLIT + 14, std::to_string(resources->getLongBoards()).c_str());
    mvprintw((int)trees.size() + 11, VERTICAL_SPLIT + 22, std::to_string(sawmillManager->getPreparedLongBoards()).c_str());
    mvprintw((int)trees.size() + 11, VERTICAL_SPLIT + 23, " / ");
    mvprintw((int)trees.size() + 11, VERTICAL_SPLIT + 26, std::to_string(sawmillManager->getOrderedLongBoards()).c_str());
    mvprintw((int)trees.size() + 11, VERTICAL_SPLIT + 29, std::string(sawmillManager->getLongBoardsNeeded() ? "T": "F").c_str());
    mvprintw((int)trees.size() + 10, VERTICAL_SPLIT + 2, "-NORMAL");
    mvprintw((int)trees.size() + 10, VERTICAL_SPLIT + 14, std::to_string(resources->getNormalBoards()).c_str());
    mvprintw((int)trees.size() + 10, VERTICAL_SPLIT + 22, std::to_string(sawmillManager->getPreparedNormalBoards()).c_str());
    mvprintw((int)trees.size() + 10, VERTICAL_SPLIT + 23, " / ");
    mvprintw((int)trees.size() + 10, VERTICAL_SPLIT + 26, std::to_string(sawmillManager->getOrderedNormalBoards()).c_str());
    mvprintw((int)trees.size() + 10, VERTICAL_SPLIT + 29, std::string(sawmillManager->getNormalBoardsNeeded() ? "T": "F").c_str());
    mvprintw((int)trees.size() + 9, VERTICAL_SPLIT + 2, "-SHORT");
    mvprintw((int)trees.size() + 9, VERTICAL_SPLIT + 14, std::to_string(resources->getShortBoards()).c_str());
    mvprintw((int)trees.size() + 9, VERTICAL_SPLIT + 22, std::to_string(sawmillManager->getPreparedShortBoards()).c_str());
    mvprintw((int)trees.size() + 9, VERTICAL_SPLIT + 23, " / ");
    mvprintw((int)trees.size() + 9, VERTICAL_SPLIT + 26, std::to_string(sawmillManager->getOrderedShortBoards()).c_str());
    mvprintw((int)trees.size() + 9, VERTICAL_SPLIT + 29, std::string(sawmillManager->getShortBoardsNeeded() ? "T": "F").c_str());

    //
    // SAWMILLS
    //
    attron(COLOR_PAIR(1));
    mvprintw((int)trees.size()+4, 10, "SAWMILLS");
    attroff(COLOR_PAIR(1));
    for(int i = 0; i < (int)sawmills.size(); i++) {
      mvprintw((int)trees.size()+5+i, 5, (sawmills[i]->getBoardTypeStr()).c_str());
      mvprintw((int)trees.size()+5+i, 13, sawmills[i]->getStateStr().c_str());
      mvprintw((int)trees.size()+5+i, 22, sawmills[i]->getSpeedStateStr().c_str());
      attron(COLOR_PAIR(2));
      mvprintw((int)trees.size()+5+i, 31, progressBar(sawmills[i]->getProgress(), 30).c_str());
      mvprintw((int)trees.size()+5+i, 62, (std::to_string((int)sawmills[i]->getProgress())).c_str());
      mvprintw((int)trees.size()+5+i, 64, " / 100");
      attroff(COLOR_PAIR(2));
    }
    //
    // TRANSPORT
    //
    attron(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 6, 10, "TRANSPORT");
    mvprintw(HORIZONTAL_SPLIT + 6, 20, std::to_string(transport->getShortBoards()).c_str());
    mvprintw(HORIZONTAL_SPLIT + 6, 25, std::to_string(transport->getNormalBoards()).c_str());
    mvprintw(HORIZONTAL_SPLIT + 6, 30, std::to_string(transport->getLongBoards()).c_str());
    attroff(COLOR_PAIR(1));
    mvprintw(HORIZONTAL_SPLIT + 7, 5, "COUNTER");
    mvprintw(HORIZONTAL_SPLIT + 7, 15, "STATE");
    mvprintw(HORIZONTAL_SPLIT + 7, 40, "PROGRESS");
    mvprintw(HORIZONTAL_SPLIT + 8, 5, std::to_string(transport->getTransportCounter()).c_str());
    mvprintw(HORIZONTAL_SPLIT + 8, 15, transport->getState().c_str());
    attron(COLOR_PAIR(2));
    mvprintw(HORIZONTAL_SPLIT + 8, 31, progressBar((int)transport->getProgress(), 30).c_str());
    mvprintw(HORIZONTAL_SPLIT + 8, 62, (std::to_string((int)transport->getProgress())).c_str());
    mvprintw(HORIZONTAL_SPLIT + 8, 64, " / 100");
    attroff(COLOR_PAIR(2));
    mvprintw(HORIZONTAL_SPLIT + 11, VERTICAL_SPLIT + 2, ("MSG = "+sawmillManager->getMessage()).c_str());
    mvprintw(HORIZONTAL_SPLIT + 12, VERTICAL_SPLIT + 2, ("FRAME="+std::to_string(counter)).c_str());
    //
    // WEATHER
    //
    attron(COLOR_PAIR(1));
    mvprintw((int)trees.size() + SAWMILLS + 11, 5, "WEATHER:");
    attroff(COLOR_PAIR(1));
    mvprintw((int)trees.size() + SAWMILLS + 11, 14, (nature->getConditions()).c_str());
    attron(COLOR_PAIR(2));
    mvprintw((int)trees.size() + SAWMILLS + 11, 31, progressBar((int)nature->getChangeProgress(), 30).c_str());
    mvprintw((int)trees.size() + SAWMILLS + 11, 62, std::to_string((int)nature->getChangeProgress()).c_str());
    mvprintw((int)trees.size() + SAWMILLS + 11, 64, " / 100");
    attroff(COLOR_PAIR(2));

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

  manager->setRunning(false);
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
