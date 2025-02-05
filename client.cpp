#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#define MAX_BUFF 300
extern int errno;

int port;
int sd;
int connected = 0;
char myCommand[MAX_BUFF];
char Answer[MAX_BUFF];
int logout = 0;
void handler_SIGALARM(int sig)
{

    int RandomSpeed;
    srand(time(0));
    RandomSpeed = rand() % 100;
    int stay;
    stay = rand() % 2;
    if (stay == 0)
    {
        int RandomStreet = rand() % 23 + 1;
        sprintf(myCommand, "Updated Speed: %d %d %d", RandomSpeed, stay, RandomStreet);
    }
    else
    {
        sprintf(myCommand, "Updated Speed: %d %d", RandomSpeed, stay);
    }
    if (write(sd, myCommand, sizeof(myCommand)) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
    }
    alarm(20);
}

void *UpdateSpeed(void *arg)
{
    signal(SIGALRM, handler_SIGALARM);
    alarm(20);
    while (!logout)
    {
        pause();
    }
}

void *HandleNotifications(void *arg)
{
    while (!logout)
    {
        if (read(sd, Answer, sizeof(Answer)) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
        }
        printf("%s\n", Answer + 1);
        fflush(stdout);
    }
}

void Connected()
{
    printf("\n🚗 Welcome to the Traffic Monitoring System! 🚙 \n");
    printf("🌦️ ⚽ If you want to receive additional information, please use the commands: Weather, Sport events, Fuel prices. ⛽\n");
    printf("🚗💥 If you want to report an accident, please use the following format: Accident : [Street name] [Street number].\n");
    printf("🔕 If you want to disable an activated option, please use the following format: Disable [option].\n\n");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server;

    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    while (!connected)
    {
        printf("Please enter your username:\n");
        fflush(stdout);
        fgets(myCommand, sizeof(myCommand), stdin);
        int length = strlen(myCommand);
        myCommand[length] = '\0';

        if (write(sd, myCommand, sizeof(myCommand)) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }

        printf("Please enter your password:\n");
        fflush(stdout);
        fgets(myCommand, sizeof(myCommand), stdin);
        length = strlen(myCommand);
        myCommand[length] = '\0';

        if (write(sd, myCommand, sizeof(myCommand)) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }

        if (read(sd, Answer, sizeof(Answer)) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
        }
        if (Answer[0] == '1')
        {
            Connected();
            connected = 1;
        }
        else
        {
            printf("\nInvalid username or password. Please try again.\n");
            fflush(stdout);
            sleep(1);
        }
    }
    char myCommand[MAX_BUFF];
    int RandomSpeed, RandomStreet = 0;
    srand(time(0));
    RandomSpeed = rand() % 120;
    srand(time(0));
    RandomStreet = rand() % 23 + 1;
    // RandomStreet = 17;
    sprintf(myCommand, "Speed: %d Street: %d", RandomSpeed, RandomStreet);
    if (write(sd, myCommand, sizeof(myCommand)) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        return errno;
    }

    if (read(sd, Answer, sizeof(Answer)) < 0)
    {
        perror("[client]Eroare la read() de la server.\n");
        return errno;
    }
    printf("%s\n", Answer + 1);
    fflush(stdout);

    // strcpy(myCommand, "This is my command");
    pthread_t SpeedUpdater;
    if (pthread_create(&SpeedUpdater, NULL, UpdateSpeed, NULL) != 0)
    {
        perror("Eroare creare thread.\n");
        return errno;
    }
    pthread_t NotificationHandler;
    if (pthread_create(&NotificationHandler, NULL, HandleNotifications, NULL) != 0)
    {
        perror("Eroare creare thread.\n");
        return errno;
    }
    // signal(SIGALRM, handler_SIGALARM);
    // alarm(3);
    while (1)
    {
        // pause();
        fgets(myCommand, sizeof(myCommand), stdin);
        int length = strlen(myCommand);
        myCommand[length] = '\0';
        if (strcmp(myCommand, "Logout\n") == 0)
        {
            logout = 1;
            return 0;
        }
        if (write(sd, myCommand, sizeof(myCommand)) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }

        // if (read(sd, Answer, sizeof(Answer)) < 0)
        // {
        //     perror("[client]Eroare la read() de la server.\n");
        //     return errno;
        // }

        // printf("%s\n", Answer + 1);
        // fflush(stdout);
    }

    close(sd);
}
