#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>  

using namespace std;

vector<vector<double>> diffTable(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    vector<vector<double>> diff_table(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        diff_table[i][0] = y[i];
    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++)
            diff_table[i][j] = (diff_table[i + 1][j - 1] - diff_table[i][j - 1]) / (x[i + j] - x[i]);
    }
    return diff_table;
}

double newtonPoly(const vector<double>& x_data, const vector<double>& y_data, double x, int degree, const vector<vector<double>>& diff_table) {
    // int closest_index = 2 * (x - x_data[0]) > degree ? x - x_data[0] : 2 * (x - x_data[0]); to check that the index does not exceed the array boundaries.

    int closest_index = 0;
    double min_dist = std::abs(x - x_data[0]);

    for (int i = 1; i < x_data.size(); i++) {
        double dist = std::abs(x - x_data[i]);
        if (dist < min_dist) {
            closest_index = i;
            min_dist = dist;
        }
    }
    closest_index = std::min(closest_index, static_cast<int>(x_data.size()) - degree - 1);

    double y = diff_table[closest_index][0];
    double p = 1;
    for (int i = 1; i <= degree; i++) {
        p *= (x - x_data[closest_index + i - 1]);
        y += diff_table[closest_index][i] * p;
    }
    return y;
}

int main() {
    setlocale(LC_ALL, "");
    const int n = 11;
    vector<double> x_data(n), y_data(n);
    int degree;

    do {
        cout << "Enter the polynomial degree: ";
        cin >> degree;
        if (degree < 0)
            cout << "Invalid polynomial degree entered\n";
        degree = min(degree, n - 1);
    } while (degree < 0);

    for (int i = 0; i < n; i++) {
        x_data[i] = 0.2 + 0.5 * i;
        y_data[i] = (exp(x_data[i]) - 1) / x_data[i];
    }

    auto diff_table = diffTable(x_data, y_data);

    cout << setw(10) << "New grid\t" << setw(15) << "Polynomial values\t" << setw(15) << "Interpolation error\t" << endl;


    for (double x = 0.2; x <= x_data.back(); x += 0.25) {
        double y = newtonPoly(x_data, y_data, x, degree, diff_table);
        cout << setw(10) << setprecision(5) << x
            << setw(19) << setprecision(5) << y
            << setw(27) << setprecision(5) << abs(y - (exp(x) - 1 / x))
            << endl;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}