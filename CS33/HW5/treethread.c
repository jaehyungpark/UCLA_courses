#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread(void *vargp);

int main(int arg, char **argv)
{
    int N = atoi(argv[1]);  // get argument for number of threads
    pthread_t *tid;
    int num = 2;
    for (int i = 0; i < N-1 ; i++)
    {
        num *= 2;
    }
    num -= 2;
    tid = malloc(num*sizeof(pthread_t));
    for (int i = 0; i < num; i++)
    {
        pthread_create(&tid[i], NULL, thread, NULL);
    }
    for (int j = 0; j < num; j++)
    {
        pthread_join(tid[j], NULL);
    }
    free(tid);
    printf("Hello, world!\n");
    exit(0);
}

void *thread(void *vargp)
{
    printf("Hello, world!\n");
    return NULL;
}
