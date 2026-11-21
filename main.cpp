
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

enum RelationType { GREATER, LESS, EQUAL };

class Flight {
public:
    int id;
    string source, destination, airline;
    string departureTime, arrivalTime;
    string duration;
    float ticketPrice;
    string status;
    int delayMinutes;
    int priorityScore;

    RelationType ComparedTo(Flight item) {
        if (priorityScore > item.priorityScore) {
            return GREATER;
        }
        else if (priorityScore < item.priorityScore) {
            return LESS;
        }
        else if (departureTime < item.departureTime) {
            return GREATER;
        }
        else if (departureTime > item.departureTime) {
            return LESS;
        }
        else if (id < item.id) {
            return GREATER;
        }
        else if (id > item.id) {
            return LESS;
        }
        else {
            return EQUAL;
        }
    }

    void Print() {
        cout << endl << "Flight ID: " << id;
        cout << endl << "Source: " << source;
        cout << endl << "Destination: " << destination;
        cout << endl << "Airline: " << airline;
        cout << endl << "Departure Time: " << departureTime;
        cout << endl << "Arrival Time: " << arrivalTime;
        cout << endl << "Duration: " << duration << " hours";
        cout << endl << "Ticket Price: " << ticketPrice;
        cout << endl << "Status: " << status;
        cout << endl << "Delay Minutes: " << delayMinutes;
        cout << endl << "Priority Score: " << priorityScore;
        cout << endl;
    }
};

class RunwayCommand {
private:
    static const int MAX_FLIGHTS = 100;
    Flight heap[MAX_FLIGHTS];
    int heapSize;

public:

    int getBaseScore(string status) {
        if (status == "Emergency") {
            return 300;
        }
        else if (status == "Delayed") {
            return 200;
        }
        else {
            return 100;
        }
    }

    int calculatePriority(Flight f) {
        return getBaseScore(f.status) + f.delayMinutes;
    }

    bool higherPriority(Flight a, Flight b) {
        return a.ComparedTo(b) == GREATER;
    }

    int findFlight(int flightID) {
        for (int i = 0; i < heapSize; i++) {
            if (heap[i].id == flightID) {
                return i;
            }
        }
        return -1;
    }

    void heapIncreaseKey(int index, int newPriority) {
        heap[index].priorityScore = newPriority;

        while (index > 0) {
            int parent = (index - 1) / 2;

            if (higherPriority(heap[index], heap[parent])) {
                Flight temp = heap[index];
                heap[index] = heap[parent];
                heap[parent] = temp;

                index = parent;
            }
            else {
                break;
            }
        }
    }

