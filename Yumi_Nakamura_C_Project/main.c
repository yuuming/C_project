//  main.c
//  file
//
//  Created by Alireza Davoodi on 2017-02-02.
//  Copyright © 2017 CICCC. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>


#define CoursesFile "Courses.txt"
#define AccountsFile "Accounts.txt"
#define StudentsFile "Students.txt"
#define StudentsCoursesFile "StudentsCourses.txt"

#define BASE_PATH "/Users/yuminakamura/Yumi_Nakamura_C_Project/Yumi_Nakamura_C_Project/yuumi/"

struct Student {
    
    char* studentID;
    char* name;
    char* gender;
    int grade;
    char* address;
    int admission_year;
    char** courses;
    int numberOfCourses;
};

struct Account {
    char *username;
    char *password;
};

struct Courses{
    char *courseID;
    char *courseName;
};

struct StudentsCourses{
    char *studentID;
    char *courseID;
    int mark;
};

//　Global Variables

struct Student* listOfStudents;
struct Account* listOfAccounts;
struct Courses* listOfCourses;
struct StudentsCourses* listOfStudentCourses;
int numberOfStudents;
int numberOfAccounts;
int numberOfCourses;
int numberOfStudentsCourses;
char* prefix;
char* pronoun;

struct Student student;
struct Account account;

double gpa;
int ranking;

char firstChar(char* line);
int strLength(char* line);
char* substring(char* line, int startIndex, int endIndex);
int getTheStartIndex(char* filename, char firstChar);
char** splitString(char* a_str, const char a_delim, int* numberOfComponents);

char** readFile(char* fileAddress, int* numberOfLines);
int numberOfItemsPerRecordInFile(char* fileName);
char* checkCourseName(char* courseID, int numberOfCourses);
int convertToNumber(char *);
int getCourseMark(char* studentID, char* courseID, int numberOfStudentsCourses);

//==========================================================
//                       fanctions
//==========================================================

char* getPrefix(struct Student student){
    if(strcmp(student.gender, "male") == 0){
        prefix = "Mr.";
    }else prefix = "Ms.";
    return prefix;
}

char* getPronoun(struct Student student){
    if(strcmp(student.gender, "male") == 0){
        pronoun = "He";
    }
    else pronoun = "She";
    return pronoun;
}
char *toLowercase(char* string){
    int i = 0;
    char c;
    
    char * lowercase=(char*)(malloc(sizeof(string)));
    while (string[i]){
        c = string[i];
        c=tolower(c);
        *(lowercase+i)=c;
        i++;
    }
    return lowercase;
}


double getGPA(struct Student student){
    int sum = 0;
    int count = 0;
    for(int i=0; i<numberOfStudentsCourses; i++){
        struct StudentsCourses studentCourse = listOfStudentCourses[i];
        if(strcmp(studentCourse.studentID, student.studentID) == 0){
            sum += studentCourse.mark;
            count++;
        }
    }
    return sum/count;
}

int getRanking(double gpa){
    int rank = 1;
    
    for(int i = 0; i< numberOfStudents; i++){
        struct Student student = listOfStudents[i];
        int student_gpa = getGPA(student);
        
        if(student_gpa > gpa){
            rank++;
        }
    }
    
    return rank;
    
}

//　Student
struct Student createAStudent(char* studentID, char* name, char* gender, int grade, char* address, int admission_year, char* courses[], int numberOfCourses);

struct Student* getListOfStudentFromFile(char* fileAddress, char* fileName, int* numberOfStudents);

//　Account
struct Account createAnAccount(char* username, char*password);

struct Account* getListOfAccountFromFile(char* fileAddress, char* fileName, int* numberOfAccounts);

struct Account getLogindata(){
    printf("Username: ");
    char* username = (char*) malloc(sizeof(char));
    scanf("%s[^\n]", username);
    
    printf("Password: ");
    char* password = (char*) malloc(sizeof(char));
    scanf("%s[^\n]", password);
    printf("\n");
    
    struct Account account = createAnAccount(username, password);
    return account;
}

bool checkAccount(struct Account account, int numberOfAccounts);
struct Account userLogin(int numberOfAccounts, int numberOfStudents);
struct Student getStudent(struct Account account, int numberOfStudents);

void showMainMenu(void);

//　Courses
struct Courses createACourse(char*courseID, char*courseName);
struct Courses* getListOfCouseFromFile(char* fileAddress, char* fileName, int* numberOfCourses);

