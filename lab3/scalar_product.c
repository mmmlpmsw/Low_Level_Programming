#include <stdio.h>

const int first_vector[] = {1, 2, 3, 4, 5};
const int second_vector[] = {1, 1, 1, 1, 1};

int scalar_product(const int* first_vector, const int* second_vector, int length) {
    int result = 0;
    for (size_t i = 0; i < length; i ++) {
        result += first_vector[i] * second_vector[i];
    }
    return result;
}

int main() {
    size_t first_vector_size = sizeof(first_vector)/sizeof(first_vector[0]);
    size_t second_vector_size = sizeof(second_vector)/sizeof(second_vector[0]);
    if (first_vector_size != second_vector_size) {
        printf ("Cannot calculate the scalar product of vectors of different dimensions.");
    } else {
        print_array(first_vector, first_vector_size);
        print_array(second_vector, second_vector_size);
        printf( "The product is: %d \n", scalar_product(first_vector, second_vector, first_vector_size));
    }
    return 0;
}

void print_array(const int* array, size_t array_size) {
    	for (size_t i = 0; i < array_size; i++)
    		printf("%d ", array[i]);
    	printf("\n");
}