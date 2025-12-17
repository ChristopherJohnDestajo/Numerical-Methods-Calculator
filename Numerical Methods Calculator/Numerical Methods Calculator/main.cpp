#include <iostream>
#include<sstream>
#include <iomanip>
#include <cmath>
#include <string>
#include "tinyexpr.h"
#include <vector>

using namespace std;




/* =============================
   FUNCTION EVALUATOR
   ============================= */
double f(string equation, double x) {
    const char* expr = equation.c_str();
    te_variable vars[] = { {"x", &x} };
    int err;
    te_expr* compiled = te_compile(expr, vars, 1, &err);
    if (!compiled) {
        cout << "Error at position: " << err << endl;
        return NAN;
    }

    double result = te_eval(compiled);
    te_free(compiled);
    return result;
}

/* =============================
   NUMERICAL DIFFERENTIATION
   ============================= */
void numericalDifferentiation() {
    string eq;
    double x, h;
    int order;

    cout << "\nNUMERICAL DIFFERENTIATION\n";
    cout << "1 - First Derivative\n";
    cout << "2 - Second Derivative\n";

    cout << "Choice: ";
    cin >> order;

    cout << "\nEnter equation f(x): ";
    cin.ignore();
    getline(cin, eq);

    cout << "Enter x: ";
    cin >> x;
    cout << "Enter h: ";
    cin >> h;

    double fx = f(eq, x);
    double fxh = f(eq, x + h);
    double fxmh = f(eq, x - h);
    double fx2h = f(eq, x + 2 * h);
    double fxm2h = f(eq, x - 2 * h);

    cout << fixed << setprecision(6);



    if (order == 1) {
        /* ================= FIRST DERIVATIVE ================= */
        cout << "\nTable of x and f(x)\n";
        cout << "--------------------------------\n";
        cout << setw(12) << "x" << setw(15) << "f(x)\n";
        cout << "--------------------------------\n";
        cout << setw(12) << (x - h) << setw(15) << fxmh << "\n";
        cout << setw(12) << x << setw(15) << fx << "\n";
        cout << setw(12) << (x + h) << setw(15) << fxh << "\n";

        cout << "--------------------------------\n";

        cout << "\nFORWARD FINITE DIFFERENCE\n";
        cout << "Formula:\n";
        cout << "f'(x) ≈ [f(x+h) - f(x)] / h\n";
        cout << "Solution:\n";
        cout << "f'(" << x << ") ≈ (" << fxh << " - " << fx << ") / " << h << endl;
        cout << "Result = " << (fxh - fx) / h << endl;

        cout << "\nBACKWARD FINITE DIFFERENCE\n";
        cout << "Formula:\n";
        cout << "f'(x) ≈ [f(x) - f(x-h)] / h\n";
        cout << "Solution:\n";
        cout << "f'(" << x << ") ≈ (" << fx << " - " << fxmh << ") / " << h << endl;
        cout << "Result = " << (fx - fxmh) / h << endl;

        cout << "\nCENTRAL FINITE DIFFERENCE\n";
        cout << "Formula:\n";
        cout << "f'(x) ≈ [f(x+h) - f(x-h)] / (2h)\n";
        cout << "Solution:\n";
        cout << "f'(" << x << ") ≈ (" << fxh << " - " << fxmh << ") / (2 × " << h << ")\n";
        cout << "Result = " << (fxh - fxmh) / (2 * h) << endl;
    }
    else if (order == 2) {
        /* ================= SECOND DERIVATIVE ================= */
        cout << "\nTable of x and f(x)\n";
        cout << "--------------------------------\n";
        cout << setw(12) << "x" << setw(15) << "f(x)\n";
        cout << "--------------------------------\n";
        cout << setw(12) << (x - 2 * h) << setw(15) << fxm2h << "\n";
        cout << setw(12) << (x - h) << setw(15) << fxmh << "\n";
        cout << setw(12) << x << setw(15) << fx << "\n";
        cout << setw(12) << (x + h) << setw(15) << fxh << "\n";
        cout << setw(12) << (x + 2 * h) << setw(15) << fx2h << "\n";
        cout << "--------------------------------\n";

        cout << "\nSECOND DERIVATIVE (CENTRAL DIFFERENCE)\n";
        cout << "Formula:\n";
        cout << "f''(x) ≈ [f(x+h) - 2f(x) + f(x-h)] / h²\n";
        cout << "Solution:\n";
        cout << "f''(" << x << ") ≈ (" << fxh << " - 2(" << fx << ") + "
            << fxmh << ") / " << h * h << endl;
        cout << "Result = "
            << (fxh - 2 * fx + fxmh) / (h * h) << endl;

        cout << "\nSECOND DERIVATIVE (FORWARD DIFFERENCE)\n";
        cout << "Formula:\n";
        cout << "f''(x) ≈ [f(x+2h) - 2f(x+h) + f(x)] / h²\n";
        cout << "Solution:\n";
        cout << "f''(" << x << ") = ("
            << fx2h << " - 2(" << fxh << ") + " << fx
            << ") / (" << h << ")^2\n";

        cout << "Result = "
            << (fx2h - 2 * fxh + fx) / (h * h) << endl;

        cout << "\nSECOND DERIVATIVE (BACKWARD DIFFERENCE)\n";
        cout << "Formula:\n";
        cout << "f''(x) ≈ [f(x) - 2f(x-h) + f(x-2h)] / h²\n";
        cout << "Solution:\n";
        cout << "f''(" << x << ") = ("
            << fx << " - 2(" << fxmh << ") + " << fxm2h
            << ") / (" << h << ")^2\n";

        cout << "Result = "
            << (fx - 2 * fxmh + fxm2h) / (h * h) << endl;
    }
    else {
        cout << "Invalid choice.\n";
    }
}


/* =============================
   TRAPEZOIDAL RULE
   ============================= */
double trapezoidal(string eq, double a, double b, int n) {
    double h = (b - a) / n;
    double sum = f(eq, a) + f(eq, b);

    for (int i = 1; i < n; i++)
        sum += 2 * f(eq, a + i * h);

    return (h / 2.0) * sum;

}

double parseLimit(const string& s) {
    // try plain number first
    try {
        size_t idx;
        double val = stod(s, &idx);
        if (idx == s.size())
            return val;   // pure number
    }
    catch (...) {}

    // otherwise evaluate expression (pi, pi/2, etc.)
    return f(s, 0);
}



/* =============================
   ASCII PLOTTING FOR TRAPEZOIDAL
   ============================= 
   */

string formatPiLabel(double x) {
    const double PI = acos(-1.0);
    const double EPS = 1e-6;

    double k = x / PI;

    for (int denom = 1; denom <= 16; denom++) {
        double num = k * denom;
        if (fabs(num - round(num)) < EPS) {
            int n = (int)round(num);

            if (n == 0) return "0";
            if (denom == 1) {
                if (n == 1) return "pi";
                return to_string(n) + "pi";
            }
            if (n == 1) return "pi/" + to_string(denom);
            return to_string(n) + "pi/" + to_string(denom);
        }
    }

    // ✅ FALLBACK — numeric value
    ostringstream out;
    out << fixed << setprecision(2) << x;
    return out.str();
}

