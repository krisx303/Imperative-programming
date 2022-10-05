#include <math.h>
#include <stdio.h>
#include "../../libraries/inout.c"

// Pocz�tek zadania 6.1 /////////////////////
double int_leftrect(double x1, double x2, int n, double (*func)(double))
{
    double dx = (x2-x1)/n;
    double sum = 0.0;
    for(int i = 0; i<n;i++){
        sum += func(x1);
        x1 += dx;
    }
    return dx*sum;
}
// Koniec zadania 6.1 /////////////////////

// Pocz�tek zadania 6.2 /////////////////////
double int_rightrect(double x1, double x2, int n, double (*func)(double))
{
    double dx = (x2-x1)/n;
    double sum = 0.0;
    for(int i = 0; i<n;i++){
        x1 += dx;
        sum += func(x1);
    }
    return dx*sum;
}

double int_midrect(double x1, double x2, int n, double (*func)(double))
{
    double dx = (x2-x1)/n;
    double sum = 0.0;
    x1 += dx/2;
    for(int i = 0; i<n;i++){
        sum+= func(x1);
        x1+=dx;
    }
    return dx*sum;
}

double int_trapez(double x1, double x2, int n, double (*func)(double))
{
    double dx = (x2 - x1) / n;
    double sum = func(x1) + func(x2);
    int i;
    for(i = 1; i < n; i++)
        sum += 2.0*func(x1 + i * dx);
    return  dx * sum / 2.0;
}

typedef double (*int_func)(double, double, int, double (*)());
typedef double (*obj_func)(double);
// Koniec zadania 6.2/////////////////////

// Pocz�tek zadania 6.3 /////////////////////
double double_integr(double x1, double x2, int nx, double y1, double y2, int ny, double (*func)(double, double))
{
    double dx = (x2 - x1) / nx;
    double dy = (y2 - y1) / ny;

    double sum = 0.0, x;

    for(int i = 0; i<ny;i++){
        x = x1;
        for(int j = 0; j<nx;j++){
            sum += func(x, y1);
            x += dx;
        }
        y1 += dy;
    }
    return dx * dy * sum;
}

typedef double (*obj_func_d)(double, double);
// Koniec zadania 6.3 /////////////////////

// Pocz�tek zadania 6.4 /////////////////////
double integr3D(double x1, double x2, int nx, double y1, double y2, int ny, double z1, double z2, int nz, double (*func)(double, double, double))
{
    double dx = (x2 - x1) / nx;
    double dy = (y2 - y1) / ny;
    double dz = (z2 - z1) / nz;

    double sum=0.0, x, y, z;
    z = z1;
    for(int i = 0;i<nz;i++){
        y = y1;
        for(int j = 0;j<ny;j++){
            x = x1;
            for(int k = 0;k<nx;k++){
                sum += func(x, y, z);
                x += dx;
            }
            y += dy;
        }
        z += dz;
    }
    return dx * dy * dz * sum;
}

typedef double (*obj_func_t)(double, double, double);
// Koniec zadania 6.4 /////////////////////

// Pocz�tek zadania 6.5 /////////////////////
double doub_int_bound(double x1, double x2, int nx, double y1, double y2, int ny, int (*boundary)(), double (*func)(double, double))
{
    double dx = (x2 - x1) / nx;
    double dy = (y2 - y1) / ny;

    double sum = 0.0, x, y;

    y = y1;
    for(int i = 0; i<ny;i++){
        x = x1;
        for(int j = 0;j<nx;j++){
            if (boundary(x, y) == 1) sum += func(x, y);
            x += dx;
        }
        y += dy;
    }
    return dx * dy * sum;
}

typedef double (*obj_func_b)(double, double);
typedef int (*obj_bound)(double, double);
// Koniec zadania 6.5 /////////////////////

// Funkcje matematyczne jednej zmiennej do zada� 6.1 i 6.2
double f1(double x)
{
    return x;
}

double f2(double x)
{
    return x*x/2.0;
}

double f3(double x)
{
    return 1.0/x;
}

double f4(double x)
{
    return log10(x);
}
//////////////////////////////////////////////////////////

// Funkcje matematyczne dw�ch zmiennych do zada� 6.3 i 6.5
double f5(double x, double y)
{
    return pow(pow(x, 4) + pow(y, 5), 0.5);
}

double f6(double x, double y)
{
    return 8*sin(x)+pow(5*cos(y),2);
}

double f7(double x, double y)
{
    return 9*sqrt(x+y)-2*sqrt(y);
}
/////////////////////////////////////////////////////

// Funkcje matematyczne trzech zmiennych do zadania 6.4
double f8(double x, double y, double z)
{
    return x+3*y+5*z;
}

double f9(double x, double y, double z)
{
    return 8*sin(x)+pow(5*cos(y),2)+2*z;
}
/////////////////////////////////////////////////////

// Funkcje podca�kowe do zadania 6.5 //////////////////

double fb1(double x, double y)
{
    return x*(y*y);
}

double fb2(double x, double y)
{
    return (x*x)+(y*y);
}

double fb3(double x, double y)
{
    return 1/(pow(1 - x*x - y*y,2));
}

/////////////////////////////////////////////////////

// Funkcje boundary do zadania 6.5 //////////////////

int b1(double x, double y)
{
    if ( x*x + y*y <= 4 && x>=0 ) return 1;
    return 0;
}

int b2(double x, double y)
{
    double tmpr = x*x + y*x - 2*y;
    if ( tmpr >= 1 || tmpr <= 0) return 1;
    return 0;
}

