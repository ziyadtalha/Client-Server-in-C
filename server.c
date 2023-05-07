#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  char data[1024];

  int id = shmget((key_t)1222, 1024, 0);

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

  //getting data from shared memory
  strncpy(data, ptr, 1024);

  printf(data);
  printf("\n");

  int intCount = 0;
  while (data[intCount] != '\0')
  {
    intCount++;
  }

  int integers[intCount];

  int sum = 0;
  for (int i = 0; i < intCount; i++)
  {
    integers[i] = data[i] - '0';
    sum =  sum + integers[i];
  }

  int avg = sum / intCount;

  printf("Sum: %d\n", sum);

  printf("Average: %d\n", avg);

  int unlink_status = shmdt(ptr);

  if (unlink_status < 0)
  {
    printf("SHMDT failed!\n");
    return -1;
  }

  return 0;
}
