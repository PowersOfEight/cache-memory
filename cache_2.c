#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


# define SPINNER_DELAY 10000000L
# define COLLECTION_DELAY 100000L
# define COLLECTION_N_ITER 50000000L

# define DEFAULT_FILE_NAME "data/cache-data.csv"

# define BILLION 1000000000L
# define MAX_BUFFER = 32 * 1024 * 1024
int iters = 0;
volatile int spin = 1;
volatile char dummy_char = (char)0;
void* spinner() {
    const char spinner_chars[] = { '|', '/', '-', '\\' };
    const int num_chars = 4;
    int ndx = 0;
    int delay_count = 0;

    while (spin == 1) {
        if (++delay_count >= SPINNER_DELAY) {
            delay_count = dummy_char;
            printf("\r%c", spinner_chars[ndx]);
            fflush(stdout);
            ndx = (ndx + 1) % num_chars;
            delay_count = 0;
        }

    }
    return NULL;
    printf("\r\n");
}



// Every data point has some value associated with it
// Buffer Size, Stride (how far into the buffer we're accessing), time_diff
// Lets make a VERY big file and read these in as structs
void collect_data(size_t buffer_size, size_t stride, FILE* out) {
    char* buffer = malloc(buffer_size);

    /**
     * May fail to allocate due to th volume of memory we're
     * allocating here.
     */
    if (!buffer) {
        perror("Memory Allocation Failed");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;
    size_t iterations = buffer_size / stride;// Total iterations
    /**
     * The volatile keyword allows us to avoid compiler optimizations
     * for this value.
     */
     // volatile char dummy_char;

    for (size_t i = 0; i < iterations; i++) {
        buffer[i * stride] = (char)i;
	iters++;
    }

    for (size_t i = 0; i < iterations; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        dummy_char = buffer[i * stride];
        clock_gettime(CLOCK_MONOTONIC, &end);
        unsigned long diff = (end.tv_sec - start.tv_sec) * BILLION + (end.tv_nsec - start.tv_nsec);
        fprintf(out, "%lu,%lu,%lu\r\n", buffer_size, stride, diff);

    }

    free(buffer);
}



int main(int argc, char** argv) {
   
    const char* filename;
    pthread_t spinner_thread;
    FILE* outfile;
    const size_t stride = 64;
    size_t max_buffer = 32 * 1024;// * 1024; // 2^25  --> 2^15

    if (argc < 1) {
        filename = DEFAULT_FILE_NAME;
    }

    outfile = fopen(filename, "w");

    if (!outfile) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }


    for (size_t buffer_size = 1024; buffer_size <= max_buffer; buffer_size *= 2) {
        for (size_t stride_size = 1; stride_size <= stride; stride_size *= 2) {
            spin = 1;
            if (pthread_create(&spinner_thread, NULL, spinner, NULL) != 0) {
                perror("Thread creation failure!");
                return EXIT_FAILURE;
            }
            collect_data(buffer_size, stride, outfile);
            spin = 0;
            if (pthread_join(spinner_thread, NULL) != 0) {
                perror("Failed to join spinner thread");
                return EXIT_FAILURE;
            }
        }
    }

    fclose(outfile);
    printf("Run complete\n");
    printf("reading data\n");
    printf("iterations: %d \n", iters);
    outfile = fopen(filename, "r"); //reads the output data
    int data[iters]; //contains data
    int cnt = 0;
    char chr;
    int prev = 0;
    int twocomma = 0;
    int gogogo = 0;

    char clap[2]; 
    while((chr=fgetc(outfile))!= -1){
//	printf("%c\n", chr);
	if((gogogo == 1) && (chr != '\n') ){
		clap[0]  = chr; 
		clap[1] = '\0';
		prev = (prev*10) + atoi(clap) ;
	     //  printf("%d\n", prev);	
	}
	
	if(chr == ','){
		twocomma++;
	}

	if(twocomma == 2){
	//	text[cnt] = chr;
//		printf("%c\n", chr);
		gogogo=1;
//		cnt++;
	}

	if(chr == '\n'){
		twocomma = 0;
		gogogo = 0;
		prev = prev/10;
		printf("%d\n", prev);
		data[cnt] = prev;
		prev = 0;
		cnt++;
	}

    }

int firstAcc = data[0]; //first instance of data for main mem acc
// int firstCache = data[1]; //first instance of data from cache
cnt = 0;
int cnt2 = 0;
int mainmem[iters]; //array created to store all instances of mm access
int cachemem[iters]; //array created for store all instances of cache access
for(int i = 0 ; i < iters; i++){
	if ((data[i] >= 0) && (data[i] <= (firstAcc - (firstAcc * .15)))){
		cachemem[cnt] = data[i];
		cnt++;	
		
			}
	else if(data[i] > (firstAcc - (firstAcc * .15))){
		mainmem[cnt2] = data[i];
		cnt2++;
	}	


}

        for(int i = 0; i < cnt; i++){
                int min = cachemem[i];
                int dex = i;
                for(int j = i+1; j < cnt; j++){
                        if(cachemem[j] < min){
                                min = cachemem[j];
                                dex = j;
                        }
                }
                int temp = cachemem[i];
                cachemem[i] = min;
                cachemem[dex] = temp;
        }

        for(int i = 0; i < cnt2; i++){
                int min = mainmem[i];
                int dex = i;
                for(int j = i+1; j < cnt2; j++){
                        if(mainmem[j] < min){
                                min = mainmem[j];
                                dex = j;
                        }
                }
                int temp = mainmem[i];
                mainmem[i] = min;
                mainmem[dex] = temp;
        
	}

if(cnt % 2 == 0){
	int cacheval = (cachemem[cnt/2] + cachemem[(cnt/2)-1]) / 2;
	printf("cache mem: %d ns\n", cacheval);
}
else{
	printf("cache mem: %d ns\n", cachemem[cnt/2]);

}

if(cnt2 % 2 == 0){
        int mainval = (mainmem[cnt2/2] + mainmem[(cnt2/2)-1]) / 2;
        printf("main mem: %d ns\n", mainval);
}
else{
        printf("main mem: %d ns\n", mainmem[cnt2/2]);

}

	


cnt = 0;

//while(cachemem[cnt] != '\0'){
//	printf("count: %d, %d\n", cnt, cachemem[cnt]);
//	cnt++;
//}

//cnt = 0;

//while(mainmem[cnt] != '\0'){
//        printf("count: %d, %d\n", cnt, mainmem[cnt]);
//        cnt++;
//}


 


    return 0;
}