//　StudentCourse
struct StudentsCourses createAStudentCourse(char* studentID, char* courseID, int mark);
struct StudentsCourses* getListOfStudentCourses(char* fileAddress, char* fileName, char* numberOfStudentsCourses);

//  <------print message functions----->

void printLoginMessage(){
    printf("************************************************************\n");
    printf("Please enter your account to login:\n");
    printf("************************************************************\n");
}

void printWelcomeMessage(){
    printf("************************************************************\n");
    printf("Welcome to Cornerstone International College of Canada.\n");
    printf("************************************************************\n\n");
}

void printTryAgainMessage(){
    printf("************************************************************\n");
    printf("Your account does not exist. Please try again!\n");
    printf("************************************************************\n\n");
}

void printOptions(){
    printf("************************************************************\n");
    printf("Select from the options:\n");
    printf("************************************************************\n");
    printf("—-[1] Print my enrolment certificate\n");
    printf("—-[2] Print my courses\n");
    printf("—-[3] Print my transcript\n");
    printf("—-[4] Print my GPA\n");
    printf("—-[5] Print my ranking among all students in the college\n");
    printf("—-[6] List all available courses\n");
    printf("—-[7] List all students\n");
    printf("-—[8] Logout\n");
    printf("-—[9] Exit\n");
    printf("************************************************************\n");
    printf("Enter the number corresponding to each item to proceed: ");
}

//Option [1]
void printEnrolmentCertificate(){
    printf("------------------------------------------------------------\n");
    printf("Dear Sir/Madam,\n");
    printf("\n");
    printf("This is to certify that %s %s with student id %s is a student at grade %d at CICCC. %s was admitted to our college in %d and has taken %d course(s). Currently %s resides at %s.\n", getPrefix(student), student.name, student.studentID, student.grade, getPronoun(student), student.admission_year, student.numberOfCourses, toLowercase(pronoun), student.address);
    printf("\n");
    printf("If you have any question, please don’t hesitate to contact us.");
    printf("Thanks,\n");
    printf("\n");
    printf("Williams,");
    printf("\n");
    printf("------------------------------------------------------------\n");
}

//Option [2]

void printCourses(int numberOfCourse, int numberOfStudentsCourses, int takenCourse){
    printf("------------------------------------------------------------\n");
    printf("Hi %s %s,\n", getPrefix(student), student.name);
    printf("You have taken the following courses:\n");
    for(int i = 0; i<takenCourse; i++){
        printf("%d) %s: %s: %d\n", i+1, student.courses[i],checkCourseName(student.courses[i], numberOfCourse), getCourseMark(student.studentID, student.courses[i], numberOfStudentsCourses));
    }
    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("\n");
};

//Option [3]
void printTranscript(){
    printf("------------------------------------------------------------\n");
    printf("Hi %s %s,\n", getPrefix(student), student.name);
    printf("Here is your transcript:\n");
    printf("\n");
    int count = 1;
    for(int i = 0; i<numberOfStudentsCourses; i++){
        if(strcmp(student.studentID, listOfStudentCourses[i].studentID) == 0){
            printf("%d %s: %s: %d\n", count, listOfStudentCourses[i].courseID, checkCourseName(listOfStudentCourses[i].courseID, numberOfStudentsCourses), listOfStudentCourses[i].mark);
            count++;
        }
    }
    printf("YOUR GPA IS: %.2f\n", getGPA(student));
    printf("\n");
    printf("------------------------------------------------------------\n");
};

////Option [4]
void printMyGPA(){
    printf("------------------------------------------------------------\n");
    
    printf("Hi %s %s,\n", getPrefix(student), student.name);
    printf("YOUR GPA IS: %.2f\n", getGPA(student));
    printf("------------------------------------------------------------\n");
    printf("\n");
};

//Option [5]
void printMyRanking(){
    printf("------------------------------------------------------------\n");
    printf("Hi %s %s,\n", getPrefix(student), student.name);
    printf("Your GPA is %.2f and therefore your rank is %d.\n", getGPA(student), getRanking(gpa));
    printf("------------------------------------------------------------\n");
    printf("\n");
};

//Option [6]
void printlistAllAvaiableCourses(){
    
    printf("------------------------------------------------------------\n");
    printf("The following courses are offered in CICCC:\n");
    for(int i = 0; i<numberOfCourses; i++){
        printf("%d) %s: %s\n", i+1, listOfCourses[i].courseID, listOfCourses[i].courseName);
    }
    printf("------------------------------------------------------------\n");
    printf("\n");
    
};

