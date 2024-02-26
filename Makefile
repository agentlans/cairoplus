.PHONY: all clean

all: libcairoplus.so

libcairoplus.so:
	$(CC) -fPIC -c src/*.c -Iinclude/ `pkg-config --cflags --libs pangocairo poppler-glib Wand librsvg-2.0`
	$(CC) -shared *.o -o libcairoplus.so

clean:
	rm -rf *.o libcairoplus.so

