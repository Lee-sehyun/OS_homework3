Lee sehyun's Operating System homework3
========================

Compile Method
---------------

After cloning the repagitori, the TA goes into the HW3 folder.
Open the terminal inside the HW3 folder and run make. 
Create an executable named pfind.

How to execute
-------------

When the compilation is complete, enter ./pfind [<option>]* <dir> [<keyword>]+ in the HW3 folder to execute the executable.
Here, ./pfind -p 5 -c -a dir1 char stR kkk

Initialization Method
-------------------------

If you enter "make clean" inside the HW3 folder, all files created will be removed.

Video link
---------------
https://youtu.be/60_F6NT2hMQ



subtitle
----------

Hello, I'm Lee Sehyun from 21900516.

I'll start the presentation now.

First, I will explain the protocol of the worker and manager.
Please look at this white board. Here's the manager, and there's workers down there. And there is a pipe called tasks, results. The tasks role hands over the path of the directory to workers in the manager. where path can be an absolute path. And in the worker, you can take over the path. The results are opened in tasks, handed over the new path to the directory, handed over the letters that indicate that the file is, and then opened the file. And we compare each line, which can eliminate case classification. Compare each line and print out the line if there is a keyword character and hand over the letter that there is a line.

Now, let's look at the source code of the program.
This is how folders and files are in the folder in dir1. And if you look at pfind.c, it's divided into pipes, functions for lock, functions for receiving options, functions for executing. If you look at the main, you can send SIGINT (ctrl+c) through the signal to end the function and perform a function that outputs the result. Then, do the fork according to the number of child processes you received and run each worker. The worker reads from tasks and runs findKeyword. findKeyword opens the directory to locate the file, and then outputs the line containing the keyword. The manager sends the first directory to the worker and prepares to read what the worker sends. This is one of the path, file flag, line flag, and end flag of the directory. If all workers are finished, perform a function that terminates the function and outputs the result.

When you run the code, you can see these multiple workers working at the same time, and you can see that you press Ctrl+c to get good results. I showed you ctrl+c because there are more than 50,000 lines of data in the file now.

It's hard to understand the part "reader first reads 4 bytes to demine" while creating the pfind, so I'm sorry I couldn't implement it. Also, I wanted to refine the block more, but I am sorry that it didn't work out. But most of the others have been implemented.


-?????????-
Hello, I'm Lee Sehyun from 21900516.

???????????? ????????? ??????????????? ???????????????.

?????? Worker??? manager??? protocol??? ????????? ?????????????????????.
??? ????????? ????????? ????????????. ????????? manager??? ??????, ????????? fork??? worker?????? ????????????. ????????? tasks, results?????? pipe??? ????????????. tasks??? ????????? manager?????? worker????????? ??????????????? path??? ???????????????. ????????? path??? ?????? ????????? ??? ??? ????????????. ????????? worker????????? path??? ???????????? ??? ????????????. results??? tasks?????? ????????? ??????????????? ????????? path??? ????????????, ???????????? ???????????? ???????????? ????????? ????????? ??? ????????? ?????????. ????????? ????????? ????????? ????????? ?????????, ????????? ???????????? ????????? ?????? ??? ??? ????????????. ????????? ????????? ????????? ?????? ?????? keyword ????????? ????????? ??? ?????? ???????????? ??? ????????? ????????? ????????? ???????????????.

?????? ??????????????? ??????????????? ???????????????. 
dir1??? ?????? ?????? ?????? ????????? ????????? ???????????? ????????????. ????????? pfind.c??? ?????? pipe, lock??? ?????? ??????, ????????? ???????????? ??????, ???????????? ???????????? ???????????? ????????????. main??? ?????? ???????????? ?????? SIGINT(ctrl+c)??? ???????????? ????????? ???????????? ????????? ??????????????? ????????? ????????? ?????????. ????????? ????????? child process??? ????????? fork??? ?????? ??? ????????? worker??? ????????? ???????????????. worker????????? tasks?????? ????????? findKeyword??? ???????????????. findKeyword??? ??????????????? ????????? ????????? ?????? ???, ???????????? ???????????? ????????? ???????????????. manager????????? ??? ??????????????? worker??? ????????? ??? worker?????? ???????????? ?????? ?????? ????????? ?????????. ????????? ??????????????? ??????, ?????? ?????????, ?????? ?????????, ??? ????????? ??? ???????????????. ?????? ?????? worker??? ??? ????????? ????????? ???????????? ????????? ??????????????? ????????? ????????? ?????????.

????????? ??????????????? ????????? ?????? worker??? ????????? ???????????? ?????? ??? ??? ?????????, ctrl+c??? ????????? ??? ????????? ????????? ?????? ??? ??? ????????????. ?????? ???????????? 5?????? ????????? ???????????? ?????? ????????? ctrl+c??? ?????????????????????. 

pfind??? ???????????? ???reader first reads 4 bytes to determine????????? ????????? ???????????? ????????? ????????? ?????? ?????? ???????????? ????????????. ?????? flock????????? ??? ??? ????????? ????????????, ??? ?????? ?????? ???????????? ????????????. ????????? ??? ??? ????????? ????????? ???????????????.







???????????? Operating System homework3
========================

????????? ??????
--------------

?????????????????? clone??? ??? (TA?????? ????????? ??? ???) HW3????????? ????????????.
HW3?????? ????????? ???????????? ??? ?????? make??? ????????????. 
pfind????????? ?????? ????????? ?????????.

?????? ??????
-------------

???????????? ???????????????, HW3?????? ????????? ./pfind [<option>]* <dir> [<keyword>]+??? ????????? ??????????????? ???????????????.
???????????? ./pfind -p 5 -c dir1 char stR kkk??? ?????????.


????????? ??????
-------------

HW3?????? ????????? make clean??? ???????????? ???????????? ???????????? ?????? ?????????.

????????? ??????
------------
https://youtu.be/60_F6NT2hMQ