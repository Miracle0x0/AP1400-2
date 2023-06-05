#ifndef Q1_H
#define Q1_H

#include <functional>

namespace q1 {
    double gradient_descent(double init, double step,
                            std::function<double(double)> func);

    const double delta = 1e-3;
    const double epsilon = 1e-10;

    double derivative(double x, const std::function<double(double)>& func) {
        return (func(x + delta) - func(x)) / delta;
    }

    template <typename T, typename F>
    T gradient_descent(T init, T step) {
        return gradient_descent((double)init, (double)step, F());
    }

    double gradient_descent(double init, double step,
                            std::function<double(double)> func) {
        double nxt = init;
        double d = derivative(init, func);
        while (d > epsilon || d < -epsilon) {
            nxt -= step * d;
            d = derivative(nxt, func);
        }
        return nxt;
    }
}  // namespace q1

#endif  // Q1_H