/*
 * Flashcard Study System
 *
 * Compilation: g++ -std=c++17 -o project project.cpp
 * Author: Group Project
 * Date: February 2026
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

class Question {
public:
    string question;
    string answer;
    Question() = default;
    Question(const string& q, const string& a) : question(q), answer(a) {}
};

class Flashcard {
public:
    string title;
    vector<Question> questions;
    Flashcard() = default;
    Flashcard(const string& t) : title(t) {}
};

template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class CircularLinkedList {
private:
    Node<T>* tail;
public:
    CircularLinkedList() : tail(nullptr) {}

    CircularLinkedList& operator=(CircularLinkedList&& other) noexcept {
        if (this != &other) {
            if (tail) {
                Node<T>* curr = tail->next;
                Node<T>* nextNode;
                while (curr != tail) {
                    nextNode = curr->next;
                    delete curr;
                    curr = nextNode;
                }
                delete tail;
            }
            tail = other.tail;
            other.tail = nullptr;
        }
        return *this;
    }

    ~CircularLinkedList() {
        if (!tail) return;
        Node<T>* curr = tail->next;
        Node<T>* nextNode;
        while (curr != tail) {
            nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        delete tail;
    }

    bool isEmpty() const { return tail == nullptr; }
    Node<T>* getHead() const { return tail ? tail->next : nullptr; }
    Node<T>* getTail() const { return tail; }

    void insert(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!tail) {
            tail = newNode;
            tail->next = tail;
        } else {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
    }

    int getSize() const {
        if (!tail) return 0;
        int count = 0;
        Node<T>* curr = tail->next;
        do {
            count++;
            curr = curr->next;
        } while (curr != tail->next);
        return count;
    }
};

//////////////////////////////////////////////////////////////
// FLASHCARD MANAGER
//////////////////////////////////////////////////////////////
class FlashcardManager {
private:
    CircularLinkedList<Flashcard> cards;
    const string folderName = "flashcards";

    // Kiem tra title trung trong memory (bo qua node dang edit)
    bool titleExistsInMemory(const string& title, const string& excludeTitle = "") const {
        if (cards.isEmpty()) return false;
        Node<Flashcard>* curr = cards.getHead();
        do {
            if (curr->data.title == title && curr->data.title != excludeTitle)
                return true;
            curr = curr->next;
        } while (curr != cards.getHead());
        return false;
    }

    // Luu flashcard xuong file
    void saveToFile(const Flashcard& fc) const {
        if (!fs::exists(folderName)) {
            fs::create_directory(folderName);
        }
        string filename = folderName + "/" + fc.title + ".txt";
        ofstream outputFile(filename);
        if (!outputFile) {
            cout << "Error saving file!\n";
            return;
        }
        for (const auto& q : fc.questions) {
            outputFile << q.question << "|" << q.answer << "\n";
        }
        outputFile.close();
        cout << "Saved to " << filename << "\n";
    }

    // Xoa file cu (dung khi doi ten title)
    void deleteFile(const string& title) const {
        string filename = folderName + "/" + title + ".txt";
        if (fs::exists(filename)) {
            fs::remove(filename);
        }
    }

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
        while (true) {
            cout << "Did you answer correctly? (y/n): ";
            cin >> correct;
            cin.ignore();
            correct = tolower(correct);
            if (correct == 'y' || correct == 'n') break;
            cout << "Invalid input! Please enter 'y' or 'n'.\n";
        }

        if (correct == 'y') {
            cout << "Correct!\n";
        } else {
            cout << "Incorrect. Review this question again!\n";
        }
        cout << endl;
    }

    // Chon so nguyen hop le trong khoang [min, max]
    int inputInt(int min, int max) const {
        int val;
        while (true) {
            cin >> val;
            cin.ignore();
            if (cin.fail() || val < min || val > max) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number between "
                     << min << " and " << max << ": ";
            } else {
                break;
            }
        }
        return val;
    }

public:
    void addCard() {
        string title;
        cout << "\n+======== Add New Flashcard ========+\n";

        while (true) {
            cout << "Enter flashcard title: ";
            getline(cin, title);

            if (title.empty()) {
                cout << "Title cannot be empty! Please enter again.\n";
                continue;
            }
            if (titleExistsInMemory(title)) {
                cout << "Title \"" << title << "\" already exists! Please enter a different title.\n";
                continue;
            }
            string filename = folderName + "/" + title + ".txt";
            if (fs::exists(filename)) {
                cout << "Title \"" << title << "\" already exists in folder! Please enter a different title.\n";
                continue;
            }
            break;
        }

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

        char saveChoice;
        while (true) {
            cout << "\nDo you want to save this flashcard to file? (y/n): ";
            cin >> saveChoice;
            cin.ignore();
            saveChoice = tolower(saveChoice);
            if (saveChoice == 'y' || saveChoice == 'n') break;
            cout << "Invalid input! Please enter 'y' or 'n'.\n";
        }
        if (saveChoice == 'y') {
            saveToFile(newFlashcard);
        }
    }

    //////////////////////////////////////////////////////////////
    // EDIT CARD
    //////////////////////////////////////////////////////////////
    void editCard() {
        if (cards.isEmpty()) {
            cout << "\nNo flashcards loaded.\n";
            return;
        }

        // Buoc 1: Hien thi danh sach va chon flashcard
        cout << "\n+======== Select Flashcard to Edit ========+\n";
        Node<Flashcard>* curr = cards.getHead();
        int idx = 1;
        do {
            cout << "| " << idx++ << ". " << curr->data.title
                 << " (" << curr->data.questions.size() << " questions)\n";
            curr = curr->next;
        } while (curr != cards.getHead());
        cout << "+==========================================+\n";

        int totalCards = cards.getSize();
        cout << "Enter flashcard number (1-" << totalCards << "): ";
        int choice = inputInt(1, totalCards);

        // Di chuyen den node duoc chon
        Node<Flashcard>* targetNode = cards.getHead();
        for (int i = 1; i < choice; ++i) {
            targetNode = targetNode->next;
        }

        Flashcard& fc = targetNode->data;  // Reference truc tiep de sua trong memory

        // Buoc 2: Menu chinh sua
        bool editDone = false;
        while (!editDone) {
            cout << "\n+======== Edit: \"" << fc.title << "\" ========+\n";
            cout << "| 1. Edit title                           |\n";
            cout << "| 2. Edit a question                      |\n";
            cout << "| 3. Add new question                     |\n";
            cout << "| 4. Delete a question                    |\n";
            cout << "| 5. Save changes to file                 |\n";
            cout << "| 6. Done (back to main menu)             |\n";
            cout << "+=========================================+\n";
            cout << "Enter choice > ";
            int editChoice = inputInt(1, 6);

            switch (editChoice) {

                // --- Sua title ---
                case 1: {
                    string newTitle;
                    while (true) {
                        cout << "Enter new title (current: \"" << fc.title << "\"): ";
                        getline(cin, newTitle);

                        if (newTitle.empty()) {
                            cout << "Title cannot be empty!\n";
                            continue;
                        }
                        if (newTitle == fc.title) {
                            cout << "That is the same title!\n";
                            continue;
                        }
                        if (titleExistsInMemory(newTitle, fc.title)) {
                            cout << "Title \"" << newTitle << "\" already exists! Please enter a different title.\n";
                            continue;
                        }
                        string newFilename = folderName + "/" + newTitle + ".txt";
                        if (fs::exists(newFilename)) {
                            cout << "Title \"" << newTitle << "\" already exists in folder! Please enter a different title.\n";
                            continue;
                        }
                        break;
                    }
                    // Xoa file cu neu ton tai
                    deleteFile(fc.title);
                    cout << "Title changed: \"" << fc.title << "\" -> \"" << newTitle << "\"\n";
                    fc.title = newTitle;
                    break;
                }

                // --- Sua cau hoi ---
                case 2: {
                    if (fc.questions.empty()) {
                        cout << "No questions to edit!\n";
                        break;
                    }
                    cout << "\n--- Questions in \"" << fc.title << "\" ---\n";
                    for (size_t i = 0; i < fc.questions.size(); ++i) {
                        cout << i+1 << ". Q: " << fc.questions[i].question << "\n";
                        cout << "   A: " << fc.questions[i].answer << "\n";
                    }
                    cout << "Enter question number to edit (1-" << fc.questions.size() << "): ";
                    int qChoice = inputInt(1, (int)fc.questions.size());
                    Question& q = fc.questions[qChoice - 1];

                    cout << "\n--- Editing Question " << qChoice << " ---\n";
                    cout << "| 1. Edit question text\n";
                    cout << "| 2. Edit answer\n";
                    cout << "| 3. Edit both\n";
                    cout << "Enter choice > ";
                    int subChoice = inputInt(1, 3);

                    if (subChoice == 1 || subChoice == 3) {
                        cout << "Current question: " << q.question << "\n";
                        cout << "New question: ";
                        getline(cin, q.question);
                    }
                    if (subChoice == 2 || subChoice == 3) {
                        cout << "Current answer: " << q.answer << "\n";
                        cout << "New answer: ";
                        getline(cin, q.answer);
                    }
                    cout << "Question updated!\n";
                    break;
                }

                // --- Them cau hoi moi ---
                case 3: {
                    string questionText, answerText;
                    cout << "\n--- Add New Question ---\n";
                    cout << "Enter question: ";
                    getline(cin, questionText);
                    cout << "Enter answer: ";
                    getline(cin, answerText);
                    fc.questions.push_back(Question(questionText, answerText));
                    cout << "Question added! Total questions: " << fc.questions.size() << "\n";
                    break;
                }

                // --- Xoa cau hoi ---
                case 4: {
                    if (fc.questions.empty()) {
                        cout << "No questions to delete!\n";
                        break;
                    }
                    cout << "\n--- Questions in \"" << fc.title << "\" ---\n";
                    for (size_t i = 0; i < fc.questions.size(); ++i) {
                        cout << i+1 << ". Q: " << fc.questions[i].question << "\n";
                        cout << "   A: " << fc.questions[i].answer << "\n";
                    }
                    cout << "Enter question number to delete (1-" << fc.questions.size() << "): ";
                    int delChoice = inputInt(1, (int)fc.questions.size());

                    char confirm;
                    while (true) {
                        cout << "Delete question " << delChoice << " \"" << fc.questions[delChoice-1].question << "\"? (y/n): ";
                        cin >> confirm;
                        cin.ignore();
                        confirm = tolower(confirm);
                        if (confirm == 'y' || confirm == 'n') break;
                        cout << "Invalid input! Please enter 'y' or 'n'.\n";
                    }
                    if (confirm == 'y') {
                        fc.questions.erase(fc.questions.begin() + delChoice - 1);
                        cout << "Question deleted! Total questions: " << fc.questions.size() << "\n";
                    } else {
                        cout << "Cancelled.\n";
                    }
                    break;
                }

                // --- Luu file ---
                case 5: {
                    saveToFile(fc);
                    break;
                }

                // --- Thoat ---
                case 6: {
                    editDone = true;
                    cout << "Done editing \"" << fc.title << "\".\n";
                    break;
                }
            }
        }
    }

    void loadFlashcards() {
        if (!fs::exists(folderName)) {
            fs::create_directory(folderName);
            return;
        }

        cards = CircularLinkedList<Flashcard>();

        int loadedCount = 0;
        for (const auto& entry : fs::directory_iterator(folderName)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                string filepath = entry.path().string();
                string filename = entry.path().filename().string();

                ifstream inputFile(filepath);
                if (!inputFile) continue;

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

        cout << "Loaded " << loadedCount << " flashcard(s).\n";
    }

    void reviewCards() {
        if (cards.isEmpty()) {
            cout << "\nNo flashcards loaded.\n";
            return;
        }

        cout << "\n+======== Select Flashcard to Review ========+\n";
        Node<Flashcard>* curr = cards.getHead();
        int idx = 1;
        do {
            cout << "| " << idx++ << ". " << curr->data.title
                 << " (" << curr->data.questions.size() << " questions)\n";
            curr = curr->next;
        } while (curr != cards.getHead());
        cout << "+============================================+\n";

        int totalCards = cards.getSize();
        cout << "Enter flashcard number (1-" << totalCards << "): ";
        int choice = inputInt(1, totalCards);

        Node<Flashcard>* currentNode = cards.getHead();
        for (int i = 1; i < choice; ++i) {
            currentNode = currentNode->next;
        }

        Node<Flashcard>* startNode = currentNode;
        do {
            cout << "\n+==============================+\n";
            cout << "| Flashcard: " << currentNode->data.title << "\n";
            cout << "+==============================+\n";

            for (size_t i = 0; i < currentNode->data.questions.size(); ++i) {
                printQuestionBox(currentNode->data.questions[i], i + 1);
            }

            currentNode = currentNode->next;

            if (currentNode == startNode) {
                cout << "\n[You have completed a full cycle through all flashcards!]\n";
            }

            char continueChoice;
            while (true) {
                cout << "Continue to next flashcard (Flashcard: " << currentNode->data.title << ")? (y/n): ";
                cin >> continueChoice;
                cin.ignore();
                continueChoice = tolower(continueChoice);
                if (continueChoice == 'y' || continueChoice == 'n') break;
                cout << "Invalid input! Please enter 'y' or 'n'.\n";
            }
            if (continueChoice != 'y') break;

        } while (true);
    }

    void showAll() const {
        cout << "\n--- All Loaded Flashcards ---\n";
        if (cards.isEmpty()) {
            cout << "No flashcards available.\n";
            return;
        }
        Node<Flashcard>* curr = cards.getHead();
        int idx = 1;
        do {
            cout << idx++ << ". " << curr->data.title
                 << " (" << curr->data.questions.size() << " questions)\n";
            curr = curr->next;
        } while (curr != cards.getHead());
    }
};

//////////////////////////////////////////////////////////////
// MENU
//////////////////////////////////////////////////////////////
void showMenu() {
    cout << "\n+====================================+\n";
    cout << "|      FLASHCARD STUDY SYSTEM         |\n";
    cout << "+====================================+\n";
    cout << "| 1. Add Flashcard                    |\n";
    cout << "| 2. Edit Flashcard                   |\n";
    cout << "| 3. Reload Flashcards from folder    |\n";
    cout << "| 4. Review Cards                     |\n";
    cout << "| 5. Show All Flashcards              |\n";
    cout << "| 6. Exit                             |\n";
    cout << "+====================================+\n";
    cout << "Enter choice > ";
}

int main() {
    FlashcardManager manager;
    int userChoice = 0;

    manager.loadFlashcards();

    do {
        showMenu();
        cin >> userChoice;
        cin.ignore();

        switch (userChoice) {
            case 1: manager.addCard();        break;
            case 2: manager.editCard();       break;
            case 3: manager.loadFlashcards(); break;
            case 4: manager.reviewCards();    break;
            case 5: manager.showAll();        break;
            case 6: cout << "Goodbye!\n";     break;
            default: cout << "Invalid choice. Please enter 1-6.\n";
        }

    } while (userChoice != 6);

    return 0;
}