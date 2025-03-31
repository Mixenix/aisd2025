#include "all.h"

void printTable(Table *tbl);
int insert(Table *tbl, char *keyToInsertTo, char *infoToInsert);
void freedom(Table *tbl);


int main() {
	Table *tbl = malloc(sizeof(Table));
	if (tbl == NULL){
		printf(ALLOC_ERROR);
		return 1;
	}
	tbl->msize = 0;
	tbl->csize = 0;
	tbl->ks = calloc(tbl->msize, sizeof(KeySpace));
	if (tbl->ks == NULL){
		printf(ALLOC_ERROR);
		return 1;
	}
	int inp = BAD;
	int inpT = BAD;
	int *tmp;
	int checker = BAD;
	while (inp != EXIT){
		int *action = calloc(1, sizeof(int));
		if (action == NULL){
			print(ALLOC_ERROR);
			freedom(tbl);
			return 1;
		}
		inp = input(action, MENU);
		while (inp != GOOD){
			if (inp == EXIT){
				free(action);
				freedom(tbl);
				return 0;
			}
			inp = input(action, MENU);
		}
		switch (*action){
		case 0: // инициализация новой
			freedom(tbl);
			tmp = calloc(1, sizeof(int));
			if (tmp == NULL){
				printf(ALLOC_ERROR);
				return 1;
			}
			*tmp = -1;
			inpT = input(tmp, "\nВведите размер пространства ключей: ");
			while (inpT != GOOD && *tmp < 0){
				if (inpT == EXIT){
					free(tmp);
					free(action);
					return 0;
				}
				printf("\nЧто-то не так, ещё раз...");
				inpT = input(tmp, "\nВведите размер пространства ключей: ");
			}
			tbl = malloc(sizeof(Table));
			if (tbl == NULL){
				printf(ALLOC_ERROR);
				free(action);
				free(tmp);
				return 1;
			}
			tbl->msize = *tmp;
			tbl->csize = 0;
			tbl->ks = calloc(tbl->msize, sizeof(KeySpace));
			if (tbl->ks == NULL){
				printf(ALLOC_ERROR);
				freedom(tbl);
				free(action);
				free(tmp);
				return 1;
			}
			free(tmp);
			break;
		case 1: // вставка	
			// printf("\nВведите сначала ключ, а потом информацию (char* и char*):");
			char *keyToInsertTo = readline("\nВведите ключ: ");
			if (keyToInsertTo == NULL){
				free(action);
				freedom(tbl);
				return 0;
			}
			char *infoToInsert = readline("\nВведите информацию: ");
			if (infoToInsert == NULL){
				free(keyToInsertTo);
				free(action);
				freedom(tbl);
				return 0;
			}
			checker = insert(tbl, keyToInsertTo, infoToInsert);
			switch (checker){
			case SIZEEXCEEDED:
				printf("\nТаблица заполнена, вставить не получилось...\n");
				break;
			case NOTUNIQUE:
				printf("\nИнформация неуникальная, вставлять не будем...\n");
				break;
			case MEMERR:
				printf(ALLOC_ERROR);
				freedom(tbl);
				free(keyToInsertTo);
				free(infoToInsert);
				free(action);
				return 1;
			}
			
			break;
		case 2:
			char *keyToDel2 = readline("\nВведите ключ для удаления значения по нему: ");
			if (keyToDel2 == NULL){
				freedom(tbl);
				free(action);
				return 0;
			}
			tmp = calloc(1, sizeof(int));
			if (tmp == NULL){
				printf(ALLOC_ERROR);
				freedom(tbl);
				free(action);
				return 1;
			}
			inpT = input(tmp, "\nВведите версию для удаления: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					free(tmp);
					free(action);
					freedom(tbl);
					return 0;
				}
				printf("\nЧто-то не так, ещё раз...");
				inpT = input(tmp, "\nВведите версию для удаления: ");
			}
			
			checker = removeByKeyAndRelease(tbl, keyToDel2, *tmp);
			if (checker == BAD){
				printf("\nerror or key wasnt found or release wasnt found...");
			}
			free(tmp);
			free(keyToDel2);
			break;
		case 3:
			char *keyToDel = readline("\nВведите ключ для удаления всех значений по нему: ");
			if (keyToDel == NULL){
				freedom(tbl);
				free(action);
				return 0;
			}
			checker = removeByKey(tbl, keyToDel);
			if (checker == BAD){
				printf("\nerror or key wasnt found...");
			}
			free(keyToDel);
			break;
		case 4:
			char *keyToFind = readline("\nВведите ключ для поиска: ");
			if (keyToFind == NULL){
				freedom(tbl);
				free(action);
				return 0;
			}
			// Table *res = searchByKey(&tbl, keyToFind);
			Table *res = searchAllByKey(tbl, keyToFind);
			if (res){
				freedom(tbl);
				tbl = res;
			}
			else{
				printf("\nКлюч не найден, вот вам исходная таблица:");
			}
			free(keyToFind);
			break;
		case 5:
			char *keyToFind2 = readline("\nВведите ключ для поиска: ");
			tmp = calloc(1, sizeof(int));
			inpT = input(tmp, "\nВведите версию для поиска: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					free(tmp);
					freedom(tbl);
					return 0;
				}
				printf("\nЧто-то не так, ещё раз...");
				inpT = input(tmp, "\nВведите версию для поиска: ");
			}
			char *result = searchByKeyAndRelease(tbl, keyToFind2, *tmp);
			if (result == NULL){
				printf("\nnothing found or there is an error...");
			}
			else{
				printf("\n%d %s", *tmp, result);
			}
			free(tmp);
			free(keyToFind2);
			free(result);
			break;
		case 6:
			freedom(tbl);
			char *filename = readline("\nВведите имя файла: ");
			if (filename == NULL){
				free(action);
				return 0;
			}
			tbl = readTableFromFile(filename);
			if (tbl == NULL){
				printf("\nThere is an error or the file is empty...");
			}
			free(filename);
			break;
		case 7:
			break;
		}
		printTable(tbl);
		free(action);
	}

	printTable(tbl);
	freedom(tbl);
	return 0;
}

