/*************************************************************************
	> File Name: database.c
	> Author: SongYuXuan
	> Mail: songyuxuan666@outlook.com
	> Created Time: Sat 31 Aug 2024 09:48:53 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <database.h>
#include <string.h>

struct Database db;

__attribute__((constructor))
void init_db() {
    db.table = NULL;
    db.table_file = NULL;
    db.table_name = NULL;
    db.head.next = NULL;
    return ;
}

struct TableInfo tables[100];
int tables_cnt = 0;

void register_table(const char *table_name, InitTable_T init_table) {
    tables[tables_cnt].table_name = table_name;
    tables[tables_cnt].init_table = init_table;
    tables_cnt += 1;
    return ;
}


static struct table_data *getNewTableData(void *data, long offset) {
    struct table_data *p = (struct table_data *)malloc(sizeof(struct table_data));
    p->data = malloc(db.getDataSize());
    memcpy(p->data, data, db.getDataSize());
    p->offset = offset;
    p->next = NULL;
    return p;
}

static void load_table_data() {
    char buff[db.getDataSize()];
    struct table_data *p = &(db.head);
    int data_cnt = 0;
    while (1) {
        long offset = ftell(db.table);
        if (fread(buff, db.getDataSize(), 1, db.table) == 0) break;
        p->next = getNewTableData(buff, offset);
        p = p->next;
        data_cnt += 1;
    }
    printf("load data success : %d\n", data_cnt);
    return ;
}

static void open_table() {
    db.table = fopen(db.table_file, "rb+");
    if (db.table == NULL) {
        printf("cant open file : %s\n", db.table_file);
        exit(1);
    }
    load_table_data();
    return ;
}

static void clear_table_data() {
    struct table_data *p = db.head.next ,*q;
    while (p)  {
        q = p->next;
        free(p->data);
        free(p);
        p = q;
    }
    return ;
}

static void close_table() {
    clear_table_data();
    if (db.table == NULL) return ;
    fclose(db.table);
    return ;
}


static enum OP_TYPE choose_table() {
    for (int i = 0; i < tables_cnt; i++) {
        printf("%d : %s\n", i, tables[i]);
    }
    printf("%d : quit\n", tables_cnt);
    int x;
    do {
        printf("input : ");
        scanf("%d", &x);
    } while (x < 0 || x > tables_cnt);

    if (x < tables_cnt) {
        close_table();
        tables[x].init_table(&db);
        open_table();
        return TABLE_USAGE;
    }
    return OP_END;
}

static enum OP_TYPE table_usage() {
    printf("1 : list %s\n", db.table_name);
    printf("2 : add an item to %s\n", db.table_name);
    printf("3 : modify an item to %s\n", db.table_name);
    printf("4 : delete an item to %s\n", db.table_name);
    printf("5 : back\n");

    int x;
    do {
        printf("input : ");
        scanf("%d", &x);
    } while (x < 1 || x > 5);
    if (x == 1) return LIST_TABLE;
    if (x == 2) return ADD_TABLE;
    if (x == 3) return MODIFY_TABLE;
    if (x == 4) return DELETE_TABLE;
    return CHOOSE_TABLE;
}

static void printTableHeader() {
    int len = 0;
    len += printf("%5s|", "id");
    char frm[100];
    for (int i = 0; i < db.header_cnt; i++) {
        sprintf(frm, "%%%ds|", db.header_len[i]);
        len += printf(frm, db.header_name[i]);
    }
    printf("\n");
    for (int i = 0; i < len; i++) printf("-");
    printf("\n");
    return ;
}

static enum OP_TYPE list_table() {
    __list_table();
    return TABLE_USAGE;
}


int __list_table() {
    struct table_data *p = db.head.next;
    int id = 0;
    printTableHeader();
    while (p) {
        printf("%5d|", id);
        db.printData(p->data) ;
        p = p->next;
        id += 1;
    }
    return id;
}

static long add_one_table_data(void *buff) {
    fseek(db.table, 0, SEEK_END);
    long offset = ftell(db.table);
    struct table_data *p = &(db.head);
    while (p->next) p = p->next;
    p->next = getNewTableData(buff, offset);
    fwrite(buff, db.getDataSize(), 1, db.table);
    fflush(db.table);
    return offset;
}

static enum OP_TYPE add_table() {
    printf("add new item : (");
    for (int i = 0; i < db.header_cnt; i++) {
        if (i) printf(", ");
        printf("%s", db.header_name[i]);
    }
    printf(")\n");
    printf("input : ");
    char buff[db.getDataSize()];
    db.scanData(buff);
    db.printData(buff);
    add_one_table_data(buff);
    printf("add one item to %s : success\n", db.table_name);
    printf("\n");
    return TABLE_USAGE;
}

static void modity_one_table_data(void *buff, int id) {
    struct table_data *p = db.head.next;
    for (int i = 0; i < id; i++) {
        p = p->next;
    }
    memcpy(p->data, buff, db.getDataSize());
    fseek(db.table, p->offset, SEEK_END);
    fwrite(buff, db.getDataSize(), 1, db.table);
    fflush(db.table);
    return ;
}

static enum OP_TYPE modify_table() {
    int n = __list_table();
    int id;
    printf("modify id : ");
    do {
        printf("modify id (%d back) : ", n);
        scanf("%d", &id);
    } while (id < 0 || id >= n);
    if (id == n) return TABLE_USAGE;
    printf("modify item (id = %d) : (", id);
    for (int i = 0; i < db.header_cnt; i++) {
        if (i) printf(", ");
        printf("%s", db.header_name[i]);
    }
    printf(")\n");
    printf("input : ");
    char buff[db.getDataSize()];
    db.scanData(buff);
    modity_one_table_data(buff, id);
    printf("modify one item at %s : success\n\n", db.table_name);
    return TABLE_USAGE;
}

static void destoryTableData(struct table_data *p) {
    free(p->data);
    free(p);
    return ;
}

static void clearTableData() {
    fclose(db.table);
    db.table = fopen(db.table_file, "w");
    fclose(db.table);
    db.table = fopen(db.table_file, "rb+");
    struct table_data *p = db.head.next, *q;
    while (p) {
        q = p->next;
        destoryTableData(p);
        p = q;
    }
    db.head.next = NULL;
    return ;
}

static void restoreTableData() {
    struct table_data *p = db.head.next, *q;
    db.head.next = NULL;
    clearTableData();
    while (p) {
        q = p->next;
        add_one_table_data(p->data);
        destoryTableData(p);
        p = q;
    }
    return ;
}

static enum OP_TYPE delete_table() {
    int n = __list_table();
    int id;
    do {
        printf("delete id (%d back) : ", n);
        scanf("%d", &id);
    } while (id < 0 || id >= n);
    if (id == n) return TABLE_USAGE;
    struct table_data *p = &(db.head), *q;
    for (int i = 0; i < id; i++) p = p->next;
    q = p->next;
    q->next = p->next;
    destoryTableData(q);
    restoreTableData();
    return TABLE_USAGE;
}


enum OP_TYPE run(enum OP_TYPE status) {
    switch (status) {
        case CHOOSE_TABLE: {
            return choose_table();
        } break;
        case TABLE_USAGE  : {
            return table_usage();
        } break;
        case LIST_TABLE : {
            return list_table();
        } break;
        case ADD_TABLE : {
            return add_table();
        } break;
        case MODIFY_TABLE : {
            return modify_table();
        } break;
        case DELETE_TABLE : {
            return delete_table();
        } break;
        default : {
            printf("unknown status : %d\n", status);
        } break;

    }
    return OP_END;
}


void run_database() {
    enum OP_TYPE status = CHOOSE_TABLE;
    while (1) {
        status = run(status);
        if (status == OP_END) break; 
    }

    return ;
}



