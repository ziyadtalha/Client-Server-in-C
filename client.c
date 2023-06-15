#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//gets integer (0 - 9 only) count that are in a string
int integerCount(char* data)
{
  int count = 0;
  int i = 0;

  while (data[i] != '\0')
  {
    if ((data[i] >= '0') && (data[i] <= '9'))
    {
      count++;
    }
    i++;
  }

  return count;
}

char* extractIntegers(char* data, int size, int* intCount)
{
  int count = integerCount(data);

  *intCount = count;

  char* integers = (char*) malloc((count + 1) * sizeof(char));

  integers[count] = '\0';

  int j = 0;
  for (int i = 0; i < size; i++)
  {
    if ((data[i] >= '0') && (data[i] <= '9'))
    {
      integers[j] = data[i];
      j++;
    }
  }

  return integers;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    printf("Pass file name as well!\n");
    return -1;
  }

  if (argc > 2)
  {
    printf("Too many arguments!\n");
    return -2;
  }

  char* filename = argv[1];
  int file_desc = open(filename, O_RDONLY);

  if (file_desc == -1)
  {
    printf("File could not be opened!\n");
    return -3;
  }

  //a large temporary array
  char* buffer = (char*) malloc(200 *sizeof(char));

  int size = read(file_desc, buffer, 200);

  close(file_desc);

  buffer[size] = '\0';

  int intCount = 0;

  char* integers = extractIntegers(buffer, size, &intCount);

  //free temporary array
  free(buffer);

  printf("Data read from file: \n");
  printf("%s\n", integers);

  //creating shared memory region
  int id = shmget((key_t)1222, 1024, 0777 | IPC_CREAT);

  if (id < 0)
  {
    printf("SHMGET failed!\n");
    return -4;
  }

  char* ptr = shmat(id, NULL, 0);

  if (ptr == NULL)
  {
    printf("SHMAT failed!\n");
    return -5;
  }

  //copying to memory

  strncpy(ptr, integers, intCount);

  sleep(5);

  free(integers);

  //unlinking shared memory

  int unlinkStatus = shmdt(ptr);

  if (unlinkStatus < 0)
  {
    printf("SHMDT failed!\n");
    return -6;
  }

  shmctl(1222, IPC_RMID, NULL);

  return 0;
}
