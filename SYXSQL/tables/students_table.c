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
static const char *header_name[] = {"name", "age", "class", "height"};
static int header_len[] = {15, 6, 6, 6};


typedef struct Student {
    char name[20];
    int age;
    int class;
    double height;
} table_data;


static void init_table(struct Database *);
static size_t getDataSize();
static void printData(void *);
static void scanData(void *);

__attribute__((constructor))
static void __register_table() {
    register_table(table_name, init_table);
    return ;
}

void init_table(struct Database *db) {
    db->table_name = table_name;
    db->table_file = table_file;
    db->getDataSize = getDataSize;
    db->printData = printData;
    db->header_name = header_name;
    db->scanData = scanData;
    db->header_len = header_len;
    db->header_cnt = sizeof(header_len) / sizeof(header_len[0]);
    return ;
};

size_t getDataSize() {
    return sizeof(table_data);
}
static void printData(void *__data) {
    table_data *data = (table_data *)(__data);
    char frm[100];
    sprintf(frm, "%%%ds|%%%dd|%%%dd|%%%d.2lf|",
           header_len[0], header_len[1], 
           header_len[2], header_len[3]);
    printf(frm, 
          data->name, 
          data->age, 
          data->class, 
          data->height
          );
    printf("\n");
    return ;
};

static void scanData(void *__data) {
    table_data *data = (table_data *)(__data);
    scanf("%s%d%d%lf", 
         data->name, 
         &(data->age),
         &(data->class),
         &(data->height)
         );
    return ;
}

