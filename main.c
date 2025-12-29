//Name: Ahmad Bahaa Hamad
//Student ID: 1230618
//Section: 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASH_SIZE 401// prime number
//Status
#define EMPTY 0
#define OCCUPIED 1
#define DELETED 2
int hashOccupied;

//course
typedef struct course{
    char courseCode[20];
    char courseTitle[50];
    int creditHours;
    char semester[20];
    struct course* next;
}course;
typedef course* coursePtr;
//student hash
typedef struct{
    char name[50]; //used as key for hash
    int id;
    char major[50];
    int status;//0: EMPTY, 1:OCCUPIED, 2: DELETED
    coursePtr courses;
}HashEntry;
HashEntry hashTable[HASH_SIZE];
//student
typedef struct student{
    char name[50];
    int id;
    char major[50];
    coursePtr courses;

    struct student* left;
    struct student* right;
    int height;
}student;
typedef student* studentPtr;

// File loading functions
studentPtr loadFileAVL(studentPtr root);
void loadFileHASH();

// Print and display functions
void printMenu();
void printCourseStudents(studentPtr root, char* courseTitle);
void printHashedTable();

// Registration functions
studentPtr newRegistrationAVL(studentPtr root);
void newRegistrationHASH();

// Update functions
studentPtr findAndUpdate(studentPtr root, char* name);

// AVL tree functions
int height(studentPtr node);
studentPtr rotateRight(studentPtr k2);
studentPtr rotateLeft(studentPtr k1);
studentPtr DLR(studentPtr k3);
studentPtr DRL(studentPtr k3);
studentPtr findByName(studentPtr student, char* name);
studentPtr findByID(studentPtr root, int id);
studentPtr findMin(studentPtr root);
studentPtr findMax(studentPtr root);
studentPtr insertStudentAVL(studentPtr root, studentPtr newStudent);
studentPtr rebalance(studentPtr root);
studentPtr deleteStudent(studentPtr root, int id);
void freeAVL(studentPtr root);

// File save functions
void AVLtoFILE(studentPtr root, FILE *file);
void HASHtoFILE();

// Linked list functions
void insertCourseAtHead(studentPtr student, coursePtr newCourse);
coursePtr searchCourse(studentPtr student, char* courseTitle);
void insertCourseAtHeadHash(coursePtr newCourse, int hash);
coursePtr searchCourseHash(int hash, char* courseCode);
void freeCourseList(coursePtr head);
// Hash table functions
int hashFunction(char* key);
void initializeHash();
int insertStudentHash(studentPtr student);
int searchStudentHash(char* name);
int deleteStudentHash(char* name);
char* returnStatus(int status);

// Utility functions
void readLine(char *buffer, int size);
int max(int a,int b);

