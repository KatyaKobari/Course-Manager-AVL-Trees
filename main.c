//Katya Kobari-1201478
//Date:28/5/2022.
//call lip
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  size 200 //max size of string.
#define fileSize 100 //size is the maximum number of char that we can read from file.
typedef struct AVLnode *AVLNode;
typedef struct Info { //declare struct info that have all data of course.
    char cName[size];
    int hours;
    char cCode[size];
    char topic[100];
    char department[size];
} info;
struct AVLnode { //declare node struct
    info data;
    AVLNode Left;
    AVLNode Right;
    int Height; //Balance information
};

//----------------------prototype stack functions-----------------------
int isempty();

int isFull();

void push();

char *pop();

//----------------------prototype menu options functions---------------------
int printSameDep(AVLNode mainTree, char dep[size]);

void pushCourseByDep(AVLNode mainTree, char dep[size]);

void pushCourseByLetter(AVLNode mainTree, char x);

AVLNode loadCourses(AVLNode mainTree);

AVLNode delCLetter(AVLNode mainTree);

void creatNameTree(AVLNode mainTree);

void save(AVLNode mainTree);

AVLNode delCDep(AVLNode mainTree);

void printInLexicographicOrder(AVLNode nameT);

AVLNode updateCourse(AVLNode mainTree);

void printPreOrderINFile(AVLNode node);

AVLNode InsertByName(info data, AVLNode T);

void printMenu();

//--------------------------prototype Avl tree functions---------------
AVLNode delete(char code[size], AVLNode node);

AVLNode MakeEmpty(AVLNode T);

int balance(AVLNode node);

AVLNode Find(char code[size], AVLNode T);

AVLNode FindMin(AVLNode T);

AVLNode FindMax(AVLNode T);

int Height(AVLNode P);

int Max(int Lhs, int Rhs);

AVLNode SingleRotateWithLeft(AVLNode K2);

AVLNode SingleRotateWithRight(AVLNode K1);

AVLNode DoubleRotateWithLeft(AVLNode K3);

AVLNode DoubleRotateWithRight(AVLNode K1);

AVLNode Insert(info data, AVLNode T);

void PrintInOrder(AVLNode t);

//----------------------- Declare global variables-----------------
FILE *output = NULL;
int top1 = -1;
char stack1[size][size]; //stack--->to store codes we want to delete it from tree.
AVLNode nameTree = NULL;
char line[500]; //for delete line when read .
int main() {
    AVLNode mainTree = NULL; //declare main avl tree(key=code).
    int choice;

    do {
        printMenu(); //show menu until the choice doesn't equal 11.
        scanf("%d", &choice);
        if (choice == 1) { //read data from file and creat an avl tree.
            mainTree = loadCourses(mainTree);
            PrintInOrder(mainTree);
        }
        if (choice == 2) { //insert new node  by user.
            info d;
            printf("Enter course name:");
            gets(line);
            gets(d.cName);
            printf("Enter credit hours:");
            scanf("%d", &d.hours);
            printf("Enter course code:");
            scanf("%s", d.cCode);
            printf("Enter department:");
            gets(line);
            gets(d.department);
            printf("Enter all the topics and put commas between them:");
            gets(d.topic);
            mainTree = Insert(d, mainTree);
        }
        if (choice == 3) {
            mainTree = updateCourse(mainTree);
            PrintInOrder(mainTree);
        }
        if (choice == 4) { //print courses inorder (key:name)
            creatNameTree(mainTree); //copy main tree to name tree and order it .
            printInLexicographicOrder(nameTree); //print data inorder(left,root,right).
            MakeEmpty(nameTree);//free-->copy the tree every time we choose 4 due to maybe different happen in tree.
        }
        if (choice == 5) {
            printf("Enter course code: ");
            char code[size];
            scanf("%s", code);
            AVLNode t = Find(code, mainTree); //find a node
            if (t == NULL) {
                printf("This course dose not exist.");
            } else {
                printf("Topics: %s", t->data.topic);//print its topic.
            }
        }
        if (choice == 6) {

            printf("Enter department: ");
            char dep[size];
            gets(line);
            gets(dep);
            int f = printSameDep(mainTree, dep);// print all courses that from the same department inorder
            if (f == 0) { //f==0 means that there is no department like that.
                printf("this department dose not exist.\n");
            }

        }
        if (choice == 7) {//delete a course by code.
            printf("Enter course code: ");
            char code[size];
            scanf("%s", code);
            mainTree = delete(code, mainTree);
            PrintInOrder(mainTree);
        }
        if (choice == 8) { //delete all courser that start with same latter(key:course name).
            char x;
            printf("Enter the letter: ");
            scanf(" %c", &x);
            pushCourseByLetter(mainTree, x); //push course codes the start with this latter.
            mainTree = delCLetter(mainTree); //delete them.
            PrintInOrder(mainTree);
        }
        if (choice == 9) { //delete all courses from the same department.
            char dep[size];
            printf("Enter department: ");
            gets(line);
            gets(dep);
            pushCourseByDep(mainTree, dep); //push course codes  from this department.
            mainTree = delCDep(mainTree); //delete them (pop)then delete from main tree.
            PrintInOrder(mainTree);
        }
        if (choice == 10) {
            save(mainTree); //save a new avl tree(after actions)in file.
        }
    } while (choice != 11); //Exit
    MakeEmpty(mainTree);
    if (choice == 11) {
        exit(0);
    }
    return 0;
}

