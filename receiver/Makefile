all: main.o utilities.o generateSecretKey.o computePublicKey.o modularArithmetic.o returnPublicKey.o getEncodedMessage.o decoding.o transmitterConnection.o myTime.o
	gcc -o receiver main.o utilities.o generateSecretKey.o computePublicKey.o modularArithmetic.o returnPublicKey.o getEncodedMessage.o decoding.o transmitterConnection.o myTime.o -lm
main.o: main.c
	gcc -c -o main.o main.c
utilities.o: utilities.c
	gcc -c -o utilities.o utilities.c
generateSecretKey.o: generateSecretKey.c
	gcc -c -o generateSecretKey.o generateSecretKey.c
computePublicKey.o: computePublicKey.c
	gcc -c -o computePublicKey.o computePublicKey.c
modularArithmetic.o: modularArithmetic.c
	gcc -c -o modularArithmetic.o modularArithmetic.c
returnPublicKey.o: returnPublicKey.c
	gcc -c -o returnPublicKey.o returnPublicKey.c
getEncodedMessage.o: getEncodedMessage.c
	gcc -c -o getEncodedMessage.o getEncodedMessage.c
decoding.o: decoding.c
	gcc -c -o decoding.o decoding.c
transmitterConnection.o: transmitterConnection.c
	gcc -c -o transmitterConnection.o transmitterConnection.c
myTime.o: myTime.c
	gcc -c -o myTime.o myTime.c
clean:
	rm -rf *.o
