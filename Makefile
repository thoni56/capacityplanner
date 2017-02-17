all: capacity_planner_tests


capacity_planner_tests: capacity_planner_tests.so
	cgreen-runner capacity_planner_tests.so

capacity_planner_tests.so: capacity_planner_tests.o planner.o
	$(CC) -shared -o$@ $^ -lcgreen -lhpdf
