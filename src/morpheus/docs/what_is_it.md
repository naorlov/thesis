# What is Morpheus?

## Assumptions

We assume that programs tested would be only mappers and reducers, reading data
from stdin and writing data to stdout. Also we assume that program does not
open any sockets and does not perform additional reads or writes from any data
storage. 

## What does morpheus measure?

Overall performance, estimated FLOPS equivalent of program execution. In
addition it builds analytical model of program execution. We assume that no
program will surpass time and space complexity of O(n^15), thus redusing amount
of testing required to build reliable model.

## What is FLOPS equivalent?



## Examples:

`morpheus wordcount/mapper.exe std.in std.out std.err`

If you need to pass additional arguments to your executable, you can wrap
startup command either in quotes or write all neseccary preparations to
`run.sh` file.


