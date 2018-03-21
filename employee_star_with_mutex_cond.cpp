#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstring>

#define NUM_EMPLOYEE 2      /* 两个员工 */

/* 全局的互斥锁 */
pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;

struct employee
{
    int number;
    int id;
    char first_name[20];
    char last_name[30];
    char department[30];
    int room_number;
};

/* 员工信息，全局 */
struct employee employees[] = {
    {1, 12345678, "he", "cheng", "programmer", 611}, 
    {2, 87654321, "he", "yu", "accounting", 110}
};

/* 明日之星 */
struct employee employee_of_the_day;

/* 赋值函数 */
void CopyEmployee(struct employee* from, struct employee* to)
{
    //printf("thread id is : %ld\n", pthread_self());
    int rc;
    /* 操作互斥量 */
    rc = pthread_mutex_lock(&a_mutex);
    
    /* 拷贝 */
    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name, from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;

    /* 解锁 */
    rc = pthread_mutex_unlock(&a_mutex);
}

void* DoLoop(void* data)
{
    int my_num = *((int*)data);

    while(1)
    {
        /* 竞选明日之星 */
        CopyEmployee(&employees[my_num - 1], &employee_of_the_day);
    }
}

int main()
{
    int thr_id1;
    int thr_id2;
    pthread_t p_thread1;
    pthread_t p_thread2;
    int num1 = 1;
    int num2 = 2;
    struct employee eotd;
    struct employee* worker;    /* 指向当前竞选的员工 */

    /* 初始化明日之星 */
    CopyEmployee(&employees[0], &employee_of_the_day);

    thr_id1 = pthread_create(&p_thread1, NULL, DoLoop, (void*)&num1);
    thr_id2 = pthread_create(&p_thread2, NULL, DoLoop, (void*)&num2);

    /* many many time */
    for(int i = 0; i < 1000; i++)
    {
        CopyEmployee(&employee_of_the_day, &eotd);
        worker = &employees[eotd.number-1];
        printf("current number: %d\n", worker->number);
        /* compare employees */
        if (eotd.id != worker->id) {
            printf("mismatching 'id' , %d != %d (loop '%d')\n",eotd.id, worker->id, i);
            exit(0);
        }
        if (strcmp(eotd.first_name, worker->first_name) != 0) {
            printf("mismatching 'first_name' , %s != %s (loop '%d')\n",eotd.first_name, worker->first_name, i);
            exit(0);
        }
        if (strcmp(eotd.last_name, worker->last_name) != 0) {
            printf("mismatching 'last_name' , %s != %s (loop '%d')\n",eotd.last_name, worker->last_name, i);
            exit(0);
        }
        if (strcmp(eotd.department, worker->department) != 0) {
            printf("mismatching 'department' , %s != %s (loop '%d')\n",eotd.department, worker->department, i);
            exit(0);
        }
        if (eotd.room_number != worker->room_number) {
            printf("mismatching 'room_number' , %d != %d (loop '%d')\n",eotd.room_number, worker->room_number, i);
            exit(0);
        }
    }

    printf("My lord, employee contens was always consistent\n");
    return 0;
}