//----------------------AVL TREE LIBRARY------------------------------
AVLNode MakeEmpty(AVLNode T) {
    if (T != NULL) {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

AVLNode Find(char code[size], AVLNode T) {
    if (T == NULL)
        return NULL;
    if (strcmp(code, T->data.cCode) < 0)
        return Find(code, T->Left);
    else if (strcmp(code, T->data.cCode) > 0)
        return Find(code, T->Right);
    else
        return T;
}

AVLNode FindMin(AVLNode T) {
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

AVLNode FindMax(AVLNode T) {
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;

    return T;
}

int Height(AVLNode P) {
    if (P == NULL)
        return -1;
    else
        return P->Height;
}

int Max(int Lhs, int Rhs) {
    return Lhs > Rhs ? Lhs : Rhs;
}

AVLNode SingleRotateWithLeft(AVLNode K2) {
    AVLNode K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), K2->Height) + 1;

    return K1;  /* New root */
}

AVLNode SingleRotateWithRight(AVLNode K1) {
    AVLNode K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Right), K1->Height) + 1;

    return K2;  /* New root */
}

AVLNode DoubleRotateWithLeft(AVLNode K3) {
    /* Rotate between K1 and K2 */
    K3->Left = SingleRotateWithRight(K3->Left);

    /* Rotate between K3 and K2 */
    return SingleRotateWithLeft(K3);
}

AVLNode DoubleRotateWithRight(AVLNode K1) {
    /* Rotate between K3 and K2 */
    K1->Right = SingleRotateWithLeft(K1->Right);

    /* Rotate between K1 and K2 */
    return SingleRotateWithRight(K1);
}

