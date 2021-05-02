#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <fstream>
#include <unordered_set>
#include <chrono>
#include <array>
using namespace std;

#define MIN_WORD_LENGTH 3

template<typename T>
std::vector<T> subArr(const std::vector<T> &a, int start, int end)
{
    return std::vector<T>(a.begin() + start, a.begin() + end);
}

std::string charArrToStr(const std::vector<char> a)
{
    return std::string(a.begin(), a.end());
}

int main()
{
    // ====== Setup =====

    // Lists of valid words used to check possible words found the the puzzle
    std::unordered_set<std::string> valid_words;

    // Reads file that contains lists of words
    std::ifstream file("words.txt");
    std::string str;
    while(std::getline(file, str))
        valid_words.emplace(str);
    
    // Layout of the crossword puzzle
    std::vector<std::vector<char>> crossword_puzzle 
    {
        {'d', 'a', 't', 'g'}, 
        {'f', 'a', 's', 't'}, 
        {'c', 'h', 'm', 'o'}, 
        {'e', 'i', 'i', 'p'},
        {'f', 'c', 'u', 'p'}
    };
    
    // Save current state to calculate the duration later
    const auto start_time = std::chrono::high_resolution_clock::now();  
    
    // ====== Crossword puzzle solver algorithm =======

    // Get its size
    int crossword_puzzle_width = crossword_puzzle.at(0).size();
    int crossword_puzzle_height = crossword_puzzle.size();

    // Lists of valid words found on the crossword puzzle
    std::vector<std::string> words_found;

    // Search for rows and columns
    for (int word_length = MIN_WORD_LENGTH; word_length <= crossword_puzzle_width; word_length++)
    {
        // (west to east) lane
        for (std::vector<char> &row: crossword_puzzle)
        {
            for (int x = 0; x < crossword_puzzle_width - word_length + 1; x++)
            {
                std::string possible_valid_word = charArrToStr(subArr<char>(row, x, x + word_length));    

                // Check if it's valid word
                if (valid_words.find(possible_valid_word) != valid_words.end())
                    words_found.push_back(possible_valid_word);         
            }
        }
        // (north to south) lane
        for (int col = 0; col < crossword_puzzle_width; col++)
        {
            for (int row = 0; row < crossword_puzzle_height - word_length + 1; row++)
            {
                std::string possible_valid_word;
                for (int i = 0; i < word_length; i++) 
                    possible_valid_word += crossword_puzzle.at(row + i).at(col);
                
                // Check if it's valid word
                if (valid_words.find(possible_valid_word) != valid_words.end())
                    words_found.push_back(possible_valid_word);     
            } 
        }
           
    }       

    // Search for two intersecting horizontal lane
    for (int word_length = MIN_WORD_LENGTH; word_length <= std::min(crossword_puzzle_width, crossword_puzzle_height); word_length++)
    {
        for (int i = 0; i <= std::min(crossword_puzzle_width, crossword_puzzle_height) - word_length; i++)
        {
            // (northwest -> southeast) lane (1)
            std::string possible_valid_word;
            for (int j = 0; j < word_length; j++)
                possible_valid_word += crossword_puzzle.at(i + j).at(i + j);    

            // Check if it's valid word
            if (valid_words.find(possible_valid_word) != valid_words.end())
                words_found.push_back(possible_valid_word);
             
            // (southwest -> northeast) lane (2)
            possible_valid_word.clear();     
            for (int j = 0; j < word_length; j++)           
                possible_valid_word += crossword_puzzle.at(crossword_puzzle_height - i - j - 1).at(i + j);

            // Check if it's valid word 
            if (valid_words.find(possible_valid_word) != valid_words.end())
                words_found.push_back(possible_valid_word);       
        }
    }   

    // Check for all horizontal lane with the two interesecting horizontal lane mentioned before excluded

    // (base to down)
    for (int y = 1; y <= crossword_puzzle_height - MIN_WORD_LENGTH; y++)
    {
        for (int word_length = MIN_WORD_LENGTH; word_length <= crossword_puzzle_height - y; word_length++)
        {
            // (northwest -> southeast) lane (1)
            std::string possible_valid_word;
            for (int i = 0, j = y; i < word_length; i++, j++)
                possible_valid_word += crossword_puzzle.at(j).at(i);

            // Check if it's valid word 
            if (valid_words.find(possible_valid_word) != valid_words.end())
                words_found.push_back(possible_valid_word);

            // (southwest -> northeast) lane (2)
            possible_valid_word.clear();
            for (int i = 0, j = crossword_puzzle_height - y - 1; i < word_length; i++, j--)
                possible_valid_word += crossword_puzzle.at(j).at(i);

            // Check if it's valid word 
            if (valid_words.find(possible_valid_word) != valid_words.end())
                words_found.push_back(possible_valid_word);
        }
    }
    
    //  (base to left)
    for (int x = 1; x <= crossword_puzzle_width - MIN_WORD_LENGTH; x++)
    {
        for (int word_length = MIN_WORD_LENGTH; word_length <= crossword_puzzle_width - x; word_length++)
        {
            // (northwest -> southeast) lane (1)
            std::string possible_valid_word;
            for (int i = 0, j = x; i < word_length; i++, j++)
                possible_valid_word += crossword_puzzle.at(j).at(i);

            // Check if it's a valid word
            if (valid_words.find(possible_valid_word) != valid_words.end())
                words_found.push_back(possible_valid_word);

            // (northeast -> southwest) lane (2)
            possible_valid_word.clear();
            for (int i = x, j = crossword_puzzle_width; i < word_length + x; i++, j--)
                possible_valid_word += crossword_puzzle.at(j).at(i);

            // Check if it's a valid word
            if (valid_words.find(possible_valid_word) != valid_words.end())
                words_found.push_back(possible_valid_word);
        }
    }

    // Calculate execution time
    auto stop_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

    // Dsiplay all the words found
    for (unsigned int i = 0; i < words_found.size(); i++)
    {
        cout << "[" << i << "] " << words_found.at(i) << endl;
    }
    cout << "-------------------" << endl;
    cout << words_found.size() << " words found" << endl;
    cout << "Execution time Took " << duration.count() << " us" << endl;

    return 0;
}