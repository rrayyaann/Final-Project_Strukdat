#include <iostream>
#include <ctime>
#include <vector>
#include <limits>
#include <algorithm> // Added for find_if

using namespace std;

static int bookingCount = 0; // Renamed from count to bookingCount

class Person {
protected:
    string name;
    string phone;

public:
    virtual void getInfo() {
        cout << "Name: " << name << endl;
        cout << "Phone: " << phone << endl;
    }

    string getName() {
        return name;
    }

    string getPhone() {
        return phone;
    }
};

class Customer : public Person {
public:
    Customer() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        do {
            cout << "Enter name (letters only): ";
            getline(cin, name);
        } while (name.empty());

        do {
            cout << "Enter phone number (numbers only): ";
            getline(cin, phone);
        } while (!isNumber(phone));
    }

    void getInfo() override {
        cout << "Customer Name: " << name << endl;
        cout << "Customer Phone: " << phone << endl;
    }

private:
    bool isNumber(const string& s) {
        return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
    }
};

class Room : public Customer {
    string roomNo;
    int slotNo, bookingNo, billNo, charges;
    time_t checkInTime, checkOutTime;

public:
    void getBooking(int slot) {
        slotNo = slot;
        do {
            cout << "Enter room number (numbers only): ";
            getline(cin, roomNo);
        } while (!isValidRoomNumber(roomNo));

        checkInTime = time(0);
        bookingCount++; // Renamed from count to bookingCount
        string t = ctime(&checkInTime);
        bookingNo = bookingCount; // Renamed from count to bookingCount
        system("cls"); // To clear the screen in Visual C++, utilize the code
        displayBooking();
    }

    void displayBooking() {
        cout << "*** IT08 HOTEL BOOKING RECEIPT ***";
        cout << "\n\nBooking number: " << bookingNo;
        cout << "\nRoom Number: " << roomNo;
        cout << "\nCheck-in Time: " << ctime(&checkInTime);
        cout << "\nRoom Slot: " << slotNo;
        cout << "\nCurrent Status: UNPAID";
        cout << "\n\nEnter any key to go back to main menu..";
        char c = getchar();
        system("cls");
    }

    void getBill() {
        checkOutTime = time(0);
        billNo = bookingNo;
        calculateCharges();
    }

    int getBookingNo() {
        return bookingNo;
    }

    int getSlot() {
        return slotNo;
    }

    void calculateCharges() {
        long long duration = checkOutTime - checkInTime;
        int mins = duration / 60;
        int hours = mins / 60;
        int extra_min = mins % 60;
        if (hours == 0 && extra_min <= 30)
            charges = 50000;
        else if (hours <= 2 && extra_min == 0)
            charges = 150000;
        else if (hours <= 7)
            charges = 150000 + (hours - 2) * 50000;
        else
            charges = 500000;
        displayBill();
    }

    void displayBill() {
        cout << "*** IT08 HOTEL BILL ***";
        cout << "\n\nBill Number: " << billNo;
        cout << "\nRoom Number: " << roomNo;
        cout << "\nRoom Slot: " << slotNo;
        cout << "\nCheck-in Time: " << ctime(&checkInTime);
        cout << "\nCheck-out Time: " << ctime(&checkOutTime);
        cout << "\nTotal Charges: Rp. " << charges << "/-";
        cout << "\nCurrent Status: PAID";
        cout << "\n\nEnter any key to go back to main menu..";
        char c = getchar();
    }

private:
    bool isValidRoomNumber(const string& s) {
        if (!isNumber(s)) return false;
        int roomNumber = stoi(s);
        return roomNumber >= 1 && roomNumber <= 100;
    }

    bool isNumber(const string& s) {
        return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
    }
};

class Hotel {
    int capacity;
    int totalFilled;
    vector<bool> roomSlots;
    vector<Room> rooms;

public:
    Hotel() {
        capacity = 100;
        totalFilled = 0;
        for (int i = 0; i < capacity; ++i)
            roomSlots.push_back(false);
    }

    bool ifRoomAvailable() {
        return totalFilled < capacity;
    }

