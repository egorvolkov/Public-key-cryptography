all: main.o coding.o createMessage.o getPublicKey.o returnEncodedMessage.o utilities.o longArithmetic.o myTime.o
	gcc -o transmitter main.o coding.o createMessage.o getPublicKey.o returnEncodedMessage.o utilities.o longArithmetic.o myTime.o
main.o: main.c
	gcc -c -o main.o main.c
coding.o: coding.c
	gcc -c -o coding.o coding.c
createMessage.o: createMessage.c
	gcc -c -o createMessage.o createMessage.c
getPublicKey.o: getPublicKey.c
	gcc -c -o getPublicKey.o getPublicKey.c
returnEncodedMessage.o: returnEncodedMessage.c
	gcc -c -o returnEncodedMessage.o returnEncodedMessage.c
utilities.o: utilities.c
	gcc -c -o utilities.o utilities.c
longArithmetic.o: longArithmetic.c
	gcc -c -o longArithmetic.o longArithmetic.c
myTime.o: myTime.c
	gcc -c -o myTime.o myTime.c
clean:
	rm -rf *.o