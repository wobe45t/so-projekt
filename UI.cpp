#include "UI.h"

UI::UI(Manager *manager) : manager(manager)
{
  trees = manager->getTrees();
  lumberjacks = manager->getLumberjacks();
  nature = manager->getNature();
  resources = manager->getResources();
  //sawmill = manager->getSawmill();
  sawmills = manager->getSawmills();
  transport = manager->getTransport();
  sawmillManager = manager->getSawmillManager();
}

UI::~UI()
{
  endwin();
}

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
    mvprintw(2, 31, "PROGRESS");
    attroff(COLOR_PAIR(4));
    for (int i = 0; i < trees.size(); i++)
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
      mvprintw(i + 3, 12, trees[i]->getState().c_str());
      mvprintw(i + 3, 31, progressBar(progressBarValue, 10).c_str());
      if (trees[i]->state == TreeState::CUTTING)
      {
        attroff(COLOR_PAIR(2));
      }
      else if (trees[i]->state == TreeState::GROWING)
      {
        attroff(COLOR_PAIR(3));
      }
      
    }
    int breakRow = 42;
    for (int i = 0; i < trees.size() + 2; i++) {
      mvprintw(i, breakRow, "|");
    }

    // print horizontal spacer below lumberjack/resource table
    for(int i=44;i<96;i++) {
      mvprintw(lumberjacks.size()+3, i, "=");
    }
    // print vertical spacel between resources and right table
    for(int i=0; i<trees.size()+2; i++) {
      mvprintw(i, 96, "|");
    }
    // print horizontal bottom border
    for(int i=0; i<130; i++) {
      mvprintw(trees.size()+2, i, "=");
    }
    // print vertical spacer between lumberjacks and resource table
    for(int i=0; i< lumberjacks.size()+3; i++) {
      mvprintw(i, 70, "|");
    }
    for(int i=0; i<130; i++) {
      mvprintw(1, i, "=");
    }
    for(int i=0; i<trees.size()+2; i++) {
      mvprintw(i, 129, "|");
    }
    //! PRINTING LUMBERJACKS
    mvprintw(0, 50, "LUMBERJACKS");
    attron(COLOR_PAIR(1));
    mvprintw(2, 44, "ID");
    mvprintw(2, 50, "STATE");
    mvprintw(2, 60, "TREE_ID");
    attroff(COLOR_PAIR(1));
    for (int i = 0; i < lumberjacks.size(); i++)
    {

      mvprintw(i + 3, 44, std::to_string(lumberjacks[i]->getId()).c_str());
      mvprintw(i + 3, 50, lumberjacks[i]->getState().c_str());
      mvprintw(i + 3, 60, std::to_string(lumberjacks[i]->getTreeId()).c_str());
    }

    //! Printing resources 
    mvprintw(0, 79, "RESOURCES");
    attron(COLOR_PAIR(1));
    mvprintw(2, 72, "NAME");
    mvprintw(2, 83, "QUANTITY");
    attroff(COLOR_PAIR(1));
    mvprintw(3, 72, "WOOD");
    mvprintw(3, 83, std::to_string(resources->getWood()).c_str());
    attron(COLOR_PAIR(1));
    mvprintw(4, 75, "BOARDS");
    attroff(COLOR_PAIR(1));
    mvprintw(5, 72, "LONG");
    mvprintw(5, 83, std::to_string(resources->getLongBoards()).c_str());
    mvprintw(6, 72, "NORMAL");
    mvprintw(6, 83, std::to_string(resources->getNormalBoards()).c_str());
    mvprintw(7, 72, "SHORT");
    mvprintw(7, 83, std::to_string(resources->getShortBoards()).c_str());
    attron(COLOR_PAIR(1));
    mvprintw(9, 50, "SAWMILLS");
    attroff(COLOR_PAIR(1));
    for(int i = 0; i < sawmills.size(); i++) {
      mvprintw(10+i, 44, (sawmills[i]->getBoardTypeStr()).c_str());
      mvprintw(10+i, 65, sawmills[i]->getStateStr().c_str());
      mvprintw(10+i, 75, sawmills[i]->getWorkRequestStr().c_str());
      attron(COLOR_PAIR(2));
      mvprintw(10+i, 85, progressBar(sawmills[i]->getProgress(), 10).c_str());
      attroff(COLOR_PAIR(2));
    }
    mvprintw(0, 110, "OTHER");
    attron(COLOR_PAIR(1));
    mvprintw(2, 98, "WEATHER CONDITIONS");
    attroff(COLOR_PAIR(1));
    mvprintw(3, 100, (nature->getConditions()).c_str());
    attron(COLOR_PAIR(1));
    mvprintw(5, 98, "TRANSPORT");
    attroff(COLOR_PAIR(1));
    mvprintw(6, 100, transport->getState().c_str());
    attron(COLOR_PAIR(2));
    mvprintw(6, 110, progressBar(transport->getProgress(), 15).c_str());
    attroff(COLOR_PAIR(2));
    mvprintw(7, 100, ("COUNT "+ std::to_string(transport->getTransportCounter())).c_str());
    mvprintw(12, 98, ("MSG = "+sawmillManager->getMessage()).c_str());
    mvprintw(11, 98, ("FRAME="+std::to_string(counter)).c_str());
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