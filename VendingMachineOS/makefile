VendingMachine: main.o register.o dispenser.o
	g++ main.o register.o dispenser.o -o VendingMachine

main.o: main.cpp register.h dispenser.h
	g++ main.cpp -c

regsiter.o: register.cpp register.h
	g++ register.cpp -c

dispenser.o: dispenser.cpp dispenser.h
	g++ dispenser.cpp -c