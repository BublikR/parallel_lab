#include <iostream>
#include <cmath>
#include <cstdlib>
#include <omp.h>

using namespace std;

void matrix(double **a, double *y, int n) // Вывод матрицы
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      cout << a[i][j];
      if (j < n - 1) cout << "\t";
    }
    cout << "\t| " << y[i] << endl;
  }
}

double * gauss(double **a, double *y, int n)
{
  double *x, max;
  int i, j, k, index;
  const double eps = 0.000001;  // точность
  x = new double[n];
  k = 0;
  while (k < n)
  { // Поиск строки с максимальным a[i][k]
    max = abs(a[k][k]); 
    index = k;
    #pragma omp parallel for shared(a, max, index) private(i) 
    for (i = k + 1; i < n; i++)
    {
      if (abs(a[i][k]) > max)
      {
        max = abs(a[i][k]);
        index = i;
      }
    }
    
    if (max < eps)
    { // проверка на нулевой столбец
      cout << "Рішення отримати неможливо. Стовпчик ";
      cout << index << " матриці нульовий." << endl;
      return 0;
    }
    #pragma omp parallel for private(j) shared(a)
    for (j = 0; j < n; j++) // Перестановка строк
    {
      double temp = a[k][j];
      a[k][j] = a[index][j];
      a[index][j] = temp;
    }
    double temp = y[k];
    y[k] = y[index];
    y[index] = temp;
   
    for (i = k; i < n; i++) // Нормализация уравнений
    {
      double temp = a[i][k];
      if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
      #pragma omp parallel for shared(a) private(j)
      for (j = 0; j < n; j++)
        a[i][j] = a[i][j] / temp;
      y[i] = y[i] / temp;
      
      if (i == k)  continue; // уравнение не вычитать само из себя
      #pragma omp parallel for shared(a) private(j)
      for (j = 0; j < n; j++)
        a[i][j] = a[i][j] - a[k][j];
      y[i] = y[i] - y[k];
    }
    k++;
  } // end while (k < n)
  
  for (k = n - 1; k >= 0; k--) // Обратная подстановка
  {
    x[k] = y[k];
    #pragma omp parallel for shared(y, a, x) private(i)
    for (i = 0; i < k; i++)
      y[i] = y[i] - a[i][k] * x[k];
  }
  return x;
}
int main(int argc, char* argv[])
{
  int n; // количество уравнений
  double time;
  char ans;
  double **a, *y, *x;

  if (argc == 1) n = 4000;
  else if (argc == 2) n = atoi(argv[1]);
  else {
    cout << "Error! Too many arguments." << endl;
    return 0;
  }

  a = new double*[n];
  y = new double[n];
  for (int i = 0; i < n; i++)
  {
    a[i] = new double[n];
    for (int j = 0; j < n; j++)
    {
      a[i][j] = rand() % 10000;
    }
  }
  for (int i = 0; i < n; i++)
  {
    y[i] = rand() % 10000;
  }
  /*cout << "Бажаєте побачити матрицю?[y/n]" << endl;
  cin >> ans;
  if (ans == 'y') matrix(a, y, n);*/
  time = omp_get_wtime();
  x = gauss(a, y, n);
  time = omp_get_wtime() - time;
  for (int i = 0; i < n; i++)
    cout << "x[" << i << "] = " << x[i] << endl;
  cout << "Time is " << time << endl;
  return 0;
}

