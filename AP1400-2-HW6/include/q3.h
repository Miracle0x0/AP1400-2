#ifndef Q3_H
#define Q3_H

#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

namespace q3 {

    static size_t time_conversion(const std::string &time) {
        std::string pattern = "(\\d+)h(\\d*)m*";
        std::regex reg(pattern);
        std::smatch result;
        size_t res = 0;
        if (std::regex_match(time, result, reg)) {
            res += std::stoi(result[1]) * 60;
            if (result[2] != "") res += std::stoi(result[2]);
        }
        return res;
    }

    static std::vector<std::string> split(const std::string &str) {
        std::vector<std::string> res;
        std::string cur = "";
        for (const char ch : str) {
            if (ch != ',')
                cur.push_back(ch);
            else {
                res.push_back(cur);
                cur = "";
            }
        }
        res.push_back(cur);
        return res;
    }

    /// @brief struct for each flight
    struct Flight {
        // constructor
        Flight(const std::string &item) {
            std::string pattern =
                "\\d- flight_number:(.+) - duration:(.+) - connections:(\\d+) "
                "- connection_times:(.+) - price:(\\d+)";
            std::regex reg(pattern);
            std::smatch result;
            if (std::regex_match(item, result, reg)) {
                flight_number = result[1];
                duration = time_conversion(result[2]);
                connections = std::stoi(result[3]);

                std::vector<std::string> vecs = split(result[4]);
                connection_times = 0;
                for (const auto &i : vecs)
                    connection_times += time_conversion(i);
                price = std::stoi(result[5]);
            }
            sum = duration + connection_times + 3 * price;
        }

        // methods
        bool operator<(const Flight &f) const { return sum >= f.sum; }

        // members
        std::string flight_number;
        size_t duration;
        size_t connections;
        size_t connection_times;  // overall connection time of the flight
        size_t price;
        size_t sum;  // * added
    };

    std::priority_queue<Flight> gather_flights(const std::string &filename) {
        std::priority_queue<Flight> que;
        std::ifstream in(filename, std::ios::in);
        if (!in) throw std::runtime_error("can't open file flights.txt");
        char buf[256];
        while (in.getline(buf, 256)) {
            std::string item(buf);
            if (item.back() == '\r') item.pop_back();
            que.push(Flight(item));
        }
        return que;
    }

}  // namespace q3

#endif  // Q3_H