void plotTrapezoidal(string eq, double a, double b, int n) {
    const int HEIGHT = 20;
    const int WIDTH_PER_SEG = 8;

    double h = (b - a) / n;

    vector<double> y(n + 1);
    for (int i = 0; i <= n; i++) {
        double xi = a + i * h;
        y[i] = f(eq, xi);
    }
    double ymin = y[0], ymax = y[0];
    for (double v : y) {
        ymin = min(ymin, v);
        ymax = max(ymax, v);
    }
    if (fabs(ymax - ymin) < 1e-9) ymax = ymin + 1.0;


    double scale = HEIGHT / (ymax - ymin);
    vector<int> hgt(n + 1);
    for (int i = 0; i <= n; i++) {
        hgt[i] = (int)((y[i] - ymin) * scale);
    }




    cout << "\nTRAPEZOIDAL VISUALIZATION\n\n";

    for (int row = HEIGHT; row >= 0; row--) {
        double level = ymin + row / scale;


        for (int i = 0; i <= n; i++) {
            // vertical partition
            cout << "|";

            // trapezoid top
            if (i < n) {
                double avg = (y[i] + y[i + 1]) / 2.0;
                for (int k = 0; k < WIDTH_PER_SEG - 1; k++) {
                    if (avg >= level) cout << "-";
                    else cout << " ";
                }
            }
        }
        cout << "\n";
    }

    // x-axis
    for (int i = 0; i <= n; i++) {
        cout << "+";
        for (int k = 0; k < WIDTH_PER_SEG - 1; k++) cout << "-";
    }
    cout << "\n";

    // x labels (π-formatted)
    for (int i = 0; i <= n; i++) {
        double xi = a + i * h;
        cout << setw(WIDTH_PER_SEG) << formatPiLabel(xi);
    }
    cout << "\n";

}

void trapezoidalMethod() {
    const double PI = acos(-1.0);   // 3.141593 internally

    string eq;
    double a, b;
    int n;

    cout << "\nTRAPEZOIDAL RULE\n";
    cout << "===============================\n";

    cout << "Enter equation f(x): ";
    cin.ignore();
    getline(cin, eq);

    string a_str, b_str;
    

    cout << "Enter lower limit a: ";
    cin >> a_str;
    a = parseLimit(a_str);
   


    cout << "Enter upper limit b: ";
    cin >> b_str;
   
    b = parseLimit(b_str);

    cout << "Enter number of subintervals n: ";
    cin >> n;

    double h = (b - a) / n;

    cout << fixed << setprecision(6);

    /* -----------------------------
       DISPLAY GENERAL FORMULA
       ----------------------------- */
    cout << "\nFormula:\n";
    cout << "J =  f(x) dx ≈ h [ 1/2 f(a) + f(x1) + ... + f(xn-1) + 1/2 f(b) ]\n";

   
    plotTrapezoidal(eq, a, b, n);

    /* -----------------------------
       SUBSTITUTED FORMULA
       ----------------------------- */
    cout << "\nSolution:\n";
    cout << "J = " << h << " [\n";
    cout << "    1/2 f(" << a << ")";

    for (int i = 1; i < n; i++) {
        cout << " + f(" << a + i * h << ")";
    }

    cout << " + 1/2 f(" << b << ")\n";
    cout << "]\n";

    /* -----------------------------
       FINAL RESULT
       ----------------------------- */

    double result = trapezoidal(eq, a, b, n);
    cout << "\nJ = " << result << "\n";
}

/* =============================
   SIMPSON'S 1/3 RULE
   ============================= */
double simpson13(string eq, double a, double b, int n) {
    if (n % 2 != 0) return NAN;

    double h = (b - a) / n;
    double sum = f(eq, a) + f(eq, b);

    for (int i = 1; i < n; i++) {
        if (i % 2 == 0)
            sum += 2 * f(eq, a + i * h);
        else
            sum += 4 * f(eq, a + i * h);
    }
    return (h / 3) * sum;
}

/* =============================
   SIMPSON'S 3/8 RULE
   ============================= */
double simpson38(string eq, double a, double b, int n) {
    if (n % 3 != 0) return NAN;

    double h = (b - a) / n;
    double sum = f(eq, a) + f(eq, b);

    for (int i = 1; i < n; i++) {
        if (i % 3 == 0)
            sum += 2 * f(eq, a + i * h);
        else
            sum += 3 * f(eq, a + i * h);
    }
    return (3 * h / 8) * sum;
}
void plotRombergLevel(string eq, double a, double b, int k) {
    int n = pow(2, k);
    double h = (b - a) / n;

    const int HEIGHT = 15;
    const int WIDTH = 6;

    vector<double> y(n + 1);
    for (int i = 0; i <= n; i++) {
        double x = a + i * h;
        y[i] = f(eq, x);
    }

    double ymin = *min_element(y.begin(), y.end());
    double ymax = *max_element(y.begin(), y.end());
    if (fabs(ymax - ymin) < 1e-9) ymax = ymin + 1.0;

    double scale = HEIGHT / (ymax - ymin);

    cout << "\n@ k = " << k
        << "   n = 2^" << k << " = " << n
        << "   h = " << h << "\n";

    for (int row = HEIGHT; row >= 0; row--) {
        double level = ymin + row / scale;

        for (int i = 0; i < n; i++) {
            cout << "|";
            double avg = (y[i] + y[i + 1]) / 2.0;

            for (int w = 0; w < WIDTH; w++) {
                if (avg >= level) cout << "-";
                else cout << " ";
            }
        }
        cout << "|\n";
    }

    // x-axis
    for (int i = 0; i < n; i++) {
        cout << "+";
        for (int w = 0; w < WIDTH; w++) cout << "-";
    }
    cout << "+\n";

    // x labels
    // x labels (π-formatted)
    for (int i = 0; i <= n; i++) {
        double xi = a + i * h;
        cout << setw(WIDTH + 1) << formatPiLabel(xi);
    }
    cout << "\n";

}
/* =============================
   DISPLAY TRAPEZOIDAL SOLUTION
   ============================= */
void displayTrapezoidalSolution(string eq, double a, double b, int n) {
    double h = (b - a) / n;

    cout << "\n∫[" << a << "," << b << "] f(x) dx\n";
    cout << "J = h [ 1/2 f(a) + f(x1) + ... + 1/2 f(b) ]\n";
    cout << "h = (" << b << " - " << a << ") / " << n
        << " = " << h << "\n\n";

    cout << "J = " << h << " [ ";

    cout << "1/2 f(" << a << ")";

    for (int i = 1; i < n; i++) {
        cout << " + f(" << a + i * h << ")";
    }

    cout << " + 1/2 f(" << b << ") ]\n";

    double result = trapezoidal(eq, a, b, n);
    cout << "\nJ = " << fixed << setprecision(6) << result << "\n";
}

/* =============================
   ROMBERG INTEGRATION
   ============================= */
void romberg(string eq, double a, double b) {
    int kmax;

    cout << "\nEnter maximum Romberg level k: ";
    cin >> kmax;

    vector<vector<double>> R(kmax + 1, vector<double>(kmax + 1, 0.0));

    cout << fixed << setprecision(6);

    /* -----------------------------
       BUILD ROMBERG WITH GRAPHS
       ----------------------------- */
    for (int k = 0; k <= kmax; k++) {

        // GRAPH FOR THIS LEVEL
        plotRombergLevel(eq, a, b, k);

        int n = pow(2, k);
        R[k][0] = trapezoidal(eq, a, b, n);

        displayTrapezoidalSolution(eq, a, b, n);


        for (int j = 1; j <= k; j++) {
            R[k][j] = R[k][j - 1]
                + (R[k][j - 1] - R[k - 1][j - 1])
                / (pow(4, j) - 1);
        }


    }

    /* -----------------------------
       DISPLAY ROMBERG TABLE
       ----------------------------- */
    cout << "\nROMBERG INTEGRATION TABLE\n";
    cout << "-------------------------------------------------\n";
    cout << setw(8) << "k/j";

    for (int j = 0; j <= kmax; j++)
        cout << setw(14) << j;

    cout << "\n-------------------------------------------------\n";

    for (int i = 0; i <= kmax; i++) {
        cout << setw(8) << i;
        for (int j = 0; j <= i; j++) {
            cout << setw(14) << R[i][j];
        }
        cout << "\n";
    }

    cout << "-------------------------------------------------\n";
    cout << "\nFinal Answer  = "
        << R[kmax][kmax] << "\n";
}

