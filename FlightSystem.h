#ifndef FLIGHTSYSTEM_H
#define FLIGHTSYSTEM_H

#include <string>
#include "VectorList.h"

class Passenger
{
    string name;
    string surname;
    char gender;
public:
    mutable string seat;
    Passenger();
    Passenger(string n, string s, char g);
    string getName() const;
    string getSurname() const;
    char getGender() const;
    void display() const;
};

class Flight {
    string flightNo;
    string destination;
    int maxSeats;
    int numPassangers;
    VectorList<Passenger> passangers;
    
public:
    Flight(string f, string d, int m, int p);
    Flight(string f, string d, int p);
    string getDest() const;
    string getNo() const;
    void flightDisplay() const;
    void reserveSeat(const Passenger& passenger);
    bool validseat(const string& s);
    void cancelReservation(const string& seatID);
    int numberOfPassengers() const;
    void printPassengers();
    bool isFlyingTo(const string& destination);
    void display() const;
};

class FlightManager {
    VectorList<Flight> flights;
    
public:
    void addFlight(const Flight& flight);
    void removeFlight(const string& FlightNumber);
    void listAllFlights();
    Flight* getFlightByNumber(const string& FlightNumber);
    Flight* getFlightByDestination(const string& Destination);
};

void flight_menu(Flight* flight);
void top_level_menu();

#endif
