#include <stdlib.h>
#include <stdio.h>
//student
typedef student* studentPtr;
typedef struct student{
    char name[50];
    int id;
    char major[50];
    coursePtr courses;

    studentPtr left;
    studentPtr right;
    int height;
}student;
//course
typedef course* coursePtr;
typedef struct course{
    char courseCode[20];
    char courseTitle[50];
    int creditHours;
    char semester[20];
    coursePtr next;
}course;

int main(){

}
void loadFile(){
    FILE *file = fopen("reg.txt","r");

    if(file==NULL){
        printf("\nError reading file");
        return;
    }
    char line[255];
    while(fgets(line,sizeof(line),file)){
        //make new student and initiate its data
        studentPtr newStudent = (studentPtr) malloc(sizeof(student));
        //make new course and initiate its data
        coursePtr newCourse = (coursePtr) malloc(sizeof(course));
        if(!newStudent){
            printf("\nError allocating memory to the student!");
            fclose(file);
            return;
        }
        //assign name
        char *token = strtok(line,"#");
        strcpy(newStudent->name,token);
        //assign id
        token = strtok(line,"#");
        newStudent->id = atoi(token);
        //assign major
        token = strtok(line,"#");
        strcpy(newStudent->major,token); 
        //assign course code
        token = strtok(line,"#");
        strcpy(newCourse->courseCode,token);
        //assign course title
        token = strtok(line,"#");
        strcpy(newCourse->courseTitle,token);
        //assign credit hours
        token = strtok(line,"#");
        newCourse->creditHours = atoi(token);
        //assign semester
        token = strtok(line,"#");
        strcpy(newCourse->semester,token);

        fclose(file);
        printf("\nFile read successfully");
        return;
    }
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
}
studentPtr rotateLeft(studentPtr k1){
    studentPtr k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    
    k2->height = max(height(k2->left),height(k2->right)) + 1;
    k1->height = max(height(k1->left),height(k1->right)) + 1;
}
studentPtr DLR(studentPtr k3){
    k3->left = rotateRight(k3->left);
    return rotateLeft(k3);
}
studentPtr DRL(studentPtr k3){
    k3->right = rotateLeft(k3->right);
    return rotateRight(k3);
}
studentPtr findByName(studentPtr student,char name[50]){
    if(student==NULL){
        return NULL;
    }
    findByName(student->left,name);

    if(strcmp(student->name,name) == 0){
        return student;
    }
    findByName(student->right,name);
}
studentPtr findByID(studentPtr root, int id){
    if(root == NULL){
        return NULL;
    }
    else if(id<root->id){
        return find(root->left,id);
    }
    else if(id>root->id){
        return find(root->right,id);
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
        return findMin(root->right);
    }
}
studentPtr insertStudent(studentPtr root, studentPtr newStudent){
    if(root==NULL){
        root = newStudent;
    }
    if(newStudent->id < root->id){
        root->left = insertStudent(root->left,newStudent);
    }
    else{
        root->right = insertStudent(root->right,newStudent);
    }
    return root;
    //Update height
}