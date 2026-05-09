#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

template <typename T>
T getMaxValue(T a, T b) {
    if (a > b) return a;
    return b;
}

template <class T>
class Inventory {
private:

    T* arr;
    int capacity;
    int current;
public:
    Inventory(int c) : capacity(c), current(0) {
        arr = new T[capacity];
    }
    ~Inventory() {
        delete[] arr;
    }
    void add(T item) {
        if (current < capacity) {
            arr[current++] = item;
        }
    }
    T get(int index) const {
        if (index >= 0 && index < current) {
            return arr[index];
        }
        throw out_of_range("Index out of bounds");
    }
    int getSize() const {
        return current;
    }
};

class Location {
private:
    string city;
    string area;
public:
    Location() : city(""), area("") {}
    Location(string c, string a) : city(c), area(a) {}
    void setCity(string c) { this->city = c; }
    void setArea(string a) { this->area = a; }
    string getCity() const { return city; }
    string getArea() const { return area; }
};

class Engine {
private:
    string type;
    int horsepower;
public:
    Engine() : type(""), horsepower(0) {}
    Engine(string t, int hp) : type(t), horsepower(hp) {}
    void setType(string t) { this->type = t; }
    void setHorsepower(int hp) { this->horsepower = hp; }
    string getType() const { return type; }
    int getHorsepower() const { return horsepower; }
    
    friend ostream& operator<<(ostream& os, const Engine& e) {
        os << e.type << " - " << e.horsepower << "HP";
        return os;
    }
};

class Vehicle {
protected:
    int id;
    string brand;
    string model;
    int year;
    double price;
    Engine engine;
    Location location;
    static int totalVehicles;
    const string vin;

public:
    Vehicle(int i, string b, string m, int y, double p, Engine e, string v, Location l)
        : id(i), brand(b), model(m), year(y), price(p), engine(e), location(l), vin(v) {
        totalVehicles++;
    }
    virtual ~Vehicle() {
        totalVehicles--;
    }

    void setId(int i) { this->id = i; }
    void setBrand(string b) { this->brand = b; }
    void setModel(string m) { this->model = m; }
    void setYear(int y) { this->year = y; }
    void setPrice(double p) { this->price = p; }
    void setEngine(Engine e) { this->engine = e; }
    void setLocation(Location l) { this->location = l; }

    int getId() const { return id; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    double getPrice() const { return price; }
    Engine getEngine() const { return engine; }
    Location getLocation() const { return location; }
    string getVin() const { return vin; }

    static int getTotalVehicles() { return totalVehicles; }

    virtual void displayInfo() const = 0;
    virtual double calculateTax() const = 0;
    virtual void saveToFile(ofstream& out) const = 0;

    friend class Admin;
    friend void applyDiscount(Vehicle& v, double discount);

    bool operator>(const Vehicle& other) const {
        return this->price > other.price;
    }
    double operator+(const Vehicle& other) const {
        return this->price + other.price;
    }
};

int Vehicle::totalVehicles = 0;

void applyDiscount(Vehicle& v, double discount) {
    v.price -= (v.price * (discount / 100.0));
}

class Admin {
public:
    void printConfidentialVin(const Vehicle& v) const {
        cout << "Confidential VIN for ID " << v.id << ": " << v.vin << endl;
    }
};

class Car : public Vehicle {
private:
    int doors;
    bool isSedan;
public:
    Car(int i, string b, string m, int y, double p, Engine e, string v, Location l, int d, bool s)
        : Vehicle(i, b, m, y, p, e, v, l), doors(d), isSedan(s) {}

    void setDoors(int d) { this->doors = d; }
    void setIsSedan(bool s) { this->isSedan = s; }
    int getDoors() const { return doors; }
    bool getIsSedan() const { return isSedan; }

    void displayInfo() const override {
        cout << left << setw(5) << id << setw(15) << brand << setw(15) << model
             << setw(6) << year << setw(12) << price << setw(20) << engine.getType()
             << setw(15) << location.getCity() << setw(6) << doors 
             << (isSedan ? "Sedan" : "Other") << endl;
    }

    double calculateTax() const override {
        return price * 0.05;
    }

    void honk() const {
        cout << brand << " " << model << " honks: Beep!" << endl;
    }

    void honk(int times) const {
        cout << brand << " " << model << " honks: ";
        for (int i = 0; i < times; i++) {
            cout << "Beep! ";
        }
        cout << endl;
    }

