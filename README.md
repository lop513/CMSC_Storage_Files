The way I approached the buffer part of the project was to first figure out how
to do the init function, so that I could have a buffer to work with. After
figuring out how to use the buffer write node like the head node of a
linked list, it became easy enough to understand.
After that I worked on the print function which was also simple due to the
use of the buffer read node acting like a temp variable that goes through
your list.
Once print was done I worked on the delete function, this was a little tricky
becuase I was infinite looping, but once I realized that I could use a temp
variable to point to head->next, it became simple enough.
Then I worked on the insert function, through the use of the buffer write node
it was clear that I just had to iterate one by one through the list each time
it was called.
After I finished the functions I created a variety of tests in test.c and
test_user.c. These tests mainly focused on the normal operation of the buffer
and then the edge cases that I could think of.

In regards to compiling in user space, run the following command,
   "gcc buffer.h buffer.c test_user.c -o buffer".

Then run the program by typing "./buffer". This will then go through
my tests in test_user.c.

In regards to compiling in kernel space, you will to compile and install
the kernel and then reboot. After that you can use the Makefile I provided
by typing, "Make test" then type "./test" and it will run the test with
buffer.h, buffer.c and test.c. In order to see the kernel log statements
you will have to run, "sudo dmesg". Now with the stdout and kernel log output
that should show you all of my tests.