/* =============================
SIMPSON'S 1/3 RULE (AUTO)
============================= */
void simpson13_auto(string eq, double a, double b) {
    double x0 = a;
    double x2 = b;
    double x1 = (a + b) / 2.0;

    double f0 = f(eq, x0);
    double f1 = f(eq, x1);
    double f2 = f(eq, x2);

    cout << fixed << setprecision(6);

    cout << "\nSIMPSON'S 1/3 RULE\n";
    cout << "Since x0 = a = " << a
        << " and x2 = b = " << b
        << ", the middle term x1 =\n";
    cout << "(" << b << " - " << a << ") / 2 = " << x1 << "\n\n";

    cout << "Evaluating the functions, we have\n";
    cout << "f(x0) = f(" << x0 << ") = " << f0 << "\n";
    cout << "f(x1) = f(" << x1 << ") = " << f1 << "\n";
    cout << "f(x2) = f(" << x2 << ") = " << f2 << "\n\n";

    double result = (b - a) * (f0 + 4 * f1 + f2) / 6.0;

    cout << "I = (" << b << " - " << a << ")\n";
    cout << "    [" << f0 << " + 4(" << f1 << ") + "
        << f2 << "] / 6\n";

    cout << "\nI = " << result << "\n";
}

/* =============================
   SIMPSON'S 3/8 RULE (AUTO)
   ============================= */
void simpson38_auto(string eq, double a, double b) {
    double h = (b - a) / 3.0;

    double x0 = a;
    double x1 = a + h;
    double x2 = a + 2 * h;
    double x3 = b;

    double f0 = f(eq, x0);
    double f1 = f(eq, x1);
    double f2 = f(eq, x2);
    double f3 = f(eq, x3);

    cout << fixed << setprecision(6);

    cout << "\nSIMPSON'S 3/8 RULE\n";
    cout << "Since x0 = a = " << a
        << " and x3 = b = " << b << ", the middle terms are\n";
    cout << "x1 = (" << b << " - " << a << ") / 3 = " << x1 << "\n";
    cout << "x2 = 2(" << b << " - " << a << ") / 3 = " << x2 << "\n\n";

    cout << "Evaluating the functions, we have\n";
    cout << "f(x0) = f(" << x0 << ") = " << f0 << "\n";
    cout << "f(x1) = f(" << x1 << ") = " << f1 << "\n";
    cout << "f(x2) = f(" << x2 << ") = " << f2 << "\n";
    cout << "f(x3) = f(" << x3 << ") = " << f3 << "\n\n";

    double result = (b - a) *
        (f0 + 3 * f1 + 3 * f2 + f3) / 8.0;

    cout << "I = (" << b << " - " << a << ")\n";
    cout << "    [" << f0 << " + 3(" << f1 << ") + 3("
        << f2 << ") + " << f3 << "] / 8\n";

    cout << "\nI = " << result << "\n";
}




/* =============================
   NUMERICAL INTEGRATION MENU
   ============================= */
void numericalIntegration() {
    int method;
    string a_str, b_str;
    const double PI = acos(-1.0);   // 3.141593 internally
    cout << "\nNUMERICAL INTEGRATION\n";
    cout << "Choose method:\n";
    cout << "1 - Trapezoidal Rule\n";
    cout << "2 - Romberg Integration\n";
    cout << "3 - Simpson's 1/3 Rule\n";
    cout << "4 - Simpson's 3/8 Rule\n\n";
    cout << "Choice: ";
    cin >> method;

    switch (method) {
    case 1:
        trapezoidalMethod();
        break;
    case 2: {
        string eq;
        double a, b;
        cout << "Enter equation f(x): ";
        cin.ignore();
        getline(cin, eq);
        cout << "Enter lower limit a: ";
        cin >> a_str;
        a = parseLimit(a_str);
        


        cout << "Enter upper limit b: ";
        cin >> b_str;
       
        b = parseLimit(b_str);

        romberg(eq, a, b);
        break;
    }
    case 3: {
        string eq;
        double a, b;
        cout << "Enter equation f(x): ";
        cin.ignore();
        getline(cin, eq);
        cout << "Enter lower limit a: "; cin >> a;
        cout << "Enter upper limit b: "; cin >> b;
        simpson13_auto(eq, a, b);
        break;
    }
    case 4: {
        string eq;
        double a, b;
        cout << "Enter equation f(x): ";
        cin.ignore();
        getline(cin, eq);
        cout << "Enter lower limit a: "; cin >> a;
        cout << "Enter upper limit b: "; cin >> b;
        simpson38_auto(eq, a, b);
        break;
    }
    default:
        cout << "Invalid choice.\n";
    }
}

double fxy(string eq, double x, double y) {
    const char* expr = eq.c_str();
    te_variable vars[] = {
        {"x", &x},
        {"y", &y}
    };
    int err;
    te_expr* compiled = te_compile(expr, vars, 2, &err);
    if (!compiled) {
        cout << "Error in equation.\n";
        return NAN;
    }
    double result = te_eval(compiled);
    te_free(compiled);
    return result;
}

