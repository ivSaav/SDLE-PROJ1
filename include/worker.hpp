#ifndef WORKER_H
#define WORKER_H
#include <thread>

using namespace std;

class Worker {
  private:
    thread t;

  public:
    Worker() {}

    void run();
    void join();
};

#endif /* ifndef WORKER_H */