    void maxHeapify(Flight arr[], int size, int index) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && higherPriority(arr[left], arr[largest])) {
            largest = left;
        }

        if (right < size && higherPriority(arr[right], arr[largest])) {
            largest = right;
        }

        if (largest != index) {
            Flight temp = arr[index];
            arr[index] = arr[largest];
            arr[largest] = temp;

            maxHeapify(arr, size, largest);
        }
    }

    bool insertFlight(Flight f) {
        if (heapSize == MAX_FLIGHTS) {
            cout << endl << "Flight queue is full!" << endl;
            return false;
        }

        heap[heapSize] = f;
        int index = heapSize;
        heapSize++;

        heapIncreaseKey(index, f.priorityScore);

        return true;
    }

    RunwayCommand() {
        heapSize = 0;
    }

    void loadFlights() {
        ifstream file("priority_flights.txt");

        if (!file) {
            cout << endl << "No existing flight file found.";
            cout << endl << "Starting with an empty queue." << endl;
            return;
        }

        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            Flight f;
            int savedPriority;

            ss >> f.id;
            ss.ignore();

            getline(ss, f.source, ',');
            getline(ss, f.destination, ',');
            getline(ss, f.airline, ',');
            getline(ss, f.departureTime, ',');
            getline(ss, f.arrivalTime, ',');

            getline(ss, f.duration, ',');

            ss >> f.ticketPrice;
            ss.ignore();

            getline(ss, f.status, ',');

            ss >> f.delayMinutes;
            ss.ignore();

            ss >> savedPriority;

            if (ss.fail()) {
                continue;
            }

            if (findFlight(f.id) == -1 && heapSize < MAX_FLIGHTS) {
                f.priorityScore = calculatePriority(f);
                insertFlight(f);
            }
        }

        file.close();

        cout << endl << "Flights loaded successfully!";
        cout << endl << "Total flights: " << heapSize << endl;
    }

    void registerFlight() {
        Flight f;

        cout << endl << "Enter Flight ID: ";
        cin >> f.id;

        if (findFlight(f.id) != -1) {
            cout << endl << "This Flight ID already exists!" << endl;
            return;
        }

        cout << "Enter Source: ";
        cin >> f.source;

        cout << "Enter Destination: ";
        cin >> f.destination;

        cout << "Enter Airline: ";
        cin >> f.airline;

        cout << "Enter Departure Time (HH:MM): ";
        cin >> f.departureTime;

        cout << "Enter Arrival Time (HH:MM): ";
        cin >> f.arrivalTime;

        cout << "Enter Duration (hours): ";
        cin >> f.duration;

        cout << "Enter Ticket Price: ";
        cin >> f.ticketPrice;

        cout << "Enter Status (Scheduled/Delayed): ";
        cin >> f.status;

        if (f.status != "Scheduled" && f.status != "Delayed") {
            cout << endl << "Invalid status!" << endl;
            return;
        }

        if (f.status == "Delayed") {
            cout << "Enter Delay Minutes (1-99): ";
            cin >> f.delayMinutes;

            if (f.delayMinutes < 1 || f.delayMinutes > 99) {
                cout << endl << "Invalid delay minutes!" << endl;
                return;
            }
        }
        else {
            f.delayMinutes = 0;
        }

        f.priorityScore = calculatePriority(f);

        if (insertFlight(f)) {
            cout << endl << "Flight registered successfully!";
            cout << endl << "Priority Score: " << f.priorityScore << endl;
        }
    }

    void viewNextFlight() {
        if (heapSize == 0) {
            cout << endl << "No flights waiting for takeoff!" << endl;
            return;
        }

        cout << endl << "Next Flight for Takeoff:";
        heap[0].Print();
    }

    void clearFlightForTakeoff() {
        if (heapSize == 0) {
            cout << endl << "No flights to clear. Queue is empty!" << endl;
            return;
        }

        cout << endl << "Flight cleared for takeoff:";
        heap[0].Print();

        heap[0] = heap[heapSize - 1];
        heapSize--;

        if (heapSize > 0) {
            maxHeapify(heap, heapSize, 0);
        }
    }

    void declareEmergency() {
        int flightID;

        cout << endl << "Enter Flight ID: ";
        cin >> flightID;

        int index = findFlight(flightID);

        if (index == -1) {
            cout << endl << "Flight ID not found!" << endl;
            return;
        }

        if (heap[index].status == "Emergency") {
            cout << endl << "This flight is already an Emergency!" << endl;
            return;
        }

        heap[index].status = "Emergency";

        int newScore = calculatePriority(heap[index]);

        heapIncreaseKey(index, newScore);

        cout << endl << "Emergency declared successfully!";
        cout << endl << "New Priority Score: " << newScore << endl;
    }

    void reportFlightDelay() {
        int flightID;
        int extraMinutes;

        cout << endl << "Enter Flight ID: ";
        cin >> flightID;

        int index = findFlight(flightID);

        if (index == -1) {
            cout << endl << "Flight ID not found!" << endl;
            return;
        }

        cout << "Enter additional delay minutes: ";
        cin >> extraMinutes;

        if (extraMinutes < 1) {
            cout << endl << "Delay must be at least 1 minute!" << endl;
            return;
        }

        if (heap[index].delayMinutes + extraMinutes > 99) {
            cout << endl << "Total delay cannot exceed 99 minutes!" << endl;
            return;
        }

        heap[index].delayMinutes =
            heap[index].delayMinutes + extraMinutes;

        if (heap[index].status == "Scheduled") {
            heap[index].status = "Delayed";
        }

        int newScore = calculatePriority(heap[index]);

        heapIncreaseKey(index, newScore);

        cout << endl << "Delay reported successfully!";
        cout << endl << "New Priority Score: " << newScore << endl;
    }

    void displayDepartureBoard() {
        if (heapSize == 0) {
            cout << endl << "No flights on the departure board!" << endl;
            return;
        }

        Flight tempHeap[MAX_FLIGHTS];
        int tempSize = heapSize;

        for (int i = 0; i < heapSize; i++) {
            tempHeap[i] = heap[i];
        }

        cout << endl << "====================== DEPARTURE BOARD ======================";
        cout << endl;
        cout << left << setw(10) << "Position" << setw(8) << "ID"
            << setw(14) << "Destination" << setw(12) << "Airline"
            << setw(12) << "Status" << "Score" << endl;

        int position = 1;

        while (tempSize > 0) {
            cout << left << setw(10) << position;
            cout << setw(8) << tempHeap[0].id;
            cout << setw(14) << tempHeap[0].destination;
            cout << setw(12) << tempHeap[0].airline;
            cout << setw(12) << tempHeap[0].status;
            cout << tempHeap[0].priorityScore << endl;

            tempHeap[0] = tempHeap[tempSize - 1];
            tempSize--;

            if (tempSize > 0) {
                maxHeapify(tempHeap, tempSize, 0);
            }

            position++;
        }

        cout << "==========================================================" << endl;
    }

    void saveFlights() {
        ofstream file("priority_flights.txt");

        if (!file) {
            cout << endl << "Unable to save flight data!" << endl;
            return;
        }

        for (int i = 0; i < heapSize; i++) {
            file << heap[i].id << ",";
            file << heap[i].source << ",";
            file << heap[i].destination << ",";
            file << heap[i].airline << ",";
            file << heap[i].departureTime << ",";
            file << heap[i].arrivalTime << ",";
            file << heap[i].duration << ",";
            file << heap[i].ticketPrice << ",";
            file << heap[i].status << ",";
            file << heap[i].delayMinutes << ",";
            file << heap[i].priorityScore << endl;
        }

        file.close();

        cout << endl << "Waiting flights saved successfully!" << endl;
    }
};

int main() {
    RunwayCommand runway;

    runway.loadFlights();

    int choice;

    do {
        cout << endl << "============== RUNWAY COMMAND ==============";
        cout << endl << "1. Register Flight";
        cout << endl << "2. View Next Flight";
        cout << endl << "3. Clear Flight for Takeoff";
        cout << endl << "4. Declare Emergency";
        cout << endl << "5. Report Flight Delay";
        cout << endl << "6. Display Departure Board";
        cout << endl << "0. Exit";
        cout << endl << "=============================================";
        cout << endl << "Enter your choice: ";

        cin >> choice;

        switch (choice) {
        case 1:
            runway.registerFlight();
            break;

        case 2:
            runway.viewNextFlight();
            break;

        case 3:
            runway.clearFlightForTakeoff();
            break;

        case 4:
            runway.declareEmergency();
            break;

        case 5:
            runway.reportFlightDelay();
            break;

        case 6:
            runway.displayDepartureBoard();
            break;

        case 0:
            runway.saveFlights();
            cout << endl << "Program ended. Goodbye!" << endl;
            break;

        default:
            cout << endl << "Invalid choice! Try again." << endl;
        }

    } while (choice != 0);

    return 0;
}