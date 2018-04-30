#include<stdio.h>

int main(int argc, char *argv[]) {

  FILE * file; // file pointer
  int sum, z1, z2, prod; // integers we need for calculation
  int csum = 0; // intermediate sum with initial value of 0

  // open file in readOnly mode
  if (argc > 1) {
    // if commandline argument is given
    // take second argument value as filename
    file = fopen(argv[1], "r");
  } else {
    // else default filename to "test.dat"
    file = fopen("test.dat", "r");
  }


  // return error if file couldn't be read
  if (file == NULL)
    return 1;

  // read the first line by specified pattern
  // from file pointer
  if (fscanf(file, "%d", &sum) == 1) {
    printf("Aimed sum is %d\n", sum);
  }

  // read all consecutive lines with pattern %d*%d
  while (fscanf(file, "%d*%d", &z1, &z2) == 2) {
    // scanf casts z1, z2 to intergers
    // thus we can multiply them without any
    // further type conversion
    prod = z1 * z2;
    printf("Intermediate Sum is %d + (%d * %d) = %d\n", csum, z1, z2, prod);
    // we add the product of z1 and z2 to our intermediate sum
    csum += prod;
  }

  // evaluation if the sum we aim for
  // and the sum we calculated are equal
  if (sum == csum) {
    printf("Calculated sum of %d is correct.\n", csum);
  } else {
    printf("Calculated sum of %d is NOT correct\n", csum);
  }

  // close file
  fclose(file);
  // we end the program "on good terms"
  return 0;
}