    void saveToFile(ofstream& out) const override {
        out << "C\n" << id << "\n" << brand << "\n" << model << "\n" << year << "\n"
            << price << "\n" << engine.getType() << "\n" << engine.getHorsepower() << "\n"
            << vin << "\n" << location.getCity() << "\n" << location.getArea() << "\n"
            << doors << "\n" << isSedan << "\n";
    }
};

class Bike : public Vehicle {
private:
    bool hasCarrier;
public:
    Bike(int i, string b, string m, int y, double p, Engine e, string v, Location l, bool c)
        : Vehicle(i, b, m, y, p, e, v, l), hasCarrier(c) {}

    void setHasCarrier(bool c) { this->hasCarrier = c; }
    bool getHasCarrier() const { return hasCarrier; }

    void displayInfo() const override {
        cout << left << setw(5) << id << setw(15) << brand << setw(15) << model
             << setw(6) << year << setw(12) << price << setw(20) << engine.getType()
             << setw(15) << location.getCity() << setw(6) << "N/A"
             << (hasCarrier ? "Carrier" : "No Carrier") << endl;
    }

    double calculateTax() const override {
        return price * 0.02;
    }

    void saveToFile(ofstream& out) const override {
        out << "B\n" << id << "\n" << brand << "\n" << model << "\n" << year << "\n"
            << price << "\n" << engine.getType() << "\n" << engine.getHorsepower() << "\n"
            << vin << "\n" << location.getCity() << "\n" << location.getArea() << "\n"
            << hasCarrier << "\n";
    }
};

class Review {
private:
    string username;
    string content;
    int rating;
public:
    Review() : username(""), content(""), rating(0) {}
    Review(string u, string c, int r) : username(u), content(c), rating(r) {}
    void display() const {
        cout << "Rating: [" << rating << "/5] | User: " << username << " | " << content << endl;
    }
};

class Showroom {
private:
    Inventory<Vehicle*> inventory;
    Review reviews[10];
    int reviewCount;
    Admin admin;
    const double MAX_DISCOUNT;

public:
    Showroom() : inventory(100), reviewCount(0), MAX_DISCOUNT(30.0) {}

    ~Showroom() {
        for (int i = 0; i < inventory.getSize(); i++) {
            delete inventory.get(i);
        }
    }

    void addVehicle(Vehicle* v) {
        inventory.add(v);
    }

    void viewAllVehicles() const {
        cout << string(105, '-') << endl;
        cout << left << setw(5) << "ID" << setw(15) << "Brand" << setw(15) << "Model"
             << setw(6) << "Year" << setw(12) << "Price" << setw(20) << "Engine"
             << setw(15) << "City" << setw(6) << "Doors" << "Type" << endl;
        cout << string(105, '-') << endl;
        for (int i = 0; i < inventory.getSize(); i++) {
            inventory.get(i)->displayInfo();
        }
        cout << string(105, '-') << endl;
        cout << "Total Active Listings: " << Vehicle::getTotalVehicles() << endl;
    }

    void executeDiscount(double percentage) {
        if (percentage > MAX_DISCOUNT) {
            cout << "Discount exceeds maximum allowed limit." << endl;
            return;
        }
        for (int i = 0; i < inventory.getSize(); i++) {
            applyDiscount(*inventory.get(i), percentage);
        }
        cout << percentage << "% discount applied successfully." << endl;
    }

    void compareFirstTwo() const {
        if (inventory.getSize() < 2) {
            cout << "Need at least two vehicles to compare." << endl;
            return;
        }
        Vehicle* v1 = inventory.get(0);
        Vehicle* v2 = inventory.get(1);
        
        cout << "Comparing " << v1->getBrand() << " and " << v2->getBrand() << endl;
        if (*v1 > *v2) {
            cout << v1->getBrand() << " is priced higher." << endl;
        } else {
            cout << v2->getBrand() << " is priced higher or equal." << endl;
        }
        
        double maxPrice = getMaxValue<double>(v1->getPrice(), v2->getPrice());
        double combinedValue = *v1 + *v2;
        
        cout << "Highest Price: PKR " << maxPrice << endl;
        cout << "Combined Value: PKR " << combinedValue << endl;
    }

    void runSystemAudit() const {
        for (int i = 0; i < inventory.getSize(); i++) {
            admin.printConfidentialVin(*inventory.get(i));
        }
    }

    void appendReview(string u, string c, int r) {
        if (reviewCount < 10) {
            reviews[reviewCount] = Review(u, c, r);
            reviewCount++;
        }
    }

    void showReviews() const {
        cout << "\n--- Customer Reviews ---" << endl;
        for (int i = 0; i < reviewCount; i++) {
            reviews[i].display();
        }
    }

