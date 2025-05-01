#include "all.h"


void free_search_results(KeySpace *results, int count) {
	if (!results) return;
	
	for (int i = 0; i < count; i++) {
		free(results[i].info);
	}
	free(results);
}

int main() {
	int initial_size = 101;
	ERROR glob_err = GOOD;
	if (glob_err == NULL){
		printf(BAD_ALLOC_MESSAGE);
		return 1;
	}
	glob_err = GOOD;
	Table *tbl = init(initial_size, &glob_err);
	if (glob_err == BAD_ALLOC) {
		printf(BAD_ALLOC_MESSAGE);
		return 1;
	}
	if (glob_err == BAD) {
		printf("\n%d", BAD);
		return 1;
	}
	
	// int choice;
	unsigned int key;
	unsigned int info;
	int inp = BAD;
	int inpT = BAD;
	unsigned int tmp;
	unsigned int tmp2;
	tmp = 0;
	tmp2 = 0;
	
	unsigned int action = 0;
	while (inp != EXIT) {
		inp = input(&action, MENU, CHOICE);
		while (inp != GOOD){
			if (inp == EXIT){
				tbl_free(tbl);
				return 0;
			}
			inp = input(&action, MENU, CHOICE);
		}
		
		switch ((int)action) {
			case 0:
				int ins = insertion(tbl, &tmp, &tmp2, &glob_err);
				if (ins == EXIT || ins == BAD_ALLOC){return ins;}
				break;
				
			case 1:
				int rm = removal(tbl, &tmp, &tmp2, &glob_err);
				if (rm == EXIT){return rm;}
				break;
			case 2:
				int rmbyver = removalByVersion(tbl, &tmp, &tmp2, &glob_err);
				if (rmbyver == EXIT){return rmbyver;}
				break;

			case 3:
				int srchh = srch(tbl, &tmp, &tmp2, &glob_err);
				if (srchh == EXIT || srchh == BAD_ALLOC){return srchh;}
				break;
			case 4:
				int srchbyver = srchByVersion(tbl, &tmp, &tmp2, &glob_err);
				if (srchbyver == EXIT || srchbyver == BAD_ALLOC){return srchbyver;}
				break;
				
			case 5:
				int imp = flImport(tbl, &glob_err);
				if (imp == EXIT || imp == BAD_ALLOC){return imp;}
				break;
				
			case 6:
				int exp = flExport(tbl, &glob_err);
				if (exp == EXIT){return exp;}
				break;
				
			case 7:
				tbl_free(tbl);
				printf("exiting...\n");
				return 0;
			default:
				printTable(tbl);
				printf("Неверный ввод, выберите ещё раз\n");
			
		}
		printTable(tbl);
	}
}

int insertion(Table *tbl, unsigned int *tmp, unsigned int *tmp2, ERROR *glob_err){
	int inpT = input(tmp, "\nВведите ключ для вставки: ", KEY);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp, "\nВведите ключ для вставки: ", KEY);
	}
	inpT = input(tmp2, "\nВведите значение для вставки: ", INFO);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp2, "\nВведите значение для вставки: ", INFO);
	}
	*glob_err = GOOD;
	insert(tbl, (unsigned int)*tmp, (unsigned int)*tmp2, glob_err);
	switch (*glob_err){
		case BAD:
			printf("Вставка нот гуд\n");
			break;
		case GOOD:
			printf("Вставка гуд\n");
			break;
		case BAD_ALLOC:
			printf(BAD_ALLOC_MESSAGE);
			tbl_free(tbl);
			return 1;
	}
	return GOOD;
}

int removal(Table *tbl, unsigned int *tmp, unsigned int *tmp2, ERROR *glob_err){
	int inpT = input(tmp, "\nВведите ключ для удаления: ", KEY);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp, "\nВведите ключ для удаления: ", KEY);
	}
	
	*glob_err = GOOD;
	elem_remove(tbl, (unsigned int)*tmp, -1, glob_err);
	if (*glob_err != BAD) {
		printf("Элемент(ы) удален(ы)\n");
	} else {
		printf("Элемент(ы) не найден(ы)\n");
	}
	return GOOD;
}


