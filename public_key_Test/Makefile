all:
	gcc ./receiver/*.c ./receiver/*.h -o Receiver -lm -std=c11 -lrt -Ofast -m64 -funroll-loops -flto -march=native -mfpmath=sse
	gcc ./transmitter/*.c ./transmitter/*.h -o Transmitter -std=c11 -lrt -Ofast -m64 -funroll-loops -flto -march=native -mfpmath=sse