void rk4_ordinary() {
    string eq;
    double x0, y0, h, target;
    char cont = 'y';

    cout << "\nORDINARY DIFFERENTIAL EQUATION (RK4)\n";
    cout << "dy/dx = f(x,y)\n";

    cout << "Enter f(x,y): ";
    cin.ignore();
    getline(cin, eq);

    cout << "Enter x0: "; cin >> x0;
    cout << "Enter y0: "; cin >> y0;
    cout << "Enter h: "; cin >> h;
    cout << "Enter target x: "; cin >> target;

    while (x0 < target - 1e-9 && cont == 'y') {

        double k1 = h * fxy(eq, x0, y0);
        double k2 = h * fxy(eq, x0 + h / 2, y0 + k1 / 2);
        double k3 = h * fxy(eq, x0 + h / 2, y0 + k2 / 2);
        double k4 = h * fxy(eq, x0 + h, y0 + k3);

        cout << fixed << setprecision(6);

        cout << "\nUsing RK4 Formula:\n";
        cout << "y(xi + h) = yi + 1/6 (k1 + 2k2 + 2k3 + k4)\n\n";

        cout << "y(" << x0 << " + " << h << ") = " << y0
            << " + 1/6 (k1 + 2k2 + 2k3 + k4)\n\n";

        cout << "k1 = h f(xi, yi) = "
            << h << " f(" << x0 << ", " << y0 << ") = "
            << k1 << "\n";

        cout << "k2 = h f(xi + h/2, yi + k1/2) = "
            << h << " f(" << x0 + h / 2 << ", "
            << y0 + k1 / 2 << ") = "
            << k2 << "\n";

        cout << "k3 = h f(xi + h/2, yi + k2/2) = "
            << h << " f(" << x0 + h / 2 << ", "
            << y0 + k2 / 2 << ") = "
            << k3 << "\n";

        cout << "k4 = h f(xi + h, yi + k3) = "
            << h << " f(" << x0 + h << ", "
            << y0 + k3 << ") = "
            << k4 << "\n\n";

        double y1 = y0 + (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;

        cout << "y(" << x0 + h << ") = " << y0
            << " + 1/6 (" << k1
            << " + 2(" << k2 << ") + 2("
            << k3 << ") + " << k4 << ")\n";

        cout << "y(" << x0 + h << ") = " << y1 << "\n";


        cout << "\nContinue to next step? (y/n): ";
        cin >> cont;

        if (cont != 'y') break;

        x0 += h;
        y0 = y1;
    }


}
double Gxyz(string eq, double x, double y, double u) {
    const char* expr = eq.c_str();
    te_variable vars[] = {
        {"x", &x},
        {"y", &y},
        {"u", &u}
    };
    int err;
    te_expr* compiled = te_compile(expr, vars, 3, &err);
    if (!compiled) {
        cout << "Error in equation.\n";
        return NAN;
    }
    double result = te_eval(compiled);
    te_free(compiled);
    return result;
}
void rk4_higher_order() {
    string eq;
    double x0, y0, u0, h, x_target;
    char cont = 'y';

    cout << "\nHIGHER ORDER DIFFERENTIAL EQUATION (RK4)\n";
    cout << "d²y/dx² = G(x,y,u)\n";

    cout << "Enter G(x,y,u): ";
    cin.ignore();
    getline(cin, eq);



    cout << "Enter x0: "; cin >> x0;
    cout << "Enter y(x0): "; cin >> y0;
    cout << "Enter y'(x0): "; cin >> u0;
    cout << "Enter h: "; cin >> h;
    cout << "Enter target x: "; cin >> x_target;


    cout << "\nLet u = dy/dx\n";
    cout << "dy/dx = F(x,y,u) = u\n";
    cout << "du/dx = G(x,y,u)\n\n";

    while (x0 < x_target - 1e-9 && cont == 'y') {
        double k1 = h * u0;
        double Q1 = h * Gxyz(eq, x0, y0, u0);

        double k2 = h * (u0 + Q1 / 2);
        double Q2 = h * Gxyz(eq, x0 + h / 2, y0 + k1 / 2, u0 + Q1 / 2);

        double k3 = h * (u0 + Q2 / 2);
        double Q3 = h * Gxyz(eq, x0 + h / 2, y0 + k2 / 2, u0 + Q2 / 2);

        double k4 = h * (u0 + Q3);
        double Q4 = h * Gxyz(eq, x0 + h, y0 + k3, u0 + Q3);

        cout << fixed << setprecision(6);

        cout << fixed << setprecision(6);

        cout << "\nUsing RK4 for Higher Order ODE\n\n";

        cout << "k1 = hF(x0,y0,u0)\n";
        cout << "   = " << h << "(" << u0 << ") = " << k1 << "\n";

        cout << "Q1 = hG(x0,y0,u0)\n";
        cout << "   = " << h << "("
            << Gxyz(eq, x0, y0, u0) << ") = " << Q1 << "\n\n";

        cout << "k2 = hF(x0+h/2, y0+k1/2, u0+Q1/2)\n";
        cout << "   = " << h << "(" << u0 + Q1 / 2 << ") = " << k2 << "\n";

        cout << "Q2 = hG(x0+h/2, y0+k1/2, u0+Q1/2)\n";
        cout << "   = " << h << "("
            << Gxyz(eq, x0 + h / 2, y0 + k1 / 2, u0 + Q1 / 2)
            << ") = " << Q2 << "\n\n";

        cout << "k3 = hF(x0+h/2, y0+k2/2, u0+Q2/2)\n";
        cout << "   = " << h << "(" << u0 + Q2 / 2 << ") = " << k3 << "\n";

        cout << "Q3 = hG(x0+h/2, y0+k2/2, u0+Q2/2)\n";
        cout << "   = " << h << "("
            << Gxyz(eq, x0 + h / 2, y0 + k2 / 2, u0 + Q2 / 2)
            << ") = " << Q3 << "\n\n";

        cout << "k4 = hF(x0+h, y0+k3, u0+Q3)\n";
        cout << "   = " << h << "(" << u0 + Q3 << ") = " << k4 << "\n";

        cout << "Q4 = hG(x0+h, y0+k3, u0+Q3)\n";
        cout << "   = " << h << "("
            << Gxyz(eq, x0 + h, y0 + k3, u0 + Q3)
            << ") = " << Q4 << "\n\n";


        double y1 = y0 + (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
        double u1 = u0 + (Q1 + 2 * Q2 + 2 * Q3 + Q4) / 6.0;

        cout << "y(x0 + h) = y0 + 1/6 (k1 + 2k2 + 2k3 + k4)\n";
        cout << "y(" << x0 << " + " << h << ") = " << y0 << " + 1/6 ["
            << k1 << " + 2(" << k2 << ") + 2(" << k3 << ") + " << k4 << "]\n\n";

       // cout << "u(x0 + h) = u0 + 1/6 (Q1 + 2Q2 + 2Q3 + Q4)\n";
       // cout << "u(" << x0 << " + " << h << ") = " << u0 << " + 1/6 ["
           // << Q1 << " + 2(" << Q2 << ") + 2(" << Q3 << ") + " << Q4 << "]\n\n";

        cout << "y(" << x0 + h << ") = " << y1 << "\n";
       // cout << "u(" << x0 + h << ") = " << u1 << "\n";
        cout << "\nContinue to next step? (y/n): ";


        cin >> cont;

        if (cont != 'y') break;

        x0 += h;
        y0 = y1;
        u0 = u1;
    }



}
void numericalDifferentialEquations() {
    int choice;
    cout << "\nNUMERICAL DIFFERENTIAL EQUATIONS\n";
    cout << "1 - Ordinary Differential Equation (RK4)\n";
    cout << "2 - Higher Order Differential Equation (RK4)\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) rk4_ordinary();
    else if (choice == 2) rk4_higher_order();
    else cout << "Invalid choice.\n";
}

void  MatrixCalculator() {
    int size;
    int matrix[3][3];
    int det = 0;

start:
    system("cls");

    cout << "======================================\n";
    cout << "        MATRIX DETERMINANT FINDER\n";
    cout << "        Supports 2x2 and 3x3\n";
    cout << "======================================\n";

    cout << "\nEnter matrix size (2 for 2x2, 3 for 3x3): ";
    cin >> size;

    if (size != 2 && size != 3) {
        cout << "⚠ Error: Only 2x2 and 3x3 matrices are allowed.\n";
        cout << "Press any key to try again...";
        cin.ignore(); cin.get();
        goto start;
    }

    cout << "\nEnter elements of the matrix:\n";
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            cin >> matrix[i][j];

    if (size == 2) {
        // 2x2 determinant: ad - bc
        det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    else {
        // 3x3 determinant using Sarrus Rule
        det =
            matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
            matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
            matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    }

    cout << "\n================ RESULT ================\n";
    cout << "Determinant = " << det << endl;
    cout << "========================================\n";

    char again;
    cout << "\nDo you want to calculate another determinant? (Y/N): ";
    cin >> again;

    if (again == 'Y' || again == 'y') {
        goto start;
    }

    cout << "\nThank you for using the program!\n";
}

void matrixadd() {

    int r1, c1, r2, c2;
    int A[3][3], B[3][3], R[3][3];
    char op;

Start:
    system("cls");

    cout << "-------------------------------------------------------------------------\n";
    cout << "\t\tMatrix Calculator (+, -, *)\n";
    cout << "\t\tAllowed sizes: 2x2, 2x3, 3x2, 3x3\n";
    cout << "-------------------------------------------------------------------------\n";

    cout << "Enter number of rows for Matrix A : ";
    cin >> r1;
    cout << "Enter number of columns for Matrix A : ";
    cin >> c1;

    cout << "Enter number of rows for Matrix B: ";
    cin >> r2;
    cout << "Enter number of columns for Matrix B : ";
    cin >> c2;

    if ((r1 < 2 || r1 > 3) || (c1 < 2 || c1 > 3) ||
        (r2 < 2 || r2 > 3) || (c2 < 2 || c2 > 3)) {
        cout << "Error: Only 2x2, 2x3, 3x2, and 3x3 are allowed.\n";
        cout << "Press any key to try again...\n";
        cin.ignore(); cin.get();
        goto Start;
    }


    cout << "\nChoose operation (+ for Add, - for Subtract, * for Multiply): ";
    cin >> op;

    // Validate operation 
    if ((op == '+' || op == '-') && (r1 != r2 || c1 != c2)) {
        cout << "Error: Addition/Subtraction requires same size matrices.\n";
        cout << "Press any key to try again...\n";
        cin.ignore(); cin.get();
        goto Start;
    }
    if (op == '*' && c1 != r2) {
        cout << "Error: Multiplication requires A columns = B rows.\n";
        cout << "Press any key to try again...\n";
        cin.ignore(); cin.get();
        goto Start;
    }

    // Input matrices
    cout << "\nEnter Matrix A:\n";
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c1; j++)
            cin >> A[i][j];

    cout << "Enter Matrix B:\n";
    for (int i = 0; i < r2; i++)
        for (int j = 0; j < c2; j++)
            cin >> B[i][j];

    // Perform operation
    if (op == '+') {
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c1; j++)
                R[i][j] = A[i][j] + B[i][j];
    }
    else if (op == '-') {
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c1; j++)
                R[i][j] = A[i][j] - B[i][j];
    }
    else if (op == '*') {
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++) {
                R[i][j] = 0;
                for (int k = 0; k < c1; k++)
                    R[i][j] += A[i][k] * B[k][j];
            }
    }
    else {
        cout << "Invalid operator.\n";
        cout << "Press any key to try again...\n";
        cin.ignore(); cin.get();
        goto Start;
    }

    //  result
    cout << "\nResult:\n";
    int outRows = (op == '*') ? r1 : r1;
    int outCols = (op == '*') ? c2 : c1;
    for (int i = 0; i < outRows; i++) {
        for (int j = 0; j < outCols; j++)
            cout << R[i][j] << "\t";
        cout << endl;
    }

    cout << "\nPress any key to try again...\n";
    cin.ignore(); cin.get();
    goto Start;
}

