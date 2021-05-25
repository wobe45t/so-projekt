#include <iostream>

using namespace std;

int main() {
    int counter = 0;
    Thread1 thread1 = new Thread1();
    Thread2 thread2 = new Thread2();

    while(counter < 100) {
        counter ++;
    }

    return 0;
}