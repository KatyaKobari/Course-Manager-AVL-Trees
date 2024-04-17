//“P1_1201478_KatyaKobari_3” .
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 1000 //size is the maximum number of char that we can read from file.
typedef struct Bus Bus;
typedef struct Student Student;

//----------prototype all function in project--------
void printMenu();

void loadBuses(Bus *bus);

void loadStudents(Student *mainStudentList);

void MainAssignStudents(Student *mainStudentList, Bus *bus, Student *unmatchedStu);

int assignStu(Student *tempStu, Bus *bus);

int NotOnTimeAssign(Student *tempStu, Bus *tempBus);

void printSpcBusInfo(Bus *bus, int num);

void printBusesPassengers(Bus *bus);

//declare student struct.
struct Student {
    int id;
    int Date;
    char time[30];
    char stuFromDes[30];
    char stuToDes[30];
    struct Student *Next;
};
//declare bus struct.
struct Bus {
    int num;
    int Date;
    char time[30];
    char busFromDes[30];
    char busToDes[30];
    double price;
    int capacity;
    int space; //to know how much available Seating in the bus.
    struct Bus *Next;
    struct Student *StudentLinkedList;// every bus will point to a set of passengers on it(so it's a linked list of another linked list).

};


//------student linked list functions------------------
Student *StuMakeEmpty(Student *L);

int StuIsEmpty(Student *L);

int StuIsLast(Student *P, Student *L);

Student *StuFind(int Id, Student *L);

Student *StuFindPrevious(int id, Student *L);

void StuDelete(int id, Student *L);

void StuInsert(int id, int date, char time[30], char from[30], char to[30], Student *L, Student *P);

void StuPrintList(Student *L);

void StuDeleteList(Student *L);

int StuSize(Student *L);

//--------bus linked list functions-------------
Bus *busMakeEmpty(Bus *L);

int busIsEmpty(Bus *L);

int busIsLast(Bus *P, Bus *L);

Bus *busFind(int num, Bus *L);

Bus *busFindPrevious(int num, Bus *L);

void busDelete(int num, Bus *L);

void
busInsert(int num, int date, char time[30], char from[30], char to[30], double price, int capacity, Bus *L, Bus *P);

void busPrintList(Bus *L);

void busDeleteList(Bus *L);

int busSize(Bus *L);

