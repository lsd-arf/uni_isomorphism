#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// если difference[i][j] != 0 или a[i][j] то неизоморфны
// в контексте данной программы:
// полный граф/матрица - который нужно построить
// неполный граф/матрица - из которого нужно построить

int **full;							   // полная матрица
int **unfull;						   // неполная матрица размера k (n)
int **unfull2;						   // клон для unfull
int n;								   // размер полной матрицы
int k;								   // размер неполной матрицы
int *p;								   // перестановки
int **difference;					   // разница матриц
int num = 1;						   // номер перестановки
string file_vertexes = "vertexes.txt"; // вершины
string file_ribs = "ribs.txt";		   // рёбра
bool f_alg = false;					   // флаг для проверки изоморфизма графов для alg

// инициализация массива перестановок
// и матрицы разности
void init_somedata()
{
	p = new int[n];
	difference = new int *[n];
	for (int i = 0; i < n; i++)
	{
		p[i] = i;
		difference[i] = new int[n];
	}
}

// матрица разности
void func_diff()
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			// разность полной и неполной матриц
			difference[i][j] = full[i][j] - unfull[p[i]][p[j]];
}

// клонирование матрицы unfull2
void clone_unfull()
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			unfull[i][j] = unfull2[i][j];
}

// чтение матриц из файлов
void get_matrix(string &g1, string &g2)
{
	ifstream fin1(g1);
	ifstream fin2(g2);
	fin1 >> n;
	fin2 >> k;
	full = new int *[n];
	unfull = new int *[n];
	unfull2 = new int *[n];

	for (int i = 0; i < n; i++)
	{
		full[i] = new int[n];
		unfull[i] = new int[n];
		unfull2[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			fin1 >> full[i][j];
			unfull[i][j] = 0;
			unfull2[i][j] = 0;
		}
	}

	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
		{
			fin2 >> unfull[i][j];
			unfull2[i][j] = unfull[i][j];
		}

	fin1.close();
	fin2.close();
}

// вывод матрицы
void show(int **matrix)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << matrix[i][j] << "\t";
		cout << endl;
	}
}

// меняет элементы местами
void swap(int i, int j)
{
	//cout << "\t" << i << "\t" << j;
	int s = p[i];
	p[i] = p[j];
	p[j] = s;
}

// поиск перестановки
bool transposition()
{
	int j = n - 2;
	while (j != -1 && p[j] >= p[j + 1])
		j--;

	// если перестановок больше нет
	if (j == -1)
		return false;
	int k = n - 1;

	while (p[j] >= p[k])
		k--;
	swap(j, k);

	// сортировка оставшейся части последовательности
	int l = j + 1, r = n - 1;
	while (l < r)
		swap(l++, r--);
	return true;
}

// вывод перестановки
void Print()
{
	cout << num++ << ": ";
	for (int i = 0; i < n; i++)
		cout << p[i] << " ";
	cout << endl;
}

// построение матрицы разности
void alg()
{
	func_diff();
	f_alg = true;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// если разность < 0, то графы неизоморфны - false
			if (difference[i][j] < 0)
				f_alg = false;
			//return false;
		}
	}
	//return true;
}

// поиск максимумов
void searching_max(int &max_full, int &max_unfull)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (full[i][j] > max_full)
				max_full = full[i][j];
			if (unfull[i][j] > max_unfull)
				max_unfull = unfull[i][j];
		}
}

// блок, при котором f_alg = true
void f_alg_true(int &count, bool &f)
{
	clone_unfull();
	f = true;
	ofstream vout(file_vertexes);
	ofstream rout(file_ribs);

	// добавленные вершины
	for (int i = 0; i < n - k; i++)
		vout << i + k << endl;

	for (int i = 0; i < n && f; i++)
	{
		for (int j = 0; j < n && f; j++)
		{
			// если элемент матрицы разности не равен 0 и элементу a[i][j]
			if (difference[i][j] != 0 &&
				(difference[i][j] != full[i][j] ||
				 unfull[p[i]][p[j]] != 0 && difference[i][j] == full[i][j]))
				// то графы не могут быть изоморфны - false
				f = false;
		}
	}

	// иначе составляем изоморфный граф
	if (f)
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (difference[i][j] != 0)
				{
					count++;
					// добавленные рёбра
					rout << p[i] << "\t" << p[j] << endl;
					// составляем изоморфный граф
					unfull[p[i]][p[j]] += difference[i][j];
				}
			}
		}

	vout.close();
	rout.close();
}

// блок f = true
void f_true(int &count)
{
	ifstream vin(file_vertexes);
	ifstream rin(file_ribs);
	cout << "---------------------------------------------\n";

	cout << "Matrix of difference\n\n";
	show(difference);

	cout << "---------------------------------------------\n";

	cout << "Isomorphic graph\n\n";
	show(unfull);

	cout << "---------------------------------------------\n";
	string s_vertex;
	string s_i, s_j;
	cout << "Vertexes: ";
	while (n - k != 0)
	{
		k++;
		vin >> s_vertex;
		cout << s_vertex << "  ";
	}

	cout << "\nRibs: ";
	while (count != 0)
	{
		count--;
		rin >> s_i >> s_j;
		cout << "(" << s_i << "," << s_j << ")  ";
	}
	cout << "\n---------------------------------------------\n";
	vin.close();
	rin.close();
}

int main()
{
	system("cls");
	string g1; // полный граф
	string g2; // неполный граф
	cout << "First graph: ";
	cin >> g1;
	cout << "Second graph: ";
	cin >> g2;
	system("cls");

	get_matrix(g1, g2);

	init_somedata();

	int max_full = 0,	// максимальное расстояние между вершинами полного графа
		max_unfull = 0; // максимальное расстояние между вершнами неполного графа

	searching_max(max_full, max_unfull);

	// если максимальный элемент полного меньше, чем
	// максимальный элемент неполного - графы не изоморфны
	if (max_full < max_unfull)
		cout << "Graphs can't be isomorphic (1-123)";
	// иначе продолжается выполнение блока
	else
	{
		// пока матрицы не "равны" или перестановки не закончились
		alg();
		while (!f_alg && transposition())
			alg();

		// не изоморфны - выполнение программы завершается
		if (!f_alg)
			cout << "Graphs can't be isomorphic (2-132)";
		// иначе продолжается выполнение блока
		else
		{
			int count = 0;	// количество добавленных рёбер
			bool f = false; // флаг для проверки изоморфизма графов

			f_alg_true(count, f);
			if (!f)
				alg();

			while (!f && transposition())
			{
				f_alg_true(count, f);
				if (!f)
					alg();
			}

			// не изоморфны - выполнение программы завершается
			if (!f)
				cout << "Graphs can't be isomorphic (3-213)";
			// иначе продолжается выполнение блок-функции
			else
				f_true(count);
		}
	}

	return 0;
}