
LOCAL_SRC_FILES+= autofm.c


.PHONY : clean

#INCLUDES= I./

CPPFLAGS= -g -o0  $(INCLUDES)
#LDFLAGS= -shared -lstdc++ -lc


#OBJs=$(LOCAL_SRC_FILES:.cpp=.o)
OBJs+=$(LOCAL_SRC_FILES:.c=.o)


TARGET=autofm

all: $(TARGET)

clean:
	rm -f $(OBJs) $(TARGET)

$(TARGET) : $(OBJs)
	$(CC) $(CPPFLAGS) $(OBJs) -o $@ $(LDFLAGS)

