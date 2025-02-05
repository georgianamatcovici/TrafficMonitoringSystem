#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sqlite3.h>
#include <unordered_set>
#include <iostream>
#define MAX 1000
#define MAX_BUFF 300
#define MAX_INT_STREETS 3
#define MAX_INTERSECTIONS 15
#define MAX_STREETS 30

#define PORT 2908

extern int errno;

int countClients = 0;
struct Car
{
    int IdCar;
    int Speed;
    bool WantsInfoWeather, WantsInfoSport, WantsInfoPrices;
    int CarStreet;

} Cars[MAX];

sqlite3 *Database;

std::unordered_set<int> Weather;
std::unordered_set<int> Sports;
std::unordered_set<int> Fuel;

typedef struct thData
{
    int idThread;
    int cl;
} thData;

static void *treat(void *);
void raspunde(void *);
char Command[MAX_BUFF];
char User[MAX_BUFF], Password[MAX_BUFF];
char AnswerToCommand[MAX_BUFF], Notification[MAX_BUFF], Aux[MAX_BUFF];

void FuelPrices(int street, char answer[MAX_BUFF])
{
    sqlite3_stmt *Stmt, *Stmt1;
    const char *command;
    answer[0] = 0;
    command = "SELECT GasStation FROM Streets WHERE Id=?";

    if (sqlite3_prepare_v2(Database, command, -1, &Stmt, NULL) != SQLITE_OK)
    {
        perror("Eroare pregatire comanda here");
        sqlite3_close(Database);
    }
    sqlite3_bind_int(Stmt, 1, street);
    char Station[20];
    if (sqlite3_step(Stmt) == SQLITE_ROW)
    {
        strcpy(Station, (const char *)sqlite3_column_text(Stmt, 0));
    }
    strcpy(answer, "u");
    if (Station[0] == 'N')
    {
        strcat(answer, "⛽ Unfortunately, there is no gas station on your street. The closest one can be found on street ");
        // char *p = strchr(Station + 3, ';');
        char aux[10];
        int i = 0;
        aux[0] = 0;
        // printf("Inainte : %s da \n", Station);
        fflush(stdout);
        while (Station[i + 3] != ';')
        {
            aux[i] = Station[i + 3];
            i++;
        }
        aux[i] = 0;
        //   printf("Street : %s da \n", aux);
        fflush(stdout);

        strcat(answer, aux);
        strcat(answer, ". The list of its fuel prices is:\n");
        aux[0] = 0;
        i = i + 4;
        int k = 0;
        while (Station[i] != '\0')
        {
            aux[k] = Station[i];
            i++;
            k++;
        }
        aux[k] = 0;
        strcpy(Station, aux);
        // printf("Aici : %s da \n", Station);
        fflush(stdout);
    }
    else
    {
        strcat(answer, "⛽ There's a ");
        strcat(answer, Station);
        strcat(answer, " gas station on your street. The list of its fuel prices is:\n");
    }
    command = "SELECT Type, Price FROM GasStations WHERE Name=?";

    if (sqlite3_prepare_v2(Database, command, -1, &Stmt1, NULL) != SQLITE_OK)
    {
        perror("Eroare pregatire comanda here");
        sqlite3_close(Database);
    }
    sqlite3_bind_text(Stmt1, 1, Station, -1, SQLITE_STATIC);
    while (sqlite3_step(Stmt1) == SQLITE_ROW)
    {

        const char *type = (const char *)sqlite3_column_text(Stmt1, 0);
        double price = sqlite3_column_double(Stmt1, 1);

        strcat(answer, type);
        strcat(answer, " ");
        char aux[20];
        aux[0] = 0;
        sprintf(aux, "%.2f", price);
        strcat(answer, aux);
        strcat(answer, "\n");
    }
}

void SportEvents()
{
    sqlite3_stmt *Stmt;
    const char *command;
    int random;
    srand(time(0));
    random = rand() % 40 + 1;
    command = "SELECT Text FROM SportEvents WHERE Id=?";

    if (sqlite3_prepare_v2(Database, command, -1, &Stmt, NULL) != SQLITE_OK)
    {
        perror("Eroare pregatire comanda here");
        sqlite3_close(Database);
    }
    sqlite3_bind_int(Stmt, 1, random);
    strcpy(Notification, "n⚽");

    if (sqlite3_step(Stmt) == SQLITE_ROW)
    {
        strcat(Notification, (const char *)sqlite3_column_text(Stmt, 0));
    }
    strcat(Notification, "\n");
}

