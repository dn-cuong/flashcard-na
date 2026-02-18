
# Flashcard Study System 

## 1. Project Name
**Flashcard Study System**

---

## 2. Overview
The Flashcard Study System is a C++17 console application that provides a new way to retain information through the use of flashcards. With this application, users won't end up at a 'dead end,' unlike the usual linear lists. This is accomplished through the use of a Circular Linked List, a data structure that allows users to continually cycle through their card sets, mimicking an actual stack of flashcards. The system offers a form of persistent storage by saving the user's decks as text files within the user's local storage.


---

## 3. Group Members & Work Distribution

| Student Name | Contribution |
|--------------|--------------|
| Dinh Gia Bao Ngo - U30653179 | 34% |
| Dang Cuong Nguyen - U96338337 | 33% |
| Khai Dong Nguyen - U76714282 | 33% |

---

## 4. How to Run the Program

### Compile
```bash
g++ -std=c++17 -o project project.cpp
````

### Run

**Windows**

```bash
project.exe
```

**Linux / macOS / Student Cluster**

```bash
./project
```

The program will automatically create a folder named `flashcards/` if it does not already exist.

---

## 5. User Interaction Guide



### File Format

Flashcards are saved as text files inside the `flashcards/` folder.
Each line contains:

```
question|answer
```

---

### Main Menu Navigation

After launching, select an option by entering its number.

---

### A. Creating a New Flashcard Set

1. Select **[1] Add Flashcard**
2. Enter a unique title
3. Enter number of questions
4. For each question:

   * Enter question text
   * Enter answer text
5. Choose whether to save to file (`y/n`)

---

### B. Editing an Existing Set

1. Select **[2] Edit Flashcard**
2. Choose the deck index
3. Options:

   * Edit title
   * Edit question/answer
   * Add question
   * Delete question
   * Save changes
   * Return to menu

---

### C. Reviewing (Study Mode)

1. Select **[5] Review Cards**
2. Choose shuffle mode (`y/n`)
3. Select deck index
4. For each question:

   * Type your answer
   * View correct answer
   * Mark yourself correct/incorrect
5. Continue to next deck or exit

---

### D. Deleting a Flashcard Set

1. Select **[3] Delete Flashcard**
2. Choose deck index
3. Confirm deletion (`y/n`)
4. Deck is removed from memory and disk

---

### E. Reloading Data

1. Select **[4] Reload Flashcards**
2. Program rescans the `flashcards/` directory and rebuilds the linked list

---

### F. Displaying All Decks

1. Select **[6] Show All Flashcards**
2. Displays title and number of questions for each deck

---

### G. Exiting

Select **[7] Exit** to close the application.

---

## 6. Academic Integrity Statement

We understand that there will be no tolerance towards academic dishonesty, and that cheating will lead to an academic referral. We are aware of the identified behaviors that are considered violations of the academic standards for Undergraduate and Graduate students per USF policy.

