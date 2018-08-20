#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int i;
  int offset;
  int n;
  void* arr;
  void* ctxp;
  void* storage;
} FunctionalArgs;

typedef struct {
  bool (*filter)(FunctionalArgs* args);
  void (*map)(FunctionalArgs* args);
  void* (*derefInput)(int i, void* j);  // callback used to deference arr[i]
  void (*shiftLeft)(FunctionalArgs* f);
  FunctionalArgs args;
} Functional;

void func_map(Functional* f);

void func_filter(Functional* f);

void* func_find(Functional* f);

#endif
