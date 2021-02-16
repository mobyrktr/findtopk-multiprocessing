#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

int findTopK(int *arr, int k, int n)
{
    int tmp = 0;

    for (int i = 0; i < n - k; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }

    return arr[n - k - 1];
}

void sort(int *arr, int n)
{
    int tmp;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] < arr[j + 1])
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void printArray(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        printf("%d, ", arr[i]);
    }

    printf("%d\n", arr[n - 1]);
}

void generateIntFiles(int k, int N, char filenames[][20])
{
    int fd, tmp_num_index = 0, input_index = 0, f_int;
    char tmp_num[11], tmp[2], intermediate[2], result[12];
    static int inputs[INT_MAX];

    for (int i = 0; i < N; i++)
    {
        pid_t c = fork();

        if (c == 0)
        {
            fd = open(filenames[i], O_RDONLY);

            while(read(fd, tmp, 1) != 0)
            {
                if (tmp[0] == ' ')
                {
                    tmp_num[tmp_num_index] = '\0';
                    tmp_num_index = 0;
                    sscanf(tmp_num, "%d", &inputs[input_index]);
                    input_index++;
                }

                else
                {
                    tmp_num[tmp_num_index] = tmp[0];
                    tmp_num_index++;
                }
            }

            close(fd);

            sprintf(intermediate, "%d", i);
            f_int = open(intermediate, O_CREAT | O_WRONLY, S_IRUSR);
            sprintf(result, "%d", findTopK(inputs, k, input_index));
            write(f_int, result, strlen(result));
            close(f_int);
            exit(0);
        }
    }
    for(int i = 0; i < N; i++)
    wait(NULL);
}

void main(int argc, char *argv[])
{
    int k, N, f_int, numbers[N], number, tmp_num_index = 0;

    sscanf(argv[1], "%d", &k);
    sscanf(argv[2], "%d", &N);

    char filenames[N][20], tmp[2], outfile[20], tmp_num[12], result[12], tmp_filename[2];

    for (int i = 3; i < argc - 1; i++)
    {
        strcpy(filenames[i - 3], argv[i]);
    }

    strcpy(outfile, argv[argc - 1]);

    generateIntFiles(k, N, filenames);
    // wait(NULL);

    for (int j = 0; j < N; j++)
    {
        sprintf(tmp_filename, "%d", j); // ara dosyalar açıldı
        f_int = open(tmp_filename, O_RDONLY);
        tmp_num_index = 0;

        while (read(f_int, tmp, 1) != 0)
        {
            tmp_num[tmp_num_index] = tmp[0]; //ara dosyaların içindeki sayıları bir diziye atamak için
            tmp_num_index++;
        }

        tmp_num[tmp_num_index] = '\0'; // 123445\0
        sscanf(tmp_num, "%d", &number); // dosyadan alınan str --> int
        numbers[j] = number;
        remove(tmp_filename);
    }
    
    sort(numbers, N);
    printArray(numbers, N);
    int out_fd = open(outfile, O_CREAT | O_WRONLY, S_IRUSR);

    for (int k = 0; k < N; k++)
    {
        sprintf(result, "%d", numbers[k]);
        strcat(result, "\n");
        write(out_fd, result, strlen(result));
    }

    close(out_fd);
}