/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * EXERCISE 03 : Recursive Fractals
 * DESCRIPTION : This application runs a recursive fractal with output
 *      statements to show every level of recursion stages.
 ******************************************************************************/
#include <cassert>   // assesrtions
#include <cstdlib>   // random generator
#include <iomanip>   // stream formatting
#include <iostream>  // io stream objects
#include <string>    // string objects

// recursive fractal
void random_fractal(double left_height, double right_height, double width,
                    double epsilon, int level);

// display x number of stars
void display(double x);

// random decimal
double random_fraction();

// random decimal within range
double random_real(double low, double high);

int main() {
    // call recursive fractal
    random_fractal(10.0, 10.0, 16.0, 1.0, 0);
}

void random_fractal(double left_height, double right_height, double width,
                    double epsilon, int level) {
    double mid_height;  // height of midpoint of line segment

    assert(width > 0);
    assert(epsilon > 0);

    if(width <= epsilon) {
        std::cout << std::setw(10 * level) << std::right << "B" << level
                  << "     ";
        display(right_height);
    } else {
        mid_height = (left_height + right_height) / 2;
        mid_height += random_real(-width, width);
        std::cout << std::setw(10 * level) << std::right << "F" << level
                  << std::endl;
        random_fractal(left_height, mid_height, width / 2, epsilon, level + 1);
        std::cout << std::setw(10 * level) << std::right << "S" << level
                  << std::endl;
        random_fractal(mid_height, right_height, width / 2, epsilon, level + 1);
        std::cout << std::setw(10 * level) << std::right << "L" << level
                  << std::endl;
    }
}

void display(double x) {
    const char STAR = '*';
    const char BLANK = ' ';
    const char VERTICAL_BAR = '|';
    const int LIMIT = 39;
    int i;

    if(x < -LIMIT) {
        x = -LIMIT;
    } else if(x > LIMIT) {
        x = LIMIT;
    }

    for(i = -LIMIT; i < 0; i++) {
        if(i >= x) {
            std::cout << STAR;
        }
    }
    std::cout << VERTICAL_BAR;
    for(i = 1; i <= LIMIT; i++) {
        if(i <= x) {
            std::cout << STAR;
        }
    }
    std::cout << std::endl;
}

double random_fraction() { return rand() / double(RAND_MAX); }

double random_real(double low, double high) {
    assert(low <= high);
    return low + random_fraction() * (high - low);
}
