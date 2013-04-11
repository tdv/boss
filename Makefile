INCLUDE = -Iboss/include

all: service_registry class_factory
	g++ -Wall -O3 -g0 -std=c++11 $(INCLUDE) *.cpp -o boss.test -lpthread -lrt -ldl

service_registry:
	g++ -Wall -O3 -g0 -std=c++11 -shared -fPIC $(INCLUDE) boss/src/service_registry/*.cpp -o libservice_registry.so -lpthread

class_factory:
	g++ -Wall -O3 -g0 -std=c++11 -shared -fPIC $(INCLUDE) boss/src/class_factory/*.cpp -o libclass_factory.so -lpthread -ldl
