#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

struct mq_attr attributes = {
    .mq_msgsize = 15,
    .mq_maxmsg = 10,
    .mq_curmsgs = 0,
    .mq_flags = 0
};

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

int** getNumbersFromFile(char* filename) {
    int fd = open(filename, O_RDONLY);
    int** return_val = (int**)malloc(sizeof(int*) * 2);
    static int inputs[INT_MAX];
    int input_index = 0, tmp_num_index = 0;
    char tmp_c, tmp_num[11];

    while(read(fd, &tmp_c, 1) != 0) {
        if(tmp_c == ' ') {
            tmp_num[tmp_num_index] = '\0';
            tmp_num_index = 0;
            sscanf(tmp_num, "%d", &inputs[input_index]);
            input_index++;
        }

        else {
            tmp_num[tmp_num_index] = tmp_c;
            tmp_num_index++;
        }
    }

    close(fd);
    return_val[0] = &input_index;
    return_val[1] = inputs;
    return return_val;
}

void addToQueue(int topK) {
    mqd_t mq = mq_open("/testqueue", O_WRONLY);
    
    char text[20];
    sprintf(text, "%d", topK);

    if(fork() == 0) {
        if(mq == -1) {
            perror("Failed to open queue");
            exit(1);
        }

        if(mq_send(mq, text, strlen(text) + 1, 1) == -1) {
            perror("Failed to add to queue");
            exit(1);
        }

        mq_close(mq);
    }
}

void findTopKMQueue(int k, int N, char filenames[][20]) {
    int* inputs;
    int topK;
    int** output;
    
    for(int i = 0; i < N; i++) {
        if(fork() == 0) {
            output = getNumbersFromFile(filenames[i]);
            topK = findTopK(output[1], k, *output[0]);
            addToQueue(topK);
            exit(0);
        }
    }

    for(int i = 0; i < N; i++) {
        wait(NULL);
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

void main(int argc, char *argv[]) {

    int k, N;

    sscanf(argv[1], "%d", &k);
    sscanf(argv[2], "%d", &N);

    int outputs[N];

    char filenames[N][20], outfile[20];

    for (int i = 3; i < argc - 1; i++)
    {
        strcpy(filenames[i - 3], argv[i]);
    }

    strcpy(outfile, argv[argc - 1]);

    mq_unlink("/testqueue");
    mqd_t mq = mq_open("/testqueue", O_CREAT | O_RDWR, 0644, &attributes);
    
    if(mq == -1) {
        perror("Failed to open queue");
        exit(1);
    }
    
    findTopKMQueue(k, N, filenames);
    
    mq = mq_open("/testqueue", O_RDONLY);
    char recvd_text[30];

    if(mq == -1) {
        perror("Failed to open queue");
        exit(1);
    }
    
    for(int i = 0; i < N; i++) {
        if(mq_receive(mq, recvd_text, 20, NULL) == -1) {
            perror("Failed to receive from queue");
            exit(1);
        }

        sscanf(recvd_text, "%d", &outputs[i]);
    }

    

    sort(outputs, N);
    printArray(outputs, N);

    int out_fd = open(outfile, O_CREAT | O_WRONLY, S_IRUSR);
    char result[11];

    for (int k = 0; k < N; k++)
    {
        sprintf(result, "%d", outputs[k]);
        strcat(result, "\n");
        write(out_fd, result, strlen(result));
    }

    close(out_fd);

    mq_close(mq);
    mq_unlink("/testqueue");
}