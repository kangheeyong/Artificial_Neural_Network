
FLAGS = -o
TARGET = test
OBJECTS = main.o my_interface.o my_data.o sub_main.o my_Self_Organizing_Map.o

$(TARGET) : $(OBJECTS)
				$(CXX) $(FLAGS) $(TARGET) $(OBJECTS)
clean :
				rm *.o $(TARGET)

new :
				$(MAKE) clean
				$(MAKE)
