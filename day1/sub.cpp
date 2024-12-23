#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>

/** Populates the lists from the input file. */
void parse_input(const std::string& file_name, 
    std::vector<int>& list1, 
    std::vector<int>& list2) {
    // reserve size based off input files
    list1.reserve(1024);
    list2.reserve(1024);

    // prep i/o
    std::ifstream in_strm(file_name);
    std::string str;
    int read_num;

    // read an individual line
    while (std::getline(in_strm, str)) {
        std::stringstream strm(str);

        // read two numbers from the line
        strm >> read_num;
        list1.push_back(read_num);
        strm >> read_num;
        list2.push_back(read_num);
    }
}


/*
 *  Assume both lists are the same size. 
 *
 *  1. Sort the lists to get smallest to smallest, second smallest
 *  to second smallest, etc.
 *      -> O(n lg n)
 *  2. Iterate over one list's size, summing the differences to some 
 *  tot_distance.
 *      -> O(n)
 *  3. Return the tot_distance
 */
int get_tot_distance(std::vector<int>& list1, 
    std::vector<int>& list2) {
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    int tot_diff (0);
    for (std::size_t i = 0; i < list1.size(); ++i) {
        tot_diff += std::abs(list1[i] - list2[i]);
    }
    return tot_diff;
}

/*
 *  1. Populate a hash map of value to frequency 
 *  for the right list.
 *      -> O(n)
 *  2. Iterate through the left list with a running
 *  similarity score that is updated.
 *      -> O(n)
 *  3. Return the similarity score
 */
long get_similarity(const std::vector<int>& l1, 
    const std::vector<int>& l2) {
    // get frequency of eles in l2
    std::unordered_map<int, std::size_t> mp;
    for (const auto& ele : l2) {
        ++mp[ele];
    }

    long similarity_score {0};
    for (const auto& ele : l1) {
        similarity_score += ele * mp[ele];
    }
    return similarity_score;
}

int main() {
    std::vector<int> l1, l2;
    parse_input("input.txt", l1, l2);
    const int tot_distance (get_tot_distance(l1, l2));

    // write the total distance to a file
    std::ofstream out_strm("output.txt");
    out_strm << "total distance:" << std::endl;
    out_strm << tot_distance << std::endl;


    // write the similarity
    long similarity_score = get_similarity(l1, l2);
    out_strm << "Similarity Score:" << std::endl;
    out_strm << similarity_score;
    out_strm.close();

    return 0;
};
