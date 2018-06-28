#makefile

OBJECTS = main.o \
	  I2C_RW.o \
	  motorControl.o 
CFLAGS = -g
TARGET = main
CC = gcc
LIBS = -lwiringPi
LIB_DIRS = /home/pi/wiringPi/wiringPi


$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm *.o $(TARGET)
