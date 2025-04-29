#include <iostream>


class Product {
private:
    int id;
    string name;
    double price;
    int quantity;
    string category;
    using namespace std;


public:
    Product(int id, const string& name, double price, int quantity, const string& category)
        : id(id), name(name), price(price), quantity(quantity), category(category) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    string getCategory() const { return category; }

    // Setters
    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setName(const string& newName) { name = newName; }
    void setCategory(const string& newCategory) { category = newCategory; }

    // Update stock level
    void updateStock(int amount) { quantity += amount; }

    // Display product information
    void display() const {
        cout << setw(5) << id << " | "
                  << setw(20) << name << " | $"
                  << fixed << setprecision(2) << setw(8) << price << " | "
                  << setw(8) << quantity << " | "
                  << setw(15) << category << endl;
    }
};

class InventorySystem {
private:
    vector<Product> products;
    int nextId;
    string filename;

public:
    InventorySystem(const string& file = "inventory.txt") : nextId(1), filename(file) {
        loadFromFile();
    }

    ~InventorySystem() {
        saveToFile();
    }

    void addProduct() {
        string name, category;
        double price;
        int quantity;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter product name: ";
        getline(cin, name);

        cout << "Enter product category: ";
        getline(cin, category);

        cout << "Enter price: $";
        cin >> price;

        cout << "Enter quantity: ";
        cin >> quantity;

        products.push_back(Product(nextId++, name, price, quantity, category));
        cout << "Product added successfully!\n";
    }

    void updateProduct() {
        int id;
        cout << "Enter product ID to update: ";
        cin >> id;

        for (auto& product : products) {
            if (product.getId() == id) {
                int choice;
                cout << "What would you like to update?\n"
                          << "1. Name\n"
                          << "2. Price\n"
                          << "3. Quantity\n"
                          << "4. Category\n"
                          << "Enter choice: ";
                cin >> choice;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (choice) {
                    case 1: {
                        string name;
                        cout << "Enter new name: ";
                        getline(cin, name);
                        product.setName(name);
                        break;
                    }
                    case 2: {
                        double price;
                        cout << "Enter new price: $";
                        cin >> price;
                        product.setPrice(price);
                        break;
                    }
                    case 3: {
                        int quantity;
                        cout << "Enter new quantity: ";
                        cin >> quantity;
                        product.setQuantity(quantity);
                        break;
                    }
                    case 4: {
                        string category;
                        cout << "Enter new category: ";
                        getline(cin, category);
                        product.setCategory(category);
                        break;
                    }
                    default:
                        cout << "Invalid choice!\n";
                        return;
                }
                cout << "Product updated successfully!\n";
                return;
            }
        }
        cout << "Product not found!\n";
    }

    void updateStock() {
        int id, amount;
        cout << "Enter product ID: ";
        cin >> id;

        for (auto& product : products) {
            if (product.getId() == id) {
                cout << "Current stock: " << product.getQuantity() << endl;
                cout << "Enter amount to add (use negative for removal): ";
                cin >> amount;

                if (product.getQuantity() + amount < 0) {
                    cout << "Error: Not enough items in stock!\n";
                    return;
                }

                product.updateStock(amount);
                cout << "Stock updated successfully!\n";
                return;
            }
        }
        cout << "Product not found!\n";
    }

    void displayAllProducts() const {
        if (products.empty()) {
            cout << "No products in inventory!\n";
            return;
        }

        displayHeader();
        for (const auto& product : products) {
            product.display();
        }
    }

    void searchProducts() const {
        if (products.empty()) {
            cout << "No products in inventory!\n";
            return;
        }

        string searchTerm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter search term: ";
        getline(cin, searchTerm);

        // Convert search term to lowercase for case-insensitive search
        transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(),
                       [](unsigned char c) { return tolower(c); });

        bool found = false;
        displayHeader();

        for (const auto& product : products) {
            string name = product.getName();
            string category = product.getCategory();

            // Convert to lowercase for comparison
            transform(name.begin(), name.end(), name.begin(),
                           [](unsigned char c) { return tolower(c); });
            transform(category.begin(), category.end(), category.begin(),
                           [](unsigned char c) { return tolower(c); });

            if (name.find(searchTerm) != string::npos ||
                category.find(searchTerm) != string::npos) {
                product.display();
                found = true;
            }
        }

