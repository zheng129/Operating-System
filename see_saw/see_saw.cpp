// Yixing Zheng
// zheng129@mail.chapman.edu
// Apr 15, 2019
// Project2

#include <iostream>
#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

using namespace std;

double fredHeight = 1;
double wilmaHeight = 7;

void *fredSee(void *param);
void *wilmaSaw(void *param);
void signal();
void wait();
bool try_wait();

mutex mutex_;
condition_variable condition_;
unsigned long count_ = 0; // Initialized as locked.

void signal() {
    lock_guard<decltype(mutex_)> lock(mutex_);
    ++count_;
    condition_.notify_one();
}

void wait() {
    unique_lock<decltype(mutex_)> lock(mutex_);
    while(!count_) // Handle spurious wake-ups.
        condition_.wait(lock);
    --count_;
}

bool try_wait() {
    lock_guard<decltype(mutex_)> lock(mutex_);
    if(count_) {
        --count_;
        return true;
    }
    return false;
}

void *fredSee(void *param) {

    try_wait();
    cout << "Fred going up..." << endl;

    while (fredHeight < 7) {
        fredHeight++;
        wilmaHeight--;
        cout << "Fred's Height: " << fredHeight << endl;
        cout << "Wilma's Height: " << wilmaHeight << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    cout << "Fred going down..." << endl;
    cout << endl;

    signal();
    pthread_exit(NULL);
}

void *wilmaSaw(void *param) {

    wait();
    cout << "Wilma going up..." << endl;

    while (fredHeight > 1) {
        wilmaHeight = wilmaHeight + 1.5;
        fredHeight = fredHeight - 1.5;

        cout << "Fred's Height: " << fredHeight << endl;
        cout << "Wilma's Height: " << wilmaHeight << endl;

        this_thread::sleep_for(chrono::milliseconds(300));
    }

    cout << "Wilma going down..." << endl;
    cout << endl;

    signal();
    pthread_exit(NULL);
}

int main() {

    pthread_t thread1;
    pthread_t thread2;

    for (int i = 0; i < 10; ++i) {
        cout << "Round: " << i + 1 << endl;

        pthread_create(&thread1, NULL, fredSee, NULL);
        pthread_create(&thread2, NULL, wilmaSaw, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    return 0;
}
