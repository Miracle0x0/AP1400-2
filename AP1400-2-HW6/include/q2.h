#ifndef Q2_H
#define Q2_H

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace q2 {

    /// @brief Patient's information
    struct Patient {
        // constructor
        Patient(std::string item) {
            if (item.back() == '\r') item.pop_back();
            std::vector<std::string> vecs;
            std::string cur = "";
            for (const auto ch : item) {
                if (ch == ' ') continue;
                if (ch != ',')
                    cur.push_back(ch);
                else {
                    vecs.push_back(cur);
                    cur = "";
                }
            }
            vecs.push_back(cur);
            // translate strs in vecs to members
            name = vecs[0] + " " + vecs[1];
            age = std::stoi(vecs[2]);
            smokes = std::stoi(vecs[3]);
            area_q = std::stoi(vecs[4]);
            alkhol = std::stoi(vecs[5]);
            // calculate the risk of cancer
            risk = 3 * age + 5 * smokes + 2 * area_q + 4 * alkhol;
        }

        // methods
        bool operator<(const Patient& p) const { return risk >= p.risk; }

        // members
        std::string name;
        size_t age;
        size_t smokes;
        size_t area_q;
        size_t alkhol;
        size_t risk;  // * risk of cancer
    };

    /// @brief read the content file and create a std::vector of the patients
    /// @param filename
    /// @return std::vector<Patient> - patient's information
    std::vector<Patient> read_file(const std::string& filename) {
        std::ifstream in(filename, std::ios::in);
        if (!in) throw std::runtime_error("can't open file lung_cancer.csv");

        size_t i = 0;
        char buf[100];
        std::vector<Patient> vec_p;

        while (in.getline(buf, 100)) {
            if (i++ < 2) continue;  // skip first two lines
            vec_p.push_back(Patient(buf));
        }

        in.close();
        return vec_p;
    }

    void sort(std::vector<Patient>& vec_p) {
        std::sort(vec_p.begin(), vec_p.end());
    }

}  // namespace q2

#endif  // Q2_H