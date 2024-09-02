/*************************************************************************
	> File Name: students_table.c
	> Author: SongYuXuan
	> Mail: songyuxuan666@outlook.com
	> Created Time: Sat 31 Aug 2024 02:04:37 PM CST
 ************************************************************************/

#include <stdio.h>
#include <database.h>

static const char *table_name = "student table";
static const char *table_file = "./data/student_data.dat";

typedef struct Student {
    char name[20];
    int age;
    int class;
    double height;
} table_data;


static void init_table(struct Database *);
static size_t getDataSize();

__attribute__((constructor))
static void __register_table() {
    register_table(table_name, init_table);
    return ;
}

void init_table(struct Database *db) {
    db->table_name = table_name;
    db->table_file = table_file;
    db->getDataSize = getDataSize;
    return ;
};

size_t getDataSize() {
    return sizeof(table_data);
}

