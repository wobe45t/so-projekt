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
    attron(COLOR_PAIR(4));
    mvprintw(0, 2, "ID");
    mvprintw(0, 6, "CUT");
    mvprintw(0, 12, "STATE");
    mvprintw(0, 22, "LUMBER");
    mvprintw(0, 31, "PROGRESS");
    attroff(COLOR_PAIR(4));
    for (int i = 0; i < trees.size(); i++)
    {
    
      std::stringstream stream;
      stream << std::fixed << std::setprecision(2) << trees[i]->getCutSize();
      std::string cutSize = stream.str();
      mvprintw(i + 1, 2, std::to_string(trees[i]->getId()).c_str());
      mvprintw(i + 1, 6, cutSize.c_str());
      
      mvprintw(i + 1, 22, std::to_string(trees[i]->getCuttingLumberjacks()).c_str());
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
      mvprintw(i + 1, 12, trees[i]->getState().c_str());
      mvprintw(i + 1, 31, progressBar(progressBarValue, 10).c_str());
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
    for (int i = 0; i < trees.size() + 1; i++)
    {
      mvprintw(i, breakRow, "|");
    }

    //! PRINTING LUMBERJACKS
    attron(COLOR_PAIR(1));
    mvprintw(0, 44, "ID");
    mvprintw(0, 50, "STATE");
    mvprintw(0, 60, "TREE_ID");
    attroff(COLOR_PAIR(1));
    for (int i = 0; i < lumberjacks.size(); i++)
    {

      mvprintw(i + 1, 44, std::to_string(lumberjacks[i]->getId()).c_str());
      mvprintw(i + 1, 50, lumberjacks[i]->getState().c_str());
      mvprintw(i + 1, 60, std::to_string(lumberjacks[i]->getTreeId()).c_str());
    }

    for(int i = 0; i < sawmills.size(); i++) {
      mvprintw(6+i, 44, (sawmills[i]->getBoardType()+" BOARD SAWMILL").c_str());
      mvprintw(6+i, 65, sawmills[i]->getState().c_str());
    }

    mvprintw(1, 100, ("FRAME="+std::to_string(counter)).c_str());
    mvprintw(3, 90, (nature->getConditions()+" WEATHER CONDITIONS").c_str());
    mvprintw(5, 90, ("WOOD = "+std::to_string(resources->getWood())).c_str());
    mvprintw(6, 90, ("LONG BOARD = "+std::to_string(resources->getLongBoards())).c_str());
    mvprintw(7, 90, ("NORMAL BOARD = "+std::to_string(resources->getNormalBoards())).c_str());
    mvprintw(8, 90, ("SHORT BOARD = "+std::to_string(resources->getShortBoards())).c_str());
    mvprintw(9, 90, ("MSG = "+resources->getMessage()).c_str());
    mvprintw(10, 90, ("TRANSPORT = "+transport->getState()).c_str());
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