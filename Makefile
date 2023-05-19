PROJECT = djvufix
CXX = g++
CXXFLAGS = -Wall
LDFLAGS = -s
RM = rm -f

PRDPI = $(PROJECT)dpi
PRROT = $(PROJECT)rotate
PROGS = $(PRDPI) $(PRROT)

all: $(PROGS)

$(PRDPI): src/$(PRDPI).cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(PRROT): src/$(PRROT).cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

clean:
	$(RM) $(PROGS)
