### Feature 7: Recursive Listing (-R)

**Q1: What is a base case in recursion, and what is it here?**  
A base case is the condition that stops further recursive calls.  
In our recursive `do_ls`, the base case occurs when there are no more subdirectories to traverse — i.e., it skips files and does not recurse into "." or "..".

**Q2: Why must we construct a full path before recursion?**  
Because `lstat()` needs the complete path (like "parent/subdir") to access metadata of subdirectories correctly.  
If we used only the name, it would look for files in the wrong directory and fail to recurse properly.