//Option [7]
void printlistAllStudents(){
    
    printf("------------------------------------------------------------\n");
    printf("There are %d students in CICCC as following:\n", numberOfStudents);
    for(int i = 0; i<numberOfStudents; i++){
        printf("%d) %s: %s\n", i+1, listOfStudents[i].name, listOfStudents[i].studentID);
    }
    printf("------------------------------------------------------------\n");
    printf("\n");
    
};



// get option
int getOption(){
    int option;
    scanf("%d", &option);
    printf("\n");
    return option;
}

//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//                    MAIN stars from here!!!!
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

int main(int argc, const char * argv[]) {
    
    //Read the Students.txt file and create an array of Students
    char* studentsFileName = StudentsFile;
    
    char* studentsfileAddress = BASE_PATH "Students.txt";
    numberOfStudents = 0;
    
    listOfStudents = getListOfStudentFromFile(studentsfileAddress, studentsFileName, &numberOfStudents);
    
    //    for(int i=0; i<numberOfStudents; i++)
    //    {
    //        printf("%s\n", listOfStudents[i].name);
    //        printf("%d\n", listOfStudents[i].numberOfCourses);
    //        printf("%s\n", listOfStudents[i].address);
    //        printf("%d\n", listOfStudents[i].grade);
    //        printf("%d\n", listOfStudents[i].admission_year);
    //
    //        for(int j=0; j<listOfStudents[i].numberOfCourses; j++)
    //        {
    //            printf("\n%s", listOfStudents[i].courses[j]);
    //        }
    //
    //        printf("\n============================\n");
    //    }
    //
    //***************************************************************
    //Read the Accounts.txt file and create an array of Accounts
    //***************************************************************
    char* accountFileName = AccountsFile;
    char* accountsfileAddress = BASE_PATH "Accounts.txt";
    numberOfAccounts = 0;
    
    listOfAccounts = getListOfAccountFromFile(accountsfileAddress, accountFileName, &numberOfAccounts);
    
    //    for(int i=0; i<numberOfAccounts; i++)
    //    {
    //        printf("%s\n", listOfAccounts[i].username);
    //        printf("%s\n", listOfAccounts[i].password);
    //
    //        printf("\n============================\n");
    //
    //    }
    
    //***************************************************************
    //Read the Courses.txt file and create an array of Courses
    //***************************************************************
    char* coursesFileName = CoursesFile;
    char* coursesfileAddress = BASE_PATH "Courses.txt";
    numberOfCourses = 0;
    
    listOfCourses = getListOfCouseFromFile(coursesfileAddress, coursesFileName, &numberOfCourses);
    
    //    for(int i=0; i<numberOfCourses; i++)
    //    {
    //        printf("%s\n", listOfCourses[i].courseID);
    //        printf("%s\n", listOfCourses[i].courseName);
    //
    //        printf("\n============================\n");
    //
    //    }
    
    //***************************************************************
    //Read the StudentCourses.txt file and create an array of StudentCourses
    //***************************************************************
    char* studentCourseFileName = StudentsCoursesFile;
    char* studentCourseAddress =  BASE_PATH "StudentsCourses.txt";
    numberOfStudentsCourses = 0;
    
    listOfStudentCourses = getListOfStudentCourses(studentCourseAddress, studentCourseFileName, &numberOfStudentsCourses);
    
    //    for(int i=0; i<numberOfStudentsCourses; i++)
    //    {
    //        printf("%s\n", listOfStudentCourses[i].studentID);
    //        printf("%s\n", listOfStudentCourses[i].courseID);
    //        printf("%s\n", listOfStudentCourses[i].mark);
    //
    //        printf("\n============================\n");
    //    }
    
    
    //==========PROGEAM STARTS FROM HERE=============
    
    //    printLoginMessage();
    
    struct Account account = userLogin(numberOfAccounts, numberOfStudents);
    student = getStudent(account, numberOfStudents);
    gpa = getGPA(student);
    ranking = getRanking(gpa);
    
    showMainMenu();
    
    //    account = getLogindata();
    //    checkAccount(account, numberOfAccounts);
    
    return 0;
}

