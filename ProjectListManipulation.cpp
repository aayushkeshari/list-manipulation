#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

class CustomError : public std::runtime_error {
public:
    CustomError(const std::string& message) : std::runtime_error(message) {}
};

template <typename T>
class OrderedList {
public:
    OrderedList() {
        for (int i = 0; i < 25; i++) {
            arr[i] = nullptr;
        }
    }

    void AddItem(const T& item) {
        if (IsFull()) {
            throw CustomError("List is full. Cannot add more items.");
        }

        int index = 0;
        while (index < 25 && arr[index] != nullptr && item > *arr[index]) {
            comparisonCount++; // Counts the comparisons
            index++;
        }

        for (int i = 24; i > index; i--) {
            if (arr[i] != nullptr) {
                arr[i] = new T(*arr[i - 1]);
                moveCount++; // Counts the moves
                delete arr[i - 1];
            }
        }

        arr[index] = new T(item);
    }

    void RemoveItem(const T& item) {
        int index = 0;
        while (index < 25 && (arr[index] == nullptr || item != *arr[index])) {
            index++;
        }

        if (index == 25 || arr[index] == nullptr) {
            std::cerr << "Error: Item not found in the list." << std::endl;
            return;
        }

        delete arr[index];
        arr[index] = nullptr;

        // Shifts the elements to close the gap.
        for (int i = index; i < 24; i++) {
            if (arr[i + 1] != nullptr) {
                arr[i] = arr[i + 1];
                arr[i + 1] = nullptr;
            }
        }
        arr[24] = nullptr;
    }

    bool IsEmpty() const {
        return arr[0] == nullptr;
    }

    bool IsFull() const {
        return arr[24] != nullptr;
    }

    void MakeEmpty() {
        for (int i = 0; i < 25; i++) {
            if (arr[i] != nullptr) {
                delete arr[i];
                arr[i] = nullptr;
            }
        }
    }

    static int comparisonCount;
    static int moveCount;

protected:
    T* arr[25];
};

template <typename T>
int OrderedList<T>::comparisonCount = 0;

template <typename T>
int OrderedList<T>::moveCount = 0;

template <typename T>
class OrderedListMiddle : public OrderedList<T> {
public:
    void AddItem(const T& item) {
        if (this->IsFull()) {
            throw CustomError("List is full. Cannot add more items.");
        }

        int index = 12;

        while (index < 25 && this->arr[index] != nullptr && item > *this->arr[index]) {
            OrderedList<T>::comparisonCount++; // Counts the comparisons
            index++;
        }

        if (index == 25) {
            for (int i = 11; i >= 0; i--) {
                if (this->arr[i] != nullptr) {
                    this->arr[i + 1] = new T(*this->arr[i]);
                    OrderedList<T>::moveCount++; // Counts the moves
                    delete this->arr[i];
                    this->arr[i] = nullptr;
                }
            }
            index = 12;
        }

        for (int i = 24; i > index; i--) {
            if (this->arr[i] != nullptr) {
                this->arr[i] = new T(*this->arr[i - 1]);
                OrderedList<T>::moveCount++; // Counts the moves
                delete this->arr[i - 1];
            }
        }

        this->arr[index] = new T(item);
    }

    void RemoveItem(const T& item) {
        int index = 0;
        while (index < 25 && (this->arr[index] == nullptr || item != *(this->arr[index]))) {
            index++;
        }

        if (index == 25 || this->arr[index] == nullptr) {
            std::cerr << "Error: Item not found in the list." << std::endl;
            return;
        }

        delete this->arr[index];
        this->arr[index] = nullptr;

        // Shifts elements to close the gap.
        for (int i = index; i < 24; i++) {
            if (this->arr[i + 1] != nullptr) {
                this->arr[i] = this->arr[i + 1];
                this->arr[i + 1] = nullptr;
            }
        }
        this->arr[24] = nullptr;
    }
};

template <typename T>
class OrderedListWithGaps : public OrderedList<T> {
public:
    void AddItem(const T& item) {
        if (this->IsFull()) {
            throw CustomError("List is full. Cannot add more items.");
        }

        if (this->IsEmpty()) {
            this->arr[12] = new T(item);
        }
        else {
            int index = FindInsertionIndex(item);
            if (index >= 0) {
                InsertWithGap(index, item);
            }
            else {
                int end = this->FindEnd();
                if (end < 24) {
                    InsertWithGap(end + 1, item);
                }
                else {
                    throw CustomError("List is full. Cannot add more items.");
                }
            }
        }
    }

    void RemoveItem(const T& item) {
        int index = 0;
        while (index < 25 && (this->arr[index] == nullptr || item != *(this->arr[index]))) {
            index++;
        }

        if (index == 25 || this->arr[index] == nullptr) {
            std::cerr << "Error: Item not found in the list." << std::endl;
            return;
        }

        delete this->arr[index];
        this->arr[index] = nullptr;

        // Shifts elements to close the gap.
        for (int i = index; i < 24; i++) {
            if (this->arr[i + 1] != nullptr) {
                this->arr[i] = this->arr[i + 1];
                this->arr[i + 1] = nullptr;
            }
        }
        this->arr[24] = nullptr;
    }

private:
    int FindInsertionIndex(const T& item) {
        for (int i = 0; i < 24; i++) {
            if (this->arr[i] != nullptr && this->arr[i + 1] != nullptr) {
                if (item >= *(this->arr[i]) && item < *(this->arr[i + 1])) {
                    return i;
                }
            }
        }
        return -1;
    }

    void InsertWithGap(int index, const T& item) {
        for (int i = 24; i > index; i--) {
            if (this->arr[i] != nullptr) {
                this->arr[i] = new T(*this->arr[i - 1]);
                OrderedList<T>::moveCount++; // Counts the moves
                delete this->arr[i - 1];
            }
        }
        this->arr[index] = new T(item);
    }

    int FindEnd() {
        for (int i = 0; i < 25; i++) {
            if (this->arr[i] == nullptr) {
                return i;
            }
        }
        return 24;
    }
};

int RandomInt(int min, int max) {
    return min + (rand() % (max - min + 1));
}

template <typename T>
void RunTest(OrderedList<T>& orderedList, const std::string& className) {
    OrderedList<T>::comparisonCount = 0;
    OrderedList<T>::moveCount = 0;

    for (int i = 0; i < 30; i++) {
        int item = RandomInt(0, 100);
        try {
            orderedList.AddItem(item);
        }
        catch (const CustomError&) {
            std::cerr << "Error: List is full during insertion." << std::endl;
        }
    }

    for (int i = 0; i < 25; i++) {
        int item = RandomInt(0, 100);
        orderedList.RemoveItem(item);
    }

    std::cout << "Results for " << className << ":" << std::endl;
    std::cout << "Total comparisons: " << OrderedList<T>::comparisonCount << std::endl;
    std::cout << "Total moves: " << OrderedList<T>::moveCount << std::endl;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    OrderedList<int> orderedList;
    OrderedListMiddle<int> middleList;
    OrderedListWithGaps<int> gapsList;

    for (int i = 0; i < 100; i++) {
        std::cout << "Initiating Test Number " << i+1 << ": " << std::endl;
        RunTest(orderedList, "OrderedList");
        RunTest(middleList, "OrderedListMiddle");
        RunTest(gapsList, "OrderedListWithGaps");

        std::cout << "----------------------------------" << std::endl;

        orderedList.MakeEmpty();
        middleList.MakeEmpty();
        gapsList.MakeEmpty();
    }

    return 0;
}