int main(){
    studentPtr root = NULL;
    int choice;
    int hashCreated = 0;
    initializeHash();
    while(1){
        printMenu();
        scanf("%d",&choice);
        char name[50];
        char courseTitle[50];
        int id;
        switch(choice){
            case 1:
                root = loadFileAVL(root);
            break;
            case 2:
                root = newRegistrationAVL(root);
            break;
            case 3:
                printf("\nEnter student's name: ");
                getchar();//consume newline
                readLine(name,sizeof(name));
                root = findAndUpdate(root,name);
            break;
            case 4:
                printf("\nEnter the course's title: ");
                getchar();//consume newline
                readLine(courseTitle,sizeof(courseTitle));
                printCourseStudents(root,courseTitle);
            break;
            case 5:
                printf("\nEnter student's ID: ");
                scanf("%d",&id);
                root = deleteStudent(root,id);
            break;
            case 6:
                FILE* file = fopen("students_hash.data","w");
                if(file==NULL){
                    printf("\nError opening file!");
                }else{
                    if(root!=NULL){//tree exists
                    AVLtoFILE(root,file);
                    fclose(file);
                    //file created, now create the hash table
                    initializeHash();
                    loadFileHASH();
                    hashCreated = 1;
                    }
                    else{//tree doesn't exist, don't create file
                        printf("\nThere's no data to create hash, returning!");
                        fclose(file);
                        break;
                    }
                }
            break;
            case 7:
                if(hashCreated){
                    printHashedTable();
                }
                else{
                    printf("\nHash doesn't exist, try again after initializing hash table!");
                }
            break;
            case 8:
                if(hashCreated){
                    printf("\nTable Size: %d",HASH_SIZE);
                    printf("\nOccupied Space: %d",hashOccupied);
                    printf("\nEmpty Space: %d", (HASH_SIZE-hashOccupied));
                }
                else{
                    printf("\nHash doesn't exist, try again after initializing hash table!");
                }
            break;
            case 9:
                    printf("\nUsed hash function:");
                    printf("\nHashValue = ((HashValue << 5) + *key) %% HASH_SIZE");
            break;
            case 10://Insert a new record into the hash table
                if(hashCreated){
                    newRegistrationHASH();
                }
                else{
                    printf("\nHash doesn't exist, try again after initializing hash table!");
                }
            break;
            case 11://search for student in hash
                if(hashCreated){
                    printf("\nPlease Enter the student's name: ");
                    getchar();
                    readLine(name,sizeof(name));
                    int hashed = searchStudentHash(name);
                    //if student found, print their info
                    if(hashed!=-1){
                        printf("\nIndex: %d | ",hashed);
                        printf("Name: %s | ",hashTable[hashed].name);
                        printf("ID: %d | ",hashTable[hashed].id);
                        printf("Major: %s | ",hashTable[hashed].major);
                        printf("Status: %s | ",returnStatus(hashTable[hashed].status));
                        printf("\nCourses: \n");
                        coursePtr temp = hashTable[hashed].courses;
                        while(temp){
                            printf("Course Title: %s | ",temp->courseTitle);
                            printf("Code: %s | ",temp->courseCode);
                            printf("Credit Hours: %d | ",temp->creditHours);
                            printf("Semester: %s | ",temp->semester);
                            printf("\n");
                            temp = temp->next;
                        }
                    }
                    else{
                        printf("\nStudent not found!");
                        break;
                    }
                }
                else{
                    printf("\nHash doesn't exist, try again after initializing hash table!");
                }
            break;
            case 12:
                if(hashCreated){
                    printf("\nPlease enter the student's name: ");
                    getchar();
                    readLine(name,sizeof(name));
                    int temp = deleteStudentHash(name);
                    if(temp==-1){
                        printf("\nStudent not found!");
                    }
                    else{
                        printf("\nStudent deleted successfully");
                    }
                }
                else{
                    printf("\nHash doesn't exist, try again after initializing hash table!");
                }
            break;
            case 13:
                if(hashCreated){
                    HASHtoFILE();
                }
                else{
                    printf("\nHash doesn't exist, try again after initializing hash table!");
                }
            break;
            case 14:
                printf("\nGoodbye!");
                freeAVL(root);
                if(hashCreated){//free courses in hash table
                    for(int i=0; i<HASH_SIZE; i++){
                        freeCourseList(hashTable[i].courses);
                    }
                }
                return 1;
                break;
            default:
                printf("\nPlease enter a valid option");
            break;
        }
    }
    return 1;
}
studentPtr loadFileAVL(studentPtr root){
    FILE *file = fopen("reg.txt","r");

    if(file==NULL){
        printf("\nError reading file");
        return root;
    }
    char line[255];
    //int lineNum = 0;
    while(fgets(line,sizeof(line),file)){
        line[strcspn(line,"\n")] = '\0';//replace newline with null at the end to make sure there are no issues reading the line
        //for debugging
        //lineNum++;

        //printf("\nProcessing line %d: %s",lineNum,line);
        //make new student and initiate its data
        studentPtr newStudent = (studentPtr) malloc(sizeof(student));
        if(newStudent==NULL){
            printf("\nError allocating memory to the student!");
            fclose(file);
            return root;
        }
        newStudent->left = newStudent->right = NULL;
        newStudent->height = 1;
        newStudent->courses = NULL;
        //make new course and initiate its data
        coursePtr newCourse = (coursePtr) malloc(sizeof(course));
        if(newCourse==NULL){
            printf("\nError allocating memory to the course!");
            fclose(file);
            return root;
        }
        //assign name
        char *token = strtok(line,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newStudent->name,token);
        //assign id
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        newStudent->id = atoi(token);
        //assign major
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newStudent->major,token); 
        //assign course code
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newCourse->courseCode,token);
        //assign course title
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newCourse->courseTitle,token);
        //assign credit hours
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        newCourse->creditHours = atoi(token);
        //assign semester
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newCourse->semester,token);
        
        studentPtr checker = findByName(root,newStudent->name);//if student already exists, find them and add the new course to them

        //insert course into student's course list as a new student
        if(checker==NULL){//student doesn't exist
            insertCourseAtHead(newStudent,newCourse);
            //insert student into the tree
            root = insertStudentAVL(root,newStudent);
        }
        else{//student already exists
            insertCourseAtHead(checker,newCourse);
            free(newStudent);//not needed so free it
            //don't insert into tree since student already exists
        }
    }
    fclose(file);
    printf("\nFile read successfully");
    return root;
}
void loadFileHASH(){
    FILE *file = fopen("students_hash.data","r");

    if(file==NULL){
        printf("\nError reading file!");
        return;
    }
    char line[255];
    while(fgets(line,sizeof(line),file)){
        line[strcspn(line, "\n")] = '\0';//replace newline with null at the end to make sure there are no issues reading the line
        studentPtr newStudent = (studentPtr) malloc(sizeof(student));
        if(!newStudent){
            printf("\nError allocating memory to the student!");
            fclose(file);
            return;
        }
        newStudent->left = newStudent->right = NULL;
        newStudent->height = 1;
        newStudent->courses = NULL;
        //make new course and initiate its data
        coursePtr newCourse = (coursePtr) malloc(sizeof(course));
        if(!newCourse){
            printf("\nError allocating memory to the course!");
            fclose(file);
            return;
        }
        //assign name
        char *token = strtok(line,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newStudent->name,token);
        //assign id
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        newStudent->id = atoi(token);
        //assign major
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newStudent->major,token); 
        //assign course code
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newCourse->courseCode,token);
        //assign course title
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newCourse->courseTitle,token);
        //assign credit hours
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        newCourse->creditHours = atoi(token);
        //assign semester
        token = strtok(NULL,"#");
        if(token == NULL){ 
            printf("\nBroken line, skip");
            free(newStudent);
            free(newCourse);
            continue; 
        }
        strcpy(newCourse->semester,token);

        //check if student already exists
        int index = searchStudentHash(newStudent->name);

        if(index!=-1){//student exists, add the course to them
            insertCourseAtHeadHash(newCourse,index);
            free(newStudent);//student pointer isn't needed for hash table
        }
        else{//add student to hash table and attach their courses
            int actualIndex = insertStudentHash(newStudent);
            if(actualIndex==-1){
                printf("\nCouldn't insert student, returning!");
                free(newCourse);
                free(newStudent);
                return;
            }
            insertCourseAtHeadHash(newCourse,actualIndex);
            free(newStudent);//student pointer isn't needed for hash table
        }
    }
    fclose(file);
    printf("\nFile read successfully");
    return;
}
void printCourseStudents(studentPtr root, char* courseTitle){
    if(root==NULL){
        return;
    }
    //search through the whole tree and when finding that the student has the course, print their name
    printCourseStudents(root->left,courseTitle);

    //Visit (print)
    if(searchCourse(root,courseTitle)!=NULL){
        printf("\nName: %s | ID: %d",root->name,root->id);
    }
    printCourseStudents(root->right,courseTitle);
}
studentPtr newRegistrationAVL(studentPtr root){
    printf("\nDo you want to register a course for a new student or an already existing one?");
    printf("\nEnter 0 for existing student or 1 for new one: ");
    int new;
    scanf("%d",&new);
    if(new==1){//Student doesn't exist, add them and their courses
        studentPtr newStudent = (studentPtr) malloc(sizeof(student));
        if(newStudent==NULL){
            printf("\nError allocating memory to the student!");
            return root;
        }
        newStudent->left = NULL;
        newStudent->right = NULL;
        newStudent->height = 1;
        newStudent->courses = NULL;
        printf("\nEnter the student's info");
        printf("\nName: ");
        getchar();//consume newline
        readLine(newStudent->name,sizeof(newStudent->name));
        printf("\nID: ");
        scanf("%d",&newStudent->id);
        printf("\nMajor: ");
        getchar(); //consume newline
        readLine(newStudent->major,sizeof(newStudent->major));

        if(findByID(root,newStudent->id)!=NULL){
            printf("\nStudent ID already exists!");
            free(newStudent);
            return root;
        }

        printf("\nEnter the student's courses info\n");
        int more = 1;
        while(more==1){
            coursePtr newCourse = (coursePtr) malloc(sizeof(course));
            if(!newCourse){
                printf("\nError allocating memory to the course!");
                
                free(newStudent);
                return root;
            }
            printf("\nEnter the course's title: ");
            readLine(newCourse->courseTitle,sizeof(newCourse->courseTitle));
            printf("\nEnter the course's code: ");
            scanf("%s",newCourse->courseCode);
            printf("\nEnter the course's credit hours: ");
            scanf("%d",&newCourse->creditHours);
            printf("\nEnter the course's semester: ");
            getchar(); //consume newline
            readLine(newCourse->semester,sizeof(newCourse->semester));
            //Insert the course at the student's course list
            insertCourseAtHead(newStudent,newCourse);
            printf("\nEnter 1 if you want to enter a new course and 0 otherwise: ");
            scanf("%d",&more);
            getchar();//consume newline
        }
        //after finishing, add the student
        root = insertStudentAVL(root,newStudent);
        return root;
    }
    else if(new==0){//Student exists, find them and add courses
        int id;
        printf("\nEnter the student's ID: ");
        scanf("%d",&id);
        //Find the student
        studentPtr temp = findByID(root,id);

        if(temp==NULL){
            printf("\nError, student not found!");
            return root;
        }
        //if found enter the student's courses
        printf("\nEnter the student's courses info\n");
        int more = 1;
        while(more==1){
            coursePtr newCourse = (coursePtr) malloc(sizeof(course));
            if(!newCourse){
                printf("\nError allocating memory to the course!");
                return root;
            }
            printf("\nEnter the course's title: ");
            readLine(newCourse->courseTitle,sizeof(newCourse->courseTitle));
            printf("\nEnter the course's code: ");
            scanf("%s",newCourse->courseCode);
            if(searchCourse(temp,newCourse->courseTitle)!=NULL){//if course is found (already exists) abort
                printf("\nCourse already registered, returning!");
                free(newCourse);
                return root;
            }
            printf("\nEnter the course's credit hours: ");
            scanf("%d",&newCourse->creditHours);
            getchar(); //consume newline
            printf("\nEnter the course's semester: ");
            readLine(newCourse->semester,sizeof(newCourse->semester));
            //Insert the course at the student's course list
            insertCourseAtHead(temp,newCourse);
            printf("\nEnter 1 if you want to enter a new course and 0 otherwise: ");
            scanf("%d",&more);
            getchar();//consume newline
        }
        return root;
    }
    else{
        printf("\nInvalid option, returning!");
        return root;
    }
}
void newRegistrationHASH(){
    printf("\nDo you want to register a course for a new student or an already existing one?");
    printf("\nEnter 0 for existing student or 1 for new one");
    int new;
    scanf("%d",&new);
    if(new==1){//Student doesn't exist, add them and their courses
        studentPtr newStudent = (studentPtr) malloc(sizeof(student));
        if(!newStudent){
            printf("\nError allocating memory to the student!");
            return;
        }
        newStudent->left = NULL;
        newStudent->right = NULL;
        newStudent->height = 1;
        newStudent->courses = NULL;
        printf("\nEnter the student's info");
        printf("\nName: ");
        getchar();
        readLine(newStudent->name,sizeof(newStudent->name));
        printf("\nID: ");
        scanf("%d",&newStudent->id);
        printf("\nMajor: ");
        getchar(); //consume newline
        readLine(newStudent->major,sizeof(newStudent->major));
        
        //check if student already exists
        int hashed = searchStudentHash(newStudent->name);
        if(hashed!=-1){//student already exists, return
            printf("\nStudent already exists! returning");
            free(newStudent);
            return;
        }
        //else student doesn't exist so add them and continue
        int actualIndex = insertStudentHash(newStudent);
        if(actualIndex==-1){
            printf("\nCouldn't insert student into hash table!");
            free(newStudent);
            return;
        }
        //Add courses to student
        printf("\nEnter the student's courses info\n");
        int more = 1;
        while(more==1){
            coursePtr newCourse = (coursePtr) malloc(sizeof(course));
            if(!newCourse){
                printf("\nError allocating memory to the course!");
                return;
            }
            printf("\nEnter the course's title: ");
            readLine(newCourse->courseTitle,sizeof(newCourse->courseTitle));
            printf("\nEnter the course's code: ");
            scanf("%s",newCourse->courseCode);
            printf("\nEnter the course's credit hours: ");
            scanf("%d",&newCourse->creditHours);
            printf("\nEnter the course's semester: ");
            getchar(); //consume newline
            readLine(newCourse->semester,sizeof(newCourse->semester));
            //Insert the course at the student's course list
            insertCourseAtHeadHash(newCourse,actualIndex);
            printf("\nEnter 1 if you want to enter a new course and 0 otherwise: ");
            scanf("%d",&more);
            getchar();//consume newline
        }
        free(newStudent);//hash duplicates the data so we don't need the pointer
        return;
    }
    else if(new==0){//Student exists, find them and add courses
        char name[50];
        printf("\nEnter the student's name: ");
        getchar();
        readLine(name,sizeof(name));
        //Find the student
        int hashed = searchStudentHash(name);
        if(hashed!=-1){
            //if found enter the student's courses
            printf("\nEnter the student's courses info\n");
            int more = 1;
            while(more==1){
                coursePtr newCourse = (coursePtr) malloc(sizeof(course));
                if(!newCourse){
                    printf("\nError allocating memory to the course!");
                    return;
                }
                printf("\nEnter the course's title: ");
                readLine(newCourse->courseTitle,sizeof(newCourse->courseTitle));
                printf("\nEnter the course's code: ");
                scanf("%s",newCourse->courseCode);
                if(searchCourseHash(hashed,newCourse->courseCode)!=NULL){//if course is found (already exists) abort
                    printf("\nCourse already registered, returning!");
                    free(newCourse);
                    return;
                }
                printf("\nEnter the course's credit hours: ");
                scanf("%d",&newCourse->creditHours);
                printf("\nEnter the course's semester: ");
                getchar();//consume newline
                readLine(newCourse->semester,sizeof(newCourse->semester));
                //Insert the course at the student's course list
                insertCourseAtHeadHash(newCourse,hashed);
                printf("\nEnter 1 if you want to enter a new course and 0 otherwise: ");
                scanf("%d",&more);
                getchar();//consume newline
            }
        }
        else{
            printf("\nStudent doesn't exist! returning");
            return;
        }
    }
    else{
        printf("\nInvalid option, returning!");
        return;
    }
}
void printMenu(){
    printf("\n\n1. Load data from file into AVL tree");
    printf("\n2. Insert new registration into AVL tree");
    printf("\n3. Find student in AVL tree and update info");
    printf("\n4. List all students in the same course");
    printf("\n5. Delete student from AVL tree");
    printf("\n6. Create hash");
    printf("\n7. Print hashed table");
    printf("\n8. Print table size");
    printf("\n9. Print used hash function");
    printf("\n10. Insert new record into hash table");
    printf("\n11. Search for student in hash table");
    printf("\n12. Delete from hash");
    printf("\n13. Save hash into file");
    printf("\n14. Exit\n");
}
studentPtr findAndUpdate(studentPtr root,char* name){
    studentPtr std = findByName(root,name);
    if(std){
        printf("\nStudent %s was found, do you want to update their info?\nEnter 1 for yes or 0 for no: ",name);
        int choice = 0;
        scanf("%d",&choice);
        if(choice==1){
            printf("\nEnter student's Name: ");
            getchar();
            readLine(std->name,sizeof(std->name));
            printf("\nEnter student's Major: ");
            scanf("%s",std->major);
            printf("\nInfo updated, returning");
            return root;
        }
        else{
            return root;//return updated root
        }
    }
    //student not found
    printf("\nStudent wasn't found, returning");
    return root;
}
//AVL Functions
int height(studentPtr node){
    if(node==NULL){
        return -1;
    }
    return node->height;
}
studentPtr rotateRight(studentPtr k2){
    studentPtr k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = max(height(k2->left),height(k2->right)) + 1;
    k1->height = max(height(k1->left),height(k1->right)) + 1;
   
   return k1;
}
studentPtr rotateLeft(studentPtr k1){
    studentPtr k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    
    k1->height = max(height(k1->left),height(k1->right)) + 1;
    k2->height = max(height(k2->left),height(k2->right)) + 1;

    return k2;
}
studentPtr DLR(studentPtr k3){
    k3->left = rotateLeft(k3->left);
    return rotateRight(k3);
}
studentPtr DRL(studentPtr k3){
    k3->right = rotateRight(k3->right);
    return rotateLeft(k3);
}
studentPtr findByName(studentPtr student, char* name){
    if(student == NULL) return NULL;

    //search left subtree
    studentPtr leftResult = findByName(student->left, name);
    if(leftResult != NULL){
        return leftResult;
    }
    //check current node
    if(strcmp(student->name, name) == 0){
        return student;
    }
    //search right subtree
    return findByName(student->right, name);
}
studentPtr findByID(studentPtr root, int id){
    if(root == NULL){
        return NULL;
    }
    else if(id<root->id){
        return findByID(root->left,id);
    }
    else if(id>root->id){
        return findByID(root->right,id);
    }
    else 
        return root;
}
studentPtr findMin(studentPtr root){
    if(root == NULL){
        return NULL;
    }
    else if(root->left == NULL) {
        return root;
    }
    else{
        return findMin(root->left);
    }
}
studentPtr findMax(studentPtr root){
    if(root == NULL){
        return NULL;
    }
    else if(root->right == NULL) {
        return root;
    }
    else{
        return findMax(root->right);
    }
}
studentPtr insertStudentAVL(studentPtr root, studentPtr newStudent){
    //empty spot, newStudent becomes the new root
    if(root==NULL){
        return newStudent;
    }
    if(newStudent->id < root->id){
        root->left = insertStudentAVL(root->left,newStudent);
    }
    else if(newStudent->id > root->id){
        root->right = insertStudentAVL(root->right,newStudent);
    }
    else{
        //id already exists, don't insert
        return root;
    }
    //Update height
    root->height = max(height(root->left),height(root->right)) + 1;
    //rebalance after inserting
    return rebalance(root);
}
studentPtr rebalance(studentPtr root){
    if(root == NULL){
        return NULL;
    }
    //update height (to make sure it's right)
    root->height = 1 + max(height(root->left),height(root->right));

    int balance = height(root->left) - height(root->right);
    //left is heavier than right
    if(balance > 1){
        //extra weight from left left (simple right rotation needed)
        if(height(root->left->left) >= height(root->left->right)){
            return rotateRight(root);
        }
        //extra weight from left right (double rotation needed)
        else{
            return DLR(root);
        }
    }
    //right is heavier than left
    if(balance < -1){
        //extra weight from right right (simple left rotation needed)
        if(height(root->right->right) >= height(root->right->left)){
            return rotateLeft(root);
        }
        //extra weight from right left (double rotation needed)
        else{
            return DRL(root);
        }
    }
    //Already balanced
    return root;
}
studentPtr deleteStudent(studentPtr root, int id){
    studentPtr temp;

    if(root==NULL){
        //student not found
        return root;
    }
    //Find student
    if(id<root->id){
        root->left = deleteStudent(root->left,id);
    }
    else if(id>root->id){
        root->right = deleteStudent(root->right,id);
    }
    //Student found, delete them
    else{
        if(root->left && root->right){//student has two children
            //replace with the student with the smallest id on the right
            temp = findMin(root->right);

            //free course list
            freeCourseList(root->courses);

            //copy the data 
            root->id = temp->id;
            strcpy(root->name,temp->name);
            strcpy(root->major,temp->major);
            root->courses = temp->courses;
            temp->courses = NULL;

            root->right = deleteStudent(root->right,temp->id);

            printf("\nStudent %s with ID: %d deleted successfully", root->name, root->id);
        }
        else{//student has one or zero children
            temp = root->left? root->left : root->right;//if left exists, use it, otherwise use right node
            //free course list
            freeCourseList(root->courses);
            printf("\nStudent %s with ID: %d deleted successfully", root->name, root->id);
            free(root);
            return temp;
        }
    }
    //rebalance after deleting
    if(root!=NULL){
        root->height = 1 + max(height(root->left),height(root->right));
        root = rebalance(root);
    }
    return root;
}
void AVLtoFILE(studentPtr root,FILE *file){
    if(root==NULL){
        return;
    }
    //inorder traversal through AVL 
    AVLtoFILE(root->left,file);

    coursePtr tempCourse = root->courses;
    while(tempCourse){//For each course enter the student's data with the course's 
        fprintf(file, "%s#%d#%s#%s#%s#%d#%s\n",
                root->name,
                root->id,
                root->major,
                tempCourse->courseCode,
                tempCourse->courseTitle,
                tempCourse->creditHours,
                tempCourse->semester);
        tempCourse = tempCourse->next;
    }
    AVLtoFILE(root->right,file);//go to right side
}
void freeAVL(studentPtr root){
    if(root==NULL){
        return;
    }
    //post order traversal
    freeAVL(root->left);
    freeAVL(root->right);
    freeCourseList(root->courses);

    free(root);
}
//Linkedlist Functions
void insertCourseAtHead(studentPtr student, coursePtr newCourse){
    //insert at head since it's faster (O(1))
    newCourse->next = student->courses;
    student->courses = newCourse;
}
coursePtr searchCourse(studentPtr student, char* courseTitle){
    if(student == NULL || student->courses == NULL){
        return NULL;
    }

    coursePtr temp = student->courses;

    while(temp!=NULL){
        if(strcmp(courseTitle,temp->courseTitle)==0){
            return temp;
        }
        temp = temp->next;//if not the course check the next one
    }
    return NULL;//not found
}
void freeCourseList(coursePtr head){
    coursePtr temp;
    while(head!=NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}
//Hash functions
int hashFunction(char* key){
    unsigned int hashValue = 0;//hash doesn't need to be signed
    while(*key != '\0'){
        hashValue = (hashValue<<5) + *key;//the shift multiplies by 32 (so that it's faster since 32 is a multiple of 2)
        key++;
    }
    return hashValue % HASH_SIZE;
}
void initializeHash(){
    
    for(int i=0; i<HASH_SIZE; i++){
        if(hashTable[i].courses != NULL){//free all courses lists (in case hash had already been initialized earlier)
            freeCourseList(hashTable[i].courses);
        }
    }
    hashOccupied = 0;
    for(int i=0; i<HASH_SIZE; i++){
        hashTable[i].status = EMPTY;//set status to empty when initializing the table
        hashTable[i].courses = NULL;
    }
}
int insertStudentHash(studentPtr student){
    int hashed = hashFunction(student->name);
    int firstHashed = hashed;//keep track of the first hashed value to make sure not to enter an infinite loop
    while(hashTable[hashed].status==OCCUPIED){//while the hashed value is occupied keep searching for a place
        if(strcmp(hashTable[hashed].name,student->name) == 0){
            printf("\nDuplicate found, aborting!");
            return -1;
        }
        hashed = (hashed+1) % HASH_SIZE;
        if(hashed==firstHashed){
            printf("\nCouldn't find a place to hash the value, aborting!");
            return -1;
        }
    }
    //Copy the data into the hash table
    strcpy(hashTable[hashed].name,student->name);
    hashTable[hashed].id = student->id;
    strcpy(hashTable[hashed].major,student->major);
    hashTable[hashed].status = OCCUPIED;
    //Update table's occupied count
    hashOccupied++;
    return hashed;
}
int searchStudentHash(char* name){
    int hashed = hashFunction(name);
    int firstHashed = hashed;//keep track of the first hashed value to make sure we don't get into an infinite loop
    while(hashTable[hashed].status!=EMPTY){
        if(strcmp(hashTable[hashed].name,name)==0 && hashTable[hashed].status == OCCUPIED){//ensure it's actually occupied, not just deleted, to make sure we don't get a leftover value
            //student found
            return hashed;
        }
        hashed = (hashed+1) % HASH_SIZE;
        if(hashed==firstHashed){
            return -1;//student not found
        }
    }
    //student not found
    return -1;
}
int deleteStudentHash(char* name){
    int hashed = hashFunction(name);
    int firstHashed = hashed;//keep track of the first hashed value to make sure we don't get into an infinite loop
    while(hashTable[hashed].status!=EMPTY){
        if(strcmp(hashTable[hashed].name,name)==0 && hashTable[hashed].status==OCCUPIED){//make sure it's the correct student and it's actually occupied
            //student found, delete it
            hashTable[hashed].status = DELETED;
            //free the courses list
            freeCourseList(hashTable[hashed].courses);
            hashTable[hashed].courses = NULL;
            hashOccupied--;
            return hashed;//return the delete value's index
        }
        hashed = (hashed+1) % HASH_SIZE;
        if(hashed==firstHashed){
            return -1;//student not found
        }
    }
    return -1;//student not found
}
void printHashedTable(){
    for(int i=0; i<HASH_SIZE; i++){
        if(hashTable[i].status==OCCUPIED){
            printf("\nIndex: %d | ",i);
            printf("Name: %s | ",hashTable[i].name);
            printf("ID: %d | ",hashTable[i].id);
            printf("Major: %s | ",hashTable[i].major);
            printf("Status: %s | ",returnStatus(hashTable[i].status));
            printf("\nCourses: \n");
            coursePtr temp = hashTable[i].courses;
            while(temp){
                printf("Course Title: %s | ",temp->courseTitle);
                printf("Code: %s | ",temp->courseCode);
                printf("Credit Hours: %d | ",temp->creditHours);
                printf("Semester: %s | ",temp->semester);
                printf("\n");
                temp = temp->next;
            }
            printf("----------------------------------\n");
        }
        else{//if doesn't have data just print its status
            printf("\nIndex: %d | ",i);
            printf("\nStatus: %s",returnStatus(hashTable[i].status));
        }
        
    }
}
void HASHtoFILE(){
    FILE* file = fopen("reg.txt","w");
    if(file==NULL){
        printf("\nError opening file!");
        return;
    }
    for(int i=0; i<HASH_SIZE; i++){
        if(hashTable[i].status==OCCUPIED){
            coursePtr temp = hashTable[i].courses;
            while(temp){
                fprintf(file, "%s#%d#%s#%s#%s#%d#%s\n",
                        hashTable[i].name,
                        hashTable[i].id,
                        hashTable[i].major,
                        temp->courseCode,
                        temp->courseTitle,
                        temp->creditHours,
                        temp->semester);
                temp = temp->next;
            }
        }
    }
    printf("\nFile saved successfully!");
    fclose(file);
}
char* returnStatus(int status){
    if(status==0){
        return "EMPTY";
    }
    else if(status==1){
        return "OCCUPIED";
    }
    else{
        return "DELETED";
    }
}
void insertCourseAtHeadHash(coursePtr newCourse,int hash){
    newCourse->next = hashTable[hash].courses;
    hashTable[hash].courses = newCourse;
}
coursePtr searchCourseHash(int hash,char* courseCode){
    coursePtr temp = hashTable[hash].courses;
    while(temp!=NULL){
        if(strcmp(temp->courseCode,courseCode)==0){//course already exists
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
//To scan lines that might have spaces
void readLine(char *buffer,int size){
    fgets(buffer,size,stdin);
    buffer[strcspn(buffer,"\n")] = '\0';
}
//max function
int max(int a,int b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}