void WeatherNews()
{
    sqlite3_stmt *Stmt;
    const char *command;
    int random;
    srand(time(0));
    random = rand() % 10 + 1;
    command = "SELECT Text FROM Weather WHERE Id=?";

    if (sqlite3_prepare_v2(Database, command, -1, &Stmt, NULL) != SQLITE_OK)
    {
        perror("Eroare pregatire comanda here");
        sqlite3_close(Database);
    }
    sqlite3_bind_int(Stmt, 1, random);
    strcpy(Notification, "n⛅");

    if (sqlite3_step(Stmt) == SQLITE_ROW)
    {
        strcat(Notification, (const char *)sqlite3_column_text(Stmt, 0));
    }
    strcat(Notification, "\n");
}

void handler_SIGALARM(int sig)
{

    if (!Weather.empty())
    {
        Notification[0] = 0;
        for (const int &it : Weather)
        {
            WeatherNews();
            if (write(Cars[it].IdCar, Notification, sizeof(Notification)) <= 0)
            {
                perror("[client]Eroare la write() spre server.\n");
            }
        }
    }

    if (!Sports.empty())
    {
        // strcpy(Notification, "uSports!");
        for (const int &it : Sports)
        {
            Notification[0] = 0;
            SportEvents();

            if (write(Cars[it].IdCar, Notification, sizeof(Notification)) <= 0)
            {
                perror("[client]Eroare la write() spre server.\n");
            }
        }
    }

    if (!Fuel.empty())
    {
        int CurrentStreet;
        // strcpy(Notification, "uFuel!");
        for (const int &it : Fuel)
        {
            CurrentStreet = Cars[it].CarStreet;
            FuelPrices(CurrentStreet, Notification);
            if (write(Cars[it].IdCar, Notification, sizeof(Notification)) <= 0)
            {
                perror("[client]Eroare la write() spre server.\n");
            }
        }
    }

    alarm(25);
}

void *SendUpdates(void *arg)
{
    signal(SIGALRM, handler_SIGALARM);
    alarm(25);
    while (1)
    {
        pause();
    }
}

void NotifyAll(char msg[MAX_BUFF], int current)
{
    int i;
    char answer[MAX_BUFF];
    strcpy(answer, "\n⚠️ Traffic Alert: Accident on ");
    strcat(answer, msg + 11);
    char *p = strchr(answer, '\n');
    p[0] = '.';
    strcat(answer, " Please avoid the area and use alternative routes if possible.\n");
    strcpy(msg + 1, msg);
    msg[0] = 'n';
    for (i = 1; i <= countClients; i++)
    {
        if (Cars[i].IdCar != current)
        {
            if (write(Cars[i].IdCar, answer, strlen(answer) + 1) <= 0)
            {
                perror("[Thread]Eroare la write() catre client.\n");
            }
        }
    }
}

