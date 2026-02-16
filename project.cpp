#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>   // đọc folder
using namespace std;

//////////////////////////////////////////////////////////////
// QUESTION CLASS
//////////////////////////////////////////////////////////////
class Question {
public:
    string question;
    string answer;

    Question() = default;
    Question(const string& q, const string& a) : question(q), answer(a) {}
};

//////////////////////////////////////////////////////////////
// FLASHCARD CLASS
//////////////////////////////////////////////////////////////
class Flashcard {
public:
    string title;
    vector<Question> questions;

    Flashcard() = default;
    Flashcard(const string& t) : title(t) {}
};

//////////////////////////////////////////////////////////////
// NODE TEMPLATE
//////////////////////////////////////////////////////////////
template <typename T>
class Node {
public:
    T data;
    Node<T>* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

//////////////////////////////////////////////////////////////
// CIRCULAR LINKED LIST TEMPLATE
//////////////////////////////////////////////////////////////
template <typename T>
class CircularLinkedList {
private:
    Node<T>* tail;

public:
    CircularLinkedList() : tail(nullptr) {}

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

    void debugPrint() const {
        if (!tail) {
            cout << "No flashcards available.\n";
            return;
        }
        Node<T>* curr = tail->next;
        int idx = 1;
        do {
            cout << idx++ << ". " << curr->data.title 
                 << " (" << curr->data.questions.size() << " questions)\n";
            curr = curr->next;
        } while (curr != tail->next);
    }
};

//////////////////////////////////////////////////////////////
// FLASHCARD MANAGER
//////////////////////////////////////////////////////////////
class FlashcardManager {
private:
    CircularLinkedList<Flashcard> cards;

    void printQuestionBox(const Question& q, int qnum) const {
        cout << "\n╔════════════════════════════════╗\n";
        cout << "║ Question " << qnum << ":\n";
        cout << "║ " << q.question << "\n";
        cout << "╚════════════════════════════════╝\n";

        cout << "Your answer: ";
        string userAnswer;
        getline(cin, userAnswer);

        cout << "\n╔════════════════════════════════╗\n";
        cout << "║ Correct answer:\n";
        cout << "║ " << q.answer << "\n";
        cout << "╚════════════════════════════════╝\n";

        char correct;
        cout << "Did you answer correctly? (y/n): ";
        cin >> correct;
        cin.ignore();
        cout << endl;
    }

public:
    void addCard() {
        string title;
        cout << "\n╔════════ Add New Flashcard ════════╗\n";
        cout << "Enter flashcard title: ";
        getline(cin, title);

        int n;
        cout << "Enter number of questions: ";
        cin >> n;
        cin.ignore();

        Flashcard fc(title);

        for (int i = 0; i < n; ++i) {
            string q, a;
            cout << "\n--- Question " << i+1 << " ---\n";
            cout << "Enter question: ";
            getline(cin, q);
            cout << "Enter answer: ";
            getline(cin, a);
            fc.questions.push_back(Question(q, a));
        }

        cards.insert(fc);

        // Ask to save
        char save;
        cout << "\nDo you want to save this flashcard to file? (y/n): ";
        cin >> save;
        cin.ignore();
        if (save == 'y') {
            string folder = "flashcards"; // folder đã tạo sẵn
            string filename = folder + "/" + title + ".txt";
            ofstream fout(filename);
            if (!fout) {
                cout << "Error creating file!\n";
                return;
            }
            for (auto& q : fc.questions) {
                fout << q.question << "|" << q.answer << "\n";
            }
            fout.close();
            cout << "Flashcard saved to " << filename << endl;
        }
    }

    void loadFlashcards() {
        string folder = "flashcards"; // folder đã tạo sẵn
        DIR* dir = opendir(folder.c_str());
        if (!dir) {
            cout << "Folder '" << folder << "' does not exist.\n";
            return;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename == "." || filename == "..") continue;

            string filepath = folder + "/" + filename;
            ifstream fin(filepath);
            if (!fin) continue;

            string title = filename.substr(0, filename.find_last_of('.'));
            Flashcard fc(title);

            string line;
            while (getline(fin, line)) {
                size_t sep = line.find('|');
                if (sep != string::npos) {
                    string q = line.substr(0, sep);
                    string a = line.substr(sep + 1);
                    fc.questions.push_back(Question(q, a));
                }
            }
            fin.close();
            cards.insert(fc);
        }
        closedir(dir);
        cout << "Loaded all flashcards from folder '" << folder << "'.\n";
    }

    void reviewCards() {
        if (cards.isEmpty()) {
            cout << "\nNo flashcards loaded.\n";
            return;
        }

        Node<Flashcard>* curr = cards.getHead();
        do {
            cout << "\n╔════════════════════════════════╗\n";
            cout << "║ Flashcard: " << curr->data.title << "\n";
            cout << "╚════════════════════════════════╝\n";

            for (size_t i = 0; i < curr->data.questions.size(); ++i) {
                printQuestionBox(curr->data.questions[i], i+1);
            }

            curr = curr->next;

            char cont;
            cout << "Continue to next flashcard? (y/n): ";
            cin >> cont;
            cin.ignore();
            if (cont != 'y') break;

        } while (curr != cards.getHead());
    }

    void showAll() const {
        cout << "\n--- All Loaded Flashcards ---\n";
        cards.debugPrint();
    }
};

//////////////////////////////////////////////////////////////
// MENU
//////////////////////////////////////////////////////////////
void showMenu() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║      FLASHCARD STUDY SYSTEM          ║\n";
    cout << "╠══════════════════════════════════════╣\n";
    cout << "║ 1. Add Flashcard                     ║\n";
    cout << "║ 2. Load Flashcards from folder       ║\n";
    cout << "║ 3. Review Cards                      ║\n";
    cout << "║ 4. Show All Flashcards               ║\n";
    cout << "║ 5. Exit                              ║\n";
    cout << "╚══════════════════════════════════════╝\n";
    cout << "Enter choice ➜ ";
}

//////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////
int main() {
    FlashcardManager manager;
    int choice = 0;

    do {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: manager.addCard(); break;
            case 2: manager.loadFlashcards(); break;
            case 3: manager.reviewCards(); break;
            case 4: manager.showAll(); break;
            case 5: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}
