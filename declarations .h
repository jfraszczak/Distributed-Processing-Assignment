#ifndef _declarations
#define _declarations

#define ON_EARTH 1
#define IN_ANOTHER_DIMENSION 2
#define IN_TUNNEL 3
#define IN_QUEUE 4
#define WAIT_FOR_QUEUE 5

#define REQ_TO_ANOTHER_DIMENSION 1
#define REQ_TO_EARTH 2
#define ACK_ABSOLUTE 3
#define ACK_EXCLUSION 4
#define ACK_CONDITIONAL 5
#define ACK_TO_QUEUE 6
#define RELEASE_QUEUE 7
#define RELEASE_TUNNEL 8
#define JOIN_QUEUE 9
#define ENTER_TUNNEL 10
#define LEAVE_TUNNEL 11

pthread_mutex_t lock_lamport;
pthread_mutex_t lock_leaders_of_queues;
pthread_mutex_t lock_requests;
pthread_mutex_t lock_requests_to_queue;
pthread_mutex_t lock_tunnels_ack;
pthread_mutex_t lock_ack_from_processes;
pthread_mutex_t lock_num_of_processes_on_our_site;
pthread_mutex_t lock_queues_ack;
pthread_mutex_t lock_processes_in_queue;
pthread_mutex_t lock_num_of_processes_in_queue;
pthread_mutex_t lock_state;
pthread_mutex_t lock_tunnels_conditional_ack;
pthread_mutex_t lock_location;

struct message {
   int type;
   int lamport_clock;
   int tunnel_id;
   int is_leader;
   int state;
   int location;
};

struct process_data {
   int tid;
   int size;
};

struct process_data *mpi_process_data;

static int rich_group_capacity = 3;
static int num_of_tunnels = 2;
static int tunnel_capacity = 4;

static int lamport_clock = 0;
static int request_clock = -1;
static int state = ON_EARTH;
static int location = ON_EARTH;
static int is_leader = 0;
static int in_tunnel_id = -1;
static int num_of_processes_in_queue = 0;
static int num_of_processes_on_our_site = 0;

int *tunnels_ack;
int *tunnels_conditional_ack;
int *queues_ack;
int *requests;
int *ack_from_processes;
int *processes_in_queue;
int *leaders_of_queues;
int *requests_to_queue;


#endif