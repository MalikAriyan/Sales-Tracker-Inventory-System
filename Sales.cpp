#include<iostream>
#include<fstream>
using namespace std;

const int MAX_PRODUCTS = 100;
const int MAX_CHAR = 30;
const int MAX_QUANTITY = 50;

char productNames[MAX_PRODUCTS][MAX_CHAR];
int quantity[MAX_PRODUCTS];
int prices[MAX_PRODUCTS];
int total_products = 0;

void display() {
    cout << "\n=============================================\n";
    cout << "     🧾 Sales Tracker & Inventory System     \n";
    cout << "=============================================\n\n";
    cout << " [1] Add New Product\n";
    cout << " [2] View Inventory\n";
    cout << " [3] Record a Sale\n";
    cout << " [4] View Daily Sales Report\n";
    cout << " [5] Edit Product Price & Quantity\n";
    cout << " [6] Delete a Product\n";
    cout << " [7] Load Inventory From File\n";
    cout << " [8] Save Inventory To File\n";
    cout << " [0] Exit Program\n";
    cout << "---------------------------------------------\n";
}

bool stringsAreEqual(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

bool checkDuplicateProduct(char newProduct[]) {
    for (int i = 0; i < total_products; i++) {
        if (stringsAreEqual(productNames[i], newProduct)) {
            return true;
        }
    }
    return false;
}

bool login() {
    const char correctPassword[] = "admin123";
    char inputPassword[30];

    cout << "\n=========== Login Required ===========\n\n";
    cout << "Enter Password: ";
    cin >> inputPassword;

    int i = 0;
    while (inputPassword[i] == correctPassword[i] && correctPassword[i] != '\0') {
        i++;
    }
    if (correctPassword[i] == '\0' && inputPassword[i] == '\0') {
        cout << "\n✅ Login successful!\n\n";
        return true;
    }
    else {
        cout << "\n❌ Incorrect password!\n\n";
        return false;
    }
}

void readFromFile() {
    ifstream readFile("data.txt");
    if (!readFile) {
        cout << "\n⚠️  Failed to open file.\n";
        return;
    }

    total_products = 0;
    while (readFile >> productNames[total_products] >> quantity[total_products] >> prices[total_products]) {
        total_products++;
        if (total_products >= MAX_PRODUCTS) break;
    }
    readFile.close();
}

void writeToFile() {
    ofstream writeFile("data.txt");
    if (!writeFile) {
        cout << "\n⚠️  Failed to open data.txt for writing.\n";
        return;
    }

    for (int i = 0; i < total_products; i++) {
        writeFile << productNames[i] << " " << quantity[i] << " " << prices[i] << endl;
    }
    writeFile.close();
}

void addData() {
    if (total_products >= MAX_PRODUCTS) {
        cout << "\n❌ You Cannot Enter More Products!\n";
        return;
    }

    char* proNamesPtr = productNames[total_products];
    cout << "\nEnter The Product Name: ";
    cin.ignore();
    cin.getline(proNamesPtr, MAX_CHAR);

    if (checkDuplicateProduct(proNamesPtr)) {
        cout << "\n⚠️  Product with this name already exists!\n\n";
        return;
    }

    int* quantityPtr = &quantity[total_products];
    cout << "Enter the Quantity: ";
    cin >> *quantityPtr;

    int* pricePtr = &prices[total_products];
    cout << "Enter the Price: ";
    cin >> *pricePtr;

    total_products++;
    cout << "\n✅ Product Added Successfully!\n\n";
    writeToFile();
}

void viewInventory() {
    cout << "\n====================================================\n";
    cout << "                📦 Inventory List                   \n";
    cout << "====================================================\n";
    cout << " No. | Product Name            | Quantity |  Price  \n";
    cout << "----------------------------------------------------\n";

    for (int i = 0; i < total_products; i++) {
        cout << " " << i + 1 << ".  " << productNames[i];

        // spacing between name and quantity (manual alignment)
        int nameLength = 0;
        while (productNames[i][nameLength] != '\0') nameLength++;
        for (int s = 0; s < 25 - nameLength; s++) cout << " ";

        cout << "|    " << quantity[i];
        if (quantity[i] < 10) cout << "  ";
        else if (quantity[i] < 100) cout << " ";
        cout << "   |   " << prices[i] << "\n";
    }

    cout << "----------------------------------------------------\n";
}

void editInventory() {
    int index = -2;
    viewInventory();
    cout << "Enter the index from 1 to " << total_products << " of product you want to change: ";
    cin >> index;
    index--;

    if (index < 0 || index >= total_products) {
        cout << "\n❌ Invalid Index!\n\n";
        return;
    }

    int price = 0;
    int itemQuantity = 0;
    cout << "\nEnter New Price: ";
    cin >> price;
    if (price < 0) {
        cout << "\n⚠️  Price cannot be less than 0!\n\n";
        return;
    }

    cout << "Enter New Quantity: ";
    cin >> itemQuantity;
    if (itemQuantity < 0) {
        cout << "\n⚠️  Quantity cannot be less than 0!\n\n";
        return;
    }

    prices[index] = price;
    quantity[index] = itemQuantity;
    writeToFile();
    cout << "\n✅ Product Information Updated!\n\n";
}

void deleteProduct() {
    int index = -2;
    viewInventory();
    cout << "Enter the index from 1 to " << total_products << " of product you want to delete: ";
    cin >> index;
    index--;

    if (index < 0 || index >= total_products) {
        cout << "\n❌ Invalid Index!\n\n";
        return;
    }

    for (int i = index; i < total_products - 1; i++) {
        for (int j = 0; j < MAX_CHAR; j++) {
            productNames[i][j] = productNames[i + 1][j];
        }
        quantity[i] = quantity[i + 1];
        prices[i] = prices[i + 1];
    }

    total_products--;
    cout << "\n✅ Product Deleted Successfully!\n\n";
    writeToFile();
}

void recordSales() {
    int index = -2;
    int saleQuant = 0;
    int dayNumber = 0;

    viewInventory();
    cout << "Enter the index from 1 to " << total_products << ": ";
    cin >> index;
    index--;

    if (index < 0 || index >= total_products) {
        cout << "\n❌ Invalid Index!\n\n";
        return;
    }

    cout << "Enter the Sale Quantity: ";
    cin >> saleQuant;
    if (saleQuant > quantity[index]) {
        cout << "\n⚠️  Not enough stock!\n\n";
        return;
    }

    cout << "Enter the Day Number of the week (1–7): ";
    cin >> dayNumber;
    if (dayNumber < 1 || dayNumber > 7) {
        cout << "\n⚠️  Invalid day number!\n\n";
        return;
    }

    quantity[index] -= saleQuant;
    ofstream saleWrite("sales.txt", ios::app);
    saleWrite << index << " " << saleQuant << " " << dayNumber << endl;
    saleWrite.close();
    writeToFile();

    cout << "\n✅ Sale Recorded!\n\n";
}

void viewDailySale() {
    int day = 0;
    cout << "\nEnter the Day Number to View Sales Report (1–7): ";
    cin >> day;

    ifstream salesRead("sales.txt");
    if (!salesRead) {
        cout << "\n⚠️  Error opening sales.txt file!\n\n";
        return;
    }

    int index = 0, saleQuantity = 0, saleDay = 0;
    bool sale = false;
    int total = 0;

    cout << "\n================ Sales Report (Day " << day << ") ================\n\n";
    while (salesRead >> index >> saleQuantity >> saleDay) {
        if (saleDay == day) {
            cout << "Product: " << productNames[index]
                << " | Sold: " << saleQuantity
                << " | Unit Price: " << prices[index] << endl;
            total += saleQuantity * prices[index];
            sale = true;
        }
    }
    salesRead.close();

    if (!sale) {
        cout << "\nNo sales recorded for this day.\n\n";
    }
    else {
        cout << "\n---------------------------------\n";
        cout << "Total Revenue: " << total << "\n\n";
    }
}

void saveInventoryToFile() {
    ofstream invFile("inventory.txt");
    if (!invFile) {
        cout << "\n⚠️  Failed to open inventory.txt for writing.\n\n";
        return;
    }

    for (int i = 0; i < total_products; i++) {
        invFile << productNames[i] << " " << quantity[i] << " " << prices[i] << endl;
    }
    invFile.close();
    cout << "\n✅ Inventory saved to inventory.txt\n\n";
}

void loadInventoryFromFile() {
    ifstream invFile("inventory.txt");
    if (!invFile) {
        cout << "\n⚠️  Failed to open inventory.txt for reading.\n\n";
        return;
    }

    total_products = 0;
    while (invFile >> productNames[total_products] >> quantity[total_products] >> prices[total_products]) {
        total_products++;
        if (total_products >= MAX_PRODUCTS) break;
    }
    invFile.close();

    cout << "\n✅ Inventory loaded from inventory.txt\n\n";
}

int main() {
    if (!login()) return 0;

    readFromFile();
    int choice = -2;

    do {
        display();
        cout << "Enter Your Choice: ";
        cin >> choice;
        cout << endl;

        if (choice == 1) addData();
        else if (choice == 2) viewInventory();
        else if (choice == 3) recordSales();
        else if (choice == 4) viewDailySale();
        else if (choice == 5) editInventory();
        else if (choice == 6) deleteProduct();
        else if (choice == 7) loadInventoryFromFile();
        else if (choice == 8) saveInventoryToFile();
        else if (choice != 0) cout << "⚠️  Invalid option! Try again.\n";

    } while (choice != 0);

    cout << "\n👋 Exiting program. Goodbye!\n\n";
    return 0;
}
