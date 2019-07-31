// Yixing Zheng
// 5/8/19
// CPSC 380
// zheng129@mail.chapman.edu

#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <semaphore.h>
#include <mutex>
#include <vector>

using namespace std;

// initialize count variables for when the scheduler passes or misses
int pass1 = 0;
int pass2 = 0;
int pass3 = 0;
int pass4 = 0;

int miss1 = 0;
int miss2 = 0;
int miss3 = 0;
int miss4 = 0;

// designing a 10x10 board
vector<vector<int>> board(10, vector<int> (10, 1));

// initializing and assinging semaphores and binary semaphores
sem_t sem1;
sem_t sem2;
sem_t sem3;
sem_t sem4;

sem_t bool_sem1;
sem_t bool_sem2;
sem_t bool_sem3;
sem_t bool_sem4;

// initializing mutex and assigning to a binary semaphore
mutex mtx1;
mutex mtx2;
mutex mtx3;
mutex mtx4;

mutex bool_mtx1;
mutex bool_mtx2;
mutex bool_mtx3;
mutex bool_mtx4;

// set boolean values to ensure that the thread is still working
bool isWorking1 = false;
bool isWorking2 = false;
bool isWorking3 = false;
bool isWorking4 = false;

// pointer functions to create a scheduler and various speeds
void *scheduler(void *);
void *veryfast(void *);
void *fast(void *);
void *medium(void *);
void *slow(void *);

// main function responsible for running the other functions
int main() {

	// initialize scheduled parameters and cpu affinity from sched.h
	struct sched_param parameters;

	cpu_set_t mask;
	int cpuAffinity = 1;

	CPU_ZERO(&mask);
	CPU_SET(cpuAffinity, &mask);

	// initiallizing binary semaphores
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);
	sem_init(&sem3, 0, 0);
	sem_init(&sem4, 0, 0);

	sem_init(&bool_sem1, 0, 1);
	sem_init(&bool_sem2, 0, 1);
	sem_init(&bool_sem3, 0, 1);
	sem_init(&bool_sem4, 0, 1);

	// lock and unlock functions for the mutex's
	mtx1.lock();
	mtx2.lock();
	mtx3.lock();
	mtx4.lock();

	bool_mtx1.unlock();
	bool_mtx2.unlock();
	bool_mtx3.unlock();
	bool_mtx4.unlock();

	// max priority function using fifo organization
	parameters.sched_priority = sched_get_priority_max(SCHED_FIFO);

	// start making threads, then create a switch statement that
	// indicate speeds ranging from very fast to slow. Once it's
	// been completed, the task will lower in priority, and continue
	// until fully completed.
	pthread_t myThreads[5];
	for(int i = 0; i < 5; ++i) {

		switch(i) {

			case 0:
				pthread_create(&myThreads[0], NULL, veryfast, NULL);
				pthread_setaffinity_np(myThreads[0], sizeof(mask), &mask);
				parameters.sched_priority--;
				pthread_setschedparam(myThreads[0], SCHED_FIFO, &parameters);
				break;

			case 1:
				pthread_create(&myThreads[1], NULL, fast, NULL);
				pthread_setaffinity_np(myThreads[1], sizeof(mask), &mask);
				parameters.sched_priority--;
				pthread_setschedparam(myThreads[1], SCHED_FIFO, &parameters);
				break;

			case 2:
				pthread_create(&myThreads[2], NULL, medium, NULL);
				pthread_setaffinity_np(myThreads[2], sizeof(mask), &mask);
				parameters.sched_priority--;
				pthread_setschedparam(myThreads[2], SCHED_FIFO, &parameters);
				break;

			case 3:
				pthread_create(&myThreads[3], NULL, slow, NULL);
				pthread_setaffinity_np(myThreads[3], sizeof(mask), &mask);
				parameters.sched_priority--;
				pthread_setschedparam(myThreads[3], SCHED_FIFO, &parameters);
				break;

			case 4:
				pthread_create(&myThreads[4], NULL, scheduler, NULL);
				pthread_setaffinity_np(myThreads[4], sizeof(mask), &mask);
				parameters.sched_priority--;
				pthread_setschedparam(myThreads[4], SCHED_FIFO, &parameters);
				break;
		}
	}

	// join threads once a process has been completed
	for(int i = 0; i < 5; ++i) {

		pthread_join(myThreads[i], NULL);
	}
	return 0;
}

