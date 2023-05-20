#include <unistd.h>

// ssize_t read(int fd, void *buf, size_t count);
// fd： open得到的描述符
// buf: 你要读取数据放的地方（数组）（传出参数）
// count: 指定容器的大小（截断）
//  >  0： 返回实际读到的字符数
// ==  0： 文件读取完了
// == -1： 读取失败

// DESCRIPTION
//     read() attempts to read up to count bytes from file descriptor fd into
//     the buffer starting at buf.

//     On files that support seeking, the read  operation  commences  at  the
//     file offset, and the file offset is incremented by the number of bytes
//     read.  If the file offset is at or past the end of file, no bytes  are
//     read, and read() returns zero.

//     If  count  is  zero, read() may detect the errors described below.  In
//     the absence of any errors, or if read() does not check for  errors,  a
//     read() with a count of 0 returns zero and has no other effects.

//     According  to  POSIX.1, if count is greater than SSIZE_MAX, the result
//     is implementation-defined; see NOTES for the upper limit on Linux.

// RETURN VALUE
//     On success, the number of bytes read is returned (zero  indicates  end
//     of file), and the file position is advanced by this number.  It is not
//     an error if this number is smaller than the number of bytes requested;
//     this may happen for example because fewer bytes are actually available
//     right now (maybe because we were close to end-of-file, or  because  we
//     are  reading  from  a pipe, or from a terminal), or because read() was
//     interrupted by a signal.  See also NOTES.

//     On error, -1 is returned, and errno is  set  appropriately.   In  this
//     case,  it  is  left  unspecified  whether  the  file position (if any)
//     changes.

// ssize_t write(int fd, const void *buf, size_t count);
// count: 要写的数据的大小
// return: 实际写入的大小 or -1
// DESCRIPTION
//     write()  writes  up  to count bytes from the buffer starting at buf to
//     the file referred to by the file descriptor fd.

//     The number of bytes written may be less than count  if,  for  example,
//     there  is insufficient space on the underlying physical medium, or the
//     RLIMIT_FSIZE resource limit is encountered (see setrlimit(2)), or  the
//     call  was  interrupted  by  a signal handler after having written less
//     RLIMIT_FSIZE resource limit is encountered (see setrlimit(2)), or  the
//     call  was  interrupted  by  a signal handler after having written less
//     than count bytes.  (See also pipe(7).)

//     For a seekable file (i.e., one to which lseek(2) may be  applied,  for
//     example,  a  regular file) writing takes place at the file offset, and
//     the file offset is incremented by the number of bytes  actually  writ‐
//     ten.   If  the  file  was  open(2)ed with O_APPEND, the file offset is
//     first set to the end of the file before writing.   The  adjustment  of
//     the  file  offset  and  the write operation are performed as an atomic
//     step.

//     POSIX requires that a read(2) that can be  proved  to  occur  after  a
//     write()  has  returned  will  return  the new data.  Note that not all
//     filesystems are POSIX conforming.

//     According to POSIX.1, if count is greater than SSIZE_MAX,  the  result
//     is implementation-defined; see NOTES for the upper limit on Linux.

// RETURN VALUE
//     On  success, the number of bytes written is returned.  On error, -1 is
//     returned, and errno is set to indicate the cause of the error.

//     Note that a successful write() may transfer fewer  than  count  bytes.
//     Such  partial  writes  can occur for various reasons; for example, be‐
//     cause there was insufficient space on the disk device to write all  of
//     the  requested  bytes, or because a blocked write() to a socket, pipe,
//     or similar was interrupted by a signal handler  after  it  had  trans‐
//     ferred some, but before it had transferred all of the requested bytes.
//     In the event of a partial write, the caller can make  another  write()
//     call to transfer the remaining bytes.  The subsequent call will either
//     transfer further bytes or may result in an error (e.g., if the disk is
//     now full).

//     If count is zero and fd refers to a regular file, then write() may re‐
//     turn a failure status if one of the errors below is detected.   If  no
//     errors  are  detected,  or error detection is not performed, 0 will be
//     returned without causing any other effect.  If count is  zero  and  fd
//     refers to a file other than a regular file, the results are not speci‐
//     fied.