        if (!found) {
            cout << "No products found matching '" << searchTerm << "'.\n";
        }
    }

    void lowStockAlert(int threshold = 5) const {
        bool found = false;

        cout << "Products with low stock (below " << threshold << "):\n";
        displayHeader();

        for (const auto& product : products) {
            if (product.getQuantity() < threshold) {
                product.display();
                found = true;
            }
        }

        if (!found) {
            cout << "No products below the stock threshold.\n";
        }
    }

    void removeProduct() {
        int id;
        cout << "Enter product ID to remove: ";
        cin >> id;

        auto it = find_if(products.begin(), products.end(),
                               [id](const Product& p) { return p.getId() == id; });

        if (it != products.end()) {
            products.erase(it);
            cout << "Product removed successfully!\n";
        } else {
            cout << "Product not found!\n";
        }
    }

    void displayCategories() const {
        vector<string> categories;

        for (const auto& product : products) {
            if (find(categories.begin(), categories.end(), product.getCategory()) == categories.end()) {
                categories.push_back(product.getCategory());
            }
        }

        cout << "Available Categories:\n";
        for (const auto& category : categories) {
            cout << "- " << category << endl;
        }
    }

    void displayProductsByCategory() const {
        string category;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter category: ";
        getline(cin, category);

        bool found = false;
        displayHeader();

        for (const auto& product : products) {
            if (product.getCategory() == category) {
                product.display();
                found = true;
            }
        }

        if (!found) {
            cout << "No products found in category '" << category << "'.\n";
        }
    }

private:
    void displayHeader() const {
        cout << setw(5) << "ID" << " | "
                  << setw(20) << "Name" << " | "
                  << setw(9) << "Price" << " | "
                  << setw(8) << "Quantity" << " | "
                  << setw(15) << "Category" << endl;
        cout << string(65, '-') << endl;
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) {
            cout << "No existing inventory file found. Starting with empty inventory.\n";
            return;
        }

        int id, quantity;
        string name, category;
        double price;

        // First read the next ID value
        file >> nextId;
        file.ignore(); // Skip the newline after nextId

        while (file >> id) {
            file.ignore(); // Skip the space after id

            getline(file, name, '|');
            file >> price;
            file.ignore(); // Skip the | after price
            file >> quantity;
            file.ignore(); // Skip the | after quantity
            getline(file, category);

            products.push_back(Product(id, name, price, quantity, category));
        }

        file.close();
        cout << "Inventory loaded successfully!\n";
    }

    void saveToFile() const {
        ofstream file(filename);
        if (!file) {
            cerr << "Error: Unable to open file for saving inventory.\n";
            return;
        }

        // First write the next ID value
        file << nextId << endl;

        for (const auto& product : products) {
            file << product.getId() << " "
                 << product.getName() << "|"
                 << product.getPrice() << "|"
                 << product.getQuantity() << "|"
                 << product.getCategory() << endl;
        }

        file.close();
    }
};

void displayMenu() {
    cout << "\n===== INVENTORY MANAGEMENT SYSTEM =====\n"
              << "1. Add New Product\n"
              << "2. Update Product\n"
              << "3. Update Stock\n"
              << "4. Display All Products\n"
              << "5. Search Products\n"
              << "6. Low Stock Alert\n"
              << "7. Remove Product\n"
              << "8. Display Categories\n"
              << "9. Display Products by Category\n"
              << "0. Exit\n"
              << "Enter your choice: ";
}

int main() {
    InventorySystem inventory;
    int choice;

    cout << "Welcome to the Inventory Management System!\n";

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                inventory.addProduct();
                break;
            case 2:
                inventory.updateProduct();
                break;
            case 3:
                inventory.updateStock();
                break;
            case 4:
                inventory.displayAllProducts();
                break;
            case 5:
                inventory.searchProducts();
                break;
            case 6:
                inventory.lowStockAlert();
                break;
            case 7:
                inventory.removeProduct();
                break;
            case 8:
                inventory.displayCategories();
                break;
            case 9:
                inventory.displayProductsByCategory();
                break;
            case 0:
                cout << "Thank you for using the Inventory Management System!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