    void saveSystemData() const {
        ofstream out("pakwheels_data.txt");
        if (!out) return;
        out << inventory.getSize() << "\n";
        for (int i = 0; i < inventory.getSize(); i++) {
            inventory.get(i)->saveToFile(out);
        }
        out.close();
    }

    void loadSystemData() {
        ifstream in("pakwheels_data.txt");
        if (!in) return;
        int count;
        in >> count;
        for (int i = 0; i < count; i++) {
            string typeMarker;
            in >> typeMarker;
            
            int id, year, hp, doors;
            double price;
            string brand, model, eType, vin, city, area;
            bool flag; 
            
            if (typeMarker == "C") {
                in >> id >> brand >> model >> year >> price >> eType >> hp 
                   >> vin >> city >> area >> doors >> flag;
                Engine e(eType, hp);
                Location l(city, area);
                addVehicle(new Car(id, brand, model, year, price, e, vin, l, doors, flag));
            } else if (typeMarker == "B") {
                in >> id >> brand >> model >> year >> price >> eType >> hp 
                   >> vin >> city >> area >> flag;
                Engine e(eType, hp);
                Location l(city, area);
                addVehicle(new Bike(id, brand, model, year, price, e, vin, l, flag));
            }
        }
        in.close();
    }
};

int main() {
    Showroom pakWheels;
    pakWheels.loadSystemData();

    pakWheels.appendReview("Ali", "Excellent service and genuine cars.", 5);
    pakWheels.appendReview("Sara", "The buying process was very smooth.", 4);

    int choice = 0;
    while (choice != 8) {
        cout << "\n=== PakWheels Management Console ===" << endl;
        cout << "1. Register New Car" << endl;
        cout << "2. Register New Bike" << endl;
        cout << "3. View Inventory" << endl;
        cout << "4. Apply System-wide Discount" << endl;
        cout << "5. Price Analysis (First Two Elements)" << endl;
        cout << "6. View Customer Reviews" << endl;
        cout << "7. Perform Administrator Audit" << endl;
        cout << "8. Save State and Exit" << endl;
        cout << "Select Operation: ";
        cin >> choice;

        if (choice == 1) {
            int id, year, hp, doors;
            double price;
            string brand, model, engineType, vin, city, area;
            bool isSedan;
            cout << "ID: "; cin >> id;
            cout << "Brand: "; cin >> brand;
            cout << "Model: "; cin >> model;
            cout << "Year: "; cin >> year;
            cout << "Price: "; cin >> price;
            cout << "Engine Type: "; cin >> engineType;
            cout << "Horsepower: "; cin >> hp;
            cout << "VIN: "; cin >> vin;
            cout << "City: "; cin >> city;
            cout << "Area: "; cin >> area;
            cout << "Doors: "; cin >> doors;
            cout << "Is Sedan (1/0): "; cin >> isSedan;
            
            Engine e(engineType, hp);
            Location l(city, area);
            pakWheels.addVehicle(new Car(id, brand, model, year, price, e, vin, l, doors, isSedan));
        }
        else if (choice == 2) {
            int id, year, hp;
            double price;
            string brand, model, engineType, vin, city, area;
            bool hasCarrier;
            cout << "ID: "; cin >> id;
            cout << "Brand: "; cin >> brand;
            cout << "Model: "; cin >> model;
            cout << "Year: "; cin >> year;
            cout << "Price: "; cin >> price;
            cout << "Engine Type: "; cin >> engineType;
            cout << "Horsepower: "; cin >> hp;
            cout << "VIN: "; cin >> vin;
            cout << "City: "; cin >> city;
            cout << "Area: "; cin >> area;
            cout << "Has Carrier (1/0): "; cin >> hasCarrier;
            
            Engine e(engineType, hp);
            Location l(city, area);
            pakWheels.addVehicle(new Bike(id, brand, model, year, price, e, vin, l, hasCarrier));
        }
        else if (choice == 3) {
            pakWheels.viewAllVehicles();
        }
        else if (choice == 4) {
            double disc;
            cout << "Enter discount percentage: ";
            cin >> disc;
            pakWheels.executeDiscount(disc);
        }
        else if (choice == 5) {
            pakWheels.compareFirstTwo();
        }
        else if (choice == 6) {
            pakWheels.showReviews();
        }
        else if (choice == 7) {
            pakWheels.runSystemAudit();
        }
        else if (choice == 8) {
            pakWheels.saveSystemData();
            cout << "State persisted successfully. Terminating..." << endl;
        } else {
            cout << "Invalid operation code detected." << endl;
        }
    }
    return 0;
}
