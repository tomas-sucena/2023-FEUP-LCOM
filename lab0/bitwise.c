#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
	return binstr;
}

void print_usage(char *name) {
	printf("Usage: %s <action> <byte> <bit no>\n", name);
	printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
		"\t\t <byte> 8-bit integer in hexadecimal\n"
		"\t\t <bit no> of bit to operate on\n");
}

void print_number(uint8_t n, char* type){
    char binary[9]; // array for binary representation of n,
					//  remember that in C strings are terminated with a 0

    printf("%s\n", type);
    printf("Decimal: %d\n", n);
    printf("Binary: %s\n", byte2bin(n, binary));
}

uint8_t process(char action, uint8_t n, int bit){
    uint8_t mask = BIT(bit);

    switch (action){
        case 'h' : { // test for high
            n &= mask;

            if (n){
                printf("Bit %d of the input is high.\n", bit);
                break;
            }
                
            printf("Bit %d of the input is NOT high.\n", bit);
            break;            
        }
        case 'l' : { // test for low
            n &= mask;

            if (n){
                printf("Bit %d of the input is NOT low.\n", bit);
                break;
            }
                
            printf("Bit %d of the input is low.\n", bit);
            break;            
        }
        case 's' : { // set
            print_number(n, "BEFORE");
            printf("\n");

            n |= mask;
            print_number(n, "AFTER");

            break;
        }
        case 'r' : { // reset
            print_number(n, "BEFORE");
            printf("\n");

            mask = ~mask;
            n &= mask;
            print_number(n, "AFTER");

            break;
        }
        case 't' : { // toggle
            print_number(n, "BEFORE");
            printf("\n");

            n ^= mask;
            print_number(n, "AFTER");

            break;
        }
    }

    return n;
}

int main(int argc, char *argv[]){
    if (argc < 4){
        perror("Error! Invalid number of arguments (should be 3).");
        return EXIT_FAILURE;
    }

	char* actions = argv[1];	// action: 'h', 'l', 'r', 's', 't'
	unsigned long inp = atoi(argv[2]); // value to convert must be smaller than 256
	char* bits = argv[3]; // bit to operate on: must be between 0 an 7
    
    int numActions = 1;
    if (argc > 4)
        numActions = atoi(argv[4]);
        
	// Validate command line arguments
    if (inp > 256){
        perror("Error! The unsigned integer must be lower than 256.\n");
        return EXIT_FAILURE;
    }

    if (sizeof(actions) != sizeof(bits)){
        perror("Error! The number of actions must be equal to the number of bits.\n");
        return EXIT_FAILURE;
    }

    if (numActions < 0){
        perror("Error! The number of actions to execute must be positive.\n");
        return EXIT_FAILURE;
    }

	// Do what the user asked and print the result
    uint8_t n = inp;

    printf("\n");
    for (int i = 0; i < numActions;){
        char a = actions[i];
        int bit = (int) bits[i] - '0';

        if (a != 'h' && a != 'l' && a != 'r' && a != 's' && a != 't'){
            perror("Error! The action you specified is not valid.\n");
            return EXIT_FAILURE;
        }

        if (bit < 0 || bit > 7){
            perror("Error! The bit you want to access must be between 0 and 7.\n");
            return EXIT_FAILURE;
        }

        n = process(a, n, bit);

        if (++i < numActions)
            printf("\n---------------------------------------------------------\n");
        printf("\n");
    }

    return EXIT_SUCCESS;
}
