/*
 * Flashcard Study System
 * 
 * This program implements a flashcard study system using a Circularly Linked List
 * data structure. The circular linked list allows for continuous navigation through
 * flashcards, making it ideal for study sessions where users want to cycle through
 * all cards repeatedly.
 * 
 * Key Features:
 * - Template-based Circular Linked List implementation
 * - Add new flashcards with multiple questions
 * - Load flashcards from text files
 * - Review flashcards in circular order
 * - Save flashcards to files for persistence
 * 
 * Data Structures:
 * - CircularLinkedList<T>: Template class implementing circular linked list
 * - Node<T>: Template class for list nodes
 * - Flashcard: Class representing a collection of questions
 * - Question: Class representing a question-answer pair
 * 
 * Compilation:
 * Requires C++17 or later: g++ -std=c++17 -o project project.cpp
 * 
 * Author: Group Project
 * Date: February 2026
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
using namespace std;

// Use filesystem namespace for directory operations
// Note: Requires C++17 or later standard
namespace fs = std::filesystem;

//////////////////////////////////////////////////////////////
// QUESTION CLASS
// Represents a single question-answer pair in a flashcard
//////////////////////////////////////////////////////////////
class Question {
public:
    string question;  // The question text
    string answer;    // The answer text

    /**
     * Default constructor for Question
     */
    Question() = default;

    /**
     * Parameterized constructor for Question
     * @param q The question text
     * @param a The answer text
     */
    Question(const string& q, const string& a) : question(q), answer(a) {}
};

//////////////////////////////////////////////////////////////
// FLASHCARD CLASS
// Represents a collection of questions grouped under a title
//////////////////////////////////////////////////////////////
class Flashcard {
public:
    string title;              // The title/name of the flashcard set
    vector<Question> questions; // Vector of question-answer pairs

    /**
     * Default constructor for Flashcard
     */
    Flashcard() = default;

    /**
     * Parameterized constructor for Flashcard
     * @param t The title of the flashcard set
     */
    Flashcard(const string& t) : title(t) {}
};

//////////////////////////////////////////////////////////////
// NODE TEMPLATE
// Template class representing a node in the circular linked list
// Can hold any data type T
//////////////////////////////////////////////////////////////
template <typename T>
class Node {
public:
    T data;           // The data stored in this node
    Node<T>* next;    // Pointer to the next node in the circular list

    /**
     * Constructor for Node
     * @param value The data value to store in this node
     */
    Node(const T& value) : data(value), next(nullptr) {}
};

//////////////////////////////////////////////////////////////
// CIRCULAR LINKED LIST TEMPLATE
// Template class implementing a circularly linked list data structure
// The last node points back to the first node, creating a circular structure
// Can store any data type T
//////////////////////////////////////////////////////////////
template <typename T>
class CircularLinkedList {
private:
    Node<T>* tail;  // Pointer to the tail (last) node of the circular list

public:
    /**
     * Default constructor
     * Initializes an empty circular linked list
     */
    CircularLinkedList() : tail(nullptr) {}

    /**
     * Destructor
     * Properly deallocates all nodes in the circular linked list
     */
    ~CircularLinkedList() {
        if (!tail) return;
        Node<T>* curr = tail->next;  // Start from head
        Node<T>* nextNode;
        // Traverse and delete all nodes until we reach tail
        while (curr != tail) {
            nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        delete tail;  // Delete the tail node
    }

    /**
     * Check if the list is empty
     * @return true if the list is empty, false otherwise
     */
    bool isEmpty() const { return tail == nullptr; }

    /**
     * Get the head (first) node of the circular list
     * @return Pointer to the head node, or nullptr if list is empty
     */
    Node<T>* getHead() const { return tail ? tail->next : nullptr; }

    /**
     * Get the tail (last) node of the circular list
     * @return Pointer to the tail node, or nullptr if list is empty
     */
    Node<T>* getTail() const { return tail; }

    /**
     * Insert a new element at the end of the circular list
     * @param value The value to insert
     */
    void insert(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!tail) {
            // First node: point to itself to create circular structure
            tail = newNode;
            tail->next = tail;
        } else {
            // Insert after tail and update tail
            newNode->next = tail->next;  // New node points to head
            tail->next = newNode;        // Previous tail points to new node
            tail = newNode;              // Update tail to new node
        }
    }

