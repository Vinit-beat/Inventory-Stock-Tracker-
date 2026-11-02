
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

class BankAccount {
private:
    int balance;
    mutex mtx;

public:
    BankAccount(int initialBalance) {
        balance = initialBalance;
    }

    void deposit(int amount) {
        lock_guard<mutex> lock(mtx);
        balance += amount;
        cout << "Deposited: " << amount
             << " | Balance: " << balance << endl;
    }

    void withdraw(int amount) {
        lock_guard<mutex> lock(mtx);

        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: " << amount
                 << " | Balance: " << balance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    int getBalance() {
        lock_guard<mutex> lock(mtx);
        return balance;
    }
};

int main() {
    BankAccount account(1000);

    vector<thread> transactions;

    cout << "Initial Balance: "
         << account.getBalance() << endl;

    // Multiple threads perform transactions
    transactions.emplace_back(
        [&account]() { account.deposit(500); }
    );

    transactions.emplace_back(
        [&account]() { account.withdraw(200); }
    );

    transactions.emplace_back(
        [&account]() { account.deposit(300); }
    );

    transactions.emplace_back(
        [&account]() { account.withdraw(100); }
    );

    // Wait for all transactions to finish
    for (auto &t : transactions) {
        t.join();
    }

    cout << "\nFinal Balance: "
         << account.getBalance() << endl;

    return 0;
}
