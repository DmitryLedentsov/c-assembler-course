#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>


void int64_t_print(int64_t i) {
    printf("%" PRId64 " ", i);
}

void double_print(double d) {
    printf("%lf ", d);
}
struct list_int64_t* create_int64_t_node(int64_t x);
struct list_double* create_double_node(double x);

#define _print(type, list) list_##type##_print(list)
#define _push(type, list, x) list_##type##_push(list, x)
#define DEFINE_LIST(type)\
  struct list_##type {\
    type value;\
    struct list_##type* next;\
  };                     \
  void list_##type##_print(struct list_##type* list) {\
    while (list != 0) {\
        type##_print(list->value);\
        list = list->next;\
    }\
  }\
    void list_##type##_push(struct list_##type* list, type x) { \
        while (list->next != NULL) {  \
            list = list->next;\
        }                    \
        list->next = create_##type##_node(x);                     \
    }

DEFINE_LIST(int64_t)
DEFINE_LIST(double)

struct list_int64_t* create_int64_t_node(int64_t x) {
    struct list_int64_t* node = malloc(sizeof(struct list_int64_t));
    node->value = x;
    return node;
}

struct list_double* create_double_node(double x) {
    struct list_double* node = malloc(sizeof(struct list_double));
    node->value = x;
    return node;
}
void error(const char *s) {
    fprintf(stderr, "%s", s);
    abort();
}

#define list_print(list)                                                   \
    _Generic((list),                                                       \
            struct list_int64_t* : list_int64_t_print(list),                            \
            struct list_double* : list_double_print(list),                              \
            default : error("GG GA GA"))
#define list_push(list, x) \
    _Generic((list),                                                       \
            struct list_int64_t* : list_int64_t_push(list, x),                            \
            struct list_double* : list_double_push(list, x),                              \
            default : error("GG GA GA"))

int main() {
    struct list_int64_t* list_int = create_int64_t_node(0);
    //_push(int64_t, list_int, 1);
    //_push(int64_t, list_int, 2);
    //_push(int64_t, list_int, 3);
    //_print(int64_t, list_int);
    list_push(list_int, 1);
    list_push(list_int, 2);
    list_push(list_int, 3);
    list_print(list_int);

    printf("\n##############\n");

    struct list_double* list_double = create_double_node(0);

    list_push(list_double, 1.1);
    list_push(list_double, 2.22);
    list_push(list_double, 3.333);
    list_push(list_double, 4.4444);
    list_print(list_double);

}