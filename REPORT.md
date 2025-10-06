Q1: Why read all directory entries into memory before sorting?
We need all filenames in memory because sorting requires comparing any two filenames freely. Without storing them first, we cannot rearrange or sort the list.
Drawback: For directories with millions of files, this uses a lot of memory and can slow down or crash the program.

Q2: Purpose and signature of qsort comparison function
qsort() needs a function to compare two elements. Signature:

int cmp(const void *a, const void *b);


It casts the const void* pointers to the correct type (here, char **) and returns:

<0 if a < b

0 if a == b

>0 if a > b

Reason for const void *: Allows qsort() to be generic and work with any data type.