int insert(Table *tbl, char *keyToInsertTo, char* infoToInsert) {
	Node *toAdd = malloc(sizeof(Node));
	if (toAdd == NULL){
		return MEMERR;
	}
	toAdd->release = NOTDEFINEDYET;
	toAdd->info = infoToInsert;
	toAdd->next = NULL;
	if (tbl == NULL || tbl->ks == NULL) return BAD;
	
	for (int i = 0; i < tbl->csize; i++) {
		if (tbl->ks[i].key && strcmp(tbl->ks[i].key, keyToInsertTo) == 0) {
			Node *elem = tbl->ks[i].node;
			while (elem != NULL){
				if (strcmp(elem->info, toAdd->info) == 0) {
					free(toAdd->info);
					free(toAdd);
					return NOTUNIQUE;
				}
				elem = elem->next;
			}
			toAdd->next = tbl->ks[i].node;
			toAdd->release = tbl->ks[i].node->release + 1;
			tbl->ks[i].node = toAdd;
			free(keyToInsertTo);
			return GOOD;
		}
	}
	
	if (tbl->csize >= tbl->msize) {
		free(toAdd->info);
		free(toAdd);
		free(keyToInsertTo);
		return SIZEEXCEEDED;
	}
	
	toAdd->release = 0;
	tbl->ks[tbl->csize].key = keyToInsertTo;
	tbl->ks[tbl->csize].node = toAdd;
	tbl->csize++;
	return GOOD;
}

void freedom(Table *tbl) {
	if (tbl == NULL) return;

	if (tbl->ks != NULL) {
		for (int i = 0; i < tbl->csize; i++) {
			free(tbl->ks[i].key);
			
			Node *elem = tbl->ks[i].node;
			while (elem != NULL) {
				Node *tmp = elem->next;
				free(elem->info);
				free(elem);
				elem = tmp;
			}
		}
		free(tbl->ks);
	}
	free(tbl);
}

void printTable(Table *tbl) {
	if (tbl == NULL || tbl->ks == NULL || tbl->msize == 0){
		printf("\n(таблица пуста)");
		return;
	}
	int flag = 1;
	printf("\nМаксимальный размер: <%d>", tbl->msize);
	for (int i = 0; i < tbl->csize; i++) {
		if (tbl->ks[i].key == NULL) continue;
		
		printf("\n%s", tbl->ks[i].key);
		Node *nxt = tbl->ks[i].node;
		while (nxt != NULL) {
			if (nxt->info != NULL) {
				flag = 0;
				printf("\n    %d %s", nxt->release, nxt->info);
			}
			nxt = nxt->next;
		}
	}
	if (flag){printf("\n(таблица пуста)");}
}