struct Account userLogin(int numberOfAccounts, int numberOfStudents){
    bool validAccount = false;
    while(validAccount == false){
        printLoginMessage();
        account = getLogindata();
        validAccount = checkAccount(account, numberOfAccounts);
        if(validAccount){
            printWelcomeMessage();
        } else{
            printTryAgainMessage();
        }
        sleep(2);
    }
    return account;
}

bool checkAccount(struct Account account, int numberOfAccounts){
    for(int i = 0; i<numberOfAccounts; i++){
        if(strcmp(listOfAccounts[i].username, account.username) == 0 &&
           strcmp(listOfAccounts[i].password, account.password) == 0){
            return true;
        }
    }
    return false;
}

struct Student getStudent(struct Account account, int numberOfStudents){
    for(int i = 0; i<numberOfStudents; i++){
        if(strcmp(account.username, listOfStudents[i].studentID) == 0){
            student = listOfStudents[i];
        }
    }
    return student;
}

void showMainMenu() {
    // print menu
    printOptions();
    // get input menu number from user
    int option = getOption();
    // switch
    switch (option){
        case 1:
            printEnrolmentCertificate();
            break;
        case 2:
            printCourses(numberOfCourses, numberOfStudentsCourses,  student.numberOfCourses);
            break;
        case 3:
            printTranscript();
            break;
        case 4:
            printMyGPA();
            break;
        case 5:
            //            getRanking();
            printMyRanking();
            break;
        case 6:
            printlistAllAvaiableCourses();
            break;
        case 7:
            printlistAllStudents();
            break;
        case 8:
            account.password = 0;
            student.studentID = 0;
            
            printf("+++LOG OUT+++/n");
            
            break;
        case 9:
            printf("bye bye！");
            exit(0);
            return;
        default:
            printf("please type 1 to 9.\n");
            break;
    }
    showMainMenu();
}

// account
/*
 int strcmp( const char *str1 , const char *str2 );
 str1とstr2が等しいならば0、
 str1>str2ならば正の値、
 str1<str2ならば負の値を返す。
 */
//bool checkAccount(struct Account account, int numberOfAccounts){
//    for(int i = 0; i<numberOfAccounts; i++){
//        if(strcmp(listOfAccounts[i].username, account.username) == 0){
//            if(listOfAccounts[i].password == NULL){
//                printf("Please try again!!!!!!!\n");
//                printLoginMessage();
//                struct Account account = getLogindata();
//                bool validAccount = checkAccount(account, numberOfAccounts);
//                if(validAccount){
//                    sleep(2);
//                    printWelcomeMessage();
//                }
//                else{
//                    printTryAgainMessage();
//                }
//            }
//            else if(strcmp(listOfAccounts[i].password, account.password) == 0){
//                return true;
//            }
//        }
//    }
//    return false;
//}

//check if username == studentID
struct Student registerdStudent(struct Account account, int numberOfStudents){
    for(int i = 0; i<numberOfStudents; i++){
        if(strcmp(account.username,listOfStudents[i].studentID) == 0 ){
            student = listOfStudents[i];
        }
    }
    return student;
}

int getTheStartIndex(char* filename, char firstChar)
{
    
    if(strcmp(filename, AccountsFile)==0 && firstChar=='1')  //1User:”
    {
        return 7;
    }
    else if(strcmp(filename, AccountsFile)==0 && firstChar=='2') //2Pass:”
    {
        return 7;
    }
    if(strcmp(filename, StudentsFile)==0 && firstChar=='1')  //1studentID:”
    {
        return 12;
    }
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='2') //2name:”
    {
        return 7;
    }
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='3') //3gender:”
    {
        return 9;
    }
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='4') //4grade:”
    {
        return 8;
    }
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='5') //5address:“
    {
        return 10;
    }
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='6') //6admission_year:”
    {
        return 17;
    }
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='7') //7courses:”
    {
        return 10;
    }
    
    else if(strcmp(filename, CoursesFile)==0 && firstChar=='1') //courseID
    {
        return 11;
    }//
    else if(strcmp(filename, CoursesFile)==0 && firstChar=='2') //name
    {
        return 7;
    }
    else if(strcmp(filename, StudentsCoursesFile)==0 && firstChar=='1') //1studentID:"
    {
        return 12;
    }
    else if(strcmp(filename, StudentsCoursesFile)==0 && firstChar=='2') //2courseID:"
    {
        return 11;
    }
    else if(strcmp(filename, StudentsCoursesFile)==0 && firstChar=='3') //3mark:"
    {
        return 7;
    }
    
    else
    {
        return 0;
    }
}


