all: pcm2wav

CC := gcc

# if the compile machine is 64bit, should use `-m32`.
CFLAG := -m32

OUTPUT := pcm2wav


pcm2wav : main.c wav_format.h
	$(CC) $^ $(CFLAG) -o $@

clean : 
	rm -rf $(OUTPUT)
.PHONY := clean

