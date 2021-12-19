#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

char* strremove(char* str, const char* sub) {
	char* p, * q, * r;
	if (*sub && (q = r = strstr(str, sub)) != NULL) {
		size_t len = strlen(sub);
		while ((r = strstr(p = r + len, sub)) != NULL) {
			while (p < r)
				*q++ = *p++;
		}
		while ((*q++ = *p++) != '\0')
			continue;
	}
	return str;
}

static char* readFile(const char* path) {
	strremove(path, "\n");
	FILE* file;
	errno_t err = fopen_s(&file, path, "rb");
	if (err != 0) {
		fprintf(stderr, "Could not open file \"%s\".\n", path);
		return NULL;
	}
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char* buffer = (char*)malloc(fileSize + 1);
	if (buffer == NULL) {
		fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
		exit(74);
	}
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if (bytesRead < fileSize) {
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		return NULL;
	}
	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}

static void runFile(const char* path) {
	char* source = readFile(path);
	if (source == NULL) return;
	InterpretResult result = interpret(source);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR) exit(65);
	if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}


int main() {
	initVM();
	char line[1024];
	while(fgets(line, sizeof(line), stdin) != NULL){
		runFile(line);
	}
	exit(64);

	freeVM();
	return 0;
}