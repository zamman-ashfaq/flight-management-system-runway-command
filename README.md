# Runway Command

This is my Module 2 project for Data Structures and Algorithms. It is a C++ program for an airport that has only one runway. The program decides which flight should take off first. I used a max-heap (priority queue) made with an array.

## What the program can do

1. Register a new flight
2. View the next flight to take off
3. Clear the next flight for takeoff
4. Declare an emergency for a flight
5. Report extra delay for a flight
6. Show the departure board
0. Exit and save the flights

## How the priority works

Each flight gets a score. The program calculates it, the user does not type it.

Score = base score + delay minutes

- Emergency = 300
- Delayed = 200
- Scheduled = 100

The flight with the highest score takes off first. If two flights have the same score, the one with the earlier departure time goes first. If that is also the same, the smaller flight ID goes first.

Delay can be at most 99 minutes, so a delayed flight can never go above an emergency flight.

## How to run

1. Open main.cpp in Visual Studio (or any C++ compiler).
2. Keep priority_flights.txt in the same folder as the project.
3. Run the program and choose an option from the menu.

## Saving flights

When the program starts, it reads the flights from priority_flights.txt. When you choose 0 to exit, it saves the waiting flights back to the same file. Each flight is on one line and the fields are separated by commas:

ID, Source, Destination, Airline, Departure, Arrival, Duration, Price, Status, Delay, Score

Do not put commas or spaces inside a field. For example, write SereneAir, not Serene Air.

## Tests I checked

1. Load the file and show the board. The order is 205, 203, 206, 202, 201, 204.
2. Delay flight 201 by 30 minutes. The order is 205, 201, 203, 206, 202, 204.
3. Declare an emergency for flight 204. It comes first.
4. Clear the next flight. Flight 204 leaves and the order is 205, 201, 203, 206, 202.
5. Exit and run again. The same order loads.
6. A duplicate flight ID, a flight ID that does not exist, and a delay of 0 minutes are all rejected with a message.
7. Clearing a flight when the queue is empty shows a message and does not crash.

## Files

- main.cpp
- priority_flights.txt
- README.md
