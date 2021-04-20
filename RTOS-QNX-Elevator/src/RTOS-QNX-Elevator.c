#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#define    TOP            0  // лифт вверху
#define    BOTTOM         1  // лифт внизу
#define    THIS_FLOOR     2  // лифт на нашем («данном») этаже

#define    Up	          48  // Сигналы реального времени
#define    Down	          49

int state = THIS_FLOOR;

void move (int direction) {
    int i;
    switch (direction) {
        case Up:
            for (i = 1; i <= 3; i++) {
                printf ("*** Go UP\n");
                usleep(500000);
                state = TOP;
            }

        break;
        case Down:
            for (i = 1; i <= 3; i++ ) {
                printf ("*** Go DOWN\n");
                usleep(500000);
                state = BOTTOM;
            }
        break;
    }

    switch (state) {
        case TOP:
            printf("state TOP\n");
        break;
        case BOTTOM:
            printf("state BOTTOM\n");
        break;
        case THIS_FLOOR:
            printf("state THIS_FLOOR\n");
        break;
    }
}

//Работа кнопки моделируется потоком с функцией
void* push_button(void* args) {
    char ch;

    while(ch != 'q'){
	    ch = getchar();
	    switch(ch) {
            case 'u':
                raise(Up);
            break;
            case 'd':
                raise(Down);
            break;
            case 'q':
                pthread_exit(0);
            break;
	    }
	}
}

//Работа автомата и перемещение лифта моделируются в функции обработки сигналов

void lift_control(int signo){
	switch(state) {
	    case THIS_FLOOR:
	    	if(signo == Up) {
               printf("This Floor, door is opened \n");
               sleep(1);
               move(Up);
	    	}
            if(signo == Down) {
                printf("This Floor, door is opened \n");
                sleep(1);
                move(Down);
            }
	    break;
	    case TOP:
	    	if(signo == Up) {
	    	   move(Down);
               printf("This Floor, door is opened \n");
               sleep(1);
               move(Up);
	    	}
            if(signo == Down) {
                move(Down);
                printf("This Floor, door is opened \n");
                sleep(1);
                move(Down);
            }
	    break;
	    case BOTTOM:
	    	if(signo == Up) {
	    	   move(Up);
               printf("This Floor, door is opened \n");
               sleep(1);
               move(Up);
	    	}
            if(signo == Down) {
                move(Down);
                printf("This Floor, door is opened \n");
                sleep(1);
                move(Down);
            }
	    break;
	    }
}

int main(int argc, char *argv[])
{
    pthread_t t;
    printf("Start\n");

    sigset_t set;
    sigemptyset(&set);
    struct sigaction act;
    act.sa_handler = lift_control;
    act.sa_flags = 0;
    act.sa_mask = set;
    sigaction(Up, &act, NULL);
    sigaction(Down, &act, NULL);


    pthread_create(&t,NULL,&push_button, NULL);
    pthread_join(t, NULL);

    printf("Finish\n");
    return 0;
}
