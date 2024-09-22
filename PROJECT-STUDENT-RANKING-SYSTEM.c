#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MNL 100
#define TFILE "student_records.txt"
#define BFILE "student_records.bin"

typedef struct {
    int rn;
    char n[MNL];
    int a;
    float m;
} Stu;

typedef struct {
    Stu *s;
    int sz;
    int c;
} SList;

void init_sl(SList *l) {
    l->sz = 0;
    l->c = 10;
    l->s = (Stu *)malloc(l->c * sizeof(Stu));
}

void resize_sl(SList *l) {
    l->c *= 2;
    l->s = (Stu *)realloc(l->s, l->c * sizeof(Stu));
}

void add_sr(SList *l) {
    if (l->sz == l->c) {
        resize_sl(l);
    }
    Stu *st = &l->s[l->sz];
    printf("Enter roll number: ");
    scanf("%d", &st->rn);
    printf("Enter name: ");
    getchar();
    fgets(st->n, MNL, stdin);
    st->n[strcspn(st->n, "\n")] = 0;
    printf("Enter age: ");
    scanf("%d", &st->a);
    printf("Enter marks: ");
    scanf("%f", &st->m);
    l->sz++;
}

void display_srs(SList *l) {
    for (int i = 0; i < l->sz; i++) {
        Stu *st = &l->s[i];
        printf("Roll Number: %d\n", st->rn);
        printf("Name: %s\n", st->n);
        printf("Age: %d\n", st->a);
        printf("Marks: %.2f\n", st->m);
        printf("-----------------------\n");
    }
}

Stu* search_sr(SList *l, int rn) {
    for (int i = 0; i < l->sz; i++) {
        if (l->s[i].rn == rn) {
            return &l->s[i];
        }
    }
    return NULL;
}

void delete_sr(SList *l, int rn) {
    int found = 0;
    for (int i = 0; i < l->sz; i++) {
        if (l->s[i].rn == rn) {
            found = 1;
        }
        if (found && i < l->sz - 1) {
            l->s[i] = l->s[i + 1];
        }
    }
    if (found) {
        l->sz--;
        printf("Student with roll number %d deleted.\n", rn);
    } else {
        printf("Student with roll number %d not found.\n", rn);
    }
}

void save_srs(SList *l) {
    FILE *f = fopen(TFILE, "w");
    if (!f) {
        printf("Unable to open file for writing\n");
        return;
    }

    fprintf(f, "%d\n", l->sz);
    for (int i = 0; i < l->sz; i++) {
        Stu *st = &l->s[i];
        fprintf(f, "%d %s %d %.2f\n", st->rn, st->n, st->a, st->m);
    }

    fclose(f);
    printf("Student records saved to file.\n");
}

void load_srs(SList *l) {
    FILE *f = fopen(TFILE, "r");
    if (!f) {
        printf("Unable to open file for reading\n");
        return;
    }

    int sz;
    fscanf(f, "%d\n", &sz);
    l->sz = sz;
    l->s = (Stu *)realloc(l->s, l->sz * sizeof(Stu));
    
    for (int i = 0; i < l->sz; i++) {
        Stu *st = &l->s[i];
        fscanf(f, "%d %s %d %f\n", &st->rn, st->n, &st->a, &st->m);
    }

    fclose(f);
    printf("Student records loaded from file.\n");
}

void save_srs_bin(SList *l) {
    FILE *f = fopen(BFILE, "wb");
    if (!f) {
        printf("Unable to open binary file for writing\n");
        return;
    }

    fwrite(&l->sz, sizeof(int), 1, f);
    fwrite(l->s, sizeof(Stu), l->sz, f);

    fclose(f);
    printf("Student records saved to binary file.\n");
}

void load_srs_bin(SList *l) {
    FILE *f = fopen(BFILE, "rb");
    if (!f) {
        printf("Unable to open binary file for reading\n");
        return;
    }

    fread(&l->sz, sizeof(int), 1, f);
    l->s = (Stu *)realloc(l->s, l->sz * sizeof(Stu));
    fread(l->s, sizeof(Stu), l->sz, f);

    fclose(f);
    printf("Student records loaded from binary file.\n");
}

void free_sl(SList *l) {
    free(l->s);
}

int main() {
    SList l;
    init_sl(&l);

    int n;
    printf("Enter the number of student records to add initially: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        add_sr(&l);
    }

    char c;
    while (1) {
        printf("Do you want to add another student record? (y/n): ");
        scanf(" %c", &c);
        if (c == 'y' || c == 'Y') {
            add_sr(&l);
        } else {
            break;
        }
    }

    while (1) {
        printf("\nStudent Record Management System\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Student Records\n");
        printf("3. Search Student Record by Roll Number\n");
        printf("4. Delete Student Record by Roll Number\n");
        printf("5. Save Student Records to Text File\n");
        printf("6. Load Student Records from Text File\n");
        printf("7. Save Student Records to Binary File\n");
        printf("8. Load Student Records from Binary File\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        int mc;
        scanf("%d", &mc);

        int rn;
        Stu *st;
        switch (mc) {
            case 1:
                add_sr(&l);
                break;
            case 2:
                display_srs(&l);
                break;
            case 3:
                printf("Enter roll number to search: ");
                scanf("%d", &rn);
                st = search_sr(&l, rn);
                if (st) {
                    printf("Roll Number: %d\n", st->rn);
                    printf("Name: %s\n", st->n);
                    printf("Age: %d\n", st->a);
                    printf("Marks: %.2f\n", st->m);
                } else {
                    printf("Student with roll number %d not found.\n", rn);
                }
                break;
            case 4:
                printf("Enter roll number to delete: ");
                scanf("%d", &rn);
                delete_sr(&l, rn);
                break;
            case 5:
                save_srs(&l);
                break;
            case 6:
                load_srs(&l);
                break;
            case 7:
                save_srs_bin(&l);
                break;
            case 8:
                load_srs_bin(&l);
                break;
            case 9:
                free_sl(&l);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}