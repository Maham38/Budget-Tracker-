#include <iostream>
#include <iomanip>
#include <string>
#include <map>

using namespace std;

void suggestSpending(double totalBudget, const map<string, double>& userCategories) {
    cout << "\nBased on your total budget, here are suggested amounts for your categories:\n";
    cout << left << setw(20) << "Category" << "Suggested Amount\n";
    cout << "--------------------------------\n";

    double totalWeight = 0.0;
    for (const auto& category : userCategories) {
        totalWeight += category.second; 
    }

    for (const auto& category : userCategories) {
        double suggestedAmount = (category.second / totalWeight) * totalBudget;
        cout << left << setw(20) << category.first
             << "$" << fixed << setprecision(2) << suggestedAmount << "\n";
    }
}


void suggestAdjustment(string category, double currentAmount, double suggestedAmount) {
    double overAmount = currentAmount - suggestedAmount;

    cout << "\nThe expense for '" << category << "' is over the suggested amount by $"
         << fixed << setprecision(2) << overAmount << ".\n";
    cout << "Consider reducing this expense by $" << fixed << setprecision(2) << overAmount
         << " to align with your budget.\n";
}


void adjustExpenses(map<string, double>& userExpenses, const map<string, double>& suggestedAllocation, double totalBudget) {
    double totalExpenses = 0.0;
    for (const auto& expense : userExpenses) {
        totalExpenses += expense.second;
    }

    if (totalExpenses > totalBudget) {
        double overBudget = totalExpenses - totalBudget;

        cout << "\nYou are over your budget by $" << fixed << setprecision(2) << overBudget << ".\n";

        map<string, bool> lockedCategories;
        for (const auto& expense : userExpenses) {
            cout << "\nDo you want to lock the expense for '" << expense.first
                 << "' at $" << fixed << setprecision(2) << expense.second << "? (y/n): ";
            char choice;
            cin >> choice;
            lockedCategories[expense.first] = (choice == 'y' || choice == 'Y');
        }


        double remainingOverBudget = overBudget;
        double totalAdjustable = 0.0;

        for (const auto& expense : userExpenses) {
            if (!lockedCategories[expense.first]) {
                totalAdjustable += expense.second;
            }
        }

        if (totalAdjustable > 0) {
            for (auto& expense : userExpenses) {
                if (!lockedCategories[expense.first]) {
                    double reduction = (expense.second / totalAdjustable) * remainingOverBudget;
                    expense.second -= reduction;
                    cout << "\nAdjusted '" << expense.first << "' by -$"
                         << fixed << setprecision(2) << reduction
                         << " to $" << expense.second << ".";
                }
            }
        }

        cout << "\nYour adjusted expenses are now within your budget.\n";

        for (auto& expense : userExpenses) {
            cout << "\nCurrent expense for '" << expense.first << "': $"
                 << fixed << setprecision(2) << expense.second
                 << "\nDo you want to edit this expense? (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                cout << "Enter the new expense amount for '" << expense.first << "': $";
                cin >> expense.second;
            }
        }


        totalExpenses = 0.0;
        for (const auto& expense : userExpenses) {
            totalExpenses += expense.second;
        }

        if (totalExpenses > totalBudget) {
            cout << "\nYou are still over budget by $" << fixed << setprecision(2) << (totalExpenses - totalBudget) << ".\n";
            cout << "Consider further adjustments to align your expenses with your budget.\n";
        } else {
            cout << "\nYour expenses have been successfully adjusted within your budget.\n";
        }
    } else {
        cout << "\nYour total expenses are within the budget. No adjustments needed.\n";
    }
}


int main() {
    double totalBudget;

    cout << "Enter your total budget for the month: $";
    cin >> totalBudget;

    int numberOfCategories;
    cout << "\nHow many expense categories do you want to enter? ";
    cin >> numberOfCategories;

    map<string, double> userCategories;
    cout << "\nEnter the categories and their importance (as percentages, e.g., 30 for 30%):\n";

    for (int i = 0; i < numberOfCategories; ++i) {
        string category;
        double weight;

        cout << "\nCategory name: ";
        cin.ignore(); 
        getline(cin, category);

        cout << "Importance (percentage): ";
        cin >> weight;

        userCategories[category] = weight;
    }

    map<string, double> suggestedAllocation;
    double totalWeight = 0.0;
    for (const auto& category : userCategories) {
        totalWeight += category.second;
    }
    for (const auto& category : userCategories) {
        suggestedAllocation[category.first] = (category.second / totalWeight) * totalBudget;
    }

    suggestSpending(totalBudget, userCategories);

    map<string, double> userExpenses;
    double totalExpenses = 0.0;

    cout << "\nNow, enter your actual expenses for each category:\n";
    for (const auto& category : userCategories) {
        double expense;

        cout << "\nExpense for " << category.first << ": $";
        cin >> expense;

        userExpenses[category.first] = expense;
        totalExpenses += expense;
    }

    adjustExpenses(userExpenses, suggestedAllocation, totalBudget);

    cout << "\n========== Final Expense Summary ==========\n";
    cout << left << setw(20) << "Category" << "Expense\n";
    cout << "--------------------------------\n";

    for (const auto& expense : userExpenses) {
        cout << left << setw(20) << expense.first
             << "$" << fixed << setprecision(2) << expense.second << "\n";
    }

    double remainingBudget = totalBudget - totalExpenses;
    cout << "--------------------------------\n";
    cout << "Total Budget: $" << totalBudget << "\n";
    cout << "Total Expenses: $" << totalExpenses << "\n";

    if (remainingBudget < 0) {
        cout << "You are over budget by $" << fixed << setprecision(2) << (-remainingBudget) << "!\n";
    } else {
        cout << "Remaining Budget: $" << fixed << setprecision(2) << remainingBudget << "\n";
    }

    return 0;
}
