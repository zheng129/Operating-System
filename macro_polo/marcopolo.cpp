#include <iostream>
#include <thread>
#include <pthread.h>
#include <zconf.h>

using namespace std;

void *Marco(void *ptr) 
{
    char *message;
    message = (char *) ptr;
    //pthread_exit(NULL);

    for (int i = 0; i < 9; ++i) 
    {
         cout << "Marco" << endl;
         //sleep 1 sec
         sleep(1);

     }
}

void *Polo(void *ptr) 
{
    char *message;
    message = (char *) ptr;
    //pthread_exit(NULL);

    
    for (int i = 0; i < 9; ++i) {
        cout << "Polo" << endl;
        //sleep 1 sec
        sleep(1);
    } 
}

int main() 
{
    std::cout << "Welcome to Marco Polo" << std::endl;

    pthread_t marco, polo;
    int iret1, iret2;
    const char *message1 = "Marco";
    const char *message2 = "Polo";

    iret1 = pthread_create(&marco, nullptr, Marco, (void*) message1);
    if(iret1)

    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create(&polo, nullptr, Polo, (void*) message2);
    if(iret2)

    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    pthread_join(marco, NULL);
    pthread_join(polo, NULL);

   /*
    thread first (Marco);
    thread second (Polo);

    first.join();
    second.join();

    */

    cout << "That's all folks \n" << endl;
    return 0;
}
