# Flashcard Study System

## Project Overview

This project implements a **Flashcard Study System** using a **Circularly Linked List** data structure. The program allows users to create, save, load, and review flashcards in a continuous circular manner, making it ideal for study sessions.

## Key Features

- **Circular Linked List Implementation**: Template-based circular linked list that can store any data type
- **Flashcard Management**: Create flashcards with multiple question-answer pairs
- **File Persistence**: Save and load flashcards from text files
- **Circular Navigation**: Review flashcards in a continuous loop, demonstrating the circular nature of the linked list
- **User-Friendly Interface**: Menu-driven system with formatted output

## Data Structures

### CircularLinkedList<T>
A template class implementing a circularly linked list where the last node points back to the first node, creating a circular structure. Key methods:
- `insert(T value)`: Insert a new element at the end
- `isEmpty()`: Check if the list is empty
- `getHead()`: Get the first node
- `getTail()`: Get the last node
- `getSize()`: Get the number of elements
- `debugPrint()`: Print all elements

### Node<T>
Template class representing a node in the circular linked list, containing:
- `data`: The stored data of type T
- `next`: Pointer to the next node

### Flashcard
Class representing a flashcard set with:
- `title`: Name of the flashcard set
- `questions`: Vector of Question objects

### Question
Class representing a question-answer pair with:
- `question`: The question text
- `answer`: The answer text

## Compilation

This program requires **C++17** or later due to the use of `<filesystem>` header.

```bash
g++ -std=c++17 -o project project.cpp
```

## Usage

1. **Add Flashcard**: Create a new flashcard set with a title and multiple questions
2. **Load Flashcards**: Load all flashcards from the `flashcards/` folder
3. **Review Cards**: Navigate through flashcards in circular order
4. **Show All Flashcards**: Display all loaded flashcards
5. **Exit**: Exit the program

## File Format

Flashcards are saved as text files in the `flashcards/` folder. Each line contains a question and answer separated by a pipe character (`|`):

```
What is the capital of France?|Paris
What is 2+2?|4
```

## Project Requirements Met

✅ **Circularly Linked List**: Fully implemented with template support  
✅ **Template-based**: Accepts arbitrary element types  
✅ **Original Design**: Flashcard study system (creative application)  
✅ **Documentation**: All functions and classes are well-documented  
✅ **Clear Naming**: Variables and functions have descriptive names  
✅ **Clean Code**: Follows coding best practices  
✅ **Standard Libraries Only**: Uses only C++ standard library

## Design Highlights

The circular linked list is particularly well-suited for this application because:
- Users can continuously cycle through flashcards without reaching an "end"
- The circular structure naturally supports repeated study sessions
- Navigation demonstrates the circular nature of the data structure
