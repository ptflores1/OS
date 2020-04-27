enum ProcessState
{
    RUNNING,
    READY,
    WAITING,
    FINISHED
};

typedef struct {
    int PID;
    char name[32];
    unsigned int priority;
    ProcessState state;
} Process;

Process *process_init(char name[32], int priority);

void process_destroy(Process *);