//----------------------------------------------------
int main() {
    //declare (bus,student,unmatched) linked list, and make its header node.
    Bus *bus = busMakeEmpty(NULL);
    Student *mainStudentList = StuMakeEmpty(NULL);
    Student *unmatchedStu = StuMakeEmpty(NULL);
    int choice;
    int x, y, z;//this three variables to check if user enter the first three options more than once.
    x = 0;
    y = 0;
    z = 0;
    do {
        printMenu(); //show menu until the choice doesn't equal 9.
        scanf("%d", &choice);
        if (choice == 1) {
            if (x == 0) {
                x += 1;
                loadBuses(bus);
            } else
                printf("You cannot download the same file more than once.\n");
        }
        if (choice == 2) {
            if (y == 0) {
                y += 1;
                loadStudents(mainStudentList);
            } else
                printf("You cannot download the same file more than once.\n");
        }
        if (choice == 3) {
            if (x == 0 || y == 0) { //this condition because we can't assign before load bus and student info.
                printf("You can't assign passengers before load buses and passengers.\n");
            } else {
                if (z == 0) {
                    z += 1;
                    MainAssignStudents(mainStudentList, bus, unmatchedStu); //assign function
                    printBusesPassengers(bus); //print all buses info.

                } else {
                    printf("You cannot assign the same file more than once.\n");
                }
            }
        }
        if (choice == 4) {
            int num;
            printf("Enter bus number: ");
            scanf("%d", &num);
            printSpcBusInfo(bus, num); //print bus info.
        }
        if (choice == 5) {
            printf("\nUnmatched Students: \n");
            StuPrintList(unmatchedStu); //print unmatched linked list.
        }
        if (choice == 6) {// insert new passenger-->
            printf("Enter id:\n ");
            int id;
            scanf("%d", &id);
            printf("Enter date:\n ");
            int date;
            scanf("%d", &date);
            char time[30];
            printf("Enter time:\n ");
            scanf("%s", time);
            char from[30];
            printf("Enter from destination:\n ");
            scanf("%s", from);
            char to[30];
            printf("Enter to destination:\n ");
            scanf("%s", to);
            StuInsert(id, date, time, from, to, mainStudentList, mainStudentList);
            Student *s = StuFind(id, mainStudentList); //find passenger to insert him to the suitable bus.
            int flag = 0; //flag to know if the student assign in bus in exact time.
            int flag2 = 0;//flag2 to know if the student assign in bus at another suitable time.
            Bus *tempBus = bus->Next;//start from first node.
            while (tempBus !=
                   NULL) { //loop to pass over all buses to check whither its suitable (date ,exact time ,from,to).
                if (assignStu(s, tempBus) == 1) {
                    flag = 1;
                    flag2 = 1;
                    break;
                }
                tempBus = tempBus->Next; //go to the next bus
            }
            if (flag2 ==
                0) { //if theirs no bus with exact date,time,from,to search for another bus with same date,from,to and another suitable time.
                tempBus = bus->Next;
                while (tempBus != NULL) {
                    if (NotOnTimeAssign(s, tempBus) == 1) {
                        flag = 1;
                        flag2 = 1;
                        break;
                    }
                    tempBus = tempBus->Next;
                }
            }
            if (!(flag) && !(flag2)) { //if there is no matched bus to this passenger add him to unmatched linked list.
                StuInsert(s->id, s->Date, s->time,
                          s->stuFromDes, s->stuToDes, unmatchedStu, unmatchedStu);
            }
        }
        if (choice == 7) { //delete passenger via id
            printf("Enter passenger id: ");
            int id;
            scanf("%d", &id);
            if (StuFind(id, mainStudentList) == NULL) { //check if it doesn't exist then find function will return Null.
                printf("Passenger doesn't exist.\n");
            } else {
                StuDelete(id, mainStudentList); //delete passenger from main list
                Bus *b = bus->Next;
                int flag=0;
                for (int i = 0; i <
                                busSize(bus); i++) { //loop to find the bus that the passenger in it and delete passenger from the bus.
                    if (StuFind(id, b->StudentLinkedList) != NULL) {
//                        if ((b->space) < (b->capacity)) {
                            b->space += 1; //space will increment by 1 when we delete passenger.
                            StuDelete(id, b->StudentLinkedList); //delete passenger from bus.
                            flag=1;
                            break;
//                        } else {
//                            StuDelete(id, b->StudentLinkedList);
//                            flag=1;
//                            break;
//                        }
                    } else {
                        b = b->Next;
                    }
                }
                if(!(flag)){
                    StuDelete(id,unmatchedStu);
                }
            }
        }
        if (choice == 8) {
            printf("Enter bus number: ");
            int num;
            scanf("%d", &num);
            Bus *b = busFind(num, bus);
//            b->space += 1;
            Student *s = b->StudentLinkedList;
            busDelete(num, bus); //delete bus from linked list
            MainAssignStudents(s, bus,
                               unmatchedStu); //assign there student in another buses or in unmatched linked list.
        }

    } while (choice != 9);
// delete lists
    busDeleteList(bus);
    StuDeleteList(mainStudentList);
    StuDeleteList(unmatchedStu);
    return 0;
}

//----------------Student linked list lib------------------
Student *StuMakeEmpty(Student *L) { //make student linked list and return header node.
    if (L != NULL)
        StuDeleteList(L);
    L = (Student *) malloc(sizeof(Student));
    if (L == NULL)
        printf("Out of memory!\n");
    L->Next = NULL;
    return L;

}

int StuIsEmpty(Student *L) { //if list is empty then first node will point to null.
    return L->Next == NULL;
}

int StuIsLast(Student *P, Student *L) {
    return P->Next == NULL;
}

Student *StuFind(int id, Student *L) { //find specific student from linked list through id.
    Student *P;
    P = L->Next;
    while (P != NULL && P->id != id)
        P = P->Next;
    return P;
}

Student *StuFindPrevious(int Id, Student *L) { //find previous node ,it's important in delete methode.
    Student *P;
    P = L;
    while (P->Next != NULL && P->Next->id != Id)
        P = P->Next;
    return P;
}