// create scheduler method
void* scheduler(void *) {

	int counter = 0;
	while(counter < 10) {

		//major frame period limited to 16 units of time
		++counter;
		for(int i = 1; i < 17; ++i) {
			sem_wait(&bool_sem1);

			//thread 1 with a period of % 1
			if(isWorking1) {
				miss1++;
			}
			sem_post(&bool_sem1);
			sem_post(&sem1);

			//thread 2 with a period of % 2
			if(i % 2 == 0) {
				sem_wait(&bool_sem2);
				if(isWorking2) {
					miss2++;
				}
				sem_post(&bool_sem2);
				sem_post(&sem2);
			}

			//thread 3 with a period of % 4
			if(i % 4 == 0) {

				sem_wait(&bool_sem3);
				if(isWorking3) {
					miss3++;
				}
				sem_post(&bool_sem3);
				sem_post(&sem3);
			}

			//thread 4 with a period of % 16
			if(i % 16 == 0) {
				sem_wait(&bool_sem4);
				if(isWorking4) {
					miss4++;
				}
				sem_post(&bool_sem4);
				sem_post(&sem4);
			}
		usleep(10000);
		}
	}

	cout << "The VeryFast (T1) scheduler ran: " << pass1 << " times" << endl;
	cout << "The VeryFast (T1) scheduler missed: " << miss1 << " times\n" << endl;

	cout << "The Fast (T2) scheduler ran: " << pass2 << " times" << endl;
	cout << "The Fast (T2) scheduler missed: " << miss2 << " times\n" << endl;

	cout << "The Medium (T3) scheduler ran: " << pass3 << " times" << endl;
	cout << "The Medium (T3) scheduler missed: " << miss3 << " times\n" << endl;

	cout << "The Slow (T4) scheduler ran: " << pass4 << " times" << endl;
	cout << "The Slow (T4) scheduler missed: " << miss4 << " times\n" << endl;

	exit(0);
}

// multiplies the content of each cell in the 10x10 matrix, alternating
// by columns 0 and 5, 1 and 6, etc.
// varies depending on the thread and how many times they will execute doWork
void doWork() {

	for(int i = 0; i < 10; i += 5) {
		for(int j = 0; j < 10; ++j) {

			// matrix creating the board and initializing values
			board[0][0] *= board[j][i];
		}
		if(i > 4) {
			i -= 4;
		}
		if(i == 9) {
			break;
		}
	}
}

// thread with a period of % 1
void *veryfast(void *) {
	while(true) {
		sem_wait(&sem1);
		pass1++;

		sem_wait(&bool_sem1); // lock the matrix to protect critical selection of boolean value
		isWorking1 = true;

		sem_post(&bool_sem1); // unlock the matrix
		doWork();

		sem_wait(&bool_sem1); // lock the matrix
		isWorking1 = false;

		sem_post(&bool_sem1); // unlock the matrix
	}
}

// thread with a period of % 2
void *fast(void *) {
	while(true) {
		sem_wait(&sem2);
		pass2++;

		sem_wait(&bool_sem2); // lock the matrix to protect critical selection of boolean value
		isWorking2 = true;

		sem_post(&bool_sem2); // unlock the matrix
		doWork();

		sem_wait(&bool_sem2); // lock the matrix
		isWorking2 = false;

		sem_post(&bool_sem2); // unlock the matrix
	}
}

// thread with a period of % 4
void *medium(void *) {
	while(true) {
		sem_wait(&sem3);
		pass3++;

		sem_wait(&bool_sem3); // lock the matrix to protect critical selection of boolean value
		isWorking3 = true;

		sem_post(&bool_sem3); // unlock the matrix
		doWork();

		sem_wait(&bool_sem3); // lock the matrix
		isWorking3 = false;

		sem_post(&bool_sem3); // unlock the matrix
	}
}

// thread with a period of % 16
void *slow(void *) {
	while(true) {
		sem_wait(&sem4);
		pass4++;

		sem_wait(&bool_sem4); // lock the matrix to protect critical selection of boolean value
		isWorking4 = true;

		sem_post(&bool_sem4); // unlock the matrix
		doWork();

		sem_wait(&bool_sem4); // lock the matrix
		isWorking4 = false;

		sem_post(&bool_sem4); // unlock the matrix
	}
}