int Validate(char user[MAX_BUFF], char password[MAX_BUFF])
{

    sqlite3_stmt *Stmt;
    const char *command = "SELECT 1 FROM Cars WHERE Username = ? AND Password = ?";

    if (sqlite3_prepare_v2(Database, command, -1, &Stmt, NULL) != SQLITE_OK)
    {
        perror("Eroare pregatire comanda");
        sqlite3_close(Database);
        return 0;
    }
    sqlite3_bind_text(Stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(Stmt, 2, password, -1, SQLITE_STATIC);

    int ok = 0;
    if (sqlite3_step(Stmt) == SQLITE_ROW)
    {
        ok = 1;
    }

    sqlite3_finalize(Stmt);
    // sqlite3_close(Cars_db);
    return ok;
}

void DecodeWarnings(int SpeedLimit, char answer[MAX_BUFF])
{
    answer[0] = 0;
    switch (SpeedLimit)
    {
    case 1:
        strcat(answer, ". There may be additional restrictions for heavy vehicles.🚛 \n");
        break;
    case 2:
        strcat(answer, ". The road conditions include school zones.🏫 \n");
        break;
    case 3:
        strcat(answer, " due to reduced visibility.🌫️ \n");
        break;
    case 4:
        strcat(answer, " due to construction work on the road.🚧 \n");
        break;
    case 5:
        strcat(answer, " due to narrow road and dangerous curves.⚠️ \n");
        break;
    case 6:
        strcat(answer, " due to residential area.🏘️ \n");
        break;
    case 7:
        strcat(answer, ". There are no additional restrictions.\n");
        break;
    case 8:
        strcat(answer, ". This street may be closed over the weekend.🚫 \n");
        break;
    }
}

void FindStreet(int id, char Street[50])
{
    sqlite3_stmt *Stmt;
    const char *command = "SELECT Name from Streets WHERE Id=?";

    if (sqlite3_prepare_v2(Database, command, -1, &Stmt, NULL) != SQLITE_OK)
    {
        perror("Eroare pregatire comanda");
        sqlite3_close(Database);
    }
    sqlite3_bind_int(Stmt, 1, id);
    if (sqlite3_step(Stmt) == SQLITE_ROW)
    {
        const char *CurrentStreet = (const char *)sqlite3_column_text(Stmt, 0);
        strcpy(Street, CurrentStreet);
    }
    sqlite3_finalize(Stmt);
}

void SpeedStreet(int id, char speed_char[20], char answer[MAX_BUFF], int flag)
{

    sqlite3_stmt *Stmt;
    const char *command = "SELECT Name, Speed, SpeedLimits, Accident FROM Streets WHERE Id=?";

    if (sqlite3_prepare_v2(Database, command, -1, &Stmt, NULL) != SQLITE_OK)
    {
        perror("Eroare pregatire comanda");
        sqlite3_close(Database);
    }
    sqlite3_bind_int(Stmt, 1, id);

    const char *CurrentStreet;
    int SpeedLimit;
    int Warnings;
    int accident;

    if (sqlite3_step(Stmt) == SQLITE_ROW)
    {
        CurrentStreet = (const char *)sqlite3_column_text(Stmt, 0);
        SpeedLimit = sqlite3_column_int(Stmt, 1);
        Warnings = sqlite3_column_int(Stmt, 2);
        accident = sqlite3_column_int(Stmt, 3);
    }
    if (flag == 0)
    {
        strcpy(answer, "aYou are driving on street ");
        strcat(answer, CurrentStreet);
        strcat(answer, " at a speed of ");
        strcat(answer, speed_char);
        strcat(answer, "km/h.🚗\n");
    }
    else
    {
        strcpy(AnswerToCommand, " You are on ");
        strcat(AnswerToCommand, CurrentStreet);
        strcat(AnswerToCommand, " Street now.\n");
    }
    if (accident == 1)
    {
        strcat(answer, "⚠️ An accident has occurred on this street, causing delays. Please proceed with caution.\n");
    }

    else
    {
        if (flag == 0)

        {
            strcat(answer, "On ");
            strcat(answer, CurrentStreet);
            strcat(answer, " Street, the speed limit is ");
        }
        else
        {
            strcat(answer, "The speed limit on this route is ");
        }
        char aux[MAX_BUFF];
        sprintf(aux, "%d km/h", SpeedLimit);
        strcat(answer, aux);
        DecodeWarnings(Warnings, aux);
        strcat(answer, aux);
        if (accident == 2)
        {
            strcat(answer, "🚗🚙🚛 There is traffic congestion on this street due to heavy traffic.\n");
        }
    }
    sqlite3_finalize(Stmt);
}

int UpdateAccident(char Street[30])
{
    sqlite3_stmt *Stmt;
    const char *command = "UPDATE Streets SET Accident = 1 WHERE name = ?";
    if (sqlite3_prepare_v2(Database, command, -1, &Stmt, NULL) != SQLITE_OK)
    {
        perror("Eroare prepare");
        sqlite3_close(Database);
    }

    if (sqlite3_bind_text(Stmt, 1, Street, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        return -1;
    }
    if (sqlite3_step(Stmt) != SQLITE_DONE)
    {
        perror("Eroare executare");
        return -1;
    }

    sqlite3_finalize(Stmt);
    return 0;
}

int main()
{
    struct sockaddr_in server;
    struct sockaddr_in from;
    int sd;
    pthread_t th[100];

    int i = 0;

    if (sqlite3_open("cars.db", &Database) != SQLITE_OK)
    {
        perror("eroare deschidere database");
    }

    pthread_t UpdatesHandler;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }

    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    server.sin_family = AF_INET;

    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen(sd, 2) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }

    printf("Start!\n");
    if (pthread_create(&UpdatesHandler, NULL, SendUpdates, NULL) != 0)
    {
        perror("Eroare creare thread.\n");
        return errno;
    }

    while (1)
    {
        int client;
        thData *td;
        unsigned int length = sizeof(from);

        fflush(stdout);

        if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
        {
            perror("[server]Eroare la accept().\n");
            continue;
        }
        countClients++;
        Cars[countClients].IdCar = client;

        td = (struct thData *)malloc(sizeof(struct thData));
        td->idThread = i++;
        td->cl = client;

        pthread_create(&th[i], NULL, &treat, td);
    }
};
static void *treat(void *arg)
{
    struct thData tdL;
    tdL = *((struct thData *)arg);
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData *)arg);
    close((intptr_t)arg);
    return (NULL);
};

