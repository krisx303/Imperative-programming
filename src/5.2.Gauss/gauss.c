#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../../libraries/inout.c"
#define SIZE 40

void read_vec(double x[], int n) {
  for(int i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(double x[], int n) {
  for(int i=0; i<n; ++i) printf("%.4f ",x[i]);
  printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
  for(int i=0; i<m; ++i) {
    for(int j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
  }
}

void print_mat(double A[][SIZE], int m, int n) {
  for(int i=0; i<m; ++i) {
    for(int j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
    printf("\n");
  }
}

void print_mat_ind(double A[][SIZE], int m, int n,int indicies[]) {
  for(int i=0; i<m; ++i) {
    for(int j=0; j<n; ++j)  printf("%.4f ",A[indicies[i]][j]);
    printf("\n");
  }
  printf("\n");
}

// A mxp, B pxn
// definicja powinna byc juz napisana w zadaniu 5.1.1
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
}

// Funkcja moze byc przydatna do sprawdzania poprawnosci rozwiazania Ax=b
void mat_vec_product(double A[][SIZE], double b[], double Ab[], int m, int n) {
  int i, k;
  double sum;
  for(i=0; i<m; ++i) {
    for(k=0, sum=0.0; k<n; ++k) sum += A[i][k] * b[k];
    Ab[i] = sum;
  }
  return;
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps)
{
    int rows[n];
    for (int i = 0; i < n; i++){
        rows[i] = i;
        x[i] = b[i];
    }
    int curr = 0;
    double w = 1.0;
    while (curr < n){
        double max_val = fabs(A[rows[curr]][curr]);
        int max_id = curr;
        for (int i = curr + 1; i < n; i++){
            double actual = fabs(A[rows[i]][curr]);
            if (actual > max_val){
                max_val = actual;
                max_id = i;
            }
        }
        if (max_id != curr){
            int tmp = rows[curr];
            rows[curr] = rows[max_id];
            rows[max_id] = tmp;
            w = -w;
        }
        if (fabs(A[rows[curr]][curr]) < eps)
            return 0.0;
        for (int i = curr + 1; i < n; i++){
            double q = A[rows[i]][curr] / A[rows[curr]][curr];
            for (int j = curr; j < n; j++){
                A[rows[i]][j] -= A[rows[curr]][j]*q;
            }
            x[rows[i]] -= x[rows[curr]]*q;
        }
        curr += 1;
    }
    for (int i = 0; i < n; i++)
        w *= A[rows[i]][i];
    int flag = 0;
    for(int i = 0;i<n;i++){
        if(b[i] != 0){
            flag = 1;
        }
    }
    if(flag){
        for(int i = n-1;i>-1;i--){
            double k = 0;
            for(int j = i+1;j<n;j++){
                k += A[rows[i]][j]*x[rows[j]];
            }
            x[rows[i]] = (x[rows[i]] - k)/A[rows[i]][i];
        }
        double xcopy[n];
        for(int i = 0;i<n;i++){
            xcopy[i] = x[rows[i]];
        }
        for(int i = 0;i<n;i++){
            x[i] = xcopy[i];
        }
    }
    return w;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], const int n, const double eps) {
    double COPY[n][n];
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            COPY[i][j] = A[i][j];
        }
    }
    double empty[n];
    for(int i = 0;i<n;i++) empty[i]=0;
    double w = gauss(A, empty, empty, n, eps);
    //printf("%lf", w);
    double C[n-1][SIZE];
    for(int row = 0;row<n;row++){
        for(int col = 0;col<n;col++){
            //printf("(%d,%d)\n", row, col);
            int ii = 0;
            for(int i = 0;i<n;i++){
                if(i == row) continue;
                int jj = 0;
                for(int j =0;j<n;j++){
                    if(j == col) continue;
                    C[ii][jj] = COPY[i][j];
                    jj++;
                }
                ii++;
            }
            // liczenie wyznacznika macierzy n-1 na n-1
           //print_mat(C, n-1, n-1);
            double ww = gauss(C, empty, empty, n-1, eps);
            ww = pow(-1, row+col) * ww;
            B[col][row] = ww/w;
        }
    }
    return w;
}

int main(void) {

  double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
  double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m,n,p,q;
	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);

	switch (to_do) {
    case 1:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      if(TEST) printf("Wpisz wektor b: ");
      read_vec(b,n);
      det = gauss(A, b, x, n, eps);
      printf("%.4f\n",det);
      if(det) print_vec(x,n);
      break;
    case 2:
      if(TEST) printf("Wpisz rozmiar macierzy n = ");
      scanf("%d",&n);
      if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
      read_mat(A,n,n);
      det = matrix_inv(A,B,n,eps);
      printf("%.4f\n",det);
      if(det) print_mat(B,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
