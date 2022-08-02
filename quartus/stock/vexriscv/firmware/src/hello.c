#include <stdio.h>
#include <stdint.h>
// #include <string.h>
#include <newlib.h> // newlib is better for embedded than #include <stdlib.h>

int main() {
	char msg[] = "hello";

	printf("Hello World\n");
    
	while (1) {
		printf(msg);
	}
	return 0;
}