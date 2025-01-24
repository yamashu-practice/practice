draw_rect: draw_rect.c output.c
	mkdir -p build
	gcc -Wall -o build/exec draw_rect.c output.c

draw_circle: draw_circle.c output.c
	mkdir -p build
	gcc -Wall -o build/exec draw_circle.c output.c

add_circle: add_circle.c output.c input.c
	mkdir -p build
	gcc -Wall -o build/exec add_circle.c output.c input.c

run:
	build/exec

clean:
	rm -rf *~ build

.PHONY: clean