//This method gives the length of the string. [You don't need to know how it deos it exactly]
int strLength(char* line)
{
    
    char c = '\n';
    int index = 0;
    
    int numberOfDoubleQuotes = 0;
    
    while(numberOfDoubleQuotes<2)
    {
        
        c = *(line+index);
        index++;
        
        if(c == '"')
        {
            numberOfDoubleQuotes++;
        }
    }
    
    return index;
}


//This method gives the first character of a string. [You don't need to know how it deos it exactly]

char firstChar(char* line)
{
    return *line;
}


//This method gives the substring of a string given the start and end index. [You don't need to know how it deos it exactly]
char* substring(char* line, int startIndex, int endIndex)
{
    int len = (endIndex-startIndex)+1;
    
    char* substring;
    
    //substring = (char*) malloc((len+1)*sizeof(char));
    
    substring = (char*) malloc((len+1)*sizeof(char));
    
    for(int i=0; i<=len; i++)
    {
        substring[i] = '\0';
    }
    
    for(int i=0; i<len; i++)
    {
        substring[i] = *(line+startIndex+i);
        
        substring[i] = line[startIndex+i];
        //*(substring+i) = *(line+startIndex+i);
    }
    
    substring[len+1] = '\0';
    return substring;
    
}


//This method splits the string and returns its components based on the delimeter. [You don't need to know how it deos it exactly]
char** splitString(char* a_str, const char a_delim, int* numberOfComponents)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    
    
    
    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    
    
    
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    
    *numberOfComponents = (int)count;
    
    /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
    count++;
    
    result = malloc(sizeof(char*) * count);
    
    
    
    char* components[count];
    for(int i=0; i<count; i++)
    {
        components[i] = '\0';
    }
    
    int counter = 0;
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        
        while (token)
        {
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
            
            counter++;
            
            components[counter] = token;
        }
        *(result + idx) = 0;
    }
    
    return result;
}

//This method reads the file given the address. [You don't need to know how it deos it exactly]
char** readFile(char* fileAddress, int* numberOfLines)

{
    int lines_allocated = 128;
    int max_line_len = 100;
    
    /* Allocate lines of text */
    char **words = (char **)malloc(sizeof(char*)*lines_allocated);
    if (words==NULL)
    {
        fprintf(stderr,"Out of memory (1).\n");
        exit(1);
    }
    
    FILE *fp = fopen(fileAddress, "r");
    if (fp == NULL)
    {
        fprintf(stderr,"Error opening file.\n");
        exit(2);
    }
    
    int i;
    for (i=0;1;i++)
    {
        int j;
        
        /* Have we gone over our line allocation? */
        if (i >= lines_allocated)
        {
            int new_size;
            
            /* Double our allocation and re-allocate */
            new_size = lines_allocated*2;
            words = (char **)realloc(words,sizeof(char*)*new_size);
            if (words==NULL)
            {
                fprintf(stderr,"Out of memory.\n");
                exit(3);
            }
            lines_allocated = new_size;
        }
        /* Allocate space for the next line */
        words[i] = malloc(max_line_len);
        if (words[i]==NULL)
        {
            fprintf(stderr,"Out of memory (3).\n");
            exit(4);
        }
        if (fgets(words[i],max_line_len-1,fp)==NULL)
            break;
        
        /* Get rid of CR or LF at end of line */
        for (j=(int)(strlen(words[i]))-1;j>=0 && (words[i][j]=='\n' || words[i][j]=='\r');j--)
            ;
        words[i][j+1]='\0';
    }
    /* Close file */
    fclose(fp);
    
    
    (*numberOfLines) = i;
    
    
    return words;
}


//This method converts a string to a number. [You don't need to know how it deos it exactly]
int convertToNumber(char * string)
{
    int number = atoi( string );
    return number;
}



//[You don't need to know how it deos it exactly]
int numberOfItemsPerRecordInFile(char* fileName)
{
    if(strcmp(fileName, CoursesFile)==0)
    {
        return 2;
    }
    else if(strcmp(fileName, AccountsFile)==0)
    {
        return 2;
    }
    else if(strcmp(fileName, StudentsFile)==0)
    {
        return 7;
    }
    else if(strcmp(fileName, StudentsCoursesFile)==0)
    {
        return 3;
    }
    else
    {
        return 1;
    }
}