    /**
     * Get the number of elements in the circular list
     * @return The size of the list
     */
    int getSize() const {
        if (!tail) return 0;
        int count = 0;
        Node<T>* curr = tail->next;  // Start from head
        do {
            count++;
            curr = curr->next;
        } while (curr != tail->next);  // Stop when we complete the circle
        return count;
    }

    /**
     * Print all elements in the circular list
     * Used for debugging and displaying all flashcards
     */
    void debugPrint() const {
        if (!tail) {
            cout << "No flashcards available.\n";
            return;
        }
        Node<T>* curr = tail->next;  // Start from head
        int idx = 1;
        do {
            cout << idx++ << ". " << curr->data.title 
                 << " (" << curr->data.questions.size() << " questions)\n";
            curr = curr->next;
        } while (curr != tail->next);  // Complete the circle
    }
};

//////////////////////////////////////////////////////////////
// FLASHCARD MANAGER
// Manages flashcard operations using a circular linked list
// Demonstrates the use of circular linked list for flashcard navigation
//////////////////////////////////////////////////////////////
class FlashcardManager {
private:
    CircularLinkedList<Flashcard> cards;  // Circular linked list storing all flashcards

    /**
     * Display a question in a formatted box and get user's answer
     * @param q The question object to display
     * @param qnum The question number
     */
    void printQuestionBox(const Question& q, int qnum) const {
        cout << "\n+==============================+\n";
        cout << "| Question " << qnum << ":\n";
        cout << "| " << q.question << "\n";
        cout << "+==============================+\n";

        cout << "Your answer: ";
        string userAnswer;
        getline(cin, userAnswer);

        cout << "\n+==============================+\n";
        cout << "| Correct answer:\n";
        cout << "| " << q.answer << "\n";
        cout << "+==============================+\n";

        char correct;
        cout << "Did you answer correctly? (y/n): ";
        cin >> correct;
        cin.ignore();
        cout << endl;
    }

public:
    /**
     * Add a new flashcard to the circular linked list
     * Prompts user for title and questions, then inserts into the list
     */
    void addCard() {
        string title;
        cout << "\n+======== Add New Flashcard ========+\n";
        cout << "Enter flashcard title: ";
        getline(cin, title);

        int numberOfQuestions;
        cout << "Enter number of questions: ";
        cin >> numberOfQuestions;
        cin.ignore();

        Flashcard newFlashcard(title);

        for (int i = 0; i < numberOfQuestions; ++i) {
            string questionText, answerText;
            cout << "\n--- Question " << i+1 << " ---\n";
            cout << "Enter question: ";
            getline(cin, questionText);
            cout << "Enter answer: ";
            getline(cin, answerText);
            newFlashcard.questions.push_back(Question(questionText, answerText));
        }

        cards.insert(newFlashcard);

        // Ask user if they want to save to file
        char saveChoice;
        cout << "\nDo you want to save this flashcard to file? (y/n): ";
        cin >> saveChoice;
        cin.ignore();
        if (saveChoice == 'y') {
            string folderName = "flashcards";
            // Create folder if it doesn't exist
            if (!fs::exists(folderName)) {
                fs::create_directory(folderName);
            }
            string filename = folderName + "/" + title + ".txt";
            ofstream outputFile(filename);
            if (!outputFile) {
                cout << "Error creating file!\n";
                return;
            }
            for (auto& question : newFlashcard.questions) {
                outputFile << question.question << "|" << question.answer << "\n";
            }
            outputFile.close();
            cout << "Flashcard saved to " << filename << endl;
        }
    }

