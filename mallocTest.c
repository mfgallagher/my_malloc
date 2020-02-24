// test file for my-malloc.c

//#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    void *buffer = malloc(25);
    char output[25] = "thesearecharacters\n";

    strcpy(buffer, output);
    write(1, buffer, 25);


    free(buffer);
    free(buffer);

    void *buffer2 = malloc(10);
    char output2[10] = "Hello\n";

    strcpy(buffer2, output2);
    write(1, buffer2, 6);


    void *buffer8 = realloc(buffer2, 200);

    void*buffer11;
    void*buffer10 = realloc(buffer11,50);


    void *buffer3 = calloc(200, 5);
    char output3[24] = "testtesttest\n";

    strcpy(buffer3, output3);
    write(1, buffer3, 24);

    free(buffer2);
//    free(buffer3);
    free(buffer8);


    void *buffer4 = malloc(35);
    char output4[35] = "Works\n";

    strcpy(buffer4, output4);
    write(1, buffer4, 6);


    void *buffer5 = malloc(25);
    char output5[25] = "Lets check if this works again!\n";

    strcpy(buffer5, output5);
    write(1, buffer5, 25);



    void *buffer6 = malloc(20);
    char output6[3000] = "check\n";

    strcpy(buffer6, output6);
    write(1, buffer6, 6);


    void *buffer7 = malloc(10);
    char output7[10] = "Its here\n";

    strcpy(buffer7, output7);
    write(1, buffer7, 10);



    return 0;
}
