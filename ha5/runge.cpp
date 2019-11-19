#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#pragma GCC optimize("unroll-loops")

#include <vector>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <math.h>
#include <cstdlib>

struct Term
{
    double coeff;
    size_t base_index;  // Must be present
    int auxilary_index; // Optional. Not used if < 0
};

double current[20], k_values[20], previous[20], k[20], initial_values[20], result_values[20];
std::vector<Term> functions[20];

double evaluate(const std::vector<Term> &function, double *const values)
{
    double result = 0.0;
    for (auto const &term : function)
    {
        double delta = term.coeff * values[term.base_index];
        if (term.auxilary_index >= 0)
            delta *= values[term.auxilary_index];
        result += delta;
    }
    return result;
}

void rungekutta(size_t n,
                const std::vector<Term> *functions,
                double *const initial_values,
                double *result_values,
                double target,
                double source = 0.0,
                size_t steps = 500000)
{
    double h = target / steps;
    std::memcpy(previous, initial_values, n * sizeof(double));
    for (int i = 0; i <= steps; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            k[j] = evaluate(functions[j], previous);
            k_values[j] = previous[j] + h * k[j] / 2;
        }
        std::memcpy(result_values, k_values, n * sizeof(double));
        std::memcpy(current, k, n * sizeof(double));

        for (size_t j = 0; j < n; j++)
        {
            k[j] = evaluate(functions[j], result_values);
            k_values[j] = previous[j] + h * k[j] / 2;
            current[j] += 2 * k[j];
        }
        std::memcpy(result_values, k_values, n * sizeof(double));

        for (size_t j = 0; j < n; j++)
        {
            k[j] = evaluate(functions[j], result_values);
            k_values[j] = previous[j] + h * k[j];
            current[j] += 2 * k[j];
        }
        std::memcpy(result_values, k_values, n * sizeof(double));

        for (size_t j = 0; j < n; j++)
        {
            current[j] += evaluate(functions[j], result_values);
            current[j] = previous[j] + current[j] * h / 6;
        }
        std::memcpy(previous, current, n * sizeof(double));
    }

    std::memcpy(result_values, previous, n * sizeof(double));
}

int main(int argc, char **argv)
{
    double target;
    size_t n, m;

    std::cin >> target >> n;
    for (size_t i = 0; i < n; i++)
    {
        std::cin >> initial_values[i];
    }
    std::cin >> m;

    for (size_t i = 0; i < m; i++)
    {
        int mult, d_index;
        char token;
        double coeff;
        Term term;
        term.auxilary_index = -1;

        std::cin >> mult >> term.base_index >> std::ws >> token;

        if (token == '>' && mult == 1)
        {
            std::cin >> coeff;

            term.coeff = -coeff;
            functions[term.base_index].push_back(term);
        }
        else if (token == '>' && mult == 2)
        {
            term.auxilary_index = term.base_index;
            std::cin >> coeff;

            term.coeff = -2 * coeff;
            functions[term.base_index].push_back(term);
        }
        else
        {
            std::cin >> mult >> term.auxilary_index >> std::ws >> token >> coeff;

            term.coeff = -coeff;
            functions[term.base_index].push_back(term);
            functions[term.auxilary_index].push_back(term);
        }

        std::cin >> std::ws;
        while (std::cin.peek() != '#')
        {
            std::cin >> mult >> d_index >> std::ws;
            term.coeff = mult * coeff;
            functions[d_index].push_back(term);

            if (std::cin.peek() == '+')
            {
                std::cin.get();
            }
        }
        std::cin.get();
    }

    rungekutta(n, functions, initial_values, result_values, target);

    for (size_t i = 0; i < n; i++)
    {
        std::cout << std::setprecision(20) << result_values[i] << ' ';
    }

    return 0;
}
