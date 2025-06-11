#include "graph.h"


#define crdX "Коорд X: "
#define crdY "Коорд Y: "

void print_error_message(ERROR err_code){
	switch (err_code){
		case GOOD:
			break;
		case BAD:
			printf("\nЧто-то не так...");
			break;
		case BAD_ALLOC:
			printf("\nBad alloc, exiting...");
			break;
		case NOT_FOUND:
			printf("\nЭлемент не найден\n");
			break;
		case ALREADY_EXISTS:
			printf("\nЭлемент уже существует\n");
			break;
		case INVALID_INPUT:
			printf(WRONG_INP);
			break;
		case INVALID_OPERATION:
			printf("\nНеверная операция, попробуйте ещё раз\n");
			break;
		case EXIT:
			break;
		default:
			printf("\nЧто-то не так, выход...");
			break;
	}
}

int main(){
	Graph* labyrinth = NULL;
	ERROR err = GOOD;
	unsigned int choice_val;

	labyrinth = createGraph(INITIAL_CAPACITY, &err);
	if (err == BAD_ALLOC){
		print_error_message(err);
		return 1;
	}
	if (err != GOOD){
		print_error_message(err);
		return 1;
	}

	while (err != EXIT){
		err = getUnsignedInt(&choice_val, MENU);
		if (err == EXIT){
			break;
		}
		if (err != GOOD){
			print_error_message(err);
			err = GOOD;
			continue;
		}

		unsigned int x, y, x2, y2;
		CellType type;
		ERROR op_err = GOOD;

		switch (choice_val){
			case 0:
				op_err = getCoordinates(&x, &y, crdX, crdY);
				if (op_err == GOOD){
					op_err = getCellType(&type, "Тип вершины: ");
					if (op_err == GOOD){
						op_err = addVertex(labyrinth, x, y, type);
						if (op_err == GOOD){
							printf("Вершина (%u,%u) добавлена.\n", x, y);
						}
					}
				}
				print_error_message(op_err);
				break;

			case 1:
				printf("\nВведите исходную вершину ребра:\n");
				op_err = getCoordinates(&x, &y, crdX, crdY);
				if (op_err == GOOD){
					printf("\nВведите конечную вершину ребра:\n");
					op_err = getCoordinates(&x2, &y2, crdX, crdY);
					if (op_err == GOOD){
						op_err = addEdge(labyrinth, x, y, x2, y2);
						if (op_err == GOOD){
							printf("Ребро (%u,%u) -> (%u,%u) добавлено.\n", x, y, x2, y2);
						}
					}
				}
				print_error_message(op_err);
				break;

			case 2:
				printf("\nВведите координаты вершины для удаления:\n");
				op_err = getCoordinates(&x, &y, crdX, crdY);
				if (op_err == GOOD){
					op_err = removeVertex(labyrinth, x, y);
					if (op_err == GOOD){
						printf("Вершина (%u,%u) удалена.\n", x, y);
					}
				}
				print_error_message(op_err);
				break;
			case 3:
				printf("\nВведите координаты исходной вершины ребра для удаления:\n");
				op_err = getCoordinates(&x, &y, crdX, crdY);
				if (op_err == GOOD){
					printf("\nВведите координаты конечной вершины ребра для удаления:\n");
					op_err = getCoordinates(&x2, &y2, crdX, crdY);
					if (op_err == GOOD){
						op_err = removeEdge(labyrinth, x, y, x2, y2);
						if (op_err == GOOD){
							printf("Вершина (%u,%u) -> (%u,%u) удалена.\n", x, y, x2, y2);
						}
					}
				}
				print_error_message(op_err);
				break;

			case 4:
				printf("\nВведите координаты вершины для изменения:\n");
				op_err = getCoordinates(&x, &y, crdX, crdY);
				if (op_err == GOOD){
					op_err = getCellType(&type, "Выберите новый тип вершины (0-Пусто, 1-Стена, 2-Вход, 3-Выход): ");
					if (op_err == GOOD){
						op_err = changeVertexData(labyrinth, x, y, type);
						if (op_err == GOOD){
							printf("Вершина (%u,%u) изменена\n", x, y);
						}
					}
				}
				print_error_message(op_err);
				break;

			case 5:
				printGraphAdjList(labyrinth, &op_err);
				print_error_message(op_err);
				break;

			case 6:
				visualizeGraph(labyrinth, &op_err);
				print_error_message(op_err);
				break;

			case 7:
				printf("\nВыберите исходную вершину на поиск выхода:\n");
				op_err = getCoordinates(&x, &y, crdX, crdY);
				if (op_err == GOOD){
					bool reachable_flag = false;
					op_err = isExitReachableBFS(labyrinth, x, y, &reachable_flag);
					if (op_err == GOOD){
						printf("Выход из (%u,%u) %s\n",
							   x, y, reachable_flag ? "есть" : "не существует");
					} else if (op_err == INVALID_INPUT){
						printf("Вершина по координатам должна быть ВХОДОМ\n");
					}
				}
				print_error_message(op_err);
				break;

			case 8:
				printf("\nВведите координаты первой вершины для кратчайшего пути:\n");
				op_err = getCoordinates(&x, &y, crdX, crdY);
				if (op_err == GOOD){
					printf("\nВведите координаты второй вершины для кратчайшего пути:\n");
					op_err = getCoordinates(&x2, &y2, "Exit X: ", "Exit Y: ");
					if (op_err == GOOD){
						int* path = NULL;
						int path_len = 0;
						op_err = shortestPathBellmanFord(labyrinth, x, y, x2, y2, &path, &path_len);
						if (op_err == GOOD){
							if (path_len > 0 && path != NULL){
								printf("Кратчайший путь от (%u,%u) до (%u,%u) имеет длину %d:\n", x, y, x2, y2, path_len -1);
								for (int i = 0; i < path_len; ++i){
									Vertex v = labyrinth->vertices[path[i]];
									printf("%s(%u,%u)", (i == 0 ? "" : " -> "), v.x, v.y);
								}
								printf("\n");
							} else{
								printf("Путь не найден\n");
							}
							free(path);
						} else if (op_err == NOT_FOUND){
							printf("Путь не найден\n", x,y,x2,y2);
							op_err = GOOD;
						}
					}
				}
				print_error_message(op_err);
				break;

			case 9:
				op_err = convertToSinglePath(labyrinth);
				if (op_err == GOOD){
					printf("Минимальное остовное дерево построено\n");
				}
				print_error_message(op_err);
				break;
			default:
				printf(WRONG_INP);
				break;
		}
		if (op_err == BAD_ALLOC){
			err = EXIT;
		} else{
			op_err = GOOD;
		}
		printf("\n");
	}

	freeGraph(labyrinth);
	return 0;
}
