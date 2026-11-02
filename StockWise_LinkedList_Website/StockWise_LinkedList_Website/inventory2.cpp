// StockWise - Inventory Stock Management using Singly Linked List
// Beginner-friendly C++ DSA project
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Node {
    int id;
    string name;
    int quantity;
    float price;
    Node *next;
};

Node *head = NULL;

Node* findProduct(int id);

void addProduct() {
    Node *newNode = new Node;
    cout << "\nEnter product ID (number): ";
    cin >> newNode->id;
    if (findProduct(newNode->id) != NULL) {
        cout << "This product ID already exists.\n";
        delete newNode;
        return;
    }
    cin.ignore();
    cout << "Enter product name: ";
    getline(cin, newNode->name);
    cout << "Enter quantity: ";
    cin >> newNode->quantity;
    cout << "Enter unit price: ";
    cin >> newNode->price;

    newNode->next = head;
    head = newNode;
    cout << "Product added successfully.\n";
}

Node* findProduct(int id) {
    Node *temp = head;
    while (temp != NULL) {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void displayProducts() {
    if (head == NULL) {
        cout << "\nInventory is empty.\n";
        return;
    }

    cout << "\n==================== STOCKWISE INVENTORY ====================\n";
    cout << "ID      Name                    Quantity    Unit Price\n";
    cout << "-------------------------------------------------------------\n";

    Node *current = head;

    while (current != NULL) {
        cout << current->id << "       "
             << current->name << "                    "
             << current->quantity << "           Rs. "
             << current->price << "\n";

        current = current->next;
    }

    cout << "=============================================================\n";
}
void searchProduct() {
    int id;
    cout << "\nEnter product ID to search: ";
    cin >> id;
    Node *product = findProduct(id);
    if (product == NULL) {
        cout << "Product not found.\n";
    } else {
        cout << "\nProduct found!\n";
        cout << "ID: " << product->id << "\n";
        cout << "Name: " << product->name << "\n";
        cout << "Quantity: " << product->quantity << "\n";
        cout << "Unit price: " << fixed << setprecision(2) << product->price << "\n";
    }
}

void updateStock() {
    int id, change, choice;
    cout << "\nEnter product ID: ";
    cin >> id;
    Node *product = findProduct(id);
    if (product == NULL) {
        cout << "Product not found.\n";
        return;
    }

    cout << "Current quantity: " << product->quantity << "\n";
    cout << "1. Add stock\n2. Sell stock\nChoose: ";
    cin >> choice;
    cout << "Enter quantity: ";
    cin >> change;

    if (change <= 0) {
        cout << "Quantity must be greater than zero.\n";
    } else if (choice == 1) {
        product->quantity += change;
        cout << "Stock added successfully.\n";
    } else if (choice == 2) {
        if (change > product->quantity)
            cout << "Not enough stock available.\n";
        else {
            product->quantity -= change;
            cout << "Sale recorded successfully.\n";
        }
    } else {
        cout << "Invalid choice.\n";
    }
}

void deleteProduct() {
    int id;
    cout << "\nEnter product ID to delete: ";
    cin >> id;

    if (head == NULL) {
        cout << "Inventory is empty.\n";
        return;
    }

    if (head->id == id) {
        Node *temp = head;
        head = head->next;
        delete temp;
        cout << "Product deleted successfully.\n";
        return;
    }

    Node *current = head;
    while (current->next != NULL && current->next->id != id)
        current = current->next;

    if (current->next == NULL) {
        cout << "Product not found.\n";
    } else {
        Node *temp = current->next;
        current->next = temp->next;
        delete temp;
        cout << "Product deleted successfully.\n";
    }
}

void showLowStock() {
    Node *current = head;
    bool found = false;
    cout << "\nProducts with 5 or fewer units:\n";
    while (current != NULL) {
        if (current->quantity <= 5) {
            cout << current->id << " - " << current->name
                 << " (Quantity: " << current->quantity << ")\n";
            found = true;
        }
        current = current->next;
    }
    if (!found)
        cout << "No low-stock products.\n";
}

void showTotalValue() {
    Node *current = head;
    float total = 0;
    while (current != NULL) {
        total += current->quantity * current->price;
        current = current->next;
    }
    cout << "\nTotal inventory value: Rs. " << fixed << setprecision(2) << total << "\n";
}

void freeList() {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    int choice;
    do {
        cout << "\n========== STOCKWISE INVENTORY ==========\n";
        cout << "1. Add product\n";
        cout << "2. Display all products\n";
        cout << "3. Search product by ID\n";
        cout << "4. Update stock / record sale\n";
        cout << "5. Delete product\n";
        cout << "6. Show low-stock products\n";
        cout << "7. Calculate total inventory value\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addProduct(); break;
            case 2: displayProducts(); break;
            case 3: searchProduct(); break;
            case 4: updateStock(); break;
            case 5: deleteProduct(); break;
            case 6: showLowStock(); break;
            case 7: showTotalValue(); break;
            case 0: cout << "Exiting StockWise. Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    freeList();
    return 0;
}