void matrixchoice() {

    int choice;
    cout << "\n\n\n1 - Matrix Calculator (+, -, *)\n";
    cout << "2 - Matrix Determinant Finder\n";
	cout << "choice: ";
	cin >> choice;

    if (choice == 1) {
        matrixadd();
    }
    else if (choice == 2) {
        MatrixCalculator();

	}
    else {
		cout << "Invalid choice.\n";
    }

}
void displayMatrix(double a[20][21], int n) {
    cout << "\n------------------------------------------\n";
    cout << "   Augmented Matrix \n";
    cout << "------------------------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << "| ";
        for (int j = 0; j < n; j++) {
            cout << setw(9) << fixed << setprecision(4) << a[i][j] << " ";
        }
        cout << " | " << setw(9) << fixed << setprecision(4) << a[i][n] << " |\n";
    }
    cout << "------------------------------------------\n";
}





void gaussEliminationPartialPivot(double a[20][21], int n) {
    cout << "\n Solving using GAUSS ELIMINATION METHOD ...\n";
    displayMatrix(a, n);

    for (int i = 0; i < n - 1; i++) {
        if (fabs(a[i][i]) < 1e-12) {
            cout << "\n Zero pivot detected at Row " << i + 1 << "! Cannot continue.\n";
            return;
        }
        for (int k = i + 1; k < n; k++) {
            double factor = a[k][i] / a[i][i];
            cout << "\nEliminating Row " << k + 1 << " using Row " << i + 1
                << " (Factor = " << factor << ")";
            for (int j = i; j <= n; j++)
                a[k][j] -= factor * a[i][j];
            displayMatrix(a, n);
        }
    }

    double x[20];
    for (int i = n - 1; i >= 0; i--) {
        x[i] = a[i][n];
        for (int j = i + 1; j < n; j++)
            x[i] -= a[i][j] * x[j];
        x[i] /= a[i][i];
    }

    cout << "\nFinal Solution:\n";
    for (int i = 0; i < n; i++)
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x[i] << endl;
}

/* ======================================================================
   GAUSS ELIMINATION (Partial Pivoting)

   ====================================================================== */
void gaussElimination(double a[20][21], int n) {
    cout << "\nSolving using GAUSS ELIMINATION METHOD ...\n";
    displayMatrix(a, n);

    for (int i = 0; i < n - 1; i++) {

        // 🔹 ZERO-PIVOT CHECK (swap ONLY if pivot is zero)
        if (fabs(a[i][i]) < 1e-12) {
            bool swapped = false;
            for (int k = i + 1; k < n; k++) {
                if (fabs(a[k][i]) > 1e-12) {
                    // swap rows
                    for (int j = 0; j <= n; j++)
                        swap(a[i][j], a[k][j]);

                    cout << "\nZero pivot detected at Row " << i + 1
                        << ". Swapped with Row " << k + 1 << ".\n";
                    displayMatrix(a, n);
                    swapped = true;
                    break;
                }
            }

            if (!swapped) {
                cout << "\nZero pivot detected at Row " << i + 1
                    << " and no suitable row found. Cannot continue.\n";
                return;
            }
        }

        // 🔹 ELIMINATION STEP
        for (int k = i + 1; k < n; k++) {
            double factor = a[k][i] / a[i][i];
            cout << "\n→ Eliminating Row " << k + 1
                << " using Row " << i + 1
                << " (Factor = " << factor << ")\n";

            for (int j = i; j <= n; j++)
                a[k][j] -= factor * a[i][j];

            displayMatrix(a, n);
        }
    }

    // 🔹 BACK SUBSTITUTION
    double x[20];
    for (int i = n - 1; i >= 0; i--) {
        x[i] = a[i][n];
        for (int j = i + 1; j < n; j++)
            x[i] -= a[i][j] * x[j];
        x[i] /= a[i][i];
    }

    cout << "\nFinal Solution:\n";
    for (int i = 0; i < n; i++)
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x[i] << endl;
}

/* ======================================================================
   GAUSS-JORDAN
   ====================================================================== */
void gaussJordan(double a[20][21], int n) {
    cout << "\n Solving using GAUSS–JORDAN METHOD...\n";
    displayMatrix(a, n);

    for (int i = 0; i < n; i++) {
        if (fabs(a[i][i]) < 1e-12) {
            for (int k = i + 1; k < n; k++) {
                if (fabs(a[k][i]) > fabs(a[i][i])) {
                    cout << "\n Swapping Row " << i + 1 << " with Row " << k + 1;
                    for (int j = 0; j <= n; j++) swap(a[i][j], a[k][j]);
                    break;
                }
            }
        }

        double pivot = a[i][i];
        if (fabs(pivot) < 1e-12) {
            cout << "\nSingular Matrix detected!\n";
            return;
        }

        cout << "\n Normalizing Row " << i + 1 << " (Divide by Pivot = " << pivot << ")";
        for (int j = 0; j <= n; j++) a[i][j] /= pivot;
        displayMatrix(a, n);

        for (int k = 0; k < n; k++) {
            if (k == i) continue;
            double factor = a[k][i];
            cout << "\n→ Eliminating Row " << k + 1 << " using Row " << i + 1
                << " (Factor = " << factor << ")";
            for (int j = 0; j <= n; j++) a[k][j] -= factor * a[i][j];
            displayMatrix(a, n);
        }
    }

    cout << "\n Final Solution:\n";
    for (int i = 0; i < n; i++)
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << a[i][n] << endl;
}

/* ======================================================================
   - isDiagonallyDominantVec
   ====================================================================== */

bool isDiagonallyDominantVec(const vector<vector<double>>& A) {
    int n = (int)A.size();
    for (int i = 0; i < n; ++i) {
        double diag = fabs(A[i][i]);
        double sum = 0;
        for (int j = 0; j < n; ++j) if (j != i) sum += fabs(A[i][j]);
        if (diag <= sum) return false;
    }
    return true;
}


bool makeDiagonallyDominantVec(vector<vector<double>>& A, vector<double>& B) {
    int n = (int)A.size();
    vector<int> used(n, 0);
    vector<vector<double>> newA(n, vector<double>(n));
    vector<double> newB(n);

    for (int col = 0; col < n; ++col) {
        int chosen = -1;
        for (int r = 0; r < n; ++r) {
            if (used[r]) continue;
            double diag = fabs(A[r][col]);
            double sum = 0;
            for (int j = 0; j < n; ++j) if (j != col) sum += fabs(A[r][j]);
            if (diag > sum) { chosen = r; break; }
        }
        if (chosen == -1) return false;
        used[chosen] = 1;
        newA[col] = A[chosen];
        newB[col] = B[chosen];
    }

    A = newA;
    B = newB;
    return true;
}
void displayIterationEquations(const vector<vector<double>>& A,
    const vector<double>& B,
    int n) {
    cout << "\nRearranged equations (iteration form):\n\n";

    for (int i = 0; i < n; ++i) {
        cout << "x" << i + 1 << " = (";

        bool first = true;
        for (int j = 0; j < n; ++j) {
            if (j == i) continue;

            if (!first) cout << " ";
            cout << fixed << setprecision(0)
                << -A[i][j] << "x" << j + 1;
            first = false;
        }

        cout << " + " << B[i] << ") / "
            << A[i][i] << "\n";
    }
}

