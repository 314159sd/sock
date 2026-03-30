#include <iostream>
#include <pthread.h>

void *printss(void *arg)
{
    while (true)
    {
        std::cout << "sd" << std::endl;
        // int a = *(int *)arg;
        // std::cout << a << std::endl;
    }

    return NULL;
}

void *func(void *arg)
{
    std::cout << "MAIN" << std::endl;
    pthread_t thread;

    pthread_create(&thread, NULL, printss, NULL);
    pthread_join(thread, NULL);

    return NULL;
}
int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, func, NULL);
    pthread_join(thread, NULL);
    return 0;
}
