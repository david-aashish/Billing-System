#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <vector>
#include <iomanip>

using namespace std;

class Bill
{
private:
    string Item;
    float Price, Quantity;

public:
    Bill() : Item(""), Price(0.0), Quantity(0.0)
    {
    }

    void setItem(string item)
    {
        Item = item;
    }

    void setPrice(float price)
    {
        Price = price;
    }

    void setQuantity(float quantity)
    {
        Quantity = quantity;
    }

    string getItem()
    {
        return Item;
    }

    float getPrice()
    {
        return Price;
    }

    float getQuantity()
    {
        return Quantity;
    }
};

void addItem(Bill bill)
{
    bool flag = false;
    while (!flag)
    {
        int choose;
        cout << "\t1.Add." << endl;
        cout << "\t2.Close." << endl;
        cout << "\tEnter Choice:- ";
        cin >> choose;
        cin.ignore();
        if (choose == 1)
        {
            system("cls");
            string Item;
            float Price, Quantity;
            cout << "\tEnter Item Name:- ";
            getline(cin, Item); // to read multiworded strings
            bill.setItem(Item);
            cout << "\tEnter Price of the Item:- ";
            cin >> Price;
            bill.setPrice(Price);
            cout << "\tEnter Quantity of the Item:- ";
            cin >> Quantity;
            bill.setQuantity(Quantity);
            ifstream inFile("Bill.txt");
            vector<string> fileData;
            bool itemExists = false;

            string data;
            while (getline(inFile, data))
            {
                stringstream str(data);
                string itemName;
                float itemPrice, itemQuantity;
                char delimiter = ':';
                getline(str, itemName, delimiter);
                itemName.erase(0, itemName.find_first_not_of(" \t\r\n")); 
                itemName.erase(itemName.find_last_not_of(" \t\r\n") + 1);
                str >> itemPrice >> delimiter >> itemQuantity;

                if (bill.getItem() == itemName)
                {
                    itemExists = true;
                    int choice;
                    cout << "\tThe item you have added was already added previously." << endl;
                    cout << "\t1. Add present quantity to the item with the previous price." << endl;
                    cout << "\t2. Replace previous entry with current entry." << endl;
                    cout << "\tEnter Choice: ";
                    cin >> choice;

                    if (choice == 1)
                    {
                        itemQuantity += bill.getQuantity();
                    }
                    else if (choice == 2)
                    {
                        itemPrice = bill.getPrice();
                        itemQuantity = bill.getQuantity();
                    }

                    ostringstream updatedData;
                    updatedData << "\t" << itemName << " : " << itemPrice << " : " << itemQuantity;
                    fileData.push_back(updatedData.str());
                }
                else
                {
                    fileData.push_back(data);
                }
            }

            inFile.close();

            if (!itemExists)
            {
                ostringstream newItem;
                newItem << "\t" << bill.getItem() << " : " << bill.getPrice() << " : " << bill.getQuantity();
                fileData.push_back(newItem.str());
            }

            ofstream outFile("Bill.txt", ios::trunc);
            for (const auto& line : fileData)
            {
                outFile << line << endl;
            }
            outFile.close();

            cout << "\tItem Added Successfully" << endl;
            Sleep(3000);
        }
        else if (choose == 2)
        {
            system("cls");
            flag = true;
            cout << "\tBack to Main Menu!" << endl;
            Sleep(2000);
        }
    }
}

