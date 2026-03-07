// Main en aşağıda bu headerleri doğru ayarladım mı diye kontrol için var
/*#include <iostream>
#include <exception>
#include <cctype>
#include <iomanip>
#include <algorithm>
using namespace std;

template <typename T> class VectorList
{
    struct Node
    {
        T data;
        Node* next;
        Node(T val): data(val), next(nullptr){}
    };
    int currentsize;
    Node* head;
    Node* tail;
    
public:
    VectorList(const VectorList& other) : head(nullptr), tail(nullptr), currentsize(0)
    {
            Node* walk = other.head;
            while (walk != nullptr)
            {
                this->push_back(walk->data);
                walk = walk->next;
            }
        }
    
    VectorList(VectorList&& other) noexcept : head(other.head), tail(other.tail), currentsize(other.currentsize)
    {
        other.head = nullptr;
        other.tail = nullptr;
        other.currentsize = 0;
    }

    VectorList& operator=(VectorList&& other) noexcept
    {
        if (this != &other)
        {
            Node* walk = head;
            while(walk)
            {
                Node* next = walk->next;
                delete walk;
                walk = next;
            }
            head = other.head;
            tail = other.tail;
            currentsize = other.currentsize;
            other.head = nullptr;
            other.tail = nullptr;
            other.currentsize = 0;
        }
        return *this;
    }

    VectorList(): head(nullptr), tail(nullptr), currentsize(0){}
    VectorList(int size,T val): head(nullptr), tail(nullptr), currentsize(0)
    {
        try {
            
            if(size <= 0)
            {
                cout << "Invalid number of entries." << endl;
                return;
            }
            head = new Node(val);
            tail = head;
            
            currentsize++;
            for(int i = 1; i < size; i++)
            {
                tail->next = new Node(val);
                tail = tail->next;
                currentsize++;
            }
        }catch(const bad_alloc& e){
            cout << "not enough memory from initializer: " << e.what() << endl;
            return;
        }
    }
    ~VectorList()
    {
        Node *walk = head;
        while(walk != nullptr)
        {
            Node* next_walk = walk->next;
            delete walk;
            walk = next_walk;
        }
    }
    
    void push_back(T val)
    {
        try{
            Node* node = new Node(val);
            if(head == nullptr)
            {
                head = tail = node;
            }
            else
            {
                tail->next = node;
                tail = node;
            }
            currentsize++;
        }catch(const bad_alloc& e){
            cout<< "not enough memory from push_back(): "<< e.what() << endl;
            return;
        }
    }
    
    void pop()
    {
        if(head == nullptr)
            return;
        if(head == tail)
        {
            delete head;
            head = tail = nullptr;
            currentsize--;
            return;
        }
        Node* walk = head;
        while(walk->next != tail)
        {
            walk = walk->next;
        }
        
        delete tail;
        tail = walk;
        tail->next = nullptr;
        currentsize--;
    }
    
    T& at(int index)
    {
        if(index < 0 || index >= currentsize)
        {
            cout << "inaccesible memory" << endl;
            throw out_of_range("Out of range access to the memory");
        }
        Node* walk = head;
        for(int i = 0; i < index; i++)
        {
            walk = walk->next;
        }
        return walk->data;
    }
    
    const T& at(int index) const
    {
        if(index < 0 || index >= currentsize)
        {
            cout << "inaccesible memory" << endl;
            throw out_of_range("Out of range access to the memory");
        }
        Node* walk = head;
        for(int i = 0; i < index; i++)
        {
            walk = walk->next;
        }
        return walk->data;
    }
    
    void delete_at(int index)
    {
        if(head == nullptr)
        {
            cout << "empty list" << endl;
            throw out_of_range("Empty list");
        }
        if(index < 0 || index >= currentsize)
        {
            cout << "inaccessible memory" << endl;
            throw out_of_range("Out of range access to the memory");
        }
        
        Node *walk;
        
        if(index == 0)
        {
            walk = head;
            head = head->next;
            if(head == nullptr)
            {
                tail = nullptr;
            }
            delete walk;
            currentsize--;
            return;
        }
        
        Node *steady = head;
        
        for(int i = 0; i < index - 1; ++i)
        {
            steady = steady->next;
        }
        walk = steady->next;
        steady->next = walk->next;
        if(walk == tail)
        {
            tail = steady;
        }
        delete walk;
        currentsize--;
    }
    
    void insert_at(int index, T val)
    {
        if(index < 0 || index > currentsize)
        {
            cout << "out of range access" << endl;
            throw out_of_range("out of range index");
        }
        try
        {
            if(index == 0)
            {
                Node *node = new Node(val);
                node->next = head;
                head = node;
                if(tail == nullptr)
                    tail = node;
            }
            else
            {
                Node *walk = head;
                for(int i = 0; i < index - 1; i++)
                {
                    walk = walk->next;
                }
                Node *node = new Node(val);
                node->next = walk->next;
                walk->next = node;
                if(walk == tail)
                {
                    tail = node;
                }
            }
            currentsize++;
        }catch(const bad_alloc& e){
            cout << "not enough space in memory: " << e.what() << endl;
        }
    }
    
    int size() const {return currentsize;}
};

class Passenger
{
    string name;
    string surname;
    char gender;
public:
    mutable string seat;
    Passenger(): name(""), surname(""), gender(' '){}
    Passenger(string n, string s, char g): name(n), surname(s), gender(g){}
    string getName() const {return name;}
    string getSurname() const {return surname;}
    char getGender() const {return gender;}
    void display() const
    {
        cout << left << setw(5) << seat << "|";
        if((name+surname).length() + 1 > 15)
        {
            string temp = name + " " + surname;
            cout << left << setw(15) << temp.substr(0,14) + "." << "|";
        }
        else
        {
            cout << left << setw(15) << name + " " + surname << "|";
        }
        cout << left << setw(6) << gender << endl;
    }

};

class Flight {
    string flightNo;
    string destination;
    int maxSeats;
    int numPassangers;
    VectorList<Passenger> passangers;
    
public:
    Flight(string f, string d, int m, int p):flightNo(f), destination(d), maxSeats(m), numPassangers(0){
        if(maxSeats > 80)
            maxSeats = 80;

        if(maxSeats % 2 != 0)
            maxSeats++;

        int initialToCreate = min(p, maxSeats);

        char letters[4] = {'A','B','C','D'};
        for(int i = 0; i < initialToCreate; i++)
        {
            Passenger temp("Initial","Passenger",'O');

            int row = (i / 4) + 1;
            int col = i % 4;

            temp.seat = to_string(row) + letters[col];

            passangers.push_back(temp);
            numPassangers++;
        }
    }
    Flight(string f, string d, int p):flightNo(f), destination(d), numPassangers(p), maxSeats(40){}
    string getDest() const {return destination;}
    string getNo() const {return flightNo;}
    void flightDisplay() const
    {
        cout << "-------------Front------------\n";
        char letters[4] = {'A', 'B', 'C', 'D'};
        for (int r = 0; r < maxSeats / 4; r++)
        {
            cout << "| ";
            for (int c = 0; c < 4; c++)
            {
                string currentSeat = to_string(r + 1) + letters[c];
                bool taken = false;
                
                
                for(int i = 0; i < passangers.size(); i++)
                {
                    if(passangers.at(i).seat == currentSeat)
                    {
                        taken = true;
                        break;
                    }
                }
                
                cout << currentSeat << (taken ? " [X] " : " [O] ") << "| ";
                if (c == 1) cout << "| ";
            }
            cout << endl;
        }
    }
    void reserveSeat(const Passenger& passenger)
    {
        if (numPassangers >= maxSeats)
        {
            cout << "Flight is full" << endl;
            return;
        }
        
        string choice;
        bool flag = false;
        while (!flag)
        {
            cout << "Enter Seat ID (e.g., 1A) or 'e' to exit: ";
            cin >> choice;
            if (choice == "e" || choice == "E")
                return;
            
            if(!validseat(choice))
            {
                cout << "Invalid seat ID" << endl;
                continue;
            }
            bool occupied = false;
            for (int i = 0; i < passangers.size(); i++)
            {
                if (passangers.at(i).seat == choice)
                {
                    occupied = true;
                    break;
                }
            }

            if (occupied)
            {
                cout << "That seat is already taken. Try again." << endl;
            } else
            {
                passenger.seat = choice;
                passangers.push_back(passenger);
                numPassangers++;
                cout << "Seat " << choice << " reserved for " << passenger.getName() << endl;
                flag = true;
            }
        }
    }
    bool validseat(const string& s)
    {
        if(s.length() < 2)
                return false;

            char letter = toupper(s.back());
            if(letter < 'A' || letter > 'D')
                return false;

            string numberPart = s.substr(0, s.size()-1);

            if(!all_of(numberPart.begin(), numberPart.end(), ::isdigit))
                return false;

            int row = stoi(numberPart);

            return row >= 1 && row <= maxSeats/4;

    }
    void cancelReservation(const string& seatID)
    {
        for (int i = 0; i < passangers.size(); i++)
        {
            if (passangers.at(i).seat == seatID)
            {
                passangers.delete_at(i);
                numPassangers--;
                cout << "Reservation for seat " << seatID << " canceled." << endl;
                return;
            }
        }
        cout << "No passenger found in seat " << seatID << endl;
    }
    int numberOfPassengers() const {return passangers.size();}
    void printPassengers()
    {
        cout << "Passanger List for Flight " << flightNo << endl;
        cout << "Seat|"<< "Passenger Name |" <<"Gender" << endl;
        cout << "----|---------------|------"<< endl;
        for(int i = 0; i < passangers.size(); i++)
        {
            passangers.at(i).display();
        }
    }
    bool isFlyingTo(const string& destination)
    {
        if(destination == this->destination)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void display() const
    {
        cout << "Flight No: " << flightNo << endl
             << "Destination: "<< destination << endl
             << "Max Seats: " << maxSeats << endl
        << "Number of Passangers: "<< numPassangers << endl;
    }
};

class FlightManager {
    VectorList<Flight> flights;
    
public:
    void addFlight(const Flight& flight)
    {
        try{
            for(int i = 0; i < flights.size(); i++)
            {
                if(flight.getNo() == flights.at(i).getNo())
                {
                    cout << "A flight like this already exists." << endl;
                    return;
                }
            }
                flights.push_back(flight);
                cout << "flight added successfully" << endl;
        }catch(const out_of_range& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
    void removeFlight(const string& FlightNumber)
    {
        for(int i = 0; i < flights.size(); i++)
        {
            if(flights.at(i).getNo() == FlightNumber)
            {
                try{
                    flights.delete_at(i);
                    cout << "flight deleted successfully" << endl;
                    return;
                }catch(const out_of_range& e)
                {
                    cout << "Error: " << e.what() << endl;
                }
            }
        }
        cout << "There is no flight with tihs flight number: " << FlightNumber << endl;
    }
    void listAllFlights()
    {
        for(int i = 0; i < flights.size(); i++)
        {
            cout << "Flight " << i+1 << ":" << endl;
            flights.at(i).display();
            cout << endl;
        }
    }
    Flight* getFlightByNumber(const string& FlightNumber)
    {
        for(int i = 0; i < flights.size(); i++)
        {
            if(flights.at(i).getNo() == FlightNumber)
            {
                return &flights.at(i);
            }
        }
        cout << "A flight with this flight number does not exists" << endl;
        return nullptr;
    }
    Flight* getFlightByDestination(const string& Destination)
    {
        for(int i = 0; i < flights.size(); i++)
        {
            if(flights.at(i).getDest() == Destination)
            {
                return &flights.at(i);
            }
        }
        cout << "A flight to this destination does not exists" << endl;
        return nullptr;
    }
};

void flight_menu(Flight* flight)
{
    int choice;
    while (true) {
        cout << "Second-Level Menu: Passanger Management(For a Selected Flight)" << endl;
        do {
            cout << "1. Reserve a Seat(By displaying seating plan)"<< endl
            <<"2. Cancel Reservation" << endl
            <<"3. View Passenger List" << endl
            <<"4. Back To Flight Management Menu" << endl
            << ": ";
            cin >> choice;
        } while (choice < 1 || choice > 4);
        string seat, name, surname;
        char gender;
        switch (choice) {
            case 1:
                flight->flightDisplay();
                cout << "Enter Passengers Name then Surname: ";
                cin >> name >> surname;
                do {
                    cout << "Enter Passengers Gender(Uppercase)(M for Male, F for Female, O for Other): ";
                    cin >> seat;
                    gender = toupper(static_cast<unsigned char>(seat[0]));
                    if(gender != 'M' && gender != 'F' && gender != 'O')
                    {
                        cout << "Please enter a valid gender." << endl;
                    }
                    
                    cin.clear();
                    cin.ignore(1000, '\n');
                } while (gender != 'M' && gender != 'F' && gender != 'O');
                seat = "";
                flight->reserveSeat(Passenger(name, surname, gender));
                break;
            case 2:
                flight->flightDisplay();
                cout << "Enter the seat Id for the passenger: ";
                cin >> seat;
                flight->cancelReservation(seat);
                break;
            case 3:
                flight->printPassengers();
                break;
            case 4:
                return;
        }
    }
}
void top_level_menu()
{
    FlightManager manager;
    while(true)
    {
        int choice;
        do {
            cout << "Top-Level Menu: Flight Management" << endl
            << "1. Add a Flight" << endl
            << "2. Remove a Flight" << endl
            << "3. List All Flights" << endl
            << "4. Select a Flight and Manage passengers" << endl
            << "5. Exit" << endl
            <<": ";
            
            cin >> choice;
            if(choice > 5 || choice < 1)
            {
                cout << "Invalid operation" << endl;
            }
        } while (choice > 5 || choice < 1);
        string flightdest;
        string flightNo;
        string temp;
        int seats = -3;
        int passangers = -2;
        int prefix;
        bool flag = false;
        switch(choice)
        {
            case 1:
                do {
                    do {
                        cout << "choose between LJ(0), BA(1), TK(2), PG(3): ";
                        cin >> prefix;
                        if(prefix <0 || prefix > 3)
                        {
                            cout << "Invalid choice" << endl;
                        }
                        if(prefix == 0)
                            flightNo = "LJ";
                        else if(prefix == 1)
                            flightNo = "BA";
                        else if (prefix == 2)
                            flightNo = "TK";
                        else
                            flightNo = "PG";
                        
                    } while (prefix <0 || prefix > 3);
                    
                    cout << "Enter The rest of the FlightCode(Must contain only digits): ";
                    cin >> temp;
                    
                    if(!all_of(temp.begin(), temp.end(), ::isdigit))
                    {
                        cout <<"Rest of the flight Number must be numbers." << endl;
                        continue;
                    }
                    flightNo = flightNo + temp;
                    cout << "Enter flight destination of the plane: ";
                    cin.ignore();
                    getline(cin, flightdest);
                    cout << "enter the amount of the seats the plane have: ";
                    cin >> seats;
                    cout << "enter the amount of initial passengers: ";
                    cin >> passangers;
                    if(seats > 80 || seats < 0 || seats < passangers)
                    {
                        cout << "Invalid Passenger/Seat input." << endl;
                    }
                    else
                    {
                        flag = true;
                    }
                    cin.ignore(1000, '\n');
                } while (!flag);
                manager.addFlight(Flight(flightNo, flightdest, seats, passangers));
                break;
            case 2:
                cout << "Enter the flight number of the flight you want to delete: ";
                cin >> flightNo;
                manager.removeFlight(flightNo);
                break;
            case 3:
                manager.listAllFlights();
                break;
            case 4:
                cout << "Enter Flight Number of the flight: ";
                cin >> flightNo;
                if(manager.getFlightByNumber(flightNo) != nullptr)
                    flight_menu(manager.getFlightByNumber(flightNo));
                break;
            case 5:
                return;
        }
    }
}
int main()
{
    top_level_menu();
}
*/
#include "FlightSystem.h"

int main()
{
    top_level_menu();
    return 0;
}