    int getFreeRoom() {
        for (int i = 0; i < capacity; ++i) {
            if (!roomSlots[i]) {
                roomSlots[i] = true;
                totalFilled++;
                return i + 1;
            }
        }
        return -1;
    }

    void newRoom(Room& x) {
        rooms.push_back(x);
    }

    void deleteRoom(int slot) {
        roomSlots[slot - 1] = false;
        totalFilled--;
    }

    Room* findDepartingRoom(int t) // t = booking number
    {
        for (int i = 0; i < rooms.size(); ++i) {
            int s = rooms[i].getBookingNo();
            if (t == s)
                return &rooms[i];
        }
        return nullptr;
    }

    int getCapacity() {
        return capacity;
    }

    int getTotalFilled() {
        return totalFilled;
    }

    void emptyRoomList() {
        for (int i = 0; i < roomSlots.size(); ++i) {
            if (!roomSlots[i])
                cout << i + 1 << ", ";
        }
    }

    void occupiedRoomList() {
        for (int i = 0; i < roomSlots.size(); ++i) {
            if (roomSlots[i])
                cout << i + 1 << ", ";
        }
    }
};

void roomCheckIn(Hotel& h) {
    if (h.ifRoomAvailable()) {
        int slot = h.getFreeRoom();
        Room obj;
        obj.getBooking(slot);
        h.newRoom(obj);
    }
    else {
        cout << "Sorry no empty room available!";
    }
}

void roomCheckOut(Hotel& h) {
    int bookingNo;
    cout << "Enter booking number: ";
    while (!(cin >> bookingNo) || bookingNo < 1 || bookingNo > 100) {
        cout << "Invalid input. Enter booking number (1-100): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Room* curr = h.findDepartingRoom(bookingNo);
    if (curr != nullptr) {
        h.deleteRoom(curr->getSlot());
        system("cls");
        curr->getBill();
    } else {
        cout << "Booking number does not exist.\n";
        cout << "\nEnter any key to return to main menu..";
        char c = getchar();
        system("cls");
    }
}

void displayDetails(Hotel& h) {
    cout << "\n\n1. Get the total capacity of Hotel";
    cout << "\n2. Get total number of occupied rooms";
    cout << "\n3. Get total number of empty rooms";
    cout << "\n4. Get list of empty rooms";
    cout << "\n5. Get list of occupied rooms";
    cout << "\n6. Get the total number of bookings";
    cout << "\n7. Exit";
    int choice;
    do {
        cout << "\nEnter your choice: ";
        cin >> choice;
        if (choice < 1 || choice > 7)
            cout << "\nInvalid Input";
        else
            break;
    } while (true);
    system("cls");
    int c;
    switch (choice) {
    case 1:
        c = h.getCapacity();
        cout << "\nThe capacity of hotel is " << c;
        break;
    case 2:
        c = h.getTotalFilled();
        cout << "\nThe number of occupied rooms is " << c;
        break;
    case 3:
        c = h.getCapacity() - h.getTotalFilled();
        cout << "\nThe number of empty rooms is " << c;
        break;
    case 4:
        cout << "\nList of empty rooms: ";
        h.emptyRoomList();
        break;
    case 5:
        cout << "\nList of occupied rooms: ";
        h.occupiedRoomList();
        break;
    case 6:
        cout << "\nThe total number of bookings: " << bookingCount; // Renamed from count to bookingCount
        break;
    case 7:
        break;
    }
    cin.ignore();
    cout << "\nEnter any key to return to main menu..";
    char ch = getchar();
    system("cls");
}

int main() {
    Hotel h;
    int choice;
    do {
        cout << "\n*** IT08 HOTEL MANAGEMENT SYSTEM ***";
        cout << "\n1. Check-in";
        cout << "\n2. Check-out";
        cout << "\n3. Display details";
        cout << "\n4. Exit";
        cout << "\nSelect an option (1, 2 or 3): ";
        cin >> choice;
        system("cls");
        switch (choice) {
        case 1:
            roomCheckIn(h);
            break;
        case 2:
            roomCheckOut(h);
            break;
        case 3:
            displayDetails(h);
            break;
        case 4:
            exit(0);
        default:
            cout << "Invalid Choice!";
            break;
        }
    } while (true);
    return 0;
}
