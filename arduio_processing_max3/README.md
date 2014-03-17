##Intruction
This example can get value from the 8 dials, and send the data to processing, Dont rotate too much, I have not revise the multiply parameter.

##Problem
1. I tried to use "for loop" to wrap the array, it worked weird, maybe because I did not put "delay" inside the loop
2 serial.print will send string data rather than byte data, have not find many document about string, and some says serial.print is slow. so i use serial.write
