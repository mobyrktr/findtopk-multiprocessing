# findtopk-multiprocessing
### Operating Systems Project

##### All of the codes has written to find kth max element in N files.

- findTopK1.c makes this using intermediate files.
- findTopK_mqueue.c makes this using POSIX Message Queues.
- findTopK_thread.c makes this using POSIX Threads.

Integers in input files must seperated by whitespaces.
All of the files create an output file that contains kth max integer after completing their tasks.

**Usage:** `findTopK1 <k> <N> <infile1> ...<infileN> <outfile>`

**k:** kth max integer  
**N:** Number of files  
**infiles:** Input file names  
**outfile:** File name for output file
