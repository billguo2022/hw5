#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <vector>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Function to generate possible words for a wordle game given an input string,
// a set of floating characters, and a dictionary.
void generate_words(std::string& input, std::string& remaining_chars,
                     size_t index, const std::set<std::string>& dictionary,
                     size_t num_dashes, std::set<std::string>& results);


std::set<std::string> wordle(const std::string& input,
                                           const std::string& floating_chars,
                                           const std::set<std::string>& dictionary)
{
    std::set<std::string> results;

    // Count the number of dashes in the input string
    int dashn = 0;
    for (unsigned int i = 0; i < input.length(); i++){
        if (input[i] == '-') {
            dashn++;
        }
    }

    // Make copies of the input string and floating characters to be able to modify them
    std::string input_copy = input;
    std::string floating_copy = floating_chars;

    // Call the recursive helper function to generate all possible words
    generate_words(input_copy, floating_copy, 0, dictionary, dashn, results);

    return results;
}

// Recursive helper function to generate possible words for a wordle game
void generate_words(std::string& input, std::string& charRe,
                     size_t index, const std::set<std::string>& dictionary,
                     size_t dashn, std::set<std::string>& results)
{
    // If the index has reached the end of the input string, check if the current word
    // is in the dictionary and add it to the set of results if it is.
    if (index == input.length()) {
        if (dictionary.find(input) != dictionary.end() && charRe == "") {
            results.insert(input);
        }
        return;
    }

    // If the number of remaining characters is greater than the number of dashes,
    // there are not enough dashes to use all the floating characters, so return.
    if (charRe.length() > dashn) {
        return;
    }

    // If the character at the current index is not a dash, move to the next index.
    if (input[index] != '-') {
        generate_words(input, charRe, index + 1, dictionary, dashn, results);
        return;
    }
    else {
        // If the character at the current index is a dash, try each floating character
        // and recursively call the function to fill the next dash with another floating character.
        for (char x = 'a'; x <= 'z'; x++) {
            if (charRe.find(x) != std::string::npos) {
							
                // If the floating character is available, use it to fill the current dash
                input[index] = x;

                charRe.erase(charRe.find(x), 1);

                // Recursively call the function to fill the next dash
                generate_words(input, charRe, index + 1, dictionary, dashn - 1, results);

                // Undo the changes and restore the floating character
                input[index] = '-';
                
                charRe = charRe + x;
            }
            else {
                // If the floating character is not available, skip to the next one
                input[index] = x;
                generate_words(input, charRe, index + 1, dictionary, dashn - 1, results);
                input[index] = '-';
            }
        }
    }
}