void StuDelete(int id, Student *L) { //delete specific student node via id.
    Student *P, *temp;
    P = StuFindPrevious(id, L);
    if (!StuIsLast(P, L)) {
        temp = P->Next;
        P->Next = temp->Next; //bypass delete cell
        free(temp);
    }
}

int StuSize(Student *L) {
    Student *p = L->Next;
    int count = 0;
    while (p != NULL) {
        count += 1;
        p = p->Next;
    }
    return count;
}

void StuPrintList(Student *L) { //print student linked list info
    Student *P = L;
    if (StuIsEmpty(L))
        printf("Empty list\n");
    else
        do {
            P = P->Next;
            printf("Student ID:%d\t", P->id);
            printf("Date:%d\t", P->Date);
            printf("Time:%s\t", P->time);
            printf("From:%s\t", P->stuFromDes);
            printf("To:%s\n", P->stuToDes);
        } while (!StuIsLast(P, L));
    printf("------------------------------------------------------\n");
}

void StuDeleteList(Student *L) { //delete all linked list expect header node.
    Student *P;
    Student *temp;
    P = L->Next;
    L->Next = NULL;
    while (P != NULL) {
        temp = P->Next;
        free(P);
        P = temp;
    }
}

void
StuInsert(int id, int date, char time[30], char from[30], char to[30], Student *L, Student *P) { //insert student node.
    Student *temp;
    temp = (Student *) malloc(sizeof(Student));
    temp->id = id;
    temp->Date = date;
    strcpy(temp->time, time);
    strcpy(temp->stuFromDes, from);
    strcpy(temp->stuToDes, to);
    temp->Next = P->Next;
    P->Next = temp; //make p point to the new node.
}

//-------------------------------------Bus linked list lib-------------------------
Bus *busMakeEmpty(Bus *L) { //creat bus linked list (return header node).
    if (L != NULL)
        busDeleteList(L);
    L = (Bus *) malloc(sizeof(Bus));
    if (L == NULL)
        printf("Out of memory!\n");
    L->Next = NULL;
    return L;
}

int busIsEmpty(Bus *L) {
    return L->Next == NULL;
}

int busIsLast(Bus *P, Bus *L) {
    return P->Next == NULL;
}

Bus *busFind(int num, Bus *L) { //find specific bus by number
    Bus *P;
    P = L->Next;
    while (P != NULL && P->num != num)
        P = P->Next;
    return P;
}


Bus *busFindPrevious(int num, Bus *L) {
    Bus *P;
    P = L;
    while (P->Next != NULL && P->Next->num != num) {
        P = P->Next;
    }
    return P;
}

void busDelete(int num, Bus *L) {
    Bus *P, *temp;
    P = busFindPrevious(num, L);
    if (!busIsLast(P, L)) {
        temp = P->Next;
        P->Next = temp->Next; //bypass delete cell
        free(temp);
    }
}

void
busInsert(int num, int date, char time[30], char from[30], char to[30], double price, int capacity, Bus *L, Bus *P) {
    Bus *temp;
    temp = (Bus *) malloc(sizeof(Bus));
    temp->num = num;
    temp->Date = date;
    strcpy(temp->time, time);
    strcpy(temp->busFromDes, from);
    strcpy(temp->busToDes, to);
    temp->price = price;
    temp->capacity = capacity;
    temp->space = capacity; //space initial value equal capacity
    temp->Next = P->Next;
    temp->StudentLinkedList = StuMakeEmpty(
            NULL);//every bus has a student linked list, so we make it when insert each bus.
    P->Next = temp;
}

void busPrintList(Bus *L) {
    Bus *P = L;
    if (busIsEmpty(L))
        printf("Empty list\n");
    else
        do { //loop while the last node
            P = P->Next;
            printf("Bus Number:%d\t", P->num);
            printf("Date:%d\t", P->Date);
            printf("Time:%s\t", P->time);
            printf("From:%s\t", P->busFromDes);
            printf("To:%s\t", P->busToDes);
            printf("Price:%.1f\t", P->price);
            printf("Capacity:%d\t", P->capacity);
            printf("Space:%d\n", P->space);
            printf("--------------------------------------------------------------------------\n");
        } while (!busIsLast(P, L));
    printf("\n");
}

void busDeleteList(Bus *L) { //delete bus list
    Bus *P;
    Bus *temp;
    P = L->Next;
    L->Next = NULL;
    while (P != NULL) {
        temp = P->Next;
        free(P);
        P = temp;
    }
}

