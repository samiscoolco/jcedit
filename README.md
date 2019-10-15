<font size="32"> jcedit 3 </font>
<font size = "">
  _"The working man's command line text editor."_
  
  ---
 

## How to use jcedit

#### > Getting started

Run jcedit in your terminal by simpling typing
	
    jcedit

You can also enter a filename as an arguement that will then be opened for editing by jcedit like so:
	
    jcedit yourfilenamehere
    
If no file is specified you will be met with the file entry screen:

![alt-text](https://i.imgur.com/zJ0NbLV.png", "File entry screen")

Simply enter a desired filename, and then press return to begin editing.

#### > Editing process



Upon starting the editing process, you will be met with a screen like this:

![alt-text](https://i.imgur.com/aMUEDhn.png, "Initial editing screen")

To begin editing, simply type a line and press enter, this will write your input to the current line number, and then move to the next line.

As we can see here: 

![alt-text](https://i.imgur.com/H32tK8o.png "Writing a file")

Your current line number is represented to the left of the cursor, the line will be overwritten by whatever you input.

#### > Saving your changes

To save your file, simply type `.sv` and press enter.

To stop editing, type `.qt` and press enter.

#### _Refer to commands list below for more information!_


---

## Commands
To use a command, simply time it with no extra characters included and then press enter.

- ### Save: `.sv`
   Saves the file.

- ### Quit: `.qt`
   Quits jcedit.

- ### Show file: `.ls`
   Reprints all lines in file.
   
   Feature in progress to allow scrolling through the file. (Very necessary)
   
  

- ### Change Current Line `.ln`
  Waits for user to enter a line number to move the cursor to. 

  If the input isn't valid, it will either go to line 0, or the last line of the file.


---

## Credits
 
 Created by:
 	
   - [sam0s]("https://github.com/sam0s")


Originally intended as a system tool for unreleased operating system JCOS.
