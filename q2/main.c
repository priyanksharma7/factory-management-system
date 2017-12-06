#include "definitions.h"
#include "main.h"
#include <omp.h>

sem_t sem_worker;
sem_t sem_space;

sem_t sem_skeleton;
sem_t sem_engine;
sem_t sem_chassis;
sem_t sem_body;

sem_t sem_window;
sem_t sem_tire;
sem_t sem_battery;
sem_t sem_car;

int num_cars;
int num_spaces;
int num_workers;

void *create_pack(void *args){
	int i;
	resource_pack *rpack = (resource_pack*) args;
	pthread_t workers[8];
	work_pack wpacks[8];
	for(i= 0; i < 8; i++) {
		wpacks[i].resource = rpack;
		wpacks[i].jid=i;
		wpacks[i].tid=i;
		//printf("-----Main: worker %d doing %d...\n", wpacks[i].tid, wpacks[i].jid);
		if (i==5)
		{
			wpacks[i].times=4;
		}
		else if (i==4)
		{
			wpacks[i].times=7;
		}
		else{
			wpacks[i].times=1;
		}

		int workersFlag=pthread_create(&workers[i],NULL,&work,(void *) &wpacks[i]);
		if (workersFlag)
		{
			printf("Workers Threads Not Created for Group Error\n"); exit(0);
		}


	}

	for(i=0;i<8;i++){
		pthread_join(workers[i],NULL);
	}
	pthread_exit(0);

}

int main(int argc, char** argv)
{

	  if (argc < 4) {
	  printf("Usage: %s <number of cars> <number of spaces> <number of workers>\n",
	  argv[0]);
	  return EXIT_SUCCESS;
	  }
	  num_cars     = atoi(argv[1]);
	  num_spaces   = atoi(argv[2]);
	  num_workers  = atoi(argv[3]);

	printf("Job defined, %d workers will build %d cars with %d storage spaces\n",
			num_workers, num_cars, num_spaces);

	resource_pack *rpack = (struct resource_pack*) malloc(sizeof(struct resource_pack));
	pthread_t *tid=(pthread_t*) malloc(sizeof(pthread_t)*num_cars);
	// put semaphores into resource_pack
	initResourcePack(rpack, num_spaces, num_workers);
	// Start working and time the whole process
	int i;
	double production_time = omp_get_wtime();

	for(i= 0;i < num_cars; i++){

		int workGroupFlag = pthread_create(&tid[i],NULL,&create_pack,(void *) rpack);
		if (workGroupFlag)
		{
			printf("Work Group Threads Not Created Error\n"); exit(0);
		}
	}

	int j;
	for(j=0;j < num_cars;j++){
		pthread_join(tid[j],NULL);//wait for work-groups to end work
	}

	production_time = omp_get_wtime() - production_time;
	reportResults(production_time);

	destroySem();
	free(rpack);
	return EXIT_SUCCESS;
}

void reportResults(double production_time) {
	int *sem_value = malloc(sizeof(int));
	printf("=====Final report=====\n");

	sem_getvalue(&sem_skeleton, sem_value);
	printf("Unused Skeleton: %d\n",   *sem_value);
	sem_getvalue(&sem_engine,   sem_value);
	printf("Unused Engine: %d\n",     *sem_value);
	sem_getvalue(&sem_chassis,  sem_value);
	printf("Unused Chassis: %d\n",    *sem_value);
	sem_getvalue(&sem_body,     sem_value);
	printf("Unused Body: %d\n",       *sem_value);
	sem_getvalue(&sem_window,   sem_value);
	printf("Unused Window: %d\n",     *sem_value);
	sem_getvalue(&sem_tire,     sem_value);
	printf("Unused Tire: %d\n",       *sem_value);
	sem_getvalue(&sem_battery,  sem_value);
	printf("Unused Battery: %d\n",    *sem_value);

	sem_getvalue(&sem_space, sem_value);
	if (*sem_value < num_spaces) {
		printf("There are waste car parts!\n");
	}
	sem_getvalue(&sem_car, sem_value);
	printf("Production of %d %s done, production time: %f sec, space usage: %d\n",
			*sem_value,
			*sem_value > 1 ? "cars" : "car",
			production_time, num_spaces);
	printf("==========\n");
	free(sem_value);
}

void initResourcePack(struct resource_pack *pack,
		int space_limit, int num_workers) {
	initSem();
	pack->space_limit  = space_limit;
	pack->num_workers  = num_workers;
	pack->sem_space    = &sem_space   ;
	pack->sem_worker   = &sem_worker  ;

	pack->sem_skeleton = &sem_skeleton;
	pack->sem_engine   = &sem_engine  ;
	pack->sem_chassis  = &sem_chassis ;
	pack->sem_body     = &sem_body    ;

	pack->sem_window   = &sem_window  ;
	pack->sem_tire     = &sem_tire    ;
	pack->sem_battery  = &sem_battery ;
	pack->sem_car      = &sem_car     ;
}

int destroySem(){
#if DEBUG
	printf("Destroying semaphores...\n");
#endif
	sem_destroy(&sem_worker);
	sem_destroy(&sem_space);

	sem_destroy(&sem_skeleton);
	sem_destroy(&sem_engine);
	sem_destroy(&sem_chassis);
	sem_destroy(&sem_body);

	sem_destroy(&sem_window);
	sem_destroy(&sem_tire);
	sem_destroy(&sem_battery);
	sem_destroy(&sem_car);
#if DEBUG
	printf("Semaphores destroyed\n");
#endif
	return 0;
}

int initSem(){
#if DEBUG
	printf("Initiating semaphores...\n");
#endif
	sem_init(&sem_worker,   0, num_workers);
	sem_init(&sem_space,    0, num_spaces);

	sem_init(&sem_skeleton, 0, 0);
	sem_init(&sem_engine,   0, 0);
	sem_init(&sem_chassis,  0, 0);
	sem_init(&sem_body,     0, 0);

	sem_init(&sem_window,   0, 0);
	sem_init(&sem_tire,     0, 0);
	sem_init(&sem_battery,  0, 0);
	sem_init(&sem_car,      0, 0);
#if DEBUG
	printf("Init semaphores done!\n");
#endif
	return 0;
}
