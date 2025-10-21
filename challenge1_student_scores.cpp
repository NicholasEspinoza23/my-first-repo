#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <numeric>

using namespace std;

// Function to run the grading report logic
void generateGradeReport() {
    // 1. Declare parallel vectors to store the data
    vector<string> studentNames;
    vector<int> totalScores;
    vector<double> averages;

    // 2. Open "student_scores.txt" using an std::ifstream
    ifstream inFile("student_scores.txt");

    // 3. Implement robust error checking
    if (!inFile.is_open()) {
        // If opening fails, print an error and exit.
        cerr << "Error: Could not open the file 'student_scores.txt'. "
             << "Please ensure the file exists in the same directory." << endl;
        return;
    }

    string line;
    double classTotalScoreSum = 0.0;
    int studentCount = 0;

    // Read and ignore the header line
    getline(inFile, line);

    // 4. Use a while loop with std::getline to read each data line from the file
    while (getline(inFile, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Variables to hold parsed string tokens
        string firstNameStr, lastNameStr, score1Str, score2Str, score3Str;

        // Create an std::istringstream from the current line
        istringstream iss(line);

        // --- CORE FIX: Use getline(iss, token, ',') for comma-delimited parsing ---

        // Parse data using comma as the delimiter
        if (getline(iss, firstNameStr, ',') &&
            getline(iss, lastNameStr, ',') &&
            getline(iss, score1Str, ',') &&
            getline(iss, score2Str, ',') &&
            getline(iss, score3Str))
        {
            // Convert score strings to integers
            try {
                int score1 = stoi(score1Str);
                int score2 = stoi(score2Str);
                int score3 = stoi(score3Str);

                // Combine first and last name into a single full name string
                string fullName = firstNameStr + " " + lastNameStr;
                studentNames.push_back(fullName);

                // Calculate the sum of the three scores
                int total = score1 + score2 + score3;
                totalScores.push_back(total);

                // Calculate the average score (3.0 ensures floating-point division)
                double average = static_cast<double>(total) / 3.0;
                averages.push_back(average);

                // Accumulate data for overall class calculation
                classTotalScoreSum += average;
                studentCount++;
            } catch (const std::invalid_argument& e) {
                cerr << "Warning: Skipping line due to invalid score conversion: " << line << endl;
            }

        } else {
            // Handle lines that could not be fully parsed
            cerr << "Warning: Skipping line due to incorrect number of fields: " << line << endl;
        }
    }

    // 5. Close the input file
    inFile.close();

    // Check if any data was successfully processed
    if (studentCount == 0) {
        cout << "No student data was successfully processed." << endl;
        return;
    }

    // --- Output and Final Calculations ---

    cout << "\n===================================" << endl;
    cout << "        Student Grade Report       " << endl;
    cout << "===================================" << endl;

    // A. Print a header row: "Name Average Grade" (use setw for alignment)
    cout << left << setw(25) << "Name"
         << right << setw(10) << "Average" << endl;
    cout << string(35, '-') << endl;

    // B. Print data for each student
    double highestAverage = -1.0;

    for (size_t i = 0; i < studentNames.size(); ++i) {
        // Output full name (left-aligned)
        cout << left << setw(25) << studentNames.at(i)
             // Output average score (right-aligned, 2 decimal places)
             << right << fixed << setprecision(2) << setw(10) << averages.at(i)
             << endl;

        // Track the highest average
        if (averages.at(i) > highestAverage) {
            highestAverage = averages.at(i);
        }
    }

    cout << string(35, '-') << endl;

    // C. Overall class average
    double classAverage = classTotalScoreSum / studentCount;
    cout << left << setw(25) << "Class Average:"
         << right << fixed << setprecision(2) << setw(10) << classAverage
         << endl;

    // D. The name(s) of the student(s) with the highest average score
    cout << "\n-- Highest Average --" << endl;
    bool first = true;
    for (size_t i = 0; i < studentNames.size(); ++i) {
        if (averages.at(i) == highestAverage) {
            if (!first) {
                cout << ", ";
            }
            cout << studentNames.at(i) << " (" << fixed << setprecision(2) << highestAverage << ")";
            first = false;
        }
    }
    cout << "\n===================================" << endl;
}

int main() {
    // Call the main reporting function
    generateGradeReport();
    return 0;
}