Q1. Explain the general logic for printing items in a "down then across" columnar format. Why is a simple single loop insufficient?

First, all filenames are stored in an array.

Calculate the number of columns and rows based on terminal width and longest filename.

Print row by row: for each row, print the item in that row from each column (index = row + col * num_rows).

A simple single loop prints sequentially left-to-right only, so columns would not align “down then across.”

Q2. What is the purpose of the ioctl system call? Limitations if using a fixed width?

ioctl with TIOCGWINSZ retrieves the current terminal width.

This allows column layout to adapt dynamically.

Using a fixed width (e.g., 80) may cause columns to overflow or look uneven on larger/smaller terminals.
