CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -DDEBUG 

LIBS =  -lpthread -ldl -lmpg123 -lAudioMT6572 -L../

INCLUDES = -I include

TARGET = testAudio


all:    $(TARGET)

.cpp:
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ $< $(LIBS)


clean:
	rm -f $(TARGET)
