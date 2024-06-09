#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // for min function

using namespace std;

class User {
public:
    User(string name, int id) : name(name), id(id) {}

    string getName() const {
        return name;
    }

    int getId() const {
        return id;
    }

private:
    string name;
    int id;
};

class Expense {
public:
    Expense(User payer, double amount) : payer(payer), amount(amount) {}

    User getPayer() const {
        return payer;
    }

    double getAmount() const {
        return amount;
    }

private:
    User payer;
    double amount;
};

class TripManager {
public:
    void addFriend(string name) {
        int id = friends.size() + 1;
        friends.push_back(User(name, id));
    }

    void addExpense(User payer, double amount) {
        expenses.push_back(Expense(payer, amount));
    }

    void generateReport() {
        cout << "Trip Expense Report:" << endl;

        // Calculate total trip cost
        double totalTripCost = 0;
        for (const Expense& expense : expenses) {
            totalTripCost += expense.getAmount();
        }

        // Calculate share per friend
        double sharePerFriend = totalTripCost / friends.size();

        // Create a vector to store each friend's total spending
        vector<double> totalSpending(friends.size(), 0.0);

        // Populate total spending
        for (size_t i = 0; i < friends.size(); ++i) {
            for (const Expense& expense : expenses) {
                if (expense.getPayer().getId() == friends[i].getId()) {
                    totalSpending[i] += expense.getAmount();
                }
            }
        }

        // Calculate balances
        vector<double> balances(friends.size(), 0.0);
        for (size_t i = 0; i < friends.size(); ++i) {
            balances[i] = totalSpending[i] - sharePerFriend;
        }

        // Print the report with formatting
        cout << "----------------------------------------------" << endl;
        cout << "Friend\t\tOwes/Owed\tAmount" << endl;
        cout << "----------------------------------------------" << endl;
        for (size_t i = 0; i < friends.size(); ++i) {
            for (size_t j = 0; j < friends.size(); ++j) {
                if (balances[i] < 0 && balances[j] > 0) {
                    double payment = min(-balances[i], balances[j]);
                    cout << friends[i].getName() << "\towes\t" << payment << "\tunits to " << friends[j].getName() << "." << endl;
                    balances[i] += payment;
                    balances[j] -= payment;
                }
            }
        }
        cout << "----------------------------------------------" << endl;
    }

    vector<User> getFriends() const {
        return friends;
    }

private:
    vector<User> friends;
    vector<Expense> expenses;
};

int main() {
    TripManager trip;

    // Add friends
    int numFriends;
    cout << "Enter the number of friends: ";
    cin >> numFriends;
    cin.ignore(); // Clear the input buffer
    for (int i = 0; i < numFriends; ++i) {
        string name;
        cout << "Enter name of friend " << i + 1 << ": ";
        getline(cin, name);
        trip.addFriend(name);
    }

    // Input expenses
    for (const User& friendObj : trip.getFriends()) {
        double amount;
        cout << "Enter amount spent by " << friendObj.getName() << ": ";
        cin >> amount;
        trip.addExpense(friendObj, amount);
    }

    // Generate report
    trip.generateReport();

    return 0;
}
