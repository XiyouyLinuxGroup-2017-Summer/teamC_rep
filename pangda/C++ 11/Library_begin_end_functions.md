# The Library `begin` and `end` Functions

Although we can compute an off-the-end pointer, doing so is error-prone. To make it easier and safer to use pointers, the new library includes two functions, named `begin` and `end`. These functions act like the similarly named container members. However, arrays are not class types, so these functions are not member funtions. Instead, they take an argument that is an array. Like this:

``` c++
int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int *beg = begin(ia);
int *last = end(ia);
```

`begin` returns a pointer to the first, and the `end` functions returns a pointer one past the last element in the given array: These functions are defined in the ***iterator* header**.

Using `begin` and `end`, it is easy to write a loop to process the elements in an array. For example, assuming `arr` is an array that holds `int` values, we might find the first negative value in `arr` as follows:

``` c++
// pbeg points to the first and pend points just past the last element in arr
int *pbeg = begin(arr), *pend = end(arr);
// find the first negative element, stopping if we've seen all the elements
while (pbeg != pend && *pbeg >= 0)
    ++pbeg;
```

We start by defining two `int` pointers named `pbeg` and `pend`. We position `pbeg` to denote the first element and `pend` to point one past the last element in `arr`. The `while` condition use `pend` to know whether it is safe to dereference `pbeg`. If element in negative. If so, the condition fails and we exit the loop. If not, we increment the pointer to llok at the next element.

> Note
> A pointer "one past" the end of a built-in array behaves the same way as the the iterator returned by the `end` operation of a `vector`. In particular, we may not dereference of increment an off-the-end pointer.