define COMMENT
	Kullanım:
		Tüm dosyaları derlemek için: make all
		Temizlemek için: make clean
		Belirli bir dosyayı temizlemek için: make clean_dosya_adi
		Belirli bir dosyayı almak ve derlemek için: make get_dosya_adi
endef

# Değişkenler
CC = gcc
CFLAGS = -Wall -g
LIBS = -ljson-c
SRC_DIR = src
LIBS_DIR = libs
OBJ_DIR = bin
EXEC_DIR = $(OBJ_DIR)/exec
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIBS_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(patsubst $(LIBS_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES)))
TARGET = $(EXEC_DIR)/main

# Hedefler
.PHONY: all clean

# Tüm dosyaları derle
all: $(TARGET)

# Ana hedef
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LIBS)

# .c dosyalarını .o dosyalarına derle
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) $(EXEC_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIBS_DIR)/%.c
	@mkdir -p $(OBJ_DIR) $(EXEC_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Temizleme işlemi
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Belirtilen dosyayı temizle
clean_%:
	rm -f $(OBJ_DIR)/$*.o

# Gelen Parametre dosyasını derle
compile_%: 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/$*.c -o $(OBJ_DIR)/$*.o

# Verilen .o dosyasını al ve derle
get_%:
	@mkdir -p $(EXEC_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/$*.c -o $(OBJ_DIR)/$*.o
	$(CC) $(OBJ_DIR)/$*.o -o $(TARGET) $(LIBS)
