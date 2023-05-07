#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

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
    return -1;
  }

  char* filename = argv[1];
  int file_desc = open(filename, O_RDONLY);

  if (file_desc == -1)
  {
    printf("File could not be opened!\n");
  }

  char buffer[200];

  int size = read(file_desc, buffer, 200);

  close(file_desc);

  buffer[size] = '\0';

  printf(buffer);

  int intCount = integerCount(buffer);

  char integers[intCount];
  integers[intCount] = '\0';

  int j = 0;
  for (int i = 0; i < size; i++)
  {
    if ((buffer[i] >= '0') && (buffer[i] <= '9'))
    {
      integers[j] = buffer[i];
      j++;
    }
  }

  printf(integers);
  printf("\n");

  //creating shared memory region

  int id = shmget((key_t)1222, 1024, 0777 | IPC_CREAT);

  if (id < 0)
  {
    printf("SHMGET failed!\n");
    return -1;
  }

  char* ptr = shmat(id, NULL, 0);

  if (ptr == NULL)
  {
    printf("SHMAT failed!\n");
    return -1;
  }

  //copying to memory

  strncpy(ptr, integers, 5);

  sleep(10);

  //unlining shared memory

  int unlinkStatus = shmdt(ptr);

  if (unlinkStatus < 0)
  {
    printf("SHMDT failed!\n");
    return -1;
  }

  shmctl(1222, IPC_RMID, NULL);

  return 0;
}