int removalByVersion(Table *tbl, unsigned int *tmp, unsigned int *tmp2, ERROR *glob_err){
	int inpT = input(tmp, "\nВведие ключ для удаления: ", KEY);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp, "\nВведите ключ для удаления: ", KEY);
	}
	inpT = input(tmp2, "\nВведите версию для удаления: ", REL);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp2, "\nВведите версию для удаления: ", REL);
	}
	*glob_err = GOOD;
	elem_remove(tbl, (unsigned int)*tmp, (int)*tmp2, glob_err);
	if (*glob_err != BAD) {
		printf("Элемент(ы) удален(ы)\n");
	} else {
		printf("Элемент(ы) не найден(ы)\n");
	}
	return GOOD;
}

int srch(Table *tbl, unsigned int *tmp, unsigned int *tmp2, ERROR *glob_err){
	int inpT = input(tmp, "\nВведите ключ для поиска: ", KEY);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp, "\nВведите ключ для поиска: ", KEY);
	}
	
	int all_count;
	*glob_err = GOOD;
	KeySpace *all_results = search_all_versions(tbl, (unsigned int)*tmp, &all_count, glob_err);

	if (glob_err == BAD_ALLOC){
		printf(BAD_ALLOC_MESSAGE);
		tbl_free(tbl);
		return BAD_ALLOC;
	}
	
	if (*glob_err != NOT_FOUND) {
		printf("Найдено %d версий:\n", all_count);
		for (int i = 0; i < all_count; i++) {
			printf("  Версия: %d, Значение: %u\n",
			       all_results[i].release, *(all_results[i].info));
		}
		free_search_results(all_results, all_count);
	} else {
		printf("Элементы не найдены\n");
	}
	return GOOD;
}

int srchByVersion(Table *tbl, unsigned int *tmp, unsigned int *tmp2, ERROR *glob_err){
	int inpT = input(tmp, "\nВведие ключ для поиска: ", KEY);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp, "\nВведите ключ для поиска: ", KEY);
	}
	inpT = input(tmp2, "\nВведите версию для поиска: ", REL);
	while (inpT != GOOD || *tmp < 0){
		if (inpT == EXIT){
			tbl_free(tbl);
			return EXIT;
		}
		inpT = input(tmp2, "\nВведите версию для поиска: ", REL);
	}
	
	int count;
	*glob_err = GOOD;
	KeySpace *results = search(tbl, (unsigned int)*tmp, (int)*tmp2, &count, glob_err);

	if (*glob_err == BAD_ALLOC){
		printf(BAD_ALLOC_MESSAGE);
		tbl_free(tbl);
		return BAD_ALLOC;
	}
	if (*glob_err != NOT_FOUND) {
		printf("Найдено %d элементов:\n", count);
		for (int i = 0; i < count; i++) {
			printf("  Ключ: %u, Версия: %d, Инфо: %u\n",
			       results[i].key, results[i].release, *(results[i].info));
		}
		free_search_results(results, count);
	} else{
		printf("Элемент не найден\n");
	}
	return GOOD;
}


int flImport(Table *tbl, ERROR *glob_err){
	char *filename = readline("\nВведите имя файла для импорта: ");
	if (filename == NULL){
		tbl_free(tbl);
		return EXIT;
	}

	*glob_err = GOOD;
	import(tbl, filename, glob_err);
	switch (*glob_err){
		case BAD_FORMAT:
			printf("\nНеверный формат файла");
			break;
		case NOT_FOUND_FILE:
			printf("\nФайл не найден");
			break;
		case BAD_ALLOC:
			printf(BAD_ALLOC_MESSAGE);
			tbl_free(tbl);
			return BAD_ALLOC;
		case BAD:
			break;
	}
	return GOOD;
}

int flExport(Table *tbl, ERROR *glob_err){
	char *filename = readline("\nВведите имя файла для экспорта: ");
	if (filename == NULL){
		tbl_free(tbl);
		return EXIT;
	}

	*glob_err = GOOD;
	export(tbl, filename, glob_err);
	switch (*glob_err){
		case GOOD:
			printf("\nЭкспорт гуд");
			break;
		case FAILED_TO_CREATE_FILE:
			printf("\nНе получилось создать выходной файл, ничего не произошло");
			break;
		case FAILED_TO_WRITE:
			printf("\nНе удалось записать в файл");
			break;
		case BAD:
			printf("\nЭкспорт нот гуд, ничего не произошло");
			break;
	}
	return GOOD;
}
