compile:
	gcc *.c lib/*.c -o main

run: compile
	clear && ./main && rm main
