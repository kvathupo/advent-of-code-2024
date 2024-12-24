#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

/* 
 *  TODO:
 *      - Move utility methods into a separate header under a 
 *  namespace `santas_workshop`
 */


// Define a concept of a type that we can print
template<typename T>
concept Printable = requires(T val) {
    std::cout << val;
};

/** Prints a vector on one line of form "{1, 2, 3}\n" */
template<Printable T>
void print_vector(const std::vector<T>& v) {
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i == 0) {
            std::cout << "{" << v[i] << ", ";
        } else if (i == v.size() - 1) {
            std::cout << v[i] << "}" << std::endl;
        } else {
            std::cout << v[i] << ", ";
        }
    }
}

/** Ditto as above, but with casting to `S` prior to print. */
template<Printable T, typename S>
void print_vector(const std::vector<T>& v) {
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i == 0) {
            std::cout << "{" << static_cast<S>(v[i]) << ", ";
        } else if (i == v.size() - 1) {
            std::cout << static_cast<S>(v[i]) << "}" << std::endl;
        } else {
            std::cout << static_cast<S>(v[i]) << ", ";
        }
    }
}

/*
 *******************************************************************************
 *  Move everything above to a separate header!
 *******************************************************************************
 */

/** Given a line, return the levels in that report */
template<typename T>
std::vector<T> get_report(const std::string& line) {
    /*
     *  Note that using `uint8_t` reads in individual characters.
     *  Likely has to do with `char` having 8 bits!
     */
    std::stringstream strm(line);
    std::vector<T> report;
    report.reserve(64);

    // `std::istream_iterator` relies upon the return value of `operator>>()`
    std::copy(std::istream_iterator<T>(strm),
        std::istream_iterator<T>(),
        std::back_inserter(report));

    return report;
}

/*
 *  1. Iterate from the first element to the second to last (inclusive)
 *      i. If the difference of the next ele from the current one is zero or greater than 3, return false
 *  2. Return true
 *      -> Time complexity: O(n_levels)
 *
 *  @Requires:
 *      - The size of the report is at least 2
 */
template<typename T>
bool is_increasing(const std::vector<T>& report) {
    for (std::size_t i = 0; i < report.size() - 1; ++i) {
        int diff = static_cast<int>(report[i + 1]) - report[i];

        if (diff <= 0 || diff > 3)
            return false;
    }
    return true;
}

/** Time complexity: O(n_levels) */
template<typename T>
bool is_decreasing(const std::vector<T>& report) {
    for (std::size_t i = 0; i < report.size() - 1; ++i) {
        int diff = static_cast<int>(report[i + 1]) - report[i];

        if (diff >= 0 || diff < -3)
            return false;
    }
    return true;
}

template<typename T>
bool is_safe(const std::vector<T>& report) {
    // It's vacuously true that an empty list or a single item is increasing/decreasing
    if (report.size() <= 1)
        return true;

    // if equal, we return false in either case
    if (report[0] < report[1])
        return is_increasing(report);
    else
        return is_decreasing(report);
}

/*
 *  Parses inputs and counts the number of good reports. 
 *
 *  0. Init a safe_count to 0
 *  1. For each line,                                           -> O(n_reports)
 *      i. Parse the line into an int array report              -> O(n_levels)
 *      ii. If the report is safe, increment the safe_count     -> O(n_levels)
 *  2. Return the safe_count
 *      -> Time complexity; O(n_reports * n_levels)
 */
uint16_t parse_input(const std::string& file_name = "input.txt") {
    // prep i/o
    std::ifstream in_strm(file_name);
    std::string str;
    uint16_t safe_count {0};

    // read an individual line
    while (std::getline(in_strm, str)) {
        std::vector<unsigned int> report = get_report<unsigned int>(str);
        if (is_safe(report)) {
            ++safe_count;
        }
    }

    return safe_count;
}


int main() {
    unsigned int safe_count = parse_input();
    std::cout << "Counted " << safe_count << " safe reports" << std::endl;

    return 0;
};
