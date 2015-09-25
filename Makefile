all : fireman
fireman : fireman.c
	gcc -o fireman fireman.c gpio.c test.c config.c inih/ini.c bash.c -lcurl -lexpat -lcrypto -lssl
clean :
	rm fireman