int b3(double x, double y)
{
    double tmpr = x*x + y*y;
    if (tmpr <= x && tmpr <= y) return 1;
    return 0;
}

/////////////////////////////////////////////////////

int main(void)
{
    int to_do, n, nx, ny, nz;
    double ic, x1, x2, y1, y2, z1, z2;
    if (TEST) printf("Wpisz numer testu [1, 5]: ");
    scanf("%d", &to_do);
    switch (to_do)
    {
        case 1: // 6.1
            if (TEST) printf("Zad. 6.1 - Ca�ka jednowymiarowa (wsk. do funkcji, met. prostok�t lewostr.)\n");
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w [x1 x2 n]: ");
            scanf("%lf %lf %d", &x1, &x2, &n);

            if (TEST==1) printf("(x^2)/2: ");
            printf("%.2lf ", int_leftrect(x1, x2, n, f2));
            if (TEST==1) printf("\n");

            break;
        case 2: // 6.2
            if (TEST) printf("Zad. 6.2 - Ca�ki jednowymiarowe (wsk. do funkcji, met. prostok�t, trapez)\n");
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w [x1 x2 n]: ");
            scanf("%lf %lf %d", &x1, &x2, &n);

            int_func intf[] = {int_rightrect, int_midrect, int_trapez};
            obj_func objf[] = { f1, f2, f3, f4 };
            const char *int_func_names[] = {"rightrect", "midrect", "trapez"};
            const char *obj_func_names[] = {"x", "(x^2)/2", "1/x", "log10(x)"};

            for(int j=0; j < 4; j++)
            {
                if (TEST==1) printf("%10s --> \n", obj_func_names[j]);
                for(int i=0; i < 3 ; i++) // P�tla wywo�uj�ca
                {
                    ic = (*intf[i])(x1, x2, n, objf[j]); //wskazanie funkcji ca�kuj�cej z parametrami zawieraj�cymi wskazanie funkcji do ca�kowania
                    if (TEST==1) printf("%10s: ", int_func_names[i]);
                    printf("%.2lf ", ic);
                    if (TEST==1) printf("\n");
                }
            }
            break;
        case 3: // 6.3
            if (TEST) printf("Zad. 6.3 - Ca�ka podw�jna (wsk. do funkcji)\n");
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w x [x1 x2 nx]: ");
            scanf("%lf %lf %d", &x1, &x2, &nx);
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w y [y1 y2 ny]: ");
            scanf("%lf %lf %d", &y1, &y2, &ny);

            obj_func_d objfd[] = {f5, f6, f7};
            const char *obj_func_names_d[] = {"sqrt(x^4+y^5)", "8sin(x)+(5cos(y))^2", "9sqrt(x+y)-2sqrt(y)"};
            for(int i=0; i < 3; ++i)
            {
                ic=double_integr(x1, x2, nx, y1, y2, ny, objfd[i]);
                if (TEST==1) printf("%10s: ", obj_func_names_d[i]);
                printf("%.2lf ", ic);
                if (TEST==1) printf("\n");
            }
            break;
        case 4: // 6.4
            if (TEST) printf("Zad. 6.4 - Ca�ka potr�jna (wsk. do funkcji)\n");
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w x [x1 x2 nx]: ");
            scanf("%lf %lf %d", &x1, &x2, &nx);
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w y [y1 y2 ny]: ");
            scanf("%lf %lf %d", &y1, &y2, &ny);
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w z [z1 z2 nz]: ");
            scanf("%lf %lf %d", &z1, &z2, &nz);

            obj_func_t objft[] = {f8, f9};
            const char *obj_func_names_t[] = {"x+3y+5z", "8sin(x)+(5cos(y))^2+2z"};
            for(int i=0; i < 2; ++i)
            {
                ic=integr3D(x1, x2, nx, y1, y2, ny, z1, z2, nz, objft[i]);
                if (TEST==1) printf("%10s: ", obj_func_names_t[i]);
                printf("%.2lf ", ic);
                if (TEST==1) printf("\n");
            }
            break;
        case 5: // 6.5
            if (TEST) printf("Zad. 6.5 - Ca�ka podw�jna (struktury -> prosta budowa ca�ki wielokrotnej)\n");
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w x [x1 x2 nx]: ");
            scanf("%lf %lf %d", &x1, &x2, &nx);
            if (TEST) printf("Wpisz zakres ca�kowania i liczba krok�w y [y1 y2 ny]: ");
            scanf("%lf %lf %d", &y1, &y2, &ny);

            obj_func_b objfb[] = {fb1, fb2, fb3};
            obj_bound objb[]= {b1, b2, b3};
            const char *obj_func_names_b[] = {"xy^2", "x^2+y^2", "1/(1-x^2-y^2)^2"};
            const char *obj_bound_names_b[] = {"x^2+y^2<=4, x>=0", "1<=x^2+x*y-2y<=0", "x^2+y^2<=x, x^2+y^2<=y"};
            for(int i=0; i < 3; ++i)
            {
                ic=doub_int_bound(x1, x2, nx, y1, y2, ny, objb[i], objfb[i]);
                if (TEST==1) printf("%10s, gdy %10s: ", obj_func_names_b[i], obj_bound_names_b[i]);
                printf("%.2lf ", ic);
                if (TEST==1) printf("\n");
            }
            break;
        default:
            printf("Numer testu spoza zakresu [1, 5] %d", to_do);
            break;
    }
    return 0;
}
