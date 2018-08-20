# funCtional

Generic functional programming in C with support for statically allocated memory. You should probably use a `for` loop but this is fun!

## Usage

### Setup

To get started, pack your `Functional` struct with the required pointers and callbacks:

```C
Person people[] = {{.age = 19, .name = "Alex"},
                     {.age = 42, .name = "Jenna"},
                     {.age = 55, .name = "Tom"}};
int ages[3];
Person filteredPeople[3];

Functional f = {
    .args = {.i = 0,
             .n = 3,
             .arr = (void*)people,
             .ctxp = (void*)"Harry",
             .storage = filteredPeople},
    .filter = shouldIncludePerson,
    .map = copyPerson,
    .derefInput = derefPerson,
};
```

#### f.filter

Called to determine if `f.args.arr[f.args.i]` should be included in the filtered collection (for `func_filter` or `func_find`). The caller is responsible for copying filtered items using `f.map` when calling `func_filter`.

#### f.map

Called to copy or transform `f.args.arr[f.args.i]`. This callback is required for `func_filter` and `func_map`, however when used with `func_filter`, it's only responsibility should be copying.

#### f.derefInput

Required by `func_find` to dereference the correct element of `f.args.arr`.

#### f.args.arr

A `void*` pointer to the first element of the array to be iterated over.

#### f.args.storage

A `void*` pointer to the first element of the storage array where mapped and filtered elements will be copied.

#### f.args.i

Iteration index used to iterate over `args.arr` where `args.i < args.n`.

#### f.args.n

Total length of `arg.arr`. After running, this will be changed to the length of `args.storage`

#### f.args.offset

When filtering, `args.i` increments to cover all of `args.arr`, but `arr.storage` may contain fewer items. Therefore, the callback responsible for copying when filtering (`f.map`) may need to account for this difference (see `example.c`).

#### f.args.ctxp

Context pointer that is freely available for user use.

### Example

```C
#include "functional.h"
#define MAX_STR_SIZE 400

typedef struct {
  unsigned int age;
  char* name;
} Person;

bool shouldIncludePerson(FunctionalArgs* args) {
  const char* toExclude = args->ctxp;
  Person* first = args->arr;
  Person* p = &first[args->i];
  return p->age > 30 && strcmp(toExclude, p->name);
}

void* derefPerson(int i, void* arr) {
  Person* p = arr;
  return (void*)&p[i];
}

int stringifyPerson(Person* p, char* out, size_t n) {
  return snprintf(out, n, "Person %s is %d years of age, yo\n", p->name,
                  p->age);
}

void printPerson(Person* p) {
  char personStr[MAX_STR_SIZE];
  stringifyPerson(p, personStr, MAX_STR_SIZE);
  printf("%s", personStr);
}

void shiftPersonLeft(FunctionalArgs* args) {
  Person* people = args->arr;
  for (int i = args->i; i < args->n - 1; i++) {
    people[i] = people[i + 1];
  }
  args->n--;
}

void mapPersonToAge(FunctionalArgs* args) {
  int* storage = args->storage;
  Person* people = args->arr;
  storage[args->i] = people[args->i].age;
}

void copyPerson(FunctionalArgs* args) {
  Person* src = args->arr;
  Person* dest = args->storage;
  dest[args->i - args->offset] = src[args->i];
}

void mapExample(Functional* f) {
  func_map(f);
  for (int i = 0; i < f->args.n; i++) {
    int* ages = f->args.storage;
    printf("age[%d] = %d\n", i, ages[i]);
  }
}

void filterExample(Functional* f) {
  func_filter(f);
  for (int i = 0; i < f->args.n; i++) {
    Person* people = f->args.storage;
    printPerson(&people[i]);
  }
}

void findExample(Functional* f) {
  Person* olderAndNotHarry = func_find(f);
  printPerson(olderAndNotHarry);
}

int main() {
  Person people[] = {{.age = 19, .name = "Alex"},
                     {.age = 42, .name = "Jenna"},
                     {.age = 55, .name = "Tom"}};
  int ages[3];
  Person filteredPeople[3];
  Functional func = {
      .args = {.i = 0,
               .n = 3,
               .arr = (void*)people,
               .ctxp = (void*)"Harry",
               .storage = filteredPeople},
      .filter = shouldIncludePerson,
      .map = copyPerson,
      .derefInput = derefPerson,
  };
  findExample(&func);
  filterExample(&func);
  func.args.storage = ages;
  func.map = mapPersonToAge;
  func.args.n = 3;
  mapExample(&func);
}
```
