# target
TARGET = trace

OBJ += TSamuraiTracer.o
OBJ += TSamuraiMagnetField.o

OBJ += trace.o
OBJ += traceUtil.o
OBJ += AddObjects.o

OBJ += TGeneralConfig.o
OBJ += TGeometryConfig.o
OBJ += TMagnetConfig.o

OBJ += TDetector.o
OBJ += THodoscope.o
OBJ += TDriftChamber.o

# depends
DEPDIR = .deps
DEPENDS = $(addprefix $(DEPDIR)/, $(notdir $(OBJ:.o=.d)))
# object
OBJDIR = .objects
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJ))

HDR = $(OBJ:.o=.h)

ROOTLIBS = `root-config --libs`
CXXFLAGS = -O2 -Wall -Wextra -fPIC `root-config --cflags`
LDFLAGS = $(ROOTLIBS) -lyaml-cpp

all: $(TARGET)
.PHONY: all clean

$(TARGET): $(OBJECTS)
	@echo `uname`
	$(CXX) $(LDFLAGS) -O2 -o $@ $^

-include $(DEPENDS)

$(DEPDIR)/%.d: %.cc
	@mkdir -p $(DEPDIR)
	g++ -M $(CXXFLAGS) $^ > $@
	@mv -f $@ $@.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $@.tmp > $@
	@sed -e 's/.*://' -e 's/\\$$//' < $@.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $@
	@rm -f $@.tmp

$(OBJDIR)/%.o: %.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f  $(DEPDIR)/*.d $(OBJDIR)/*.o $(TARGET)
	rmdir $(OBJDIR) $(DEPDIR)
