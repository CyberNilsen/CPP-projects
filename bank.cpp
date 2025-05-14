#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Her har inkluderer jeg input og output, string, vector og formattering

class Account {
protected:
    std::string accountNumber;
    std::string ownerName;
    double balance;

// Her er klassen beskyttet og jeg har stringer som er accountNumber og ownerName har også hvor mye kontoen har altså balance

public:

    Account(const std::string& accNum, const std::string& name, double initialBalance = 0.0)
        : accountNumber(accNum), ownerName(name), balance(initialBalance) {}

// Her har jeg en constructor/initilizer som har samme 3 variabler som i klassen. Vanlig balance/penger er 0.0

    std::string getAccountNumber() const { return accountNumber; }
    std::string getOwnerName() const { return ownerName; }
    double getBalance() const { return balance; }

// Her er konstante variabler.


    virtual bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            return true;
        }
        return false;
    }



    virtual bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

// Her er en funksjon som setter inn penger og en som tar ut penger

    virtual void displayDetails() const {
        std::cout << "Account Number: " << accountNumber << std::endl;
        std::cout << "Owner: " << ownerName << std::endl;
        std::cout << "Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:

    SavingsAccount(const std::string& accNum, const std::string& name, 
                   double initialBalance = 0.0, double rate = 2.5)
        : Account(accNum, name, initialBalance), interestRate(rate) {}

    void addInterest() {
        double interest = balance * (interestRate / 100);
        balance += interest;
        std::cout << "Interest added: $" << std::fixed << std::setprecision(2) << interest << std::endl;
    }

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "Account Type: Savings" << std::endl;
        std::cout << "Interest Rate: " << interestRate << "%" << std::endl;
    }
};

class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:

    CheckingAccount(const std::string& accNum, const std::string& name, 
                    double initialBalance = 0.0, double limit = 100.0)
        : Account(accNum, name, initialBalance), overdraftLimit(limit) {}

    bool withdraw(double amount) override {
        if (amount > 0 && amount <= (balance + overdraftLimit)) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "Account Type: Checking" << std::endl;
        std::cout << "Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }
};

class Bank {
private:
    std::string name;
    std::vector<Account*> accounts;

public:

    Bank(const std::string& bankName) : name(bankName) {}

    ~Bank() {
        for (auto account : accounts) {
            delete account;
        }
        accounts.clear();
    }

    void addAccount(Account* account) {
        accounts.push_back(account);
    }

    Account* findAccount(const std::string& accNum) {
        for (auto account : accounts) {
            if (account->getAccountNumber() == accNum) {
                return account;
            }
        }
        return nullptr;
    }

    void displayAllAccounts() const {
        std::cout << "\n=== " << name << " - All Accounts ===\n" << std::endl;
        
        if (accounts.empty()) {
            std::cout << "No accounts found." << std::endl;
            return;
        }

        for (size_t i = 0; i < accounts.size(); ++i) {
            std::cout << "Account " << (i + 1) << ":" << std::endl;
            accounts[i]->displayDetails();
            std::cout << "------------------------" << std::endl;
        }
    }

    std::string getName() const { return name; }
};

void displayMenu() {
    std::cout << "\n===== Bank System Menu =====\n";
    std::cout << "1. Create New Account\n";
    std::cout << "2. Deposit Money\n";
    std::cout << "3. Withdraw Money\n";
    std::cout << "4. Check Account Details\n";
    std::cout << "5. Add Interest to Savings Account\n";
    std::cout << "6. List All Accounts\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {

    Bank bank("C++ Bank");
    
    std::cout << "Welcome to " << bank.getName() << "!" << std::endl;
    
    int choice;
    std::string accountNumber, name;
    double amount;
    char accountType;
    
    do {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::cout << "\nEnter account number: ";
                std::cin >> accountNumber;
                
                if (bank.findAccount(accountNumber) != nullptr) {
                    std::cout << "Error: Account number already exists.\n";
                    break;
                }
                
                std::cout << "Enter account holder name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                
                std::cout << "Enter initial balance: $";
                std::cin >> amount;
                
                std::cout << "Select account type (S for Savings, C for Checking): ";
                std::cin >> accountType;
                
                if (toupper(accountType) == 'S') {
                    double rate;
                    std::cout << "Enter interest rate (%): ";
                    std::cin >> rate;
                    bank.addAccount(new SavingsAccount(accountNumber, name, amount, rate));
                } else if (toupper(accountType) == 'C') {
                    double limit;
                    std::cout << "Enter overdraft limit: $";
                    std::cin >> limit;
                    bank.addAccount(new CheckingAccount(accountNumber, name, amount, limit));
                } else {
                    std::cout << "Invalid account type selected.\n";
                    break;
                }
                
                std::cout << "Account created successfully!\n";
                break;
            }
            case 2: {
                std::cout << "\nEnter account number: ";
                std::cin >> accountNumber;
                
                Account* account = bank.findAccount(accountNumber);
                if (account == nullptr) {
                    std::cout << "Error: Account not found.\n";
                    break;
                }
                
                std::cout << "Enter amount to deposit: $";
                std::cin >> amount;
                
                if (account->deposit(amount)) {
                    std::cout << "Deposit successful. New balance: $" 
                              << std::fixed << std::setprecision(2) << account->getBalance() << std::endl;
                } else {
                    std::cout << "Error: Invalid deposit amount.\n";
                }
                break;
            }
            case 3: {
                std::cout << "\nEnter account number: ";
                std::cin >> accountNumber;
                
                Account* account = bank.findAccount(accountNumber);
                if (account == nullptr) {
                    std::cout << "Error: Account not found.\n";
                    break;
                }
                
                std::cout << "Enter amount to withdraw: $";
                std::cin >> amount;
                
                if (account->withdraw(amount)) {
                    std::cout << "Withdrawal successful. New balance: $" 
                              << std::fixed << std::setprecision(2) << account->getBalance() << std::endl;
                } else {
                    std::cout << "Error: Invalid withdrawal amount or insufficient funds.\n";
                }
                break;
            }
            case 4: {
                std::cout << "\nEnter account number: ";
                std::cin >> accountNumber;
                
                Account* account = bank.findAccount(accountNumber);
                if (account == nullptr) {
                    std::cout << "Error: Account not found.\n";
                    break;
                }
                
                std::cout << "\n=== Account Details ===\n";
                account->displayDetails();
                break;
            }
            case 5: {
                std::cout << "\nEnter savings account number: ";
                std::cin >> accountNumber;
                
                Account* account = bank.findAccount(accountNumber);
                if (account == nullptr) {
                    std::cout << "Error: Account not found.\n";
                    break;
                }
                
                SavingsAccount* savingsAccount = dynamic_cast<SavingsAccount*>(account);
                if (savingsAccount == nullptr) {
                    std::cout << "Error: This is not a savings account.\n";
                    break;
                }
                
                savingsAccount->addInterest();
                std::cout << "New balance: $" << std::fixed << std::setprecision(2) 
                          << savingsAccount->getBalance() << std::endl;
                break;
            }
            case 6: {
                bank.displayAllAccounts();
                break;
            }
            case 7: {
                std::cout << "Thank you for using " << bank.getName() << ". Goodbye!\n";
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        
    } while (choice != 7);
    
    return 0;
}