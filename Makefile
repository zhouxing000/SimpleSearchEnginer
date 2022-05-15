INC_DIR:= include/ 
SRC_DIR:= src/
SRCS:=$(wildcard src/*.cc) $(wildcard src/cache/*.cc) $(wildcard src/reactorpool/*.cc) $(wildcard src/recommander/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -llog4cpp -lpthread -lhiredis

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I , $(INC_DIR)) $(LIBS) 

EXE:=bin/SearchEngine.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)