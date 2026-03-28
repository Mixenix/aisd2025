#include "tree.h"
#include <assert.h>
#include <time.h>

void rand_str(char *dest, int length) {
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (length <= 0) return;
    
    for (int i = 0; i < length; i++) {
        int index = rand() % (sizeof(charset) - 1);
        dest[i] = charset[index];
    }
    dest[length] = '\0';
}

void cleanup_keys_and_infos(char** keys, unsigned int* infos, int count) {
    if (!keys || !infos) return;
    for (int i = 0; i < count; i++) {
        free(keys[i]);
    }
    free(keys);
    free(infos);
}


int main(){
    Node* root = NULL;
    ERROR status;
    // int action[] = {1000, 1000, 1000, 1000, 1000,
    int acti[] = 
                   // 10000, 10000, 10000, 10000, 10000,
                   // 50000, 50000, 50000, 50000, 50000,
                   // 100000, 100000, 100000, 100000, 100000,
                   // 200000, 200000, 200000, 200000, 200000,
                   // 500000, 500000, 500000, 500000, 500000,
                   // 1000000, 1000000, 1000000, 1000000, 1000000,
                   // 10000000, 10000000, 10000000, 10000000, 10000000,
                   // 100000000, 100000000, 100000000, 100000000, 100000000};
                   {25000,  25000, 25000, 25000, 25000};
    int action[40*5] = {25000, 25000, 25000, 25000, 25000};
    int init = 25000;
    for (int i=0; i<40; i++){
    	action[0+5*i] = action[0+i*5]+init*i;
    	action[1+5*i] = action[1+i*5]+init*i;
    	action[2+5*i] = action[2+i*5]+init*i;
    	action[3+5*i] = action[3+i*5]+init*i;
    	action[4+5*i] = action[4+i*5]+init*i;
    }
    int num_actions = sizeof(action)/sizeof(int);
	long double sumIns = 0;
	long double sumDel = 0;
	long double sumSrch = 0;
	long double sumSrchMax = 0;
    for (int act = 0; act < num_actions; act++) {
    	// printf("\n%d elements", action[act]);
    	// if (act == 0 || action[act - 1] != action[act]){
    	// 	printf("\n\n\n%d elements\n", action[act]);
    	// }
        char** keys = malloc(sizeof(char*) * action[act]);
        unsigned int* infos = malloc(sizeof(unsigned int) * action[act]);
        if (!keys || !infos) {
            cleanup_keys_and_infos(keys, infos, action[act]);
            free_tree(root);
            return BAD_ALLOC;
        }

        for (int i = 0; i < action[act]; i++) {
            keys[i] = malloc(11 * sizeof(char));
            if (!keys[i]) {
                cleanup_keys_and_infos(keys, infos, action[act]);
                free_tree(root);
                return BAD_ALLOC;
            }
            rand_str(keys[i], 10);
            infos[i] = rand();
        }

        struct timespec insS, insF, srchS, srchF, delS, delF, maxsrchS, maxsrchF;

        // Insertion

        for (int i = 0; i < action[act]; i++) {
            status = insert_elem(&root, keys[i], infos[i]);
            if (status != GOOD){
            	printf("\nISSSUEUSE");
            	break;
            }
        }
        int randIndex = rand() % action[act];
        clock_gettime(CLOCK_MONOTONIC, &insS);
        status = insert_elem(&root, keys[randIndex], infos[randIndex]);
        clock_gettime(CLOCK_MONOTONIC, &insF);
       	if (status != GOOD){
       		printf("\nISSSUEUSE");
       		break;
       	}

        if (status != GOOD) {
            cleanup_keys_and_infos(keys, infos, action[act]);
            free_tree(root);
            return status;
        }

        // Search
        int index = rand() % action[act];
        clock_gettime(CLOCK_MONOTONIC, &srchS);
        SearchResults results = search_by_key(root, keys[index], &status);
        clock_gettime(CLOCK_MONOTONIC, &srchF);
        free_search_results(&results);

        if (status != GOOD) {
            cleanup_keys_and_infos(keys, infos, action[act]);
            free_tree(root);
            return status;
        }

        // Max search
        clock_gettime(CLOCK_MONOTONIC, &maxsrchS);
        results = find_max_keys(root, &status);
        clock_gettime(CLOCK_MONOTONIC, &maxsrchF);
        free_search_results(&results);

        if (status != GOOD) {
            cleanup_keys_and_infos(keys, infos, action[act]);
            free_tree(root);
            return status;
        }

        // Deletion
        clock_gettime(CLOCK_MONOTONIC, &delS);
        // for (int i = 0; i < action[act]; i++) {
        //     if (root) {  // Check if tree is not empty
        //         status = remove_elem(&root, keys[i]);
        //         if (status != GOOD) break;
        //     }
        // }
        status = remove_elem(&root, keys[randIndex]);
        clock_gettime(CLOCK_MONOTONIC, &delF);

        if (status != GOOD) {
            cleanup_keys_and_infos(keys, infos, action[act]);
            free_tree(root);
            return status;
        }

        // Calculate times
        long double insTime = (insF.tv_sec - insS.tv_sec) + (insF.tv_nsec - insS.tv_nsec) / 1e9;
        long double srchTime = (srchF.tv_sec - srchS.tv_sec) + (srchF.tv_nsec - srchS.tv_nsec) / 1e9;
        long double msrchTime = (maxsrchF.tv_sec - maxsrchS.tv_sec) + (maxsrchF.tv_nsec - maxsrchS.tv_nsec) / 1e9;
        long double delTime = (delF.tv_sec - delS.tv_sec) + (delF.tv_nsec - delS.tv_nsec) / 1e9;

		sumIns += insTime;
		sumDel += delTime;
		sumSrch += srchTime;
		sumSrchMax += msrchTime;
		if (((act + 1) % 5) == 0){
			long double resIns = sumIns / 5;
			long double resDel = sumDel / 5;
			long double resSrch = sumSrch / 5;
			long double resSrchMax = sumSrchMax / 5;
			printf("\n%0.12Lf\n%0.12Lf\n%0.12Lf\n%0.12Lf", resIns, resDel, resSrch, resSrchMax);
			sumIns = 0;
			sumDel = 0;
			sumSrch = 0;
			sumSrchMax = 0;
		}
        // printf("\n%0.12Lf", insTime);
        // printf("\n%0.12Lf", srchTime);
        // printf("\n%0.12Lf", msrchTime);
        // printf("\n%0.12Lf", delTime);

        cleanup_keys_and_infos(keys, infos, action[act]);
    }

    free_tree(root);
    printf("Done.\n");
    return 0;
}