AVLNode Insert(info data, AVLNode T) {
    if (T == NULL) {
        /* Create and return a one-node tree */
        T = malloc(sizeof(struct AVLnode));
        if (T == NULL)
            printf("Out of space!!!");
        else {
            strcpy(T->data.cName, data.cName);
            T->data.hours = data.hours;
            strcpy(T->data.cCode, data.cCode);
            strcpy(T->data.topic, data.topic);
            strcpy(T->data.department, data.department);
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    } else if (strcmp(data.cCode, T->data.cCode) < 0) {
        T->Left = Insert(data, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
            //X < T->Left->Element
            if (strcmp(data.cCode, T->Left->data.cCode) < 0)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
    } else if (strcmp(data.cCode, T->data.cCode) > 0) {
        T->Right = Insert(data, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
            //X > T->Right->Element
            if (strcmp(data.cCode, T->Right->data.cCode) > 0) { T = SingleRotateWithRight(T); }
            else { T = DoubleRotateWithRight(T); }
    }
    /* Else X is in the tree already; we'll do nothing */

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

void PrintInOrder(AVLNode mainTree) { //print nodes (left,root,right).
    if (mainTree != NULL) {
        PrintInOrder(mainTree->Left);
        printf("Course: %s,Credit Hours:%d, Course Code: %s ,Department: %s,Topics: %s \n", mainTree->data.cName,
               mainTree->data.hours, mainTree->data.cCode, mainTree->data.department, mainTree->data.topic);
        printf("----------------------------------------------------------------------------\n");
        PrintInOrder(mainTree->Right);
    }
}

void printMenu() { //show menu
    printf("1. Read the file courses.txt file and create the tree.\n"
           "2. Insert a new course .\n"
           "3. Find a course and support updating of its information. \n"
           "4. List courses in lexicographic order with their associated\n"
           "information (credit hours, IDs, and topics). \n"
           "5. List all topics associated with a given course. \n"
           "6. List all courses in lexicographic order that belong to the same \n"
           "department.\n"
           "7. Delete a course.\n"
           "8. Delete all courses that start with a specific letter. \n"
           "9. Delete all courses belong to a given department.\n"
           "10. Save all courses in file offered_courses.txt\n"
           "11.Exit\n"
    );
}

AVLNode loadCourses(AVLNode mainTree) {
    info data; //data for every line we read.
    FILE *input = fopen("courses.txt",
                        "r"); //open the input file and put it in read mode.
    char line[fileSize]; //array for the lines(pointer);
    char *value;
    if (input == NULL) {
        printf("Can't opening this file.");
    }
    //while we dont reach the end of file read line by line and split data then store in  data filed.
    while (fgets(line, 1000, input)) {//read from the input file -->using fgets from string library.
        value = strtok(line, ":"); //split first value in line by :
        strcpy(data.cName, value);
        value = strtok(NULL, "#");
        data.hours = atoi(value);
        value = strtok(NULL, "#");
        strcpy(data.cCode, value);
        value = strtok(NULL, "/");
        value[strcspn(value, "\n")] = 0;
        strcpy(data.department, value);
        value = strtok(NULL, "\n");
        value[strcspn(value, "\n")] = 0;
        strcpy(data.topic, value);
        mainTree = Insert(data, mainTree); //insert new node(by call insert method).
    }
    fclose(input); //close input file.
    return mainTree;
}

AVLNode updateCourse(AVLNode mainTree) { //update by code value.
    AVLNode t1; //for the node we want to update it.
    char code[20];
    printf("Enter course code: ");
    scanf("%s", code);
    t1 = Find(code, mainTree); //find the node by call find method from avl tree lip.
    if (t1 == NULL) {
        printf("Course dose not exist.\n");
    } else {
        int c;
        do {
            printf("1.Update course name\n");
            printf("2.Update course hours\n");
            printf("3.Update topics\n");
            printf("4.Update course department\n");
            printf("5.Update course code\n");
            printf("6.Exist\n");
            scanf("%d", &c);
            if (c == 1) { //upadte name
                printf("Enter a new course name: ");
                scanf("%s", t1->data.cName);
            }
            if (c == 2) { //update hours.
                printf("Enter hours: ");
                scanf("%d", &t1->data.hours);
            }
            if (c == 3) {//update topics
                printf("Enter a new topics: ");
                scanf("%s", t1->data.topic);
            }
            if (c == 4) { //update department
                printf("Enter department: ");
                scanf("%s", t1->data.department);
            }
            if (c == 5) { //update code
                printf("Enter a new code: ");
                char code1[size];
                scanf("%s", code1);
                info d = t1->data;
                strcpy(d.cCode, code1);
                mainTree = delete(code, mainTree); //delete the current node from tree
                mainTree = Insert(d, mainTree); //then add it again with a new code(so its order in tree will change).
            }

        } while (c != 6);
    }
    return mainTree;
}

AVLNode delete(char code[size], AVLNode node) {


    if (node == NULL) {
        printf("Course dose not exist.\n");
        return node;
    } else if (strcmp(code, node->data.cCode) > 0)
        node->Right = delete(code, node->Right);//if the  node is more than the parent
    else if (strcmp(code, node->data.cCode) < 0)
        node->Left = delete(code, node->Left);//if the  node is less than the parent
    else //when found the node
    {


        if (node->Right != NULL && node->Left != NULL) //when there is 2 child .
        {

            AVLNode minRight = FindMin(node->Right);
            AVLNode temp = minRight;

            //replace the deleted node
            node->data = minRight->data;

            //free the Successor node
            node->Right = delete(minRight->data.cCode, node->Right);

        } else if (node->Right == NULL && node->Left != NULL) //for left child
        {

            AVLNode temp = node;
            node = node->Left;
            free(temp);

        } else if (node->Right != NULL && node->Left == NULL) //for right child.
        {

            AVLNode temp = node;
            node = node->Right;
            free(temp);

        } else //in leaf node case
        {

            AVLNode temp = node;
            node = NULL;
            free(temp);
            return node;

        }
        node->Height = Max(Height(node->Right), Height(node->Left)) + 1;  //update the height
        int balanced = balance(node);
        // If this node becomes unbalanced we will balance it using 4 cases.


        //right rotation case
        if (balanced > 1 && (strcmp(code, node->Left->data.cCode) < 0))
            node = SingleRotateWithRight(node);

        //left rotation case
        if (balanced < -1 && (strcmp(code, node->Right->data.cCode) > 0))
            node = SingleRotateWithLeft(node);

        // right left rotation case .
        if (balanced < -1 && (strcmp(node, node->Right->data.cCode) < 0)) {
            node->Right = SingleRotateWithRight(node->Right); // -->become right right
            node = SingleRotateWithLeft(node);

        }

        // left right rotation case .
        if (balanced > 1 && (strcmp(code, node->Left->data.cCode) > 0)) {
            node->Left = SingleRotateWithLeft(node->Left);// -->become left left.
            node = SingleRotateWithRight(node);
        }


    }


    return node;
}

int balance(AVLNode node) { //return balance for the node

    if (node == NULL)
        return 0;

    return Height(node->Left) - Height(node->Right); //balance equation.

}

void printPreOrderINFile(AVLNode mainTree) { //print nodes preorder(root,left,right) in file

    if (mainTree != NULL) {
        fprintf(output, "%s:%d#%s#%s/%s\n", mainTree->data.cName,
                mainTree->data.hours, mainTree->data.cCode, mainTree->data.department, mainTree->data.topic);
        printPreOrderINFile(mainTree->Left);
        printPreOrderINFile(mainTree->Right);
    }

}

void save(AVLNode mainTree) { //-->print an avl tree in file in preorder
    output = fopen("offered_courses.txt", "w"); //open file and put it in write mood.
    if (mainTree == NULL) { //if the tree is null.
        printf("Empty avl tree\n");
    } else {
        printPreOrderINFile(mainTree); //call the print method to print the tree in file
        fclose(output); //close output file.
    }

}

AVLNode InsertByName(info data,
                     AVLNode T) { //this method is same as insert methode but the key is name(i need it to copy the main tree to the name tree).
    if (T == NULL) {
        /* Create and return a one-node tree */
        T = malloc(sizeof(struct AVLnode));
        if (T == NULL)
            printf("Out of space!!!");
        else {
            strcpy(T->data.cName, data.cName);
            T->data.hours = data.hours;
            strcpy(T->data.cCode, data.cCode);
            strcpy(T->data.topic, data.topic);
            strcpy(T->data.department, data.department);
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    } else if (strcmp(data.cName, T->data.cName) < 0) {
        T->Left = Insert(data, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
            //X < T->Left->Element
            if (strcmp(data.cName, T->Left->data.cName) < 0)
                T = SingleRotateWithLeft(T);
            else { T = DoubleRotateWithLeft(T); }
    } else if (strcmp(data.cName, T->data.cName) > 0) {
        T->Right = Insert(data, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
            //X > T->Right->Element
            if (strcmp(data.cName, T->Right->data.cName) > 0) { T = SingleRotateWithRight(T); }
            else { T = DoubleRotateWithRight(T); }
    }
    /* Else X is in the tree already; we'll do nothing */

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

void creatNameTree(AVLNode mainTree) { //traversal node node in mainTree and insert each node in name tree.

    if (mainTree != NULL) {
        creatNameTree(mainTree->Left);
        nameTree = InsertByName(mainTree->data, nameTree); //call insertByName method to insert the node.
        creatNameTree(mainTree->Right);
    }
}

void printInLexicographicOrder(AVLNode nameT) { //print node inorder(left,root,right).

    if (nameT == NULL) //Null tree case
        printf("Empty tree.\n");
    else
        PrintInOrder(nameT); //call print method

}

int printSameDep(AVLNode mainTree, char dep[size]) { //this method print all courses from same department inorder.
    int flag = 0; //if 0->dose not exit ,1->exist
    if (mainTree != NULL) {
        printSameDep(mainTree->Left, dep);
        if (strcmp(dep, mainTree->data.department) ==
            0) { //when the department equal any course department print its information.
            printf("Course: %s,Credit Hours:%d, Course Code: %s ,Department: %s,Topics: %s \n", mainTree->data.cName,
                   mainTree->data.hours, mainTree->data.cCode, mainTree->data.department, mainTree->data.topic);
            printf("--------------------------------------------------------------------------------------\n");
            flag = 1;
        }
        printSameDep(mainTree->Right, dep);


    }
    return flag;
}

void pushCourseByDep(AVLNode mainTree,
                     char dep[size]) {  //-->to push all codes for all courses that have the same department to the stack1 to delete it later.


    if (mainTree != NULL) {

        if (strcmp(dep, mainTree->data.department) == 0) {
            char code[size];
            strcpy(code, mainTree->data.cCode);
            push(code);//push code
        }
        pushCourseByDep(mainTree->Left, dep);
        pushCourseByDep(mainTree->Right, dep);
    }

}

AVLNode delCDep(AVLNode mainTree) { //pop codes from stack then delete them one by one
    if (isempty()) {
        printf("This department dose not exist.\n");
    } else {
        while (!isempty()) {
            mainTree = delete(pop(), mainTree); //pop and delete .
        }
    }
    return mainTree;
}

void pushCourseByLetter(AVLNode mainTree,
                        char x) { //-->find all courses that's start of specific later and push its code in the stack to delete them later.


    if (mainTree != NULL) {
        char m = mainTree->data.cName[0]; //store fist char if the course name in m.
        if (x == m) { //compare characters.
            char code[size];
            strcpy(code, mainTree->data.cCode);
            push(code); //push node code if the condition is true.
        }
        pushCourseByLetter(mainTree->Left, x);
        pushCourseByLetter(mainTree->Right, x);
    }

}

AVLNode delCLetter(AVLNode mainTree) { //-->pop from stack then delete node from the mainTree.
    if (isempty()) { //if empty ->there is no coursers start with this latter.
        printf("There are no courses that start with this letter .\n");
    } else {
        while (!isempty()) {
            mainTree = delete(pop(), mainTree); //pop the code then delete it.
        }
    }
    return mainTree;
}

//------------------------------stack lib-----------------------------
int isFull() {
    if (top1 == size - 1) { //is full case-> then the top will equal the size-1.
        return 1;
    } else {
        return 0;
    }
}

int isempty() {
    if (top1 == -1) { //when top=-1 ->that means stack is empty.
        return 1;
    } else {
        return 0;
    }
}

void push(char code[]) { //send a code and push it in the stack;
    if (isFull()) {
        printf("Stack is full");
    } else {
        top1++;//increment top by one to add in the top of the stack
        strcpy(stack1[top1], code); //add it.
    }
}

char *pop() { //pop code from the top and return its value
    if (isempty()) {
        printf("Stack is empty");
    } else {
        char *c = malloc(size + 1); //store data in the heap of the memory.
        strcpy(c, stack1[top1]);//copy the top value to c .
        top1--;//pop
        return c; //return code
    }
}