int busSize(Bus *L) { //return how many nodes in bus linked list
    Bus *p = L->Next;
    int count = 0;
    while (p != NULL) {
        count += 1;
        p = p->Next;
    }
    return count;
}

//------------------------------------
void printMenu() { //print menu options.
    printf("Enter the number of the option you want to perform from the menu: \n");
    printf("1. Load the bus information file\n"
           "2. Load the passenger information file\n"
           "3. Assign passengers and print assignment information of all"
           "busses\n"
           "4. Print a specific bus information along with its passengers \n"
           "information (names and IDs)\n"
           "5. Print unmatched passengers\n"
           "6. Add new passenger\n"
           "7. Delete passenger\n"
           "8. Delete bus number\n"
           "9. Exit\n");
}

void loadBuses(Bus *bus) {
    FILE *input = fopen("busses.txt",
                        "r"); //open the input file and put it in read mode.
    char line[size]; //array for the lines(pointer);
    char *value;
    if (input == NULL) {
        printf("Can't opening this file.");
    }
    while (fgets(line, size, input)) {//read from the input file -->using fgets from string library.

        value = strtok(line, "#"); //split first value in line by #
        int num = atoi(value); //store it in num
        value = strtok(NULL, "#"); //split the next value from null because stroke but null character after first split
        int date = atoi(value);
        value = strtok(NULL, "#");
        char time[30];
        strcpy(time, value);
        value = strtok(NULL, "#");
        char from[30];
        strcpy(from, value);
        value = strtok(NULL, "#");
        char to[30];
        strcpy(to, value);
        value = strtok(NULL, "#");
        double price = atof(value);
        value = strtok(NULL, "#");
        value[strcspn(value, "\n")] = 0; //fgets but '\n' to string after read a full line so i remove it using strcnpn.
        int capacity = atoi(value);
        busInsert(num, date, time, from, to, price, capacity, bus, bus);
    }
    fclose(input); //close input file.
    busPrintList(bus);//print all buses info
}

void loadStudents(Student *mainStudentList) { //read passengers file and store it in student linked list
    FILE *input = fopen("passengers.txt",
                        "r"); //open the input file and put it in read mode.
    char line[size]; //array for the lines(pointer);
    char *value;
    if (input == NULL) { //if file doesn't exist.
        printf("Can't opening this file.");
    }
    while (fgets(line, size, input)) {//read from the input file -->using gets from string library.
        //split each line by #
        value = strtok(line, "#");
        int id = atoi(value);
        value = strtok(NULL, "#");
        int date = atoi(value);
        value = strtok(NULL, "#");
        char time[30];
        strcpy(time, value);
        value = strtok(NULL, "#");
        char from[30];
        strcpy(from, value);
        value = strtok(NULL, "#");
        char to[30];
        value[strcspn(value, "\n")] = 0;
        strcpy(to, value);
        StuInsert(id, date, time, from, to, mainStudentList, mainStudentList); //insert node.
    }
    fclose(input); //close input file.
    StuPrintList(mainStudentList);
}

void MainAssignStudents(Student *mainStudentList, Bus *bus,
                        Student *unmatchedStu) { //this method to assign each passenger in suitable bus or in unmatched linked list.
    Student *tempStu = mainStudentList->Next;
    while (tempStu != NULL) { //loop to pass over all students
        int flag = 0; //flag to know if the student assign in bus in exact time.
        int flag2 = 0;
        Bus *tempBus = bus->Next;//in every new loop we start from first node
        while (tempBus != NULL) {
            if (assignStu(tempStu, tempBus) == 1) { //call assign student function
                flag = 1;
                flag2 = 1;
                break; //exit the loop when assign success
            }
            tempBus = tempBus->Next; //if this bus not suitable go to the next bus.
        }
        if (flag2 == 0) { //if the first assign doesn't success  try if there is another bus soon
            tempBus = bus->Next;
            while (tempBus != NULL) {
                if (NotOnTimeAssign(tempStu, tempBus) == 1) {
                    flag = 1;
                    flag2 = 1;
                    break;
                }
                tempBus = tempBus->Next; //go to the next bus
            }
        }
        if (!(flag) && !(flag2)) { //if nether first assign nor second success but passenger in unmatched linked list .
            StuInsert(tempStu->id, tempStu->Date, tempStu->time,
                      tempStu->stuFromDes, tempStu->stuToDes, unmatchedStu, unmatchedStu);
        }
        tempStu = tempStu->Next; //go to the next student.
    }
}


