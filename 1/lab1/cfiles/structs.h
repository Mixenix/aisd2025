typedef struct Line {
	int n; // количество элементов в строке матрицы
	double *a; // массив элементов
} Line;


typedef struct Matrix {

int lines; // количество строк матрицы
Line *matr; // массив строк матрицы

} Matrix;