void printBill()
{
    system("cls");
    float totalAmount = 0.0;
    vector<string> items;
    vector<float> prices;
    vector<float> quantities;
    vector<float> amounts;
    bool flag = false;
    while (!flag)
    {
        system("cls");
        int choose;
        cout << "\t1.Add Bill." << endl;
        cout << "\t2.Close." << endl;
        cout << "\tEnter Choice:- ";
        cin >> choose;
        cin.ignore();
        if (choose == 1)
        {
            string Item;
            float Quantity;
            cout << "\tEnter Item:- ";
            getline(cin, Item);
            cout << "\tEnter Quantity of the Item:- ";
            cin >> Quantity;
            ifstream in("Bill.txt");
            ofstream out("Temp.txt");
            string data; // to store data of the file
            bool exist = false;
            while (getline(in, data))
            {
                stringstream str;
                str << data; // assigning the data we got to the stringstream
                string itemName;
                float itemPrice, itemQuantity;
                char delimiter = ':'; // delimiter is used to seperate quantities
                getline(str, itemName, delimiter);
                itemName.erase(0, itemName.find_first_not_of(" \t\r\n")); // remove leading spaces from itemName
                itemName.erase(itemName.find_last_not_of(" \t\r\n") + 1); // remove trailing spaces from itemName
                str >> itemPrice >> delimiter >> itemQuantity;
                if (Item == itemName)
                {
                    exist = true;
                    if (Quantity <= itemQuantity)
                    {
                        int amount = itemPrice * Quantity;
                        cout << "\t Item              | Price    | Quantity | Amount" << endl;
                        cout << "\t" << setw(18) << left << itemName
                             << setw(10) << right << itemPrice
                             << setw(10) << right << Quantity
                             << setw(10) << right << amount << endl;
                        items.push_back(itemName);
                        prices.push_back(itemPrice);
                        quantities.push_back(Quantity);
                        amounts.push_back(amount);
                        float newQuantity = itemQuantity - Quantity;
                        itemQuantity = newQuantity;
                        totalAmount = totalAmount + amount;
                        out << "\t" << itemName << " : " << itemPrice << " : " << itemQuantity << endl
                            << endl;
                    }
                    else
                    {
                        cout << "\tSorry, sufficient " << Item << " not available" << endl;
                    }
                }
                else
                {
                    out << data << endl; // to retain data of the remaining items
                }
            }
            if (!exist)
            {
                cout << "\tItem is not available" << endl;
            }
            out.close();
            in.close();
            remove("Bill.txt");
            rename("Temp.txt", "Bill.txt"); // updating the bill file
        }
        else if (choose == 2)
        {
            flag = true;
            cout << "\tYour total Bill is being generated...." << endl;
        }
        Sleep(3000);
    }
    system("cls");
    cout << endl;
    cout << "\t-------------------------------------------------" << endl
         << endl;
    cout << "\t Item              | Price    | Quantity | Amount" << endl
         << endl;
    for (int i = 0; i < items.size(); i++)
    {
        cout << "\t" << setw(18) << left << items[i]
             << setw(10) << right << prices[i]
             << setw(10) << right << quantities[i]
             << setw(10) << right << amounts[i] << endl;
    }
    cout << endl;
    cout << "\tTotal Bill ------------------------------- : " << totalAmount << endl
         << endl;
    Sleep(7000);
}

void clearBillFile()
{
    ofstream out("Bill.txt", ios ::trunc);
    if (!out)
    {
        cout << "Bill file was not generated" << endl;
    }
    out.close();
}

int main()
{
    Bill bill;
    bool flag = false;
    while (!flag)
    {
        system("cls"); // overwrites the previous output and shows the latest output
        int value;
        cout << "\tWelcome To Super Market Billing System" << endl;
        cout << "\t*************************** ****" << endl;
        cout << "\t\t1. Add Item." << endl;
        cout << "\t\t2.Print Bill." << endl;
        cout << "\t\t3. Exit. " << endl;
        cout << "\t\tEnter Choice:";
        cin >> value;
        if (value == 1)
        {
            system("cls");
            addItem(bill);
            Sleep(3000);
        }
        else if (value == 2)
        {
            system("cls");
            printBill();
            Sleep(3000);
        }
        else
        {
            clearBillFile();
            flag = true;
            cout << "\tThanks for Shopping!" << endl;
            Sleep(3000);
        }
    }
    return 0;
}