    /**
     * Load all flashcards from the flashcards folder
     * Reads all .txt files in the folder and adds them to the circular list
     */
    void loadFlashcards() {
        string folderName = "flashcards";
        
        // Check if folder exists
        if (!fs::exists(folderName)) {
            cout << "Folder '" << folderName << "' does not exist. Creating it...\n";
            fs::create_directory(folderName);
            return;
        }

        int loadedCount = 0;
        // Iterate through all files in the folder using standard C++ filesystem
        for (const auto& entry : fs::directory_iterator(folderName)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                string filepath = entry.path().string();
                string filename = entry.path().filename().string();
                
                ifstream inputFile(filepath);
                if (!inputFile) continue;

                // Extract title from filename (remove .txt extension)
                string title = filename.substr(0, filename.find_last_of('.'));
                Flashcard newFlashcard(title);

                string line;
                while (getline(inputFile, line)) {
                    if (line.empty()) continue;
                    size_t separatorIndex = line.find('|');
                    if (separatorIndex != string::npos) {
                        string questionText = line.substr(0, separatorIndex);
                        string answerText = line.substr(separatorIndex + 1);
                        newFlashcard.questions.push_back(Question(questionText, answerText));
                    }
                }
                inputFile.close();
                cards.insert(newFlashcard);
                loadedCount++;
            }
        }
        cout << "Loaded " << loadedCount << " flashcard(s) from folder '" << folderName << "'.\n";
    }

    /**
     * Review flashcards in circular order
     * Navigates through the circular linked list, allowing continuous review
     * Demonstrates the circular nature of the linked list
     */
    void reviewCards() {
        if (cards.isEmpty()) {
            cout << "\nNo flashcards loaded.\n";
            return;
        }

        Node<Flashcard>* currentNode = cards.getHead();
        do {
            cout << "\n+==============================+\n";
            cout << "| Flashcard: " << currentNode->data.title << "\n";
            cout << "+==============================+\n";

            for (size_t i = 0; i < currentNode->data.questions.size(); ++i) {
                printQuestionBox(currentNode->data.questions[i], i+1);
            }

            currentNode = currentNode->next;  // Move to next node in circular list

            char continueChoice;
            cout << "Continue to next flashcard? (y/n): ";
            cin >> continueChoice;
            cin.ignore();
            if (continueChoice != 'y') break;

        } while (currentNode != cards.getHead());  // Stop when we complete the circle
    }

    /**
     * Display all flashcards currently in the circular linked list
     */
    void showAll() const {
        cout << "\n--- All Loaded Flashcards ---\n";
        cards.debugPrint();
    }
};

//////////////////////////////////////////////////////////////
// MENU DISPLAY FUNCTION
// Displays the main menu options to the user
//////////////////////////////////////////////////////////////
void showMenu() {
    cout << "\n+====================================+\n";
    cout << "|      FLASHCARD STUDY SYSTEM         |\n";
    cout << "+====================================+\n";
    cout << "| 1. Add Flashcard                    |\n";
    cout << "| 2. Load Flashcards from folder      |\n";
    cout << "| 3. Review Cards                     |\n";
    cout << "| 4. Show All Flashcards              |\n";
    cout << "| 5. Exit                             |\n";
    cout << "+====================================+\n";
    cout << "Enter choice > ";
}

//////////////////////////////////////////////////////////////
// MAIN FUNCTION
// Entry point of the program
// Creates a FlashcardManager and runs the main menu loop
//////////////////////////////////////////////////////////////
int main() {
    FlashcardManager manager;
    int userChoice = 0;

    do {
        showMenu();
        cin >> userChoice;
        cin.ignore();

        switch (userChoice) {
            case 1: 
                manager.addCard(); 
                break;
            case 2: 
                manager.loadFlashcards(); 
                break;
            case 3: 
                manager.reviewCards(); 
                break;
            case 4: 
                manager.showAll(); 
                break;
            case 5: 
                cout << "Goodbye!\n"; 
                break;
            default: 
                cout << "Invalid choice. Please enter 1-5.\n";
        }

    } while (userChoice != 5);

    return 0;
}
