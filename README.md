# Automatic testing C programs

In every subfolder are located excercises written in C.
But in one subfolder can't be more then one program to run. This is because name of folder is also name of the test inside directory. To propely run testing you have to write one more file named: tests.test in which will be include tests.

For example, you have to write program that write Hello World to output. 
1. On start, you must create dictionary for program (which name will be displayed in testing program as program name). 
2. After you finish writing the code you save it as hello.c file. You don't have to compile it to .exe because program automaticly do it for you :). 
3. At the end create in that dictionary file tests.test with such build:

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