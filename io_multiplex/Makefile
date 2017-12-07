#usage: ./xxx_demo /dev/tty /dev/input/mouse0 1

all : select_demo poll_demo

select_demo : select_demo.c
	gcc $^ -o $@ -g

poll_demo : poll_demo.c
	gcc $^ -o $@ -g

clean :
	rm select_demo poll_demo
