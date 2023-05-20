#include <unistd.h>

// int dup(int oldfd);
// int dup2(int oldfd, int newfd);

// DESCRIPTION
// The dup() system call creates a copy of the file descriptor oldfd, us‐
// ing the lowest-numbered unused file descriptor for the new descriptor.

// After a successful return, the old and new  file  descriptors  may  be
// used  interchangeably.   They  refer to the same open file description
// (see open(2)) and thus share file offset and file  status  flags;  for
// example,  if  the  file offset is modified by using lseek(2) on one of
// the file descriptors, the offset is also changed for the other.

// The two file descriptors do  not  share  file  descriptor  flags  (the
// close-on-exec  flag).   The  close-on-exec  flag  (FD_CLOEXEC; see fc‐
// ntl(2)) for the duplicate descriptor is off.

// The dup2() system call performs the same task as dup(), but instead of
// using the lowest-numbered unused file descriptor, it uses the file de‐
// scriptor number specified in newfd.  If the file descriptor newfd  was
// previously open, it is silently closed before being reused.

// *  If  oldfd  is not a valid file descriptor, then the call fails, and
//     newfd is not closed.

// *  If oldfd is a valid file descriptor, and newfd has the  same  value
//     as oldfd, then dup2() does nothing, and returns newfd.

int main() {}