/* ======================================================================
   JACOBI
   ====================================================================== */
void jacobiImproved(const vector<vector<double>>& A_in, const vector<double>& B_in, int n) {
    cout << "\n Solving using JACOBI METHOD...\n";

    // copy to modify
    vector<vector<double>> A = A_in;
    vector<double> B = B_in;

    // check diagonal dominance
    if (!isDiagonallyDominantVec(A)) {
        cout << "\n WARNING: Matrix is NOT diagonally dominant.\n";
        cout << "Attempting to rearrange rows to make it diagonally dominant...\n";
        if (!makeDiagonallyDominantVec(A, B)) {
            cout << " Unable to make matrix diagonally dominant. Jacobi may diverge.\n";
        }
        else {
            cout << " Successfully rearranged rows into diagonally dominant form.\n";
        }
    }
    else {
        cout << " Matrix is diagonally dominant  good for Jacobi convergence.\n";
    }

    displayIterationEquations(A, B, n);


    const double TOL = 1e-3;
    const double DIVERGE_LIMIT = 1e12;
    const int MAX_ITER = 200;

    vector<double> x_old(n, 0.0), x_new(n, 0.0), err(n, 0.0);


    cout << "\n=====================================================================================\n";
    cout << "  k    ";
    for (int i = 0; i < n; ++i) cout << "   x" << (i + 1) << "      ";
    for (int i = 0; i < n; ++i) cout << "    e^k(x" << (i + 1) << ")   ";
    cout << "\n-------------------------------------------------------------------------------------\n";

    for (int k = 1; k <= MAX_ITER; ++k) {
        for (int i = 0; i < n; ++i) {
            double sum = B[i];
            for (int j = 0; j < n; ++j)
                if (j != i) sum -= A[i][j] * x_old[j];
            x_new[i] = sum / A[i][i];

            if (fabs(x_new[i]) > DIVERGE_LIMIT) {
                cout << "\n ERROR: Jacobi diverged (large value detected).\n";
                return;
            }
        }

        //  errors
        bool allSmall = true;
        for (int i = 0; i < n; ++i) {
            err[i] = fabs(x_new[i] - x_old[i]);
            if (err[i] >= TOL) allSmall = false;
        }

        // print 
        cout << setw(4) << k << "  ";
        for (int i = 0; i < n; ++i) cout << setw(12) << fixed << setprecision(6) << x_new[i];
        for (int i = 0; i < n; ++i) cout << setw(12) << fixed << setprecision(6) << err[i];
        cout << "\n";

        if (allSmall) {
            cout << "\n Jacobi converged after " << k << " iterations.\n";
            cout << "\nJacobi Final Solution:\n";
            for (int i = 0; i < n; ++i) cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x_new[i] << "\n";
            return;
        }

        x_old = x_new;
    }

    cout << "\n❌ Jacobi did NOT converge within " << MAX_ITER << " iterations.\n";

}


/* ======================================================================
   Gauss-Seidel
   ====================================================================== */
void gaussSeidelImproved(const vector<vector<double>>& A_in, const vector<double>& B_in, int n) {
    cout << "\n Solving using GAUSS-SEIDEL METHOD...\n";




    vector<vector<double>> A = A_in;
    vector<double> B = B_in;

    if (!isDiagonallyDominantVec(A)) {
        cout << "\nWARNING: Matrix is NOT diagonally dominant.\n";
        cout << "Attempting to rearrange rows to make it diagonally dominant...\n";
        if (!makeDiagonallyDominantVec(A, B)) {
            cout << " Unable to make matrix diagonally dominant. Gauss-Seidel may diverge.\n";
        }
        else {
            cout << " Successfully rearranged rows into diagonally dominant form.\n";
        }
    }
    else {
        cout << " Matrix is diagonally dominant  good for Gauss–Seidel convergence.\n";
    }

    displayIterationEquations(A, B, n);


    const double TOL = 1e-3;
    const double DIVERGE_LIMIT = 1e12;
    const int MAX_ITER = 200;

    vector<double> x(n, 0.0), old(n, 0.0), err(n, 0.0);

    cout << "\n=====================================================================================\n";
    cout << "  k    ";
    for (int i = 0; i < n; ++i) cout << "   x" << (i + 1) << "      ";
    for (int i = 0; i < n; ++i) cout << "   e^k(x" << (i + 1) << ")   ";
    cout << "\n-------------------------------------------------------------------------------------\n";

    for (int k = 1; k <= MAX_ITER; ++k) {
        old = x;

        for (int i = 0; i < n; ++i) {
            double sum = B[i];
            for (int j = 0; j < n; ++j)
                if (j != i) sum -= A[i][j] * x[j];
            x[i] = sum / A[i][i];

            if (fabs(x[i]) > DIVERGE_LIMIT) {
                cout << "\n ERROR: Gauss-Seidel diverged (large value detected).\n";
                return;
            }
        }

        bool allSmall = true;
        for (int i = 0; i < n; ++i) {
            err[i] = fabs(x[i] - old[i]);
            if (err[i] >= TOL) allSmall = false;
        }

        cout << setw(4) << k << "  ";
        for (int i = 0; i < n; ++i) cout << setw(12) << fixed << setprecision(6) << x[i];
        for (int i = 0; i < n; ++i) cout << setw(12) << fixed << setprecision(6) << err[i];
        cout << "\n";

        if (allSmall) {
            cout << "\n Gauss-Seidel converged after " << k << " iterations.\n";
            cout << "\nGauss-Seidel Final Solution:\n";
            for (int i = 0; i < n; ++i) cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x[i] << "\n";
            return;
        }
    }

    cout << "\n Gauss-Seidel did NOT converge within " << MAX_ITER << " iterations.\n";
}




void LinearEquation() {
    int n, choice;
    double a[20][21];

start:
    system("cls");
    cout << "====================================================\n";
    cout << "      SYSTEM OF LINEAR EQUATIONS SOLVER \n";
    cout << "====================================================\n";
    cout << "Available Methods:\n";
    cout << "  [1] Gauss Elimination Method\n";
    cout << "  [2] Gauss Elimination with Maximum Pivoting (GEMP)\n";
    cout << "  [3] Gauss Jordan Method\n";
    cout << "  [4] Jacobi  Method\n";
    cout << "  [5] Gauss Seidel Method\n";
    cout << "====================================================\n";

    cout << "\nEnter number of equations (2 - 10): ";
    cin >> n;
    if (cin.fail() || n < 2 || n > 10) {
        cout << "\n Invalid input! Number of equations must be between 2 and 10.\n";
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Press Enter to try again...";
        cin.get(); cin.get();
        goto start;
    }

    cout << "\nEnter the coefficients of the augmented matrix [A | b].\n";
    cout << "For each row enter " << n << " coefficients followed by the constant.\n";
    for (int i = 0; i < n; i++) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j <= n; j++) {
            cin >> a[i][j];
        }
    }

