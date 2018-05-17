Like `cat` but does not copy data to the process userspace.

But `splice(1)` (first attempt) still needs the userspace to iterate
over (apparently 8192 byte sized) chunks. `sendfile(1)` does it in one
go, but still doesn't appear to make the receiving end of a pipe see
the file size.

This is disappointing, as I was hoping to be able to actually
short-cut the filehandles; looks like once someone has made a pipe
data structure short cutting that isn't possible (perhaps for reasons
of compliancy with some definitions?)?
