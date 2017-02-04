## open()&openat()
```c++
#include <funcl.h>

int open(const char *path,int oflag,.../* mode_t mode */);
int openat(int fd,const char *path,int oflag,.../* mode_t mode */);

/* Both return:file descriptor if ok,-1 on error */
```
- **The _mode_ parameter is used only when a new fie is being created.**
- **The _oflag_ parameter are mostly:**
	1. **O_RDONLY** - Open for reading only.
	2. **O_WRONLY** - Open for writing only.
	3. **O_RDWR** - Open for reading and writing.
	4. **O_APPEND** - Append to the end of file on each write.
	5. **O_CREAT** Create the file if it doesn't exist.This option requires a third argument.
- **The _fd_ parameter has three possibilities:**
	1. The _path_ parameter specifies an absolute pathname:The _fd_ parameter is ignored and the openat() behaves like open();
	2. The _path_ parameter specifies a relative pathname and the _fd_ parameter is a file descriptor that specifies the starting location in the file system where the relative pathname is to be evaluated;
	3. The _path_ parameter specifies a relative pathname and the _fd_ parameter has the special value AT_FDCWD:The pathname is evaluated starting in the current working directory and the openat() behaves like open();
	
## creat()
```c++
#include <fcntl.h>

int creat(const char *path,mode_t mode);

/* This function is equivalent to: */
open(path,O_WRONLY|O_CREAT|O_TRUNC,mode);

/* If we are creating a temporary file that we wanted to write and then read back,we had to call creat(),close(),and then open().A better way is to use open(),as in: */
open(path,O_RDWR|O_CREAT|O_TRUNC,mode);

/* Return:file descriptor opened for write-only if ok,-1 on error */
```

## close()
```c++
#inlcude <unistd.h>

int close(int fd);

/* Return:0 if ok,-1 on error */
```
- **When a process terminates,all of its open files are closed automatically by the kernel.**

## lseek()
```c++
#include <unistd.h>

off_t lseek(int fd,off_t offset,int whence);

/* To get the current offset: */
off_t currpos=lseek(fd,0,SEEK_CUR);
/* This can also be used to determine if a file is capable of seeking.If fd refers to a pipe/FIFO/socket,lseek() set errno to ESPIPE and returns -1. */

/* Return:new file offset if ok,-1 on error */
```
- **The interpretation of the _offset_ depends on the value of the _whence_ argument:**
	1. **SEEK_SET** - The file's offset is set to offset bytes from the beginning of the file;
	2. **SEEK_CUR** - The file's offset is set to its current value plus the offset.The offset can be positive or negative;
	3. **SEEK_END** - The file's offset is set to the size of the file plus the offset.The offset can be positive or negative;
- **Because negative offsets are possible,we should be careful to compare the return value from lseek() as being equal to or not equal to -1,rather than testing whether it is less than 0;**
- **lseek() only records the current file offset within the kernel-it does not cause any I/O to take place.This offset is then used by the next read or write operation.**
- **The file's offset can be greater than the file's current size,in which case the next write to the file will extend the file.**
- **Note that even though you might enable 64-bit file offsets,your ability to create a file larger than 2GB depends on the underlying file system type.**

## read()
```c++
#include <unistd.h>

ssize_t read(int fd,void *buf,size_t nbytes);

/* Return:number of bytes read,0 if end of file,-1 on error */
```

- **The read() starts at the file's current offset.Before a successful return,the offset if incremented by the number of bytes actually read.**

## write()
```c++
#include <unistd.h>

ssize_t write(int fd,const void *buf,size_t nbytes);

/* Return:number of bytes writen if ok,-1 on error */
```
- **The return value is usually equal to the _nbytes_ argument.Otherwise,an error has occurred.**
- **If the O_APPEND option was specified when the file is opened,the file's offset is set to the current end of file before each write operation.After a successfull write,the file's offset is incremented by the number of bytes actually writen.**

## File Share
- **The kernel uses three data structures to represent an open file:**
	1. Every process has an entry in the process table.Within each process table entry is a table of open file descriptors,with one entry per descriptor.Associated with each file descriptor are:
		- The file descriptor flags(close-on-exec);
		- A pointer to a file table entry;
	2. The kernel maintains a file table for all ope files.Each file table entry contains:
		- The file status flags for the file,such as read,write,append,sync,and nonblocking;
		- The current file offset;
		- A pointer to the v-node table entry for the file;
	3. Each open file(or device) has a v-node structure that contains information about the type of file and pointers to functions that operate on the file.For most files,the v-node also contains the i-node for the file.This information is read from disk when the file is opened,so that all the pertinent information about the file is readily available(For example,the i-node contains the ownner,size,pointers to where the actual data blocks are located on disk and so on).
- **If two independent process have the same file open,each process gets its own file table entry(each process has its own current offset for the file),but only a single v-node table entry is required for a given file.**
- **After each write() is complete,the current file offset in the file table entry is incremented by the number of bytes written.If this causes the current file offset to exceed the current file size,the current file size in the i-node table entry is set to the current file offset.**
- **If a file is opened with the O_APPEND flag,a corresponding flag is set in the file status flags of the file table entry.Each time a write() is performed for a file with this append flag set,the current file offset in the file table entry is first set to the current file size from the i-node table entry(This forces every write() to be appended to the current end of file).**
- **If a file is positioned to its current end of file usig lseek(),all that happens is the current file offset in the file table entry is set to the current file size form the i-node table entry(Note that this is not the same as if the file is opened with the O_APPEND flag).**
- **The lseek() modifies only the current file offset in the file table entry,no I/O takes place.**
- **It is possible for more than one file descriptor entry to point to the same file table entry,such as dup() and fork().**
- **The difference in scope between the file descriptor flags and the file status flags are:The former apply only to a single descriptor in a single process,whereas the latter apply to all descriptors in any process that point to the given file table entry.**