void raspunde(void *arg)
{

    struct thData tdL;
    tdL = *((struct thData *)arg);
    int clientConnected = 0;

    while (!clientConnected)
    {
        int read_code;
        if ((read_code = read(tdL.cl, User, sizeof(User))) < 0)
        {
            perror("Eroare la read() de la client.\n");
        }
        if (read_code)

        {
            int size = strlen(User);
            User[size - 1] = '\0';
            // printf("Username: %s\n", User);
        }

        if ((read_code = read(tdL.cl, Password, sizeof(Password))) < 0)
        {
            perror("Eroare la read() de la client.\n");
        }
        if (read_code)

        {
            int size = strlen(Password);
            Password[size - 1] = '\0';
            // printf("Password: %s\n", Password);
        }

        if (Validate(User, Password))
        {
            printf("Valid user.\n");
            strcpy(AnswerToCommand, "1");
            if (write(tdL.cl, &AnswerToCommand, sizeof(AnswerToCommand)) <= 0)
            {
                perror("[Thread]Eroare la write() catre client.\n");
            }
            clientConnected = 1;
            printf("Car #%d connected!\n", tdL.idThread + 1);
        }
        else
        {
            printf("Invalid user.\n");
            strcpy(AnswerToCommand, "0");
            if (write(tdL.cl, &AnswerToCommand, sizeof(AnswerToCommand)) <= 0)
            {
                perror("[Thread]Eroare la write() catre client.\n");
            }
        }
    }

    while (1)
    {
        int read_code;
        if ((read_code = read(tdL.cl, Command, sizeof(Command))) < 0)
        {
            perror("Eroare la read() de la client.\n");
        }
        if (read_code)

        {
            int flag = 1;
            if (strcmp(Command, "Weather\n") == 0)
            {

                if (Cars[tdL.idThread + 1].WantsInfoWeather == 0)
                {
                    Cars[tdL.idThread + 1].WantsInfoWeather = 1;
                    Weather.insert(tdL.idThread + 1);
                    strcpy(AnswerToCommand, "aYou've enabled the option to receive information about weather. \n");
                    Notification[0] = 0;
                    WeatherNews();
                    strcat(AnswerToCommand, Notification + 1);
                }
                else
                {
                    strcpy(AnswerToCommand, "aOption already exists.\n");
                }
            }
            else if (strcmp(Command, "Sport events\n") == 0)
            {

                if (Cars[tdL.idThread + 1].WantsInfoSport == 0)
                {
                    Cars[tdL.idThread + 1].WantsInfoSport = 1;
                    Sports.insert(tdL.idThread + 1);
                    strcpy(AnswerToCommand, "aYou've enabled the option to receive information about sport events.\n");
                    Notification[0] = 0;
                    SportEvents();
                    strcat(AnswerToCommand, Notification + 1);
                }
                else
                {
                    strcpy(AnswerToCommand, "aOption already exists.\n");
                }
            }
            else if (strcmp(Command, "Fuel prices\n") == 0)
            {
                if (Cars[tdL.idThread + 1].WantsInfoPrices == 0)
                {
                    Cars[tdL.idThread + 1].WantsInfoPrices = 1;
                    Fuel.insert(tdL.idThread + 1);
                    strcpy(AnswerToCommand, "aYou've enabled the option to receive information about fuel prices.\n");
                    Notification[0] = 0;
                    FuelPrices(Cars[tdL.idThread + 1].CarStreet, Notification);
                    strcat(AnswerToCommand, Notification + 1);
                }
                else
                {
                    strcpy(AnswerToCommand, "aOption already exists.\n");
                }
            }
            else if (strcmp(Command, "Disable Fuel prices\n") == 0)
            {
                if (Cars[tdL.idThread + 1].WantsInfoPrices == 1)
                {
                    Cars[tdL.idThread + 1].WantsInfoPrices = 0;
                    Fuel.erase(tdL.idThread + 1);
                    strcpy(AnswerToCommand, "aOption disabled.\n");
                }
                else
                {
                    strcpy(AnswerToCommand, "aThis option wasn't activated.\n");
                }
            }
            else if (strcmp(Command, "Disable Sport events\n") == 0)
            {
                if (Cars[tdL.idThread + 1].WantsInfoSport == 1)
                {
                    Cars[tdL.idThread + 1].WantsInfoSport = 0;
                    Sports.erase(tdL.idThread + 1);
                    strcpy(AnswerToCommand, "aOption disabled.\n");
                }
                else
                {
                    strcpy(AnswerToCommand, "aThis option wasn't activated.\n");
                }
            }
            else if (strcmp(Command, "Disable Weather\n") == 0)
            {
                if (Cars[tdL.idThread + 1].WantsInfoWeather == 1)
                {
                    Weather.erase(tdL.idThread + 1);
                    Cars[tdL.idThread + 1].WantsInfoWeather = 0;
                    strcpy(AnswerToCommand, "aOption disabled.\n");
                }
                else
                {
                    strcpy(AnswerToCommand, "aThis option wasn't activated.\n");
                }
            }

            else if (strncmp(Command, "Speed:", 6) == 0)
            {
                // printf("\n%s\n", Command);
                char speed[20], street[20];
                int i, k = 0;
                speed[0] = 0;
                i = 7;
                while (Command[i] != ' ')
                {
                    speed[k++] = Command[i];
                    i++;
                }
                speed[k] = '\0';
                k = 0;
                i = i + 9;
                while (Command[i] != ' ' && Command[i] != '\0')
                {
                    street[k++] = Command[i];
                    i++;
                }
                street[k] = '\0';

                int CurrentSpeed = atoi(speed);
                printf("Car #%d: speed - %d km/h\n", tdL.idThread + 1, CurrentSpeed);
                Cars[tdL.idThread + 1].Speed = CurrentSpeed;
                int CurrentStreet = atoi(street);
                Cars[tdL.idThread + 1].CarStreet = CurrentStreet;
                SpeedStreet(CurrentStreet, speed, AnswerToCommand, 0);
                // printf("/n%s/n", AnswerToCommand);
            }
            else if (strncmp(Command, "Updated Speed:", 14) == 0)
            {
                int CurrentSpeed = atoi(Command + 15);
                printf("Car #%d: updated speed - %d km/h\n", tdL.idThread + 1, CurrentSpeed);
                Cars[tdL.idThread + 1].Speed = CurrentSpeed;
                strcpy(AnswerToCommand, "nYour speed is now ");
                char aux1[20];
                int i = 0;
                while (Command[15 + i] != ' ')
                {
                    aux1[i] = Command[15 + i];
                    i++;
                }
                aux1[i] = '\0';
                strcat(AnswerToCommand, aux1);
                strcat(AnswerToCommand, " km/h.");
                char *p = strchr(Command + 15, ' ');
                strcpy(Command, p + 1);
                if (Command[0] == '1')
                {
                    strcat(AnswerToCommand, " You are continuing on the same street.\n");
                }
                else
                {
                    int newStreet = atoi(Command + 2);
                    Cars[tdL.idThread + 1].CarStreet = newStreet;
                    Aux[0] = 0;
                    SpeedStreet(newStreet, aux1, Aux, 1);
                    strcat(AnswerToCommand, Aux);
                }
                // printf("\n%s\n", AnswerToCommand);
                flag = 1;
            }
            else if (strncmp(Command, "Accident :", 10) == 0)
            {

                NotifyAll(Command, Cars[tdL.idThread + 1].IdCar);
                Aux[0] = 0;
                strcpy(Aux, Command + 12);
                int i;
                for (i = 0; Aux[i] != '\n' && Aux[i] != '\0'; i++)
                {
                    if (Aux[i] == ' ')
                    {
                        Aux[i] = '\0';
                        break;
                    }
                }

                // printf("\nAccident:\n%s", Aux);
                char aux1[30];
                strcpy(aux1, Aux);
                if (UpdateAccident(aux1) == -1)
                    sprintf(AnswerToCommand, "aInvalid street.\n");
                else
                    sprintf(AnswerToCommand, "aAccident reported.\n");
            }
            else
            {
                strcpy(AnswerToCommand, "aInvalid command.");
            }
            if (flag == 1)
            {
                if (write(tdL.cl, &AnswerToCommand, sizeof(AnswerToCommand)) <= 0)
                {
                    perror("[Thread]Eroare la write() catre client.\n");
                }
            }
        }
    }
}
