#ifndef Q4_H
#define Q4_H

#include <algorithm>
#include <numeric>
#include <vector>

namespace q4 {

    /// @brief struct of 2-D plane
    struct Vector2D {
        // constructors
        Vector2D(double xx = 0, double yy = 0) : x(xx), y(yy) {}
        // members
        double x{};
        double y{};
    };

    /// @brief struct of sensor
    struct Sensor {
        // constructor
        Sensor(double x, double y, double acc) : accuracy(acc) {
            pos.x = x;
            pos.y = y;
        }
        Sensor(const Vector2D& v, double acc) : pos(v), accuracy(acc) {}
        // members
        Vector2D pos;
        double accuracy;
    };

    Vector2D kalman_filter(std::vector<Sensor> sensors) {
        double sum = 0;
        auto accumulater = [&sum](const Sensor& sensor) -> void {
            sum += sensor.accuracy;
        };
        for_each(sensors.begin(), sensors.end(), accumulater);

        std::vector<double> weights;
        auto weight_builder = [&weights, sum](const Sensor& sensor) -> void {
            weights.push_back(sensor.accuracy / sum);
        };
        for_each(sensors.begin(), sensors.end(), weight_builder);

        // do calculation
        double X = 0;
        size_t idx = 0;
        auto w_x = [weights, sensors, &X, &idx](const Sensor& sensor) -> void {
            X += weights[idx] * sensors[idx].pos.x;
            idx++;
        };
        for_each(sensors.begin(), sensors.end(), w_x);

        double Y = 0;
        idx = 0;
        auto w_y = [weights, sensors, &Y, &idx](const Sensor& sensor) -> void {
            Y += weights[idx] * sensors[idx].pos.y;
            idx++;
        };
        for_each(sensors.begin(), sensors.end(), w_y);

        return Vector2D{X, Y};
    }

}  // namespace q4

#endif  // Q4_H