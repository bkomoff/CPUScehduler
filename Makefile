scheduler: main.cpp ProcessQueue.cpp FCFSScheduler.cpp RoundRobinScheduler.cpp
	g++ -g -o main main.cpp ProcessQueue.cpp FCFSScheduler.cpp RoundRobinScheduler.cpp -I.