int assignStu(Student *tempStu,
              Bus *tempBus) { //this method to check if the student node suitable the bus node via(date ,time,from ,to , and capacity available).

    if (tempBus->space == 0) //if there is no space available
        return 0;
    if (tempBus->Date == tempStu->Date && strcmp(tempStu->time, tempBus->time) == 0 &&
        strcmp(tempBus->busFromDes, tempStu->stuFromDes) == 0 && strcmp(tempBus->busToDes, tempStu->stuToDes) == 0) {
        tempBus->space -= 1; //decrement space available in bus by 1.
        StuInsert(tempStu->id, tempStu->Date, tempStu->time, tempStu->stuFromDes, tempStu->stuToDes,
                  tempBus->StudentLinkedList,
                  tempBus->StudentLinkedList); //insert passenger in bus student linked list .

        return 1;
    }
    return 0;
}

int NotOnTimeAssign(Student *tempStu,
                    Bus *tempBus) { //this method to check if the student node suitable bus node via (date,from,to,space) but time can be different but before time bus.
    int hourS, minS, hourB, minB;
    char *value;
    char time[6];
    //split time -->
    strcpy(time, tempStu->time);
    value = strtok(time, ":");
    hourS = atoi(value);
    value = strtok(NULL, ":");
    minS = atoi(value);
    strcpy(time, tempBus->time);
    value = strtok(time, ":");
    hourB = atoi(value);
    value = strtok(NULL, ":");
    minB = atoi(value);
    if (tempBus->space == 0)
        return 0;
    //student node will assign in first bus found with suitable time.
    if (tempBus->Date == tempStu->Date &&
        strcmp(tempBus->busFromDes, tempStu->stuFromDes) == 0 && strcmp(tempBus->busToDes, tempStu->stuToDes) == 0) {
        if (hourS < hourB || ((hourB == hourS) && (minS < minB))) {
            tempBus->space -= 1;
            StuInsert(tempStu->id, tempStu->Date, tempStu->time, tempStu->stuFromDes, tempStu->stuToDes,
                      tempBus->StudentLinkedList, tempBus->StudentLinkedList); //insert passenger in bus .

            return 1;
        }
    }
    return 0;
}

void printBusesPassengers(Bus *bus) { //this method will pass all buses nods and print its info with passengers.
    Bus *tempBus = bus->Next;
    int length = busSize(bus);
    for (int i = 0; i < length; i++) { //loop will pass all buses(bus linked list length)
        printf("--------------------------------------------------------------------------------\n");
        printf("Bus number %d:-->\n Date: %d , Time: %s ,From: %s ,To: %s ,Price: %.1f ,Capacity: %d \n", tempBus->num,
               tempBus->Date, tempBus->time, tempBus->busFromDes, tempBus->busToDes, tempBus->price, tempBus->capacity);
        printf("Passengers' information:\n ");
        if (StuIsEmpty(tempBus->StudentLinkedList)) { //if there is no student.
            printf("No passengers.\n");
        } else {
            StuPrintList(tempBus->StudentLinkedList);
        }
        tempBus = tempBus->Next; //go to the next bus.
    }
}

void printSpcBusInfo(Bus *bus,
                     int num) { //this bus take a bus number and found it then print its info with its passengers' info if exist .
    Bus *b1 = busFind(num, bus); //first find the bus node if exist using find function.
    if (b1 != NULL) {
        printf("-------------------------------------------------------------------------------------\n");
        printf("Bus Number: %d:-->\nDate: %d , Time: %s ,From: %s ,To: %s ,Price: %.1f ,Capacity: %d, Space: %d \n",
               b1->num,
               b1->Date, b1->time, b1->busFromDes, b1->busToDes, b1->price, b1->capacity, b1->space);
        printf("Passengers' information:\n ");
        if (StuIsEmpty(b1->StudentLinkedList)) {
            printf("No passengers.\n");
        } else {
            StuPrintList(b1->StudentLinkedList);
        }
    } else //if bus not exist.
        printf("Bus doesn't exist.\n");
}
