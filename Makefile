ad-hoc:receive_packet.o socket.o utils.o send_packet.o cache.o flood.o DSR.o
	gcc utils.o socket.o send_packet.o receive_packet.o cache.o flood.o DSR.o -o ad-hoc -lpthread

receive_packet.o:receive_packet.c receive_packet.h header.h cache.h flood.h utils.h DSR.h socket.h
	gcc -c receive_packet.c -o receive_packet.o -lpthread

DSR.o:DSR.c DSR.h cache.h utils.h header.h flood.h socket.h
	gcc -c DSR.c -o DSR.o -lpthread

socket.o:socket.c socket.h receive_packet.h flood.h cache.h header.h
	gcc -c socket.c -o socket.o -lpthread

utils.o:utils.h utils.c socket.h header.h
	gcc -c utils.c -o utils.o -lpthread

send_packet.o:send_packet.h send_packet.c utils.h header.h flood.h DSR.h
	gcc -c send_packet.c -o send_packet.o -lpthread

cache.o:cache.c cache.h header.h
	gcc -c cache.c -o cache.o -lpthread

flood.o:flood.c flood.h
	gcc -c flood.c -o flood.o -lpthread

.PHONY:clean
clean:
	rm -rf *.o ad-hoc
