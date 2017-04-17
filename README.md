Implementation of the public-key cryptography based on integer third-degree polynomials
=======================================================================================

This version is without a long arithmetic.

INTRODUCTION
------------

This implementation allows to test the algorithm with the next input data:

	Amount of variables:	from 0 to infinity; |infinity|
	Size of a variable:	from 3 to 62;
	Size of a module:	from 4 to 63;	// Size of a module must be more then size of a variable

These restrictions are caused by this particular implementation. One of the most important our tasks was a maximum optimization. So we desided to use native arithmetic of C because a long arithmetic must work longer.
Now we understand that we need to break the restrictions. So we will implement a version of the program with a long arithmetic and without any restrictions.

There are two programs: "receiver" (Bob) and "transmitter" (Alisa). The connection between them occurs through the files in the folder "channel".

SET AN INPUT DATA
-----------------

	Open the file "defines.h";
	Set the input data according to recommendations set there;
	Save and close the file;

	We don't recommend to change any other files;

COMPILATION
-----------

	Use a command "make" or "mingw32-make.exe";
	If you can't do it then wtite the next two commands:

		gcc ./receiver/*.c ./receiver/*.h -o receiver -lm -O2
		gcc ./transmitter/*.c ./transmitter/*.h -o transmitter -O2

EXECUTION
---------

	Execute "receiver.exe" and "transmitter.exe";
	The first must be "reciever.exe" to avoid some errors of the connection between "receiver.exe" and "transmitter.exe";

	You can give one integer parameter that set amount of loop iterations before a pause (debug-mode only).

Copyright |copy| 2015, |LibreRussia (TM)| |---| все права защищены.

.. |copy| unicode:: 0xA9 .. знак копирайта
.. |LibreRussia (TM)| unicode:: LibreRussia U+2122 .. символ торговой марки
.. |---| unicode:: U+02014 .. длинное тире
.. |infinity| unicode:: U+221E