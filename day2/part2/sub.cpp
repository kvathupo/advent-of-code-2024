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
 *  Returns true if the report is increasing, ignoring the index at `ignore_idx`
 *
 *  1. Iterate from the first element to second to last (inclusive)
 *      i. If the current index is to be ignored, continue
 *      ii. If the next index is to be ignored, there are two cases:
 *          1. If the next index is the last index, return true
 *          2. If diff two ahead is invalid, return false. 
 *      iii. Neither index is to the be ignored. If the diff is invalid, return false.
 *  2. Return true
 *      -> Time complexity: O(n_levels)
 */
template<typename T>
bool is_increasing_with_removal(const std::vector<T>& report, std::size_t ignore_idx) {
    for (std::size_t i = 0; i < report.size() - 1; ++i) {
        // ignore either the current or next index?
        if (i == ignore_idx) {
            continue;
        } else if (i + 1 == ignore_idx) {
            // ignoring the last index?
            if (i + 1 == report.size() - 1)
                return true;

            int two_ahead_diff = static_cast<int>(report[i + 2]) - report[i];
            if (two_ahead_diff <= 0 || two_ahead_diff > 3) {
                return false;
            }
        } else {
            int diff = static_cast<int>(report[i + 1]) - report[i];
            if (diff <= 0 || diff > 3)
                return false;
        }
    }
    return true;
}

/*
 *  Returns true if the report is decreasing, ignoring the index at `ignore_idx`
 *      -> Time complexity: O(n_levels)
 */
template<typename T>
bool is_decreasing_with_removal(const std::vector<T>& report, std::size_t ignore_idx) {
    for (std::size_t i = 0; i < report.size() - 1; ++i) {
        // ignore either the current or next index?
        if (i == ignore_idx) {
            continue;
        } else if (i + 1 == ignore_idx) {
            // ignoring the last index?
            if (i + 1 == report.size() - 1)
                return true;

            int two_ahead_diff = static_cast<int>(report[i + 2]) - report[i];
            if (two_ahead_diff >= 0 || two_ahead_diff < -3) {
                return false;
            }
        } else {
            int diff = static_cast<int>(report[i + 1]) - report[i];
            if (diff >= 0 || diff < -3)
                return false;
        }
    }
    return true;
}


/*
 *  1. Iterate from the first element to second to last (inclusive)
 *      i. If the difference of the next ele from the current one is zero or greater than 3, then there are two cases.
 *  We can remove the current index or the next one. Return true if either such array is increasing.
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
            return is_increasing_with_removal(report, i) || is_increasing_with_removal(report, i+1);
    }
    return true;
}

/** Time complexity: O(n_levels) */
template<typename T>
bool is_decreasing(const std::vector<T>& report) {
    for (std::size_t i = 0; i < report.size() - 1; ++i) {
        int diff = static_cast<int>(report[i + 1]) - report[i];

        if (diff >= 0 || diff < -3)
            return is_decreasing_with_removal(report, i) || is_decreasing_with_removal(report, i+1);
    }
    return true;
}


template<typename T>
bool is_safe(const std::vector<T>& report) {
    // It's vacuously true that an empty list or a single item is increasing/decreasing
    if (report.size() <= 1)
        return true;

    // It could be the case that the first two elements are increasing, yet the rest are decreasing!
    // Note: if equal, we return false in either case
    return is_increasing(report) || is_decreasing(report);
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
unsigned int parse_input(const std::string& file_name = "input.txt") {
    // prep i/o
    std::ifstream in_strm(file_name);
    std::string str;
    unsigned int safe_count {0};

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
