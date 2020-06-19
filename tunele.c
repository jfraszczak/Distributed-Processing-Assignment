#include <mpi.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"


void *ActionThread(void* data) {

   while(1){

      //Losowanie kiedy proces chce wejsc do tunelu
      rest();

      reset_request_data();

      //Wysłanie żądania wejścia do tunelu
      request();

      //Czekamy aż będziemy mogli wejść do tunelu
      wait_to_join_queue();

      //Dołączamy do kolejki
      join_queue();

      sleep(3);

      //Czekamy aż kolejka sie zapelni po czym wchodzimy do tunelu
      wait_to_enter_tunnel();

      //Podróż tunelem
      travel();

      sleep(5);
      
   }

   pthread_exit(NULL);
}

void *ListeningThread(void* necessary_data) {
   MPI_Status status;
   struct message msg;

   while(1){
      MPI_Recv(&msg, sizeof(struct message), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      
      handle_message(msg, status);
      
   }

   pthread_exit(NULL);
}

int main (int argc, char **argv) {
   MPI_Init(&argc, &argv);

   int size, tid;
   MPI_Comm_rank(MPI_COMM_WORLD, &tid);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   mpi_process_data = malloc(sizeof(struct process_data));
   mpi_process_data ->tid = tid;
   mpi_process_data ->size = size;

   initialize();

   pthread_t thread_action, thread_listening;

   pthread_create(&thread_action, NULL, ActionThread, (void *) mpi_process_data);
   pthread_create(&thread_listening, NULL, ListeningThread, (void *) mpi_process_data);

   pthread_exit(NULL);
   MPI_Finalize();
}