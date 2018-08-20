#include "functional.h"

void func_map(Functional* f) {
  f->args.i = 0;
  f->args.offset = 0;
  while (f->args.i < f->args.n) {
    f->map(&f->args);
    f->args.i++;
  }
}

void func_filterGeneric(Functional* f,
                        void (*shiftLeft)(FunctionalArgs* args)) {
  bool shouldMutate = shiftLeft != NULL;
  int nInCollection = 0;
  for (f->args.i = 0; f->args.i < f->args.n; f->args.i++) {
    bool shouldInclude = f->filter(&f->args);
    if (shouldInclude) {
      f->args.offset = f->args.i - nInCollection;
      if (!shouldMutate) {
        f->map(&f->args);
      }
      nInCollection++;
    } else {
      if (shouldMutate) {
        shiftLeft(&f->args);
      }
    }
  }
  f->args.n = nInCollection;
}

void func_filter(Functional* f) {
  func_filterGeneric(f, NULL);
}

void func_filterMutate(Functional* f, void (*shiftLeft)(FunctionalArgs* args)) {
  func_filterGeneric(f, shiftLeft);
}

void* func_find(Functional* f) {
  f->args.i = 0;
  while (f->args.i < f->args.n) {
    if (f->filter(&f->args)) {
      return f->derefInput(f->args.i, f->args.arr);
    }
    f->args.i++;
  }
  return NULL;
}
