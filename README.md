## README

### Spec1 :

* Files: prompt.c and prompt.h

### Spec2 :

* Files: input.c and input.h
* Assumed maximum input length to be 4096

### Spec3 :

* Files: hop.c and hop.h
* Assumed that at max 1000 arguements to hop and max length of each is 4096
* In case of multiple arguements, everything is executed till the first wrong arguement
* if hop test - is executed in ~ then after command also it will remain in ~ as i have taken that first it will hop to test and then for '-'
it will check old pwd which will be ~
* if hop - is executed and no previous diirectory is set, the error "OLDPWD is not set" is printed.

### Spec4 :

* Files: reveal.c and reveal.h
* Assumed max number of files in a directory to be 4096
* for Q22 : if reveal - is executed and no previous diirectory is set, the error "OLDPWD is not set" is printed.
* for Q23 : only the name of the file/directory/symlink/… is colored in "reveal -l"

### Spec5 :

* Files: log.c and log.h
* Storing erroneous commands also
* Creating a file log.txt and storing commands in it to track commands along all sessions
* For log execute I only add it to history if the command is not exactly same as the latest one.
* if the input consists only of spaces , then it is not stored in history

### Spec6 :

* Files: bgfg.c and bgfg.h
* I am only checking if the LATEST foreground process(of built-in commands) is taking more than 2 seconds for execution or not
* for Q5 : if sleep 3; sleep 5 the latest foreground process is sleep 5 and i will only consider that and print sleep 5 in next prompt
* for Q6 : for erroneous commands (background) also pid and the message of completion is printed
* for Q14 : as it is mentioned that it wont be tested so i am not handling this case
* I have made a hashmap for storing the pids and name of the process
* If latest foreground process takes more than 2 seconds we will display it in prompt and if we just do enter it wont disappear untill we type any command

### Spec7 :

* Files: proclore.c and proclore.h
* Units of virtual memory size is bytes
* Some processes for which executable path cant be obtained, for them only path wont be printed. Rest info will be printed.

### Spec8 :

* Files: seek.c and seek.h
* for Q9: " seek file_name - " will search in the old pwd
* Assumed maximum matches of file/directory name to be 4096
* Assumed that we have to not search in hidden folders

### Spec9:
* Files: alias.c and alias.h
* I have implemented alias functions without the "func" keyword in .myshrc
* Alias is stored in log instad of original command
* I have assumed that to write an alias function in .myshrc the format should be:<br>
 func_name () { <br>
    }

* In "$i",  i should be a number ("@" is not allowed)

### Spec 10:
* Files: redirection.c and redirection.h
* q21 : assumed that the I/O redirection is used only at the end of the command and all its parameters.

### Spec 11:
* Files: pipe.c and pipe.h
* if & applied at last of the pipe only the command preceding it will run in background.
* echo "Hello World" > file.txt & | wc : (For such case my shell will execute sequentially from left to right untill error occurs i.e echo will work in bg and then error comes due to syntax "& | ").
* hop .. | wc will go to parent dir and wc will also work


### Spec 12:
* Files: pipe.c and pipe.h
* ifelse.c and ifelse.h used for both piping and redirection

### Spec13 : 
* Files : activities.c and activities.h
* I am displaying full command. Eg : sleep 10

### Spec14:
* Files : ping.c and ping.h
* When we do ctrl+D and background processes exit, i am not printing the termination message for them.

### Spec15 :
* Files: bg.c and bg.h
* When a bg process will become fg and it runs more than 2 seconds then in next prompt fg will be displayed as the command name.

### Spec16 :
* Files: neonate.c and neonate.h
* Assumption : Ctrl-C interrupt wont have any effect on neonate process. It can only be stopped by pressing 'x'.

### Spec17 :
* Files: iman.c iman.h