choose:
    cout << "\nChoose a method (1-5): ";
    cin >> choice;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); goto choose; }

    // copy original to b so methods that mutate it don't destroy original input
    double b[20][21];
    for (int i = 0; i < n; ++i) for (int j = 0; j <= n; ++j) b[i][j] = a[i][j];

    switch (choice) {
    case 1:
        gaussElimination(b, n);
        break;
    case 2:
        gaussEliminationPartialPivot(b, n);
        break;
    case 3:
        gaussJordan(b, n);
        break;
    case 4: {
        // convert to vector form for iterative routines
        vector<vector<double>> A(n, vector<double>(n));
        vector<double> B(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) A[i][j] = a[i][j];
            B[i] = a[i][n];
        }
        jacobiImproved(A, B, n);
        break;
    }
    case 5: {
        vector<vector<double>> A(n, vector<double>(n));
        vector<double> B(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) A[i][j] = a[i][j];
            B[i] = a[i][n];
        }
        gaussSeidelImproved(A, B, n);
        break;
    }
    default:
        cout << "\n Invalid choice! Please select 1-5.\n";
        goto choose;
    }

    char again;
    cout << "\nWould you like to solve another system? (Y/N): ";
    cin >> again;
    if (again == 'Y' || again == 'y') goto start;

    cout << "\nThank you for using SLES. Goodbye!\n";
    cout << "====================================================\n";
}



// Bisection Method
void bisection(string eq, double xp, double xm) {
    cout << "\nSolving using BISECTION METHOD\n";
    cout << "==========================================================================\n";
    cout << " k         xk+        xk-        xk+1        f(xk+1)      |f(xk+1)|   Remarks\n";
    cout << "--------------------------------------------------------------------------\n";

    double x_new;
    int k = 0;

    while (true) {
        x_new = (xp + xm) / 2.0;
        double fx = f(eq, x_new);

        cout << setw(3) << k << "  "
            << setw(12) << fixed << setprecision(6) << xp << "  "
            << setw(12) << xm << "  "
            << setw(12) << x_new << "  "
            << setw(12) << fx << "  "
            << setw(12) << fabs(fx) << "   "
            << (fabs(fx) < 0.001 ? "<0.001" : ">0.001")
            << "\n";

        if (fabs(fx) < 0.001) break;

        if (f(eq, xp) * fx < 0)
            xm = x_new;
        else
            xp = x_new;

        k++;
        if (k > 1000) break;
    }

    cout << "\nResult: x" << k << "+1 = " << fixed << setprecision(6) << x_new << "\n";
}

// Secant Method
void secant(string eq, double xp, double xm) {
    cout << "\nSolving using SECANT METHOD\n";
    cout << "==========================================================================\n";
    cout << " k          xk+        xk-        xk+1               f(xk+1)      |f(xk+1)|   Remarks\n";
    cout << "--------------------------------------------------------------------------\n";

    int k = 0;
    double x_new;

    while (true) {
        double f1 = f(eq, xp);
        double f2 = f(eq, xm);

        if (fabs(f1 - f2) < 1e-12) {
            cout << "Division by zero encountered.\n";
            break;
        }

        x_new = xp - f1 * (xp - xm) / (f1 - f2);
        double fx = f(eq, x_new);

        cout << setw(3) << k << "  "
            << setw(12) << fixed << setprecision(6) << xp << "  "
            << setw(12) << xm << "  "
            << setw(12) << x_new << "  "
            << setw(12) << fx << "  "
            << setw(12) << fabs(fx) << "   "
            << (fabs(fx) < 0.001 ? "<0.001" : ">0.001")
            << "\n";

        if (fabs(fx) < 0.001) break;

        if (fx * f1 > 0)
            xp = x_new;
        else
            xm = x_new;

        k++;
        if (k > 1000) break;
    }

    cout << "\nResult: x" << k << "+1 = " << fixed << setprecision(6) << x_new << "\n";
}

double derivative(string eq, double x) {
    double h = 1e-6;
    return (f(eq, x + h) - f(eq, x - h)) / (2 * h);
}

void newton(string eq, double xp) {

    cout << "\nSolving using NEWTON RAPHSON METHOD\n";
    cout << "==========================================================================\n";
    cout << " k          xk           xk+1        |dk|      Remarks\n";
    cout << "--------------------------------------------------------------------------\n";

    int k = 0;
    double x_new;

    while (true) {
        double fx = f(eq, xp);
        double dfx = derivative(eq, xp);

        if (fabs(dfx) < 1e-12) {
            cout << "Derivative is zero! Cannot continue.\n";
            return;
        }

        x_new = xp - fx / dfx;
        double dk = fabs(x_new - xp) / fabs(x_new);

        cout << setw(3) << k << "  "
            << setw(12) << fixed << setprecision(6) << xp << "  "
            << setw(12) << fixed << setprecision(6) << x_new << "  "
            << setw(12) << fixed << setprecision(6) << dk << "   "
            << (dk < 0.001 ? "<0.001" : ">0.001")
            << "\n";

        if (dk < 0.001)
            break;

        xp = x_new;
        k++;

        if (k > 1000) {
            cout << "Maximum iterations reached.\n";
            break;
        }
    }

    cout << "\nResult:\n";
    cout << "x" << k + 1 << " = " << fixed << setprecision(6) << x_new << "\n";
}

static void title(const string& s) {
    cout << "\n=====================================================\n";
    cout << "  " << s << "\n";
    cout << "=====================================================\n";
}

double safe_div(double a, double b) {
    if (fabs(b) < 1e-15) return NAN;
    return a / b;
}


void leastSquaresLinear() {
    int n;
    title("Least Squares Method (y = C1 + C2*x)");
    cout << "Enter number of data points: ";
    cin >> n;

    vector<double> x(n), y(n);
    cout << "\nEnter x and f(x) values:\n";
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = "; cin >> x[i];
        cout << "f(x[" << i << "]) = "; cin >> y[i];
    }

    double sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;
    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumX2 += x[i] * x[i];
        sumXY += x[i] * y[i];
    }

    cout << fixed << setprecision(6);


    cout << "\nMatrix F (design matrix):\n";
    cout << "--------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << "| 1.000000   " << setw(10) << x[i] << " |\n";
    }
    cout << "--------------------------\n";


    cout << "\nMatrix FT:\n";
    cout << "--------------------------\n";
    cout << "| ";
    for (int i = 0; i < n; i++) cout << setw(10) << 1.000000;
    cout << " |\n| ";
    for (int i = 0; i < n; i++) cout << setw(10) << x[i];
    cout << " |\n";
    cout << "--------------------------\n";

    // FT * F
    cout << "\nMatrix FT * F:\n";
    cout << "--------------------------\n";
    cout << "| " << setw(10) << (double)n << "   " << setw(10) << sumX << " |\n";
    cout << "| " << setw(10) << sumX << "   " << setw(10) << sumX2 << " |\n";
    cout << "--------------------------\n";

    // FT * y
    cout << "\nMatrix FT * y:\n";
    cout << "--------------------------\n";
    cout << "| " << setw(10) << sumY << " |\n";
    cout << "| " << setw(10) << sumXY << " |\n";
    cout << "--------------------------\n";

    //  equations (FTF)C = FTy
    cout << "\n equation (FTF){C} = FT{y}:\n";
    cout << "-----------------------------------------\n";
    cout << "| " << n << "   " << sumX << " |   | C1 |   =   | " << sumY << " |\n";
    cout << "| " << sumX << "   " << sumX2 << " |   | C2 |       | " << sumXY << " |\n";
    cout << "-----------------------------------------\n";

    double D = n * sumX2 - sumX * sumX;
    double D1 = sumY * sumX2 - sumX * sumXY;
    double D2 = n * sumXY - sumX * sumY;

    if (fabs(D) < 1e-15) {
        cout << "\nERROR: Matrix is singular.\n";
        return;
    }

    double C1 = D1 / D;
    double C2 = D2 / D;

    cout << "\nResult:\n";
    cout << "C1 = " << C1 << "\n";
    cout << "C2 = " << C2 << "\n";
    cout << "\ny = " << C1 << " + " << C2 << "x\n";
}


