Yixing Zheng

zheng129@mail.chapman.edu

May 8, 2019

Project 3

Worked on this project together with Zach Granoff. 

This program requires a library called caffe and it seems like caffe is not available on MacOS, so we had to compiler the program online. Here are two websites that we use to compile the program and they both provide the same output:
- https://repl.it/languages/cpp
- http://cpp.sh/

For compiling the program, you need to copy all the code into the online editor, and click run, then the output should be displayed. A sample output picture is also included in the submission file.

For the design of the program, we used mutex and semaphores for synchronizations and protecting shared data. The priority is scheduled as first in first out, and upon completion, the priority of each process would be lower until all processes are finished. Also, CPU affinity is implemented. 
