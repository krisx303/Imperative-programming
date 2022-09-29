# Imperative programming in C language
This repository contains my exercises for college classes (in folders 1. 2.1. 2.2. ...). In the main directory is a testing system created by myself to make testing easier. 


# Automatic testing C programs

In every subfolder are located excercises written in C.
But in one subfolder can't be more then one program to run. This is because name of folder is also name of the test inside directory. To propely run testing you have to write one more file named: tests.test in which will be include tests.

For example, you have to write program that write Hello World to output. 
1. If you don't have src directory, create it in main folder. 
2. On start, you must a create dictionary for the program (which name will be displayed in the testing program as the program name). 
3. After you finish writing the code, you save it as a hello.c file. You don't have to compile it to .exe because the program automaticly does it for you :).
4. Finally, add tests to that dictionary file. test with such a build:

> <pre>
> hello                   (#0 name of c file to test)
> #TEST                   (# tags are required informations for testing program)
> Print Hello World       (#1 first test name)
> #INPUT
>                         (#2 Program input, in this case empty line)  
> #OUTPUT                 
> Hello World             (#3 Expected program output)
> #EOF                    (#4 End of file)
</pre>

#0 and #4 tag supposed to be unique in a file. But segment with #TEST #INPUT and #OUTPUT could be used many times (for each test which you want to include).

Program during running creates 3 files in his folder:
* tmp (for storage information about dir output command)
* tmpin (for storage temporary input for test)
* tmpout (for storage temporary output for test)

Of course if you want you could remove then after closing program but they are neccesery during running :)