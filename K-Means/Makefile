
FLAGS = -o
TARGET = test
OBJECTS = main.o my_data.o my_K_Means.o

$(TARGET) : $(OBJECTS)
				$(CXX) $(FLAGS) $(TARGET) $(OBJECTS)
clean :
				rm *.o $(TARGET)

new :
				$(MAKE) clean
				$(MAKE)
