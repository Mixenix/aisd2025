#include "tree.h"

#define DOT_FILENAME "scapegoat_tree.dot"
#define PNG_FILENAME "scapegoat_tree.png"
#define CHOICE "\nВыберите действие: "
#define TREE_IS_EMPTY "Дерево пустое\n"
#define WRONG_INP "Неверный ввод, попробуйте ещё раз.\n"

void menu() {
	printf("\n|||MENU|||\n0. Вставка\t1. Удаление\t2. Обход дерева\n3. Поиск\t4.Поиск с наиб ключом\n5. Вывод дерева\t6. Визуализация\t7. Импорт из txt\n");
}

int main() {
	Node* root = NULL;
	ERROR status;
	int choice;
	char* inpStrKey = NULL;
	char* inpStrFile = NULL;
	unsigned int inpValue;

	menu();

	while (1) {
		status = input_int_action(&choice, CHOICE);

		if (status == EXIT) {
			choice = 9;
		} else if (status == INVALID_INPUT) {
			printf("Неверный ввод, попробуйте ещё раз\n");
			continue;
		}
		
		switch (choice) {
			case 0:
				status = input_string(&inpStrKey, "Введите ключ (строка): ");
				if (status == EXIT) {
					choice = 9;
					break;
				}
				if (status != GOOD || !inpStrKey || strlen(inpStrKey) == 0) {
					printf(WRONG_INP);
					free(inpStrKey);
					inpStrKey = NULL;
					continue;
				}

				status = input_unsigned_int(&inpValue, "Введите значение (беззнак целое): ");
				if (status == EXIT) {
					choice = 9;
					break;
				}
				if (status != GOOD) {
					printf(WRONG_INP);
					continue;
				}

				status = insert_elem(&root, inpStrKey, inpValue);
				if (status == BAD_ALLOC) {
					printf(BAD_ALLOC_MESSAGE);
					free_tree(root);
					root = NULL;
					goto freedomAndExit;
				} else {
					printf("Элемент вставлен. ('%s': %u)\n", inpStrKey, inpValue);
				}
				free(inpStrKey);
				inpStrKey = NULL;
				break;

			case 1:
				if (!root) {
					printf(TREE_IS_EMPTY);
					break;
				}
				status = input_string(&inpStrKey, "Введите ключ для удаления: ");
				if (status == EXIT) {
					choice = 9;
					break;
				}
				if (status != GOOD || !inpStrKey || strlen(inpStrKey) == 0) {
					printf(WRONG_INP);
					free(inpStrKey);
					inpStrKey = NULL;
					continue;
				}
				status = remove_elem(&root, inpStrKey);
				if (status == NOT_FOUND) {
					printf("Элемент '%s' не найден\n", inpStrKey);
				} else if (status == BAD_ALLOC) {
					printf(BAD_ALLOC_MESSAGE);
					free_tree(root);
					root = NULL;
					goto freedomAndExit;
				} else {
					printf("Элемент '%s' удалён\n", inpStrKey);
				}
				free(inpStrKey);
				inpStrKey = NULL;
				break;

			case 2:
				if (!root) {
					printf(TREE_IS_EMPTY);
				} else {
					printf("\n|||Обход дерева\n");
					print_in_order(root);
					printf("|||\n");
				}
				break;

			case 3:
				if (!root) {
					printf(TREE_IS_EMPTY);
					break;
				}
				status = input_string(&inpStrKey, "Введите ключ для поиска: ");
				if (status == EXIT) {
					choice = 9;
					break;
				}
				if (status != GOOD || !inpStrKey || strlen(inpStrKey) == 0) {
					printf(WRONG_INP);
					free(inpStrKey);
					inpStrKey = NULL;
					continue;
				}
				
				SearchResults results_search;
				results_search = search_by_key(root, inpStrKey, &status);
				if (status == BAD_ALLOC) {
					printf(BAD_ALLOC_MESSAGE);
					free_search_results(&results_search);
					free_tree(root);
					root = NULL;
					goto freedomAndExit;
				}

				printf("Элемент '%s найден' (%zu ):\n", inpStrKey, results_search.count);
				for (size_t i = 0; i < results_search.count; ++i) {
					printf("	-> Ключ: %s, Значение: %u\n", results_search.nodes[i]->key, results_search.nodes[i]->value);
				}
				free_search_results(&results_search);
				free(inpStrKey);
				inpStrKey = NULL;
				break;

			case 4:
				if (!root) {
					printf(TREE_IS_EMPTY);
					break;
				}
				SearchResults results_max;
				results_max = find_max_keys(root, &status);
				if (status == BAD_ALLOC) {
					printf(BAD_ALLOC_MESSAGE);
					free_search_results(&results_max);
					free_tree(root);
					root = NULL;
					goto freedomAndExit;
				}

				printf("Элементы с наибольшими ключами (%zu найдено):\n", results_max.count);
				if (results_max.count > 0) {
					printf("Наибольший ключ: '%s'\n", results_max.nodes[0]->key);
					for (size_t i = 0; i < results_max.count; ++i) {
						printf("	-> Ключ: %s, Значение: %u\n", results_max.nodes[i]->key, results_max.nodes[i]->value);
					}
				}
				free_search_results(&results_max);
				break;

			case 5:
				if (!root) {
					printf(TREE_IS_EMPTY);
				} else {
					printf("\n|||Дерево, с выводом как дерево\n");
					print_tree_formatted(root, 0, 0);
					printf("|||\n");
				}
				break;

			case 6:
				if (!root) {
					printf(TREE_IS_EMPTY);
					break;
				}
				status = visualize_tree_graphviz(root, DOT_FILENAME, PNG_FILENAME);
				if (status == FILE_ERROR) {
					printf("Ошибка при создании DOT-файла\n");
				} else if (status == BAD) {
					printf("Ошибка graphviz\n");
				}
				break;

			case 7:
				status = input_string(&inpStrFile, "Введите имя файла для импорта: ");
				if (status == EXIT) {
					choice = 9;
					break;
				}
				if (status != GOOD || !inpStrFile || strlen(inpStrFile) == 0) {
					printf("Неверное имя файла\n");
					free(inpStrFile);
					inpStrFile = NULL;
					continue;
				}

				
				free_tree(root);
				root = NULL;
				printf("Прошлое дерево удалено. Импорт нового\n");

				status = import_from_txt(&root, inpStrFile);
				if (status == FILE_ERROR) {
					printf("Не удалось открыть файл\n");
				} else if (status == BAD_ALLOC) {
					printf(BAD_ALLOC_MESSAGE);
					free_tree(root);
					root = NULL;
					goto freedomAndExit;
				} else {
					printf("Импорт гуд\n");
				}
				free(inpStrFile);
				inpStrFile = NULL;
				break;
			default:
				printf(WRONG_INP);
				menu();
		}
		if (choice == 9) {
			break;
		}
	}

freedomAndExit:
	free_tree(root);
	if (inpStrKey) {
		free(inpStrKey);
	}
	if (inpStrFile) {
		free(inpStrFile);
	}
	printf("Выход...\n");
	return 0;
}
