Q1: Difference between stat() and lstat()

stat() returns information about the target file a symbolic link points to, while lstat() returns information about the link itself.
Use lstat() in ls when you need to identify and display symbolic links instead of the files they reference.

Q2: Using st_mode with bitwise operators

The st_mode field contains both file type and permission bits.
Use bitwise AND (&) with macros to extract them, for example:

(st.st_mode & S_IFMT) == S_IFDIR   // checks if directory
(st.st_mode & S_IRUSR)             // checks user read permission


S_IFDIR, S_IFREG, S_IRUSR, etc., are predefined macros for file types and permissions
