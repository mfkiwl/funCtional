# funCtional

Generic functional programming in C with support for statically allocated memory. You should probably use a `for` loop but this is fun!

## Usage

### Setup

To get started, pack your `Functional` struct with the required pointers and callbacks:

```C
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

### f.derefInput

Required by `func_find` to dereference the correct element of `f.args.arr`.

#### f.args.arr

A `void*` pointer to the first element of the array to be iterated over.

#### f.args.storage

A `void*` pointer to the first element of the storage array where mapped and filtered elements will be copied.

#### f.args.i

Iteration index used to iterate over `args.arr` where `args.i < args.n`.

#### f.args.n

Total length of `arg.arr`. After running, this will be changed to the length of `args.storage`

### f.args.offset

When filtering, `args.i` increments to cover all of `args.arr`, but `arr.storage` may contain fewer items. Therefore, the callback responsible for copying when filtering (`f.map`) may need to account for this difference (see `example.c`).

#### f.args.ctxp

Context pointer that is freely available for user use.
