/*************************************************************************
	> File Name: student_system.c
	> Author: SongYuXuan
	> Mail: songyuxuan666@outlook.com
	> Created Time: Sun 18 Aug 2024 04:07:22 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

const char *file_name = "student_data.txt";

typedef struct Student {
    char name[20];
    int age;
    int class;
    double height;
} Student;

#define MAX_N 10000
Student stu_arr[MAX_N + 5];
int stu_cnt;

int read_from_file(Student *arr) {
    int i = 0;
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) return 0;
    while (fscanf(fp, "%s", arr[i].name) != EOF) {
        fscanf(fp, "%d%d%lf", 
               &arr[i].age,
               &arr[i].class, 
               &arr[i].height);
        i += 1;
    }
    fclose(fp);
    return i;
}

void output_to_file(Student *arr, int n) {
    FILE *fp = fopen(file_name, "a");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s %d %d %.2lf",
            arr[i].name, arr[i].age,
            arr[i].class, arr[i].height);
    }
    fclose(fp);
    return ;
}

void list_students () {
    int len = printf("%4s|%10s|%4s|%6s|%7s|",
        "id", "name", "age", "class", "height"
    );
    printf("\n");
    for (int i = 0; i < len; i++) printf("=");
    printf("\n");
    for (int i = 0; i < stu_cnt; i++) {
        printf("%4d|%10s|%4d|%6d|%7.2lf|\n",
            i, stu_arr[i].name,
            stu_arr[i].age,
            stu_arr[i].class,
            stu_arr[i].height);
    }
    return ;
}

void add_a_student() {
    printf("add new item : (name, age, class, height)\n");
    printf("syxsql > ");
    scanf("%s%d%d%lf",
         stu_arr[stu_cnt].name,
         &stu_arr[stu_cnt].age,
         &stu_arr[stu_cnt].class,
         &stu_arr[stu_cnt].height
    );
    output_to_file(stu_arr + stu_cnt, 1);
    stu_cnt += 1;
    return ;
}

enum NO_TYPE {
    LIST = 1,
    ADD,
    MODIFY,
    DELETE,
    QUIT
};


int usage() {
    int no;
    do {
        printf("%d : list students\n", LIST);
        printf("%d : add a new student\n", ADD);
        printf("%d : modify a student\n", MODIFY);
        printf("%d : delete a student\n", DELETE);
        printf("%d : exit\n", QUIT);
        printf("syxsql > ");
        scanf("%d", &no);
    } while (no < LIST || no > QUIT);
    return no;
}



int main(){
    stu_cnt = read_from_file(stu_arr);
    while (1) {
        int no = usage();
        printf("input number : %d\n", no);
        switch (no) {
            case LIST: { list_students(); break; }
            case ADD: {add_a_student(); break;}
            case MODIFY: printf("modify a student\n"); break;
            case DELETE: printf("delete a student\n"); break;
            case QUIT: printf("exit\n"); exit(0);
        }



    }

    return 0;
}
