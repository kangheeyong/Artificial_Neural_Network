
FLAGS = -o
TARGET = test
OBJECTS = main.o my_interface.o sub_main.o

$(TARGET) : $(OBJECTS)
				$(CXX) $(FLAGS) $(TARGET) $(OBJECTS)
clean :
				rm *.o $(TARGET)

new :
				$(MAKE) clean
				$(MAKE)