//*****************************
//         Student
//*****************************

//This method creates a student using the input parameter. You might need to use this sample to create other entity types like course , ...
struct Student createAStudent(char* studentID, char* name, char* gender, int grade, char* address, int admission_year, char* courses[], int numberOfCourses)
{
    struct Student* student = (struct Student*)malloc(sizeof(struct Student));
    
    (*student).studentID = studentID;
    (*student).name = name;
    (*student).gender = gender;
    (*student).grade = grade;
    (*student).address = address;
    (*student).admission_year = admission_year;
    (*student).courses = courses;
    (*student).numberOfCourses = numberOfCourses;
    
    return *student;
}


//This is what you need to learn and repeat for other entities. Thie method reads the students in the file of students and return an array of students.
struct Student* getListOfStudentFromFile(char* fileAddress, char* fileName, int* numberOfStudents)
{
    int numberOfStudentsReadFromFileSoFar = 0;
    
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    
    
    struct Student* listOfStudents = (struct Student*) malloc(numberOfRecords*sizeof(struct Student));
    
    
    for(int k=0; k<numberOfRecords; k++)
    {
        char* studentID = NULL;
        char* name = NULL;
        char* gender = NULL;
        int grade = 0;
        char* address = NULL;
        int admission_year = 0;
        char** courses= NULL;
        
        int numberOfCourses = 0;
        
        
        for(int i=0; i<numberOfLinesPerRecord; i++)
        {
            int indexOfLine = k*numberOfLinesPerRecord+i;
            
            if(lines[indexOfLine]!=NULL)
            {
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                
                int lineLength = strLength(lines[indexOfLine]);
                
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                
                
                if(firstCharacter=='1')
                {
                    studentID = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='2')
                {
                    name = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='3')
                {
                    gender = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='4')
                {
                    char* gradeStr = substring(lines[indexOfLine], startIndex, lineLength-2);
                    
                    grade = convertToNumber(gradeStr);
                }
                else if(firstCharacter=='5')
                {
                    address = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='6')
                {
                    char* admissionYearStr = substring(lines[indexOfLine], startIndex, lineLength-2);
                    admission_year = convertToNumber(admissionYearStr);
                }
                else if(firstCharacter=='7')
                {
                    int numberOfComponents = 0;
                    char* coursesStr = substring(lines[indexOfLine], startIndex, lineLength-2);
                    courses = splitString(coursesStr, ',', &numberOfComponents);
                    
                    numberOfCourses = numberOfComponents;
                    
                }
            }//end of if
            
        }//end of second for
        
        
        struct Student student= createAStudent(studentID, name, gender, grade, address, admission_year, courses, numberOfCourses);
        listOfStudents[numberOfStudentsReadFromFileSoFar] = student;
        
        numberOfStudentsReadFromFileSoFar++;
        
    }//end of first for
    
    *numberOfStudents = numberOfStudentsReadFromFileSoFar;
    return listOfStudents;
}
//*****************************
//         Account
//*****************************
struct Account createAnAccount(char* username, char* password)
{
    struct Account* account = (struct Account*)malloc(sizeof(struct Account));
    
    account->username = username;
    account->password = password;
    
    return *account;
}

struct Account* getListOfAccountFromFile(char* fileAddress, char* fileName, int* numberOfAccounts){
    int numberOfStudentsReadFromFileSoFar = 0;
    
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    
    struct Account* listOfAccounts = (struct Account*) malloc(numberOfRecords*sizeof(struct Account));
    
    for(int k=0; k<numberOfRecords; k++)
    {
        char* username= NULL;
        char* password = NULL;
        
        
        for(int i=0; i<numberOfLinesPerRecord; i++)
        {
            int indexOfLine = k*numberOfLinesPerRecord+i;
            
            if(lines[indexOfLine]!=NULL)
            {
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                
                int lineLength = strLength(lines[indexOfLine]);
                
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                
                
                if(firstCharacter=='1')
                {
                    username = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='2')
                {
                    password = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
            }//end of if
            
        }//end of second for
        
        struct Account acccount = createAnAccount(username, password);
        listOfAccounts[numberOfStudentsReadFromFileSoFar] = acccount;
        
        numberOfStudentsReadFromFileSoFar++;
        
    }//end of first for
    
    *numberOfAccounts = numberOfStudentsReadFromFileSoFar;
    
    
    return listOfAccounts;
}

//*****************************
//          Course
//*****************************
struct Courses createACourse(char*courseID, char*courseName)
{
    struct Courses* courses = (struct Courses*)malloc(sizeof(struct Courses));
    
