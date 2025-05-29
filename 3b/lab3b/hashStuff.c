#include "all.h"

static unsigned long long hash(unsigned int key) { //хэш дженкинса
	key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
	return key;
}

Table *init(int msize, ERROR *err) {
	if (msize == 0){
		*err = BAD;
		return NULL;
	}
	Table *res = malloc(sizeof(Table));
	if (res == NULL){
		*err = BAD_ALLOC;
		return NULL;
	}
	

	res->ks = (KeySpace *)calloc(msize, sizeof(KeySpace));
	if (!res->ks){
		free(res);
		*err = BAD_ALLOC;
		return NULL;
	}
	res->msize = msize;
	res->csize = 0;
	return res;
}

void tbl_free(Table *tbl) {
	if (!tbl) return;
	
	if (tbl->ks) {
		for (int i = 0; i < tbl->msize; i++) {
			if (tbl->ks[i].busy == USED && tbl->ks[i].info) {
				free(tbl->ks[i].info);
			}
		}
		free(tbl->ks);
	}
	free(tbl);
}

void insert(Table *tbl, unsigned int key, unsigned int info, ERROR *err) {
	if (!tbl || !tbl->ks){
		*err = BAD;
		return;
	}
	if ((double)tbl->csize / tbl->msize >= SIZE_PERCENTAGE) {
		ERROR err2 = GOOD;
		
		resize(tbl, &err2);
		if (err2 == BAD_ALLOC){
			*err = BAD_ALLOC;
			return;
		}
	}
	
	int index = hash(key) % tbl->msize;
	int release = 1;
	
	while (tbl->ks[index].busy == USED && tbl->ks[index].key != key) {
		index = (index + 1) % tbl->msize;
	}
	
	if (tbl->ks[index].busy == USED && tbl->ks[index].key == key) {
		release = tbl->ks[index].release + 1;
		
		int new_index = (index + 1) % tbl->msize;
		while (tbl->ks[new_index].busy == USED && new_index != index) {
			new_index = (new_index + 1) % tbl->msize;
		}
		
		if (new_index == index){
			*err = BAD;
			return;
		}
		index = new_index;
	}
	
	tbl->ks[index].busy = USED;
	tbl->ks[index].key = key;
	tbl->ks[index].release = release;
	tbl->ks[index].info = (unsigned int *)malloc(sizeof(unsigned int));
	if (!tbl->ks[index].info){
		*err = BAD_ALLOC;
		return;
	}
	
	*(tbl->ks[index].info) = info;
	tbl->csize++;
}

void elem_remove(Table *tbl, unsigned int key, int release, ERROR *err) {
	if (!tbl || !tbl->ks){
		*err = BAD;
		return;
	}
	
	int index = hash(key) % tbl->msize;
	int start_index = index;
	do {
		if (tbl->ks[index].busy == USED && 
			tbl->ks[index].key == key && 
			(release == -1 || tbl->ks[index].release == release)) {
			
			free(tbl->ks[index].info);
			tbl->ks[index].busy = DELETED;
			tbl->ks[index].info = NULL;
			tbl->csize--;
		}
		
		index = (index + 1) % tbl->msize;
	} while (index != start_index);
	
	*err = BAD;
}

KeySpace *search(Table *tbl, unsigned int key, int release, int *found_count, ERROR *err) {
	if (!tbl || !tbl->ks || !found_count) return NULL;
	
	KeySpace *result = NULL;
	*found_count = 0;
	int capacity = 2;
	
	result = (KeySpace *)malloc(capacity * sizeof(KeySpace));
	if (!result){
		*err = BAD_ALLOC;
		return NULL;
	}
	
	int index = hash(key) % tbl->msize;
	int start_index = index;
	
	do {
		if (tbl->ks[index].busy == USED && 
			tbl->ks[index].key == key &&
			(release == -1 || tbl->ks[index].release == release)) {
			
			if (*found_count >= capacity) {
				capacity *= 2;
				KeySpace *temp = (KeySpace *)realloc(result, capacity * sizeof(KeySpace));
				if (!temp) {
					free(result);
					*err = BAD_ALLOC;
					return NULL;
				}
				result = temp;
			}
			
			result[*found_count] = tbl->ks[index];
			result[*found_count].info = (unsigned int *)malloc(sizeof(unsigned int));
			if (!result[*found_count].info) {
				for (int i = 0; i < *found_count; i++) {
					free(result[i].info);
				}
				free(result);
				*err = BAD_ALLOC;
				return NULL;
			}
			*(result[*found_count].info) = *(tbl->ks[index].info);
			(*found_count)++;
			
			if (release != -1) break;
		}
		
		index = (index + 1) % tbl->msize;
	} while (index != start_index);
	
	if (*found_count == 0) {
		free(result);
		*err = NOT_FOUND;
		return NULL;
	}
	
	return result;
}

KeySpace *search_all_versions(Table *tbl, unsigned int key, int *found_count, ERROR *err) {
	return search(tbl, key, -1, found_count, err);
}

void printTable(Table *tbl) {
	if (!tbl || !tbl->ks) return;
	
	printf("Хэш-таблица, размер: %d/%d:\n", tbl->csize, tbl->msize);
	for (int i = 0; i < tbl->msize; i++) {
		if (tbl->ks[i].busy == USED) {
			printf("[%d] Ключ: %u, Версия: %hhu, Инфо: %u\n",
				   i, tbl->ks[i].key, tbl->ks[i].release, *(tbl->ks[i].info));
		} else if (tbl->ks[i].busy == DELETED) {
			printf("[%d] <удалено>\n", i);
		}
	}
}

