# Compiler
CC := cc

# Program
PROG := pc80s31

# Source files
SRC := pc80s31.c MGPIO.c
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)

# CFLAGS
CFLAGS := -I.
CFLAGS += `pkg-config --cflags libusb-1.0`
CFLAGS += -Wno-deprecated-declarations -Wunused-variable -O3 -march=native

# LDFLAGS
LDFLAGS := `pkg-config --libs libusb-1.0`
LDFLAGS += -lm -lpthread

# ================================
# Build rules
# ================================
all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJ) $(DEP) $(PROG)

# ================================
# Dependency generation
# ================================
-include $(DEP)

%.d: %.c
	$(info GEN $@)
	@$(CC) -MM $(CFLAGS) $< | \
	sed 's|\($*\)\.o[ :]*|\1.o $@ : |g' > $@
