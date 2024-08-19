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
        fprintf(fp, "%s %d %d %.2lf\n",
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

void modify_a_student() {
    list_students();
    int id;
    do {
    printf("modify id : ");
    scanf("%d", &id);
    } while (id < 0 || id >= stu_cnt);
    printf("add new item : (name, age, class, height)\n");
    printf("syxsql > ");
    scanf("%s%d%d%lf",
         stu_arr[id].name,
         &stu_arr[id].age,
         &stu_arr[id].class,
         &stu_arr[id].height
    );
    restore_data_to_file(stu_arr, stu_cnt);
    return ;
}

void clear_file() {
    FILE *fp = fopen(file_name, "w");
    fclose(fp);
    return ;
}

void restore_data_to_file(Student *arr, int n) {
    clear_file();
    output_to_file(arr, n);
    return ;
}

void delete_a_student() {
    if (stu_cnt == 0) {
        printf("There is no student.");
        return ;
    }
    list_students();
    int id;
    do {
    printf("delete id : ");
    scanf("%d", &id);
    } while (id < 0 || id >= stu_cnt);
    char s[100];
    printf("confirm (y / n) : ");
    fflush(stdin); 
    scanf("%s", s);
    if (s[0] != 'y') return ;
    for (int i = id + 1; i < stu_cnt; i++) {
        stu_arr[i - 1] = stu_arr[i];
    }
    stu_cnt -= 1;
    restore_data_to_file(stu_arr, stu_cnt);
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
            case MODIFY: { modify_a_student(); } break;
            case DELETE: { delete_a_student(); }; break;
            case QUIT: printf("exit\n"); exit(0);
        }



    }

    return 0;
}