void export(Table *tbl, char *filename, ERROR *err) {
	if (!tbl || !tbl->ks || !filename){
		*err = BAD;
		return;
	}
	
	FILE *file = fopen(filename, "wb");
	free(filename);
	if (!file){
		*err = FAILED_TO_CREATE_FILE;
		return;
	}
	
	if (fwrite(&tbl->msize, sizeof(int), 1, file) != 1 || fwrite(&tbl->csize, sizeof(int), 1, file) != 1){
		*err = FAILED_TO_WRITE;
		fclose(file);
		return;
	}
	
	for (int i = 0; i < tbl->msize; i++) {
		if (tbl->ks[i].busy == USED) {
			if (fwrite(&tbl->ks[i].busy, sizeof(BusyType), 1, file) != 1 ||
			fwrite(&tbl->ks[i].key, sizeof(unsigned int), 1, file) != 1 ||
			fwrite(&tbl->ks[i].release, sizeof(int), 1, file) != 1 ||
			fwrite(tbl->ks[i].info, sizeof(unsigned int), 1, file) != 1){
				*err = FAILED_TO_WRITE;
				fclose(file);
				return;
			}
		}
	}
	fclose(file);
}

void import(Table *tbl, char *filename, ERROR *err) {
	if (!tbl || !filename) {
		*err = BAD;
		free(filename);
		return;
	}

	FILE *file = fopen(filename, "rb");
	free(filename);
	if (!file) {
		*err = NOT_FOUND_FILE;
		return;
	}

	int msize, csize;
	if (fread(&msize, sizeof(int), 1, file) != 1 ||
		fread(&csize, sizeof(int), 1, file) != 1) {
		fclose(file);
		*err = BAD_FORMAT;
		return;
	}

	ERROR err2 = GOOD;

	Table *new_tbl = init(msize, &err2);
	if (err2 == BAD_ALLOC) {
		fclose(file);
		*err = BAD_ALLOC;
		return;
	}

	for (int i = 0; i < csize; i++) {
		KeySpace ks;
		if (fread(&ks.busy, sizeof(BusyType), 1, file) != 1 ||
			fread(&ks.key, sizeof(unsigned int), 1, file) != 1 ||
			fread(&ks.release, sizeof(int), 1, file) != 1) {
			tbl_free(new_tbl);
			fclose(file);
			*err = BAD_FORMAT;
			return;
		}

		ks.info = malloc(sizeof(unsigned int));
		if (!ks.info || fread(ks.info, sizeof(unsigned int), 1, file) != 1) {
			free(ks.info);
			tbl_free(new_tbl);
			fclose(file);
			*err = (!ks.info) ? BAD_ALLOC : BAD_FORMAT;
			return;
		}

		int index = hash(ks.key) % new_tbl->msize;
		while (new_tbl->ks[index].busy == USED) {
			index = (index + 1) % new_tbl->msize;
		}
		new_tbl->ks[index] = ks;
		new_tbl->csize++;
	}

	fclose(file);

	KeySpace *old_ks = tbl->ks;
	int old_msize = tbl->msize;

	tbl->ks = new_tbl->ks;
	tbl->msize = new_tbl->msize;
	tbl->csize = new_tbl->csize;

	if (old_ks) {
		for (int i = 0; i < old_msize; i++) {
			if (old_ks[i].busy == USED && old_ks[i].info) {
				free(old_ks[i].info);
			}
		}
		free(old_ks);
	}

	free(new_tbl);
	*err = GOOD;
}

int is_prime(int n) {
	if (n <= 1) return 0;
	if (n <= 3) return 1;
	if (n % 2 == 0 || n % 3 == 0) return 0;
	
	for (int i = 5; i * i <= n; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) return 0;
	}
	
	return 1;
}

int next_prime_size(int current_size) {
	if (current_size <= 1) return 2;
	int new_size = current_size * 2;
	while (!is_prime(new_size)) {
		new_size++;
	}
	return new_size;
}

void resize(Table *tbl, ERROR *err) {
    if (!tbl || !tbl->ks){
        *err = BAD;
        return;
    }
    
    int new_size = next_prime_size(tbl->msize);
    ERROR err2 = GOOD;
    Table *new_tbl = init(new_size, &err2);
    if (err2 == BAD_ALLOC){
        *err = BAD_ALLOC;
        return;
    }
    
    for (int i = 0; i < tbl->msize; i++) {
        if (tbl->ks[i].busy == USED) {
            int new_index = hash(tbl->ks[i].key) % new_tbl->msize;
                   
            while (new_tbl->ks[new_index].busy == USED) {
                new_index = (new_index + 1) % new_tbl->msize;
            }
        
            new_tbl->ks[new_index].busy = USED;
            new_tbl->ks[new_index].key = tbl->ks[i].key;
            new_tbl->ks[new_index].release = tbl->ks[i].release;
            new_tbl->ks[new_index].info = (unsigned int *)malloc(sizeof(unsigned int));
            if (!new_tbl->ks[new_index].info) {
                tbl_free(new_tbl);
                *err = BAD_ALLOC;
                return;
            }
            *new_tbl->ks[new_index].info = *tbl->ks[i].info;
            new_tbl->csize++;
        }
    }
    for (int i = 0; i < tbl->msize; i++) {
        if (tbl->ks[i].busy == USED && tbl->ks[i].info) {
            free(tbl->ks[i].info);
        }
    }
    free(tbl->ks);
    tbl->ks = new_tbl->ks;
    tbl->msize = new_tbl->msize;
    tbl->csize = new_tbl->csize;
    free(new_tbl);
}