    courses->courseID = courseID;
    courses->courseName = courseName;
    
    return *courses;
}

struct Courses* getListOfCouseFromFile(char* fileAddress, char* fileName, int* numberOfCourses){
    int numberOfStudentsReadFromFileSoFar = 0;
    
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    
    struct Courses* listOfCourses = (struct Courses*) malloc(numberOfRecords*sizeof(struct Courses));
    
    for(int k=0; k<numberOfRecords; k++)
    {
        char* courseID = NULL;
        char* courseName = NULL;
        
        for(int i=0; i<numberOfLinesPerRecord; i++)
        {
            int indexOfLine = k*numberOfLinesPerRecord+i;
            
            if(lines[indexOfLine]!=NULL)
            {
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                
                int lineLength = strLength(lines[indexOfLine]);
                
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                
                
                if(firstCharacter=='1')
                {
                    courseID = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='2')
                {
                    courseName = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
            }//end of if
            
        }//end of second for
        
        
        struct Courses courses= createACourse(courseID,courseName);
        listOfCourses[numberOfStudentsReadFromFileSoFar] = courses;
        
        numberOfStudentsReadFromFileSoFar++;
        
    }//end of first for
    
    *numberOfCourses = numberOfStudentsReadFromFileSoFar;
    return listOfCourses;
}
//*****************************
//        StudentCourse
//*****************************


struct StudentsCourses createAStudentCourse(char* studentID, char* courseID,int mark)
{
    struct StudentsCourses* studentCourses = (struct StudentsCourses*)malloc(sizeof(struct StudentsCourses));
    
    studentCourses->studentID = studentID;
    studentCourses->courseID = courseID;
    studentCourses->mark = mark;
    
    return *studentCourses;
}


struct StudentsCourses* getListOfStudentCourses(char* fileAddress, char* fileName, char* numberOfStudentsCourses){
    int numberOfStudentsReadFromFileSoFar = 0;
    
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    
    struct StudentsCourses* listOfStudentCourses = (struct StudentsCourses*) malloc(numberOfRecords*sizeof(struct StudentsCourses));
    
    for(int k=0; k<numberOfRecords; k++)
    {
        char* studentID = NULL;
        char* courseID = NULL;
        int mark = 0;
        
        for(int i=0; i<numberOfLinesPerRecord; i++)
        {
            int indexOfLine = k*numberOfLinesPerRecord+i;
            
            if(lines[indexOfLine]!=NULL)
            {
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                
                int lineLength = strLength(lines[indexOfLine]);
                
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                
                
                if(firstCharacter=='1')
                {
                    studentID = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='2')
                {
                    courseID = substring(lines[indexOfLine], startIndex, lineLength-2);
                }
                else if(firstCharacter=='3')
                {
                    char* strMark = substring(lines[indexOfLine], startIndex, lineLength-2);
                    mark = convertToNumber(strMark);
                }
            }//end of if
            
        }//end of second for
        
        struct StudentsCourses studentCourses= createAStudentCourse(studentID, courseID, mark);
        listOfStudentCourses[numberOfStudentsReadFromFileSoFar] = studentCourses;
        numberOfStudentsReadFromFileSoFar++;
        
    }//end of first for
    
    *numberOfStudentsCourses = numberOfStudentsReadFromFileSoFar;
    return listOfStudentCourses;
    
}
char* checkCourseName(char* courseID, int numberOfCourses){
    char* courseName = NULL;
    for(int i = 0; i<numberOfCourses; i++){
        if(strcmp(courseID, listOfCourses[i].courseID) == 0){
            courseName = listOfCourses[i].courseName;
            return courseName;
        }
    }
    return NULL;
}

int getCourseMark(char* studentID, char* courseID, int numberOfStudentsCourses){
    int courseMark = 0;
    for(int i = 0; i<numberOfStudentsCourses; i++){
        if(strcmp(listOfStudentCourses[i].studentID, studentID) == 0){
            while(courseMark == 0){
                if(strcmp(courseID, listOfStudentCourses[i].courseID) == 0){
                    courseMark = listOfStudentCourses[i].mark;
                    return courseMark;
                }
                else i++;
            }
        }
    }
    return 0;
}