void newtonMethod() {
    title("Newton's Divided Differences");

    double x_eval;
    cout << "Enter value of x where f(x) will be evaluated: ";
    cin >> x_eval;

    int n;
    cout << "Enter number of data points n (n >= 2): ";
    cin >> n;
    if (n < 2) {
        cout << "n must be >= 2.\n";
        return;
    }

    vector<double> x(n), y(n);
    cout << "\nEnter the data points (xi, f(xi)):\n";
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = "; cin >> x[i];
        cout << "f(x[" << i << "]) = "; cin >> y[i];
    }

    cout << fixed << setprecision(6);

    // General polynomial form
    cout << "\nThe " << (n - 1) << "-th order polynomial with n = " << n << " is given by the equation\n";
    cout << "f" << (n - 1) << "(x) = b0";
    for (int k = 1; k < n; k++) {
        cout << " + b" << k;
        for (int j = 0; j < k; j++) cout << "(x - x" << j << ")";
    }
    cout << "\n";

    // Build divided difference table
    vector<vector<double>> dd(n, vector<double>(n, NAN));
    for (int i = 0; i < n; i++) dd[i][0] = y[i];
    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++) {
            double num = dd[i + 1][j - 1] - dd[i][j - 1];
            double den = x[i + j] - x[i];
            dd[i][j] = safe_div(num, den);
        }
    }

    // First divided differences
    cout << "\nEvaluating the first divided differences\n";
    for (int i = 0; i + 1 < n; i++) {
        double num = dd[i + 1][0] - dd[i][0];
        double den = x[i + 1] - x[i];
        cout << "f[x" << (i + 1) << ", x" << i << "] = (" << dd[i + 1][0] << " - " << dd[i][0]
            << ") / (" << x[i + 1] << " - " << x[i] << ") = " << dd[i][1] << "\n";
    }

    // Higher-order differences
    for (int order = 2; order < n; order++) {
        cout << "\nEvaluating the " << order
            << (order == 2 ? "nd" : (order == 3 ? "rd" : "th"))
            << " divided differences\n";
        for (int i = 0; i + order < n; i++) {
            double num = dd[i + 1][order - 1] - dd[i][order - 1];
            double den = x[i + order] - x[i];
            cout << "f[x" << (i + order) << ", x" << (i + order - 1);
            for (int t = order - 2; t >= 0; --t) cout << ", x" << (i + t);
            cout << "] = (" << dd[i + 1][order - 1] << " - " << dd[i][order - 1]
                << ") / (" << x[i + order] << " - " << x[i] << ") = "
                << dd[i][order] << "\n";
        }
    }


    // Substitute into the Newton form
    cout << "\nSubstitute to the equation for the " << (n - 1) << "th order.\n";
    cout << "f" << (n - 1) << "(x) = " << dd[0][0];
    for (int k = 1; k < n; k++) {
        cout << " + " << dd[0][k];
        for (int j = 0; j < k; j++) cout << "(x - " << x[j] << ")";
    }
    cout << "\n";

    // At x = x_eval: one substitution line and final numeric result
    cout << "\nAt x = " << x_eval << "\n";
    cout << "f" << (n - 1) << "(" << x_eval << ") = " << dd[0][0];

    double total = dd[0][0];
    for (int k = 1; k < n; k++) {
        cout << " + " << dd[0][k];
        double prod = 1.0;
        cout << " * ";
        for (int j = 0; j < k; j++) {
            double diff = x_eval - x[j];
            prod *= diff;
            cout << "(" << x_eval << " - " << x[j] << ")";
            if (j < k - 1) cout << "";
        }
        total += dd[0][k] * prod;
    }
    cout << "\n";

    cout << "f" << (n - 1) << "(" << x_eval << ") = " << total << "\n";
}

void lagrangeMethod() {
    title("Lagrange Interpolation");

    double x_eval;
    cout << "Enter value of x where f(x) will be evaluated: ";
    cin >> x_eval;

    int n;
    cout << "Enter number of data points n (n >= 2): ";
    cin >> n;
    if (n < 2) {
        cout << "n must be >= 2.\n";
        return;
    }

    vector<double> x(n), y(n);
    cout << "\nEnter the data points (xi, f(xi)):\n";
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = "; cin >> x[i];
        cout << "f(x[" << i << "]) = "; cin >> y[i];
    }

    cout << fixed << setprecision(6);


    for (int m = 1; m <= n - 1; ++m) {
        cout << "\nSolution:\n";
        cout << "For the " << m
            << (m == 1 ? "st" : (m == 2 ? "nd" : (m == 3 ? "rd" : "th")))
            << " order with x = " << x_eval << "\n";



        cout << "\n";

        // Substitution line f_m(x_eval) with numeric values
        cout << "f" << m << "(" << x_eval << ") =\n";

        double Pm = 0.0;
        for (int i = 0; i <= m; i++) {
            cout << " (";
            bool first = true;
            double numProd = 1.0;
            double denProd = 1.0;
            for (int j = 0; j <= m; j++) {
                if (j == i) continue;
                double num = x_eval - x[j];
                double den = x[i] - x[j];
                if (!first) cout << " ";
                cout << "(" << x_eval << " - " << x[j] << ")";
                first = false;
                numProd *= num;
                denProd *= den;
            }
            cout << " ) / (";
            first = true;
            for (int j = 0; j <= m; j++) {
                if (j == i) continue;
                if (!first) cout << " ";
                cout << "(" << x[i] << " - " << x[j] << ")";
                first = false;
            }
            cout << ") (" << y[i] << ")";
            if (i < m) cout << " +\n";

            double Li = safe_div(numProd, denProd);
            Pm += y[i] * Li;
        }
        cout << "\n";

        cout << "f" << m << "(" << x_eval << ") = " << Pm << "\n";
    }
}



void rootsofeqmain() {
    char again;
    int choice;

    do {
        cout << "==================== Numerical Methods Solver ====================\n";
        cout << fixed << setprecision(6);
        cout << "\nChoose method:\n";
        cout << "1 - Bisection\n";
        cout << "2 - Secant\n";
        cout << "3 - Newtown-Raphson\n";
        cout << " Curve Fitting Methods: \n";
        cout << "4 - Least Squares \n";
        cout << "5 - Newton Polynomial\n";
        cout << "6 - Lagrange Interpolating Polynomial\n";
        cout << "Choice: ";
        cin >> choice;

        double xp, xm;

        if (choice >= 1 && choice <= 3) {
            string eq;
            cout << "Enter equation f(x): ";
            cin.ignore();
            getline(cin, eq);

            double xp, xm;
            if (choice == 1 || choice == 2) {
                cout << "Enter x(0)+: ";
                cin >> xp;
                cout << "Enter x(0)-: ";
                cin >> xm;
                if (choice == 1) bisection(eq, xp, xm);
                else secant(eq, xp, xm);
            }
            else {
                cout << "Enter x(0)+: ";
                cin >> xp;
                newton(eq, xp);
            }

        }
        else if (choice == 4) {
            leastSquaresLinear();

        }
        else if (choice == 5) {
            newtonMethod();

        }
        else if (choice == 6) {
            lagrangeMethod();

        }
        else {
            cout << "Invalid choice.\n";
        }
        cout << "\nDo you want to solve again? (Y/N): ";
        cin >> again;
        again = toupper(again);
        system("cls");

    } while (again == 'Y');
    
}


/* =============================
   MAIN
   ============================= */
int main() {
    int choice;

    do {
        cout << "========================================================================";
        cout << "\n\t\tNUMERICAL METHODS CALCULATOR\n";
        cout << "========================================================================\n";
        cout << "1 - Matrix Calculator\n";
        cout << "2 - Linear Equation\n";
        cout << "3 - Single Root/Curve Fitting\n";
		cout << "4 - Numerical Differentiation\n";
		cout << "5 - Numerical Integration\n";
		cout << "6 - Numerical Differential Equations\n";
        cout << "0 - Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
            matrixchoice();
        else if (choice == 2)
			LinearEquation();
        else if (choice == 3)
			rootsofeqmain();
        else if(choice ==4)
			numericalDifferentiation();
        else if (choice == 5)
			numericalIntegration();
		else if (choice == 6)
			numericalDifferentialEquations();   
		else if (choice != 0)
			cout << "Invalid choice. Please select again.\n";


    } while (choice != 0);

    return 0;
}
