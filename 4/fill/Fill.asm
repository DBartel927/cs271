// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(LOOP)
@KBD
D=M
@PRESS
D;JGT
@RELEASE
0;JMP
(PRESS)
@R0
M=-1
@APPLY
0;JMP
(RELEASE)
@R0
M=0
@APPLY
0;JMP
(APPLY)
@8191
D=A
@R1
M=D
(NEXT)
@R1
D=M
@CUR
M=D
@SCREEN
D=A
@CUR
M=D+M
@R0
D=M
@CUR
A=M
M=D
@R1
D=M-1
M=D
@NEXT
D;JGE
@LOOP
0;JMP
