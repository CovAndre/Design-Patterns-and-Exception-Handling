#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


const int MAX_PRODUCTS = 10;
const int MAX_CART_ITEMS = 20;
const int MAX_ORDERS = 50;


class Product {
private:
    char id[10];
    char name[50];
    double price;
public:
    Product() {}
    Product(const char* id, const char* name, double price) {
        strcpy(this->id, id);
        strcpy(this->name, name);
        this->price = price;
    }
    const char* getId() { return id; }
    const char* getName() { return name; }
    double getPrice() { return price; }
};


class CartItem {
private:
    Product product;
    int quantity;
public:
    CartItem() {}
    CartItem(Product product, int quantity) {
        this->product = product;
        this->quantity = quantity;
    }
    Product getProduct() { return product; }
    int getQuantity() { return quantity; }
};


class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
};

class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) {
        cout << "Paid " << amount << " using Cash." << endl;
    }
};

class CreditPayment : public PaymentStrategy {
public:
    void pay(double amount) {
        cout << "Paid " << amount << " using Credit / Debit Card." << endl;
    }
};

class GCashPayment : public PaymentStrategy {
public:
    void pay(double amount) {
        cout << "Paid " << amount << " using GCash." << endl;
    }
};


class PaymentProcessor {
private:
    static PaymentProcessor* instance;
    PaymentProcessor() {}
public:
    static PaymentProcessor* getInstance() {
        if (!instance) {
            instance = new PaymentProcessor();
        }
        return instance;
    }

    void processPayment(PaymentStrategy* strategy, double amount) {
        strategy->pay(amount);
    }
};

PaymentProcessor* PaymentProcessor::instance = nullptr;


class Order {
private:
    int orderId;
    double totalAmount;
    char paymentMethod[20];
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
public:
    Order() {}
    Order(int orderId, double totalAmount, const char* paymentMethod, CartItem* items, int itemCount) {
        this->orderId = orderId;
        this->totalAmount = totalAmount;
        strcpy(this->paymentMethod, paymentMethod);
        this->itemCount = itemCount;
        for (int i = 0; i < itemCount; i++) {
            this->items[i] = items[i];
        }
    }

    void displayOrder() {
        cout << "Order ID: " << orderId << endl;
        cout << "Total Amount: " << totalAmount << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "Order Details:" << endl;
        cout << "Product ID\tName\tPrice\tQuantity" << endl;
        for (int i = 0; i < itemCount; i++) {
            cout << items[i].getProduct().getId() << "\t\t"
                 << items[i].getProduct().getName() << "\t"
                 << items[i].getProduct().getPrice() << "\t"
                 << items[i].getQuantity() << endl;
        }
        cout << endl;
    }
};


Product products[MAX_PRODUCTS] = {
    Product("P001", "Pen", 10),
    Product("P002", "Notebook", 50),
    Product("P003", "Eraser", 5),
    Product("P004", "Ruler", 20),
    Product("P005", "Pencil", 8)
};
int productCount = 5;

CartItem cart[MAX_CART_ITEMS];
int cartCount = 0;

Order orders[MAX_ORDERS];
int orderCount = 0;


void viewProducts();
void viewCart();
void viewOrders();
void addToCart(const char* productId);
Product findProductById(const char* id);
void logOrder(int orderId, const char* paymentMethod);

int main() {
    int choice;
    do {
        cout << "\n===== MENU =====" << endl;
        cout << "1. View Products" << endl;
        cout << "2. View Shopping Cart" << endl;
        cout << "3. View Orders" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewProducts();
                break;
            case 2:
                viewCart();
                break;
            case 3:
                viewOrders();
                break;
            case 4:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}

void viewProducts() {
    cout << "\nProduct ID\tName\t\tPrice" << endl;
    for (int i = 0; i < productCount; i++) {
        cout << products[i].getId() << "\t\t"
             << products[i].getName() << "\t\t"
             << products[i].getPrice() << endl;
    }

    char choice;
    do {
        char id[10];
        cout << "\nEnter the Product ID to add to cart: ";
        cin >> id;

        try {
            Product p = findProductById(id);
            int quantity;
            cout << "Enter quantity: ";
            cin >> quantity;

            if (quantity <= 0) throw runtime_error("Invalid quantity!");

            cart[cartCount++] = CartItem(p, quantity);
            cout << "Product added successfully!" << endl;
        } catch (exception& e) {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Add another product? (Y/N): ";
        cin >> choice;
    } while (choice == 'Y' || choice == 'y');
}

void viewCart() {
    if (cartCount == 0) {
        cout << "\nShopping cart is empty!" << endl;
        return;
    }

    cout << "\nProduct ID\tName\t\tPrice\tQuantity" << endl;
    for (int i = 0; i < cartCount; i++) {
        cout << cart[i].getProduct().getId() << "\t\t"
             << cart[i].getProduct().getName() << "\t\t"
             << cart[i].getProduct().getPrice() << "\t"
             << cart[i].getQuantity() << endl;
    }

    char checkoutChoice;
    cout << "\nDo you want to checkout all products? (Y/N): ";
    cin >> checkoutChoice;

    if (checkoutChoice == 'Y' || checkoutChoice == 'y') {
        double total = 0;
        for (int i = 0; i < cartCount; i++) {
            total += cart[i].getProduct().getPrice() * cart[i].getQuantity();
        }

        int paymentOption;
        cout << "\nSelect Payment Method:" << endl;
        cout << "1. Cash" << endl;
        cout << "2. Credit / Debit Card" << endl;
        cout << "3. GCash" << endl;
        cout << "Choice: ";
        cin >> paymentOption;

        PaymentStrategy* strategy;
        char paymentMethod[20];

        switch (paymentOption) {
            case 1:
                strategy = new CashPayment();
                strcpy(paymentMethod, "Cash");
                break;
            case 2:
                strategy = new CreditPayment();
                strcpy(paymentMethod, "Credit / Debit Card");
                break;
            case 3:
                strategy = new GCashPayment();
                strcpy(paymentMethod, "GCash");
                break;
            default:
                cout << "Invalid payment method. Transaction cancelled." << endl;
                return;
        }

        PaymentProcessor::getInstance()->processPayment(strategy, total);
        orders[orderCount++] = Order(orderCount, total, paymentMethod, cart, cartCount);
        logOrder(orderCount, paymentMethod);

        delete strategy;
        cartCount = 0;

        cout << "\nYou have successfully checked out the products!" << endl;
    }
}

void viewOrders() {
    if (orderCount == 0) {
        cout << "\nNo orders available!" << endl;
        return;
    }

    for (int i = 0; i < orderCount; i++) {
        orders[i].displayOrder();
    }
}

Product findProductById(const char* id) {
    for (int i = 0; i < productCount; i++) {
        if (strcasecmp(products[i].getId(), id) == 0) {
            return products[i];
        }
    }
    throw runtime_error("Product ID not found!");
}


void logOrder(int orderId, const char* paymentMethod) {
    ofstream logfile("log.txt", ios::app);
    logfile << "[LOG] -> Order ID: " << orderId << " has been successfully checked out and paid using " << paymentMethod << endl;
    logfile.close();
}
