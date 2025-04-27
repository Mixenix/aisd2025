#include "bin.h"



void print_help() {
	printf(MENU);
}

// void load_from_file(Node** root, char* filename) {
// 	FILE* file = fopen(filename, "r");
// 	if (!file) {
// 		printf("Ошибка открытия файла %s\n", filename);
// 		return;
// 	}
// 	
// 	char key[MAX_INPUT_LENGTH];
// 	char value[MAX_INPUT_LENGTH];
// 	
// 	while (fgets(key, sizeof(key), file) && fgets(value, sizeof(value), file)) {
// 		key[strcspn(key, "\n")] = 0;
// 		value[strcspn(value, "\n")] = 0;
// 		
// 		bst_insert(root, key, value);
// 	}
// 	
// 	fclose(file);
// 	printf("Дерево загружено из файла %s\n", filename);
// }

int main(){
	Node* root = NULL;

	// char input[MAX_INPUT_LENGTH];
	// char command[MAX_INPUT_LENGTH];
	// char key[MAX_INPUT_LENGTH];
	// char value[MAX_INPUT_LENGTH];

	// print_help();

	ERROR inp = BAD;
	ERROR inpT = GOOD;

	while (inp != EXIT){
		int action = 0;

		inp = input(&action, NULL, MENU);
		while (inp != GOOD){
			if (inp == EXIT){
				freedom(root);
				return 0;
			}
			inp = input(&action, NULL, MENU);
		}

		int tmp;
		ERROR err = GOOD;
		char *key;
		switch (action){
		case 0: // добавить
			key = malloc(sizeof(1));
			if (key == NULL){
				free(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			inpT = input(&tmp, &key, "\nВведите ключ: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					return 0;
				}
				if (inpT == BAD_ALLOC){
					return 1;
				}
				printf("\nЧто-то не так, ещё раз");
				inpT = input(&tmp, &key, "\nВведите ключ: ");
			}
			char *info = malloc(sizeof(1));
			if (info == NULL){
				free(root);
				free(key);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			inpT = input(&tmp, &info, "\nВведите информацию: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					freedom(root);
					return 0;
				}
				if (inpT == BAD_ALLOC){
					freedom(root);
					printf(BAD_ALLOC_MESSAGE);
					return 1;
				}
				printf("\nЧто-то не так, ещё раз");
				inpT = input(&tmp, &info, "\nВведите информацию: ");
			}
			err = GOOD;
			insertElem(&root, key, info, &err);
			free(key);
			free(info);
			if (err == BAD_ALLOC){
				freedom(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			break;
		case 1: // удалить
			key = malloc(sizeof(1));
			if (key == NULL){
				free(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			inpT = input(&tmp, &key, "\nВведите ключ: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					return 0;
				}
				if (inpT == BAD_ALLOC){
					return 1;
				}
				printf("\nЧто-то не так, ещё раз");
				inpT = input(&tmp, &key, "\nВведите ключ: ");
			}
			err = GOOD;
			removeElem(&root, key, &err);
			free(key);
			if (err == BAD_ALLOC){
				freedom(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			else if (err == NOT_FOUND){
				printf("\nЭлемент не найден");
			}
			else{
				printf("\nУспешно удалено");
			}
			break;
		case 2: // обход
			key = malloc(sizeof(1));
			if (key == NULL){
				free(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			inpT = input(&tmp, &key, "\nВведите ключ: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					return 0;
				}
				if (inpT == BAD_ALLOC){
					return 1;
				}
				printf("\nЧто-то не так, ещё раз");
				inpT = input(&tmp, &key, "\nВведите ключ: ");
			}
			if (strcmp(key, "\n") == 0){
				printTree(root, 0);
			}
			else{
				printOut(root, key);
			}
			free(key);
			break;

		case 3: // поиск
			key = malloc(sizeof(1));
			if (key == NULL){
				freedom(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			inpT = input(&tmp, &key, "\nВведите ключ: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					return 0;
				}
				if (inpT == BAD_ALLOC){
					return 1;
				}
				printf("\nЧто-то не так, ещё раз");
				inpT = input(&tmp, &key, "\nВведите ключ: ");
			}
			err = GOOD;
			SearchResults results = search(root, key, &err);
			free(key);
			if (err == BAD_ALLOC){
				freedom(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			printf("\nНайдено %d элементов: ", results.count);
			for (int i=0; i<results.count; i++){
				printf("\n\t%s ||| %s", results.nodes[i]->key, results.nodes[i]->info);
			}
			break;

		case 4: // поиск минимального
			Node *mn = findMin(root);
			if (mn != NULL){
				printf("\nМинимальный ключ: %s ||| %s\n", mn->key, mn->info);
				break;
			}
			printf("\nДерево пустое");
			break;

		case 5: // импорт из txt
			key = malloc(sizeof(1));
			if (key == NULL){
				freedom(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			inpT = input(&tmp, &key, "\nВведите название файла: ");
			while (inpT != GOOD){
				if (inpT == EXIT){
					return 0;
				}
				if (inpT == BAD_ALLOC){
					return 1;
				}
				printf("\nЧто-то не так, ещё раз");
				inpT = input(&tmp, &key, "\nВведите название файла: ");
			}
			err = GOOD;
			importFromTxt(&root, key, &err);
			free(key);
			if (err == BAD_ALLOC){
				freedom(root);
				printf(BAD_ALLOC_MESSAGE);
				return 1;
			}
			break;
		}
		printTree(root, 0);
	}
	
	// while (1) {
	// 	printf("> ");
	// 	if (!fgets(input, sizeof(input), stdin)) break;
	// 	
	// 	input[strcspn(input, "\n")] = 0;
	// 	
	// 	int args = sscanf(input, "%s %s %[^\n]", command, key, value);
	// 	
	// 	if (strcmp(command, "exit") == 0) {
	// 		break;
	// 	} else if (strcmp(command, "help") == 0) {
	// 		print_help();
	// 	} else if (strcmp(command, "add") == 0 && args >= 3) {
	// 		bst_insert(&root, key, value);
	// 		printf("Элемент добавлен: %s -> %s\n", key, value);
	// 	} else if (strcmp(command, "remove") == 0 && args >= 2) {
	// 		char* old_value = bst_remove(&root, key);
	// 		if (old_value) {
	// 			printf("Элемент удален: %s -> %s\n", key, old_value);
	// 			free(old_value);
	// 		} else {
	// 			printf("Элемент с ключом %s не найден\n", key);
	// 		}
	// 	} else if (strcmp(command, "search") == 0 && args >= 2) {
	// 		SearchResults results = bst_search(root, key);
	// 		if (results.count > 0) {
	// 			printf("Найдено %zu элементов с ключом %s:\n", results.count, key);
	// 			for (size_t i = 0; i < results.count; i++) {
	// 				printf("  %s: %s\n", results.nodes[i]->key, results.nodes[i]->value);
	// 			}
	// 		} else {
	// 			printf("Элементы с ключом %s не найдены\n", key);
	// 		}
	// 		free(results.nodes);
	// 	} else if (strcmp(command, "min") == 0) {
	// 		Node* min_node = bst_find_min(root);
	// 		if (min_node) {
	// 			printf("Минимальный ключ: %s -> %s\n", min_node->key, min_node->value);
	// 		} else {
	// 			printf("Дерево пустое\n");
	// 		}
	// 	} else if (strcmp(command, "traverse") == 0) {
	// 		const char* min_key = (args >= 2) ? key : NULL;
	// 		printf("Элементы с ключами больше %s:\n", min_key ? min_key : "-∞");
	// 		bst_traverse_gt(root, min_key, bst_print_node);
	// 	} else if (strcmp(command, "print") == 0) {
	// 		printf("Дерево:\n");
	// 		bst_print_tree(root, 0);
	// 	} else if (strcmp(command, "load") == 0 && args >= 2) {
	// 		load_from_file(&root, key);
	// 	} else {
	// 		printf("Неизвестная команда или неверные аргументы. Введите 'help' для справки.\n");
	// 	}
	// }
	// 
	// bst_free(root);
	// return 0;
}
