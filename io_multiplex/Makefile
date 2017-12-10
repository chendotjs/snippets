#usage: ./xxx_demo /dev/tty /dev/input/mouse0 1

all : select_demo poll_demo epoll_demo_lt

select_demo : select_demo.c
	gcc $^ -o $@ -g

poll_demo : poll_demo.c
	gcc $^ -o $@ -g

epoll_demo_lt :	epoll_demo_lt.c
	gcc $^ -o $@ -g

clean :
	rm select_demo poll_demo epoll_demo_lt
