output: findTopK_1.o findTopK_mqueue.o findTopK_thread.o
	gcc findTopK_1.o -o findTopK_1
	gcc findTopK_mqueue.o -o findTopK_mqueue -lrt
	gcc findTopK_1.o -o findTopK_thread

findTopK_1.o: findTopK_1.c
	gcc -c findTopK_1.c

findTopK_mqueue.o: findTopK_mqueue.c
	gcc -c findTopK_mqueue.c -lrt

findTopK_thread.o: findTopK_thread.c
	gcc -c findTopK_thread.c -pthread

clean:
	rm *.o