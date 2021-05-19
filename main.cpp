#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <mutex>
#include <random>
#include <thread>
#include <ncurses.h>
#include "Tree.h"
#include "Lumberjack.h"
using namespace std;
using namespace std::chrono_literals;
#include <iostream>

vector<Lumberjack *> lumberjacks;
vector<Tree *> trees;
mutex global_mutex;

void init()
{
  lumberjacks = vector<Lumberjack *>();
  trees = vector<Tree *>();

  for (int i = 0; i < 50; i++)
  {
    trees.push_back(new Tree(i));
  }
  for (int i = 0; i < 10; i++)
  {
    lumberjacks.push_back(new Lumberjack(i,trees));
  }
}
int main()
{
  
  initscr();
  start_color();
  noecho();
  keypad(stdscr, true);

  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  //! PROGRAM

  move(1,2);
  clrtoeol();
  refresh();

  init();

  global_mutex.lock();
  for(int i=0 ; i< 10; i++)
  {
    mvprintw(i*2,0,"Philosopher [%d]", i+1);
    mvprintw(i*2,20, "WAITING");
  }
  attron(COLOR_PAIR(1));
  mvprintw(10*2+2, 3, "Press q to finish the program");
  attroff(COLOR_PAIR(1));
	refresh(); 
  global_mutex.unlock();

  int ch;
  while((ch = getch()) != 'q'){
    if (ch == 'q'){
      break;
    }
  }
  global_mutex.lock();
  move(10*2+2,0);
  clrtoeol();
  attron(COLOR_PAIR(1));
  mvprintw(10*2+2, 3, "Finishing program, press any key to exit");
  attroff(COLOR_PAIR(1));
  refresh();
  global_mutex.unlock();


  getch();
  endwin();
  return 0;
}
