# clone
+ (function, starting stack addr, share what?, void* arg to function)
+ to create process or thread

## flags-

### CLONE_FS
+ If CLONE_FS is set, the caller and the child process share the same
filesystem information.  This includes the root of the  filesystem,
the  current  working  directory,  and  the umask.  Any call to `chroot(2)`, `chdir(2)`, or `umask(2)` performed by the calling process  or
the child process also affects the other process.
+ If  CLONE_FS  is  not set, the child process works on a copy of the filesystem information of the calling process at the  time  of  the
clone  call.

### CLONE_FILES
+ If CLONE_FILES is set, the calling process and  the  child  process share  the `same file descriptor table`.  Any file descriptor `created` by the calling process or by the child process is also valid in the
other  process. Similarly,  if one of the processes `closes` a file descriptor, or changes its associated  flags  (using  the  fcntl(2)
F_SETFD  operation),  the  other  process  is  also affected. 
+ If a process sharing a file descriptor table calls execve(2),  its  file
descriptor table is duplicated (unshared).

+ If CLONE_FILES is not set, the child process inherits a copy of all file descriptors opened in the calling process at the time  of  the clone call. Subsequent operations that open or close file descrip‐
tors, or change file descriptor  flags,  performed  by  either  the calling  process  or  the  child  process  do  not affect the other process. 
+ Note, however, that the duplicated  file  descriptors  in the  child  refer  to the same open file descriptions as the corresponding file descriptors in the calling process,  and  thus  share file offsets and file status flags (see open(2)).

### CLONE_PARENT
If  CLONE_PARENT  is  set, then the parent of the new child (as re‐
turned by getppid(2)) will be the  same  as  that  of  the  calling
process.

If CLONE_PARENT is not set, then (as with fork(2)) the child's par‐
ent is the calling process.

Note that it is the parent  process,  as  returned  by  getppid(2),
which  is signaled when the child terminates, so that if CLONE_PAR‐
ENT is set, then the parent of the calling process, rather than the
calling process itself, will be signaled.

The  CLONE_PARENT  flag  can't be used in clone calls by the global
init process (PID 1 in the initial PID  namespace)  and  init  pro‐
cesses in other PID namespaces.  This restriction prevents the cre‐
ation of multi-rooted process trees as well as the creation of  un‐
reapable zombies in the initial PID namespace.
