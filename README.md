# Hamming-and-CRC-algorithms-main--2-
 Error Detection
 

Purpose 
The goal of this exercise is to apply both Hamming and CRC algorithms to detect errors in transmitted messages by means of CRC and to both detect and correct transmission errors by using Hamming.  
 
What to Do 
You are to modify your solution for Project 2 to include an error detection component by means of CRC and an error detection and/or correction by using Hamming. You are to use  CRC-32   =   x32+ x26+ x23+ x22+ x16+ x12+ x11+ x10+ x8+ x7+ x5+ x4+ x2+ x + 1 
and apply it to the entire message component of the frame for CRC checking and only to the data contents of the message for Hamming.  
Your solution must 
1.	Include a way to allow the user to include errors. 
2.	Display to the user the location where the error was inserted. Note that this characteristic is necessary for both debugging during your coding as well as for checking the correctness of your solution during grading your work. 
3.	Display to the user a CRC transmission error message for the CRC detected error. 
4.	Display to the user a Hamming transmission error message when more than a single bit is corrupted and also a message to inform the user about the successful single bit error correction done by the Hamming algorithm. 
5.	A control mechanism such as a flag to allow the user to select either CRC or Hamming before the transmission process takes place.  
 
Testing 
You are to use two different text files for testing. The testing file used for project2 is to be used as one of such files. Your second file must be long enough to require many frames to be created. Your testing must include the following: 
1.	An error detection by CRC, 
2.	An error detection by Hamming, and 
3.	An error detection and correction by Hamming. 
 
 
What to Submit 
a)	Your solution must be uploaded to Blackboard. 
 
 
b)	Copies of your source files for your producer/server and your consumer/client as well as their executables, and any data you used for testing your solution. 
c)	You are to place all files that are related to your solution into a .zip file. Your .zip file must follow the format: CSI 500 Project3 Your Name. 
d)	The documentation associated with your solution must be typeset and saved in MS Word. Marks will be deducted if you do not follow this requirement. 
  
 
Documentation 
Your program should be developed using GNU versions of the C compiler. It should be layered, modularized, and well commented. The following is a tentative marking scheme and what is expected to be submitted for this assignment: 
 
1.	External Documentation (as many pages necessary to fulfill the requirements listed below.) including the following: 
a.	Title page 
b.	A table of contents 
c.	[20%] System documentation 
i.	A high-level data flow diagram for the system 
ii.	A list of routines and their brief descriptions 
iii.	Implementation details 
d.	[5%] Test documentation 
i.	How you tested your program 
ii.	Test sets must include the text file used for testing project2. iii. 	Testing outputs 
e.	[5%] User documentation 
i.	How to run your program ii. 	Describe parameter (if any) 
 
2.	Source Code 
a.	[65%] Correctness 
b.	[5%] Programming style 
i.	Layering 
ii.	Readability 
iii.	Comments 
iv.	Efficiency 
 
          
 
