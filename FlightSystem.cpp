#include "FlightSystem.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;
// Passenger getter setterları ve initializerları
Passenger::Passenger(): name(""), surname(""), gender(' '){}
Passenger::Passenger(string n, string s, char g): name(n), surname(s), gender(g){}
string Passenger::getName() const {return name;}
string Passenger::getSurname() const {return surname;}
char Passenger::getGender() const {return gender;}
/*bizden istenilen şekilde yolcu değerlerini yazdırır*/
void Passenger::display() const
{
    // beş boşluk sola yatık koltuk değerini yazdır
    cout << left << setw(5) << seat << "|";
    // ismin büyüklüğüne göre isim kısalıcak ve sonunda nokta olucak
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
// Flight ın initializerı
Flight::Flight(string f, string d, int m, int p):flightNo(f), destination(d), maxSeats(m), numPassangers(0){
    // Girdi koruma
    if(maxSeats > 80)
        maxSeats = 80;

    if(maxSeats % 2 != 0)
        maxSeats++;
    // P 80 den küçük olursa p yi kaale al olmazsa maxseatı
    // numPassengers memory alloc kısmında patlayabilir 8 yolcu varken 40 yolcu kösterebilir
    int initialToCreate = min(p, maxSeats);
    // koltuk formatı
    char letters[4] = {'A','B','C','D'};
    // İstenilen yolcu kadar yolcu oluşturmaya çalış
    for(int i = 0; i < initialToCreate; i++)
    {
        // yolcuların bilgilerini al
        string name, surname, seat;
        char gender;
        // koltuk düzeninini yazdır
        flightDisplay();
        cout << "Enter Passengers Name: ";
        getline(cin, name);
        cout << "Enter Passengers Surname: ";
        getline(cin, surname);
        do {
            cout << "Enter Passengers Gender(Uppercase)(M for Male, F for Female, O for Other): ";
            cin >> seat;
            gender = toupper(static_cast<unsigned char>(seat[0]));
            if(gender != 'M' && gender != 'F' && gender != 'O')
            {
                cout << "Please enter a valid gender." << endl;
            }
            // bufferı temizlemek için
            cin.clear();
            cin.ignore(1000, '\n');
        } while (gender != 'M' && gender != 'F' && gender != 'O');
        // herhangi istenmeyen input girilmesin
        seat = "";
        Passenger temp(name,surname,gender);
        // i/4 1 2 3 ü
        // i%4 A B C yi belirler
        int row = (i / 4) + 1;
        int col = i % 4;
        // yolcunun koltuğuna ata
        temp.seat = to_string(row) + letters[col];
        // yolcuyu ekle
        passangers.push_back(temp);
        // yolcu sayısını arttır
        numPassangers++;
    }
}
// Boş initializer
Flight::Flight(string f, string d, int p):flightNo(f), destination(d), numPassangers(p), maxSeats(40){}
// getterlar
string Flight::getDest() const {return destination;}
string Flight::getNo() const {return flightNo;}
void Flight::flightDisplay() const
{
    // yolcu sayısına göre verilen formatta koltukları boş/dolu şeklinde bastırır
    cout << "Empty Seat: O      Taken Seat: X" << endl;
    cout << "-------------Front------------\n";
    char letters[4] = {'A', 'B', 'C', 'D'};
    int r;
    for (r = 0; r < maxSeats / 4; r++)
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
            if (c == 1)
                cout << "| ";
        }
        cout << endl;
    }
    // 4 ün katı olmayan girdiler için ekstra koltukları yazdırır
    if(maxSeats%4)
    {
        cout << "| ";
        for(int c = 0; c < maxSeats%4; c++)
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
            if(c == 1)
                cout << "| ";
        }
        cout << endl;
    }
}
// Yolcu ekleme fonksiyonu
void Flight::reserveSeat(const Passenger& passenger)
{
    // Kapasiteden daha fazla yolcu eklemeyi önler
    if (numPassangers >= maxSeats)
    {
        cout << "Flight is full" << endl;
        return;
    }
    // Menü de işlem yaparken bufferda girdi kalıyor
    cin.ignore(1000, '\n');
    string choice;
    bool flag = false;
    // Yolcuların bilgilerinin girildiği ve girdilerin doğruluğunu kontrol etme
    while (!flag)
    {
        cout << "Enter Seat ID (e.g., 1A): ";
        cin >> choice;
    
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
        // girlen koltuğun dolu olup olmadığını kontrol etme
        if (occupied)
        {
            cout << "That seat is already taken." << endl;
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
// Girilen koltuğun düzgün girilip girilmediğini kontrol eder
bool Flight::validseat(const string& s)
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
// Uçuştan Yolcu silme
void Flight::cancelReservation(const string& seatID)
{
    // Yolcuları tara yolcuyu bulursan sil bulamazsan hata metni yazdır
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
// Yolcu sayısını döndür listenin büyüklüğünü döndürmemin sebebi eğer memory alloc yapılamazsa
int Flight::numberOfPassengers() const {return passangers.size();}
// Yolcuları yazdır
void Flight::printPassengers()
{
    cout << "Passanger List for Flight " << flightNo << endl;
    cout << "Seat |"<< "Passenger Name |" <<"Gender" << endl;
    cout << "-----|---------------|------"<< endl;
    for(int i = 0; i < passangers.size(); i++)
    {
        passangers.at(i).display();
    }
}
// Verilen girdi(Ulaşılacak yer) ve uçağınki aynı mı diye kontrol etme
bool Flight::isFlyingTo(const string& destination)
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
// Uçuşu yazdır
void Flight::display() const
{
    cout << "Flight No: " << flightNo << endl
         << "Destination: "<< destination << endl
         << "Max Seats: " << maxSeats << endl
    << "Number of Passangers: "<< numPassangers << endl;
}
// Uçuş ekleme
void FlightManager::addFlight(const Flight& flight)
{
    try{
        // zaten böyle bir uçuş var mı diye kontrol et yoksa listese ekle varsa hata ver
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
// Uçuş kaldırma
void FlightManager::removeFlight(const string& FlightNumber)
{
    // Uçuşları gezin varsa kaldır yoksa hata ver
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
// Tüm uçuşları yazdırırı
void FlightManager::listAllFlights()
{
    for(int i = 0; i < flights.size(); i++)
    {
        cout << "Flight " << i+1 << ":" << endl;
        flights.at(i).display();
        cout << endl;
    }
}
// Numarısıyla uçuş arar yoksa hata verir
Flight* FlightManager::getFlightByNumber(const string& FlightNumber)
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
// Varış noktasıyla Uçuş Arar yoksa hata verir
Flight* FlightManager::getFlightByDestination(const string& Destination)
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
// Uçuş menüsü
void flight_menu(Flight* flight)
{
    int choice;
    // 4 girilene kadar menüde kal
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
                // Yolcu bilgilerini gir
                flight->flightDisplay();
                cout << "Enter Passengers Name: ";
                getline(cin, name);
                cout << "Enter Passengers Surname: ";
                getline(cin, surname);
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
                // Önceki menüye geri dön
                return;
        }
    }
}
// Manager menüsü
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
                    
                    cout << "Enter The rest of the FlightCode(Must contain only 3 digits): ";
                    cin >> temp;
                    // kullanıcının harf ya da 3 den fazla rakamlı sayı girmesini engelle
                    if(!all_of(temp.begin(), temp.end(), ::isdigit) || temp.length() != 3)
                    {
                        cout <<"Rest of the flight Number must be numbers with the length of 3." << endl;
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
                   if(seats > 80 || seats <= 0 || seats < passangers)
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
