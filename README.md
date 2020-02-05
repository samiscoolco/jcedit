# jcedit 4 
  _"The working man's command line text editor."_
  
  ---
 

## How to use jcedit

#### > Getting started

To install jcedit, download the files from the repository and then first run the setup file. This will install jcedit and it's default files into your `/bin` and the default syntax files into `/bin/jcdata/syntaxes`

Then you can run jcedit in your terminal by simpling typing
	
    jcedit

You can also enter a filename as an arguement that will then be opened for editing by jcedit like so:
	
    jcedit yourfilenamehere
    
If no file is specified you will be met with the file entry screen:

![alt-text](https://i.imgur.com/svWJjGZ.png, "File entry screen")

Simply enter a desired or existing filename, and then press return to begin editing your new or existing file.

#### > Editing process



Upon starting the editing process, you will be met with a screen like this:

![alt-text](https://i.imgur.com/j9nxKgi.png, "Initial editing screen")

By default jcedit will be in **command mode**. In command mode you can enter and make use of various commands (_see below_.)

Any non-command text that is entered in command mode will overwrite the current line as indicated by either `CUR_LINE` in the top status bar, or by the bold yellow number on the line display.

As we can see here: 

![alt-text](https://i.imgur.com/pJbQO9M.png, "Writing a file")

For advanced text entry, use the `.i` command to enter insert mode. This allows you to move around the file with the arrow keys and insert characters wherever needed, this will most likely be your main method of text entry.
Your current position is indicated with a cursor like such:

![alt-text](https://i.imgur.com/jAVGZ1t.png, "Insert mode")


#### > Saving your changes

To save your file, simply use the  `.sv` command.

To close the file, use the `.qt` command.

#### _Refer to commands list below for more information!_


---

## Commands
To use a command, first ensure that you are in command mode and then simply type the command with no extra characters included and then press enter.


- ### Help: `..?`
   Shows the help screen.

- ### Save: `.sv`
   Saves the file.

- ### Quit: `.qt`
   Quits jcedit.

- ### Move mode: `.mv`
   Enters view-movement mode, controlled with W/S or UP ARROW / DOWN ARROW.
   Press `ESC` to return to command mode.
   
- ### Edit mode: `.i`
   Enters instert mode which allows for non-canoncial character entry.
   Press `ESC` to return to command mode.
   

- ### Change Current Line `.ln`
  Waits for user to enter a line number to move the cursor to. 
  If the input isn't valid, it will either go to line 0, or the last line of the file.



---

## Credits
 
 Created by:
 	
   - [sam0s](https://github.com/sam0s "Sam")
   - [jdedmondt](http://github.com/jdedmondt "Jack")
   - [jaredcop](https://github.com/jaredcop "Jared")
   
   
_check out the wiki for information on the more advanced features of jcedit 4_

_originally intended as a system tool for unreleased operating system JCOS._
