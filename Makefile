all:
	gcc -o m2rtu m2rtu.c -lhammer `pkg-config --libs glib-2.0`
	gcc -o rtu2m rtu2m.c -lhammer `pkg-config --libs glib-2.0`
