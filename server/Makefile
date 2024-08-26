define COMMENT
	Kullanım:
		Tüm dosyaları derlemek için: make all
		Temizlemek için: make clean
		Belirli bir dosyayı temizlemek için: make clean_dosya_adi
		Belirli bir dosyayı almak ve derlemek için: make get_dosya_adi
endef

# Değişkenler
CC = gcc
CFLAGS = -Wall -g -I/usr/include/openssl -Iinclude
LIBS = -ljson-c -lssl -lcrypto
SRC_DIR = src
OBJ_DIR = bin
EXEC_DIR = $(OBJ_DIR)/exec
LIB_DIR = lib
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c)) \
            $(patsubst $(LIB_DIR)/%.c,$(OBJ_DIR)/%.o,$(wildcard $(LIB_DIR)/*.c))
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

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
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
