#include "bin.h"



void print_help() {
	printf(MENU);
}

int main(){
	Node* root = NULL;

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
		char *info;
		switch (action){
		case 0: // добавить
			
			inpT = input(&tmp, &key, "\nВведите ключ: ");
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
				inpT = input(&tmp, &key, "\nВведите ключ: ");
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
			inpT = input(&tmp, &key, "\nВведите ключ: ");
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
			inpT = input(&tmp, &key, "\nВведите ключ: ");
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
			
			inpT = input(&tmp, &key, "\nВведите ключ: ");
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
			
			inpT = input(&tmp, &key, "\nВведите название файла: ");
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
		case 6: //graphviz
			err = GOOD;
			visualizeTree(root, &err);
			break;
		}	
		printTree(root, 0);
	}
}
