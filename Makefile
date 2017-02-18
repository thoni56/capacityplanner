CFLAGS = -g -MMD -fPIC

all: planner_tests

planner: planner.o pdf.o vector.o -lhpdf

planner_tests: planner_tests.so
	cgreen-runner planner_tests.so

planner_tests.so: planner_tests.o planner.o pdf.o vector.o
	$(CC) -shared -fPIC -o $@ $^ -lcgreen -lhpdf

clean:
	rm *.so *.o

-include *.d
