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


-한국어-
Hello, I'm Lee Sehyun from 21900516.

이제부터 발표를 시작하도록 하겠습니다.

먼저 Worker와 manager의 protocol에 대해서 설명하겠습니다.
이 화이트 보드를 봐주세요. 여기에 manager가 있고, 아래로 fork된 worker들이 있습니다. 그리고 tasks, results라는 pipe가 있습니다. tasks의 역할은 manager에서 worker들에게 디렉토리의 path을 넘겨줍니다. 여기서 path는 절대 경로가 될 수 있습니다. 그리고 worker에서는 path을 넘겨받을 수 있습니다. results는 tasks에서 열어서 디렉토리면 새로운 path를 넘겨주고, 파일이면 파일임을 알려주는 글자를 넘겨준 뒤 파일을 엽니다. 그리고 각각의 라인을 비교를 하는데, 여기서 대소문자 구분을 없애 줄 수 있습니다. 각각의 라인을 비교를 해서 만약 keyword 글자가 있으면 그 줄을 출력해준 뒤 라인이 있다는 글자를 넘겨줍니다.

이제 프로그램의 소스코드를 보겠습니다. 
dir1의 폴더 안에 이런 식으로 폴더와 파일들이 있습니다. 그리고 pfind.c를 보면 pipe, lock을 위한 함수, 옵션을 받아오는 함수, 실행하는 함수들로 나누어져 있습니다. main을 보면 시그널을 통해 SIGINT(ctrl+c)를 보내주면 함수를 끝내주고 결과를 출력해주는 함수를 실행을 합니다. 그리고 받았던 child process의 수대로 fork를 해준 뒤 각각의 worker를 실행을 시켜줍니다. worker에서는 tasks에서 읽어서 findKeyword를 실행합니다. findKeyword는 디렉토리를 열어서 파일을 찾은 뒤, 키워드가 들어있는 라인을 출력합니다. manager에서는 첫 디렉토리를 worker에 보내준 뒤 worker에서 보내주는 것을 읽을 준비를 합니다. 이것은 디렉토리의 경로, 파일 플래그, 라인 플래그, 끝 플래그 중 하나입니다. 만약 모든 worker가 다 끝나면 함수를 끝내주고 결과를 출력해주는 함수를 실행을 합니다.

코드를 실행해보면 이렇게 여러 worker가 동시에 작동되는 것을 볼 수 있으며, ctrl+c를 누르면 잘 결과가 나오는 것을 볼 수 있습니다. 지금 파일에는 5만줄 이상의 데이터가 있기 때문에 ctrl+c를 보여드렸습니다. 

pfind를 만들면서 “reader first reads 4 bytes to determine”이란 부분을 이해하기 어려워 구현을 하지 못한 아쉬움이 있습니다. 또한 flock부분을 좀 더 다듬고 싶었는데, 잘 되지 못한 아쉬움이 있습니다. 하지만 그 외 대부분 구현이 되었습니다.







이세현의 Operating System homework3
========================

컴파일 방법
--------------

레파지토리를 clone한 뒤 (TA님은 압축을 푼 뒤) HW3폴더로 들어간다.
HW3폴더 안에서 터미널을 연 다음 make를 실행한다. 
pfind이라는 실행 파일을 만든다.

실행 방법
-------------

컴파일을 완료했으면, HW3폴더 안에서 ./pfind [<option>]* <dir> [<keyword>]+을 입력해 실행파일을 실행시킨다.
여기서는 ./pfind -p 5 -c dir1 char stR kkk를 해보자.


초기화 방법
-------------

HW3폴더 안에서 make clean을 입력하면 만들어진 파일들을 전부 없앤다.

동영상 링크
------------
https://youtu.be/60_F6NT2hMQ