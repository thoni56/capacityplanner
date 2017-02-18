CFLAGS = -g

all: planner_tests

planner_tests: planner_tests.so
	cgreen-runner planner_tests.so

planner_tests.so: planner_tests.o planner.o
	$(CC) -shared -fPIC -o $@ $^ -lcgreen -lhpdf

%.o:%.c
	$(CC) -g -c -o $@ $^ -fPIC

clean:
	rm *.so *.o
