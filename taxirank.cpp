
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stack>
#include <queue>

using namespace std;

const int MAX_CAPACITY = 5;

// Passenger as a struct
struct Passenger
{
    int passengerCount; // Passenger number as they got to the taxi rank
    int timeInst;       // Time instance the passenger arrives at the taxi rank
    char route;         // Route the passenger is taking ('S', 'L', or 'C')
    int boardingTime;   // Amount of time it takes for the passenger to board
};

class TaxiRank
{
    private:
        int timeCounter;

        // Taxi Capacity
        int taxiS;
        int taxiL;
        int taxiC;

        // Amount of passengers in taxi rank per route
        int passengerCountS;
        int passengerCountL;
        int passengerCountC;

        // Waiting queues
        queue<Passenger> waitingS;
        queue<Passenger> waitingL;
        queue<Passenger> waitingC;

        // Currently boarding
        Passenger boardingS;
        Passenger boardingL;
        Passenger boardingC;

    public:
        TaxiRank()
        {
            timeCounter = 0;

            taxiS = MAX_CAPACITY;
            taxiL = MAX_CAPACITY;
            taxiC = MAX_CAPACITY;

            passengerCountS = 0;
            passengerCountL = 0;
            passengerCountC = 0;

            // Initialise boarding passenger for each route to empty indicator ('-')
            boardingS.passengerCount = 0;
            boardingS.timeInst = 0;
            boardingS.route = '-';
            boardingS.boardingTime = 0;

            boardingL.passengerCount = 0;
            boardingL.timeInst = 0;
            boardingL.route = '-';
            boardingL.boardingTime = 0;

            boardingC.passengerCount = 0;
            boardingC.timeInst = 0;
            boardingC.route = '-';
            boardingC.boardingTime = 0;

            // Initialise table output
            cout << left << setw(5)  << "T"
                 << left << setw(10) << "S"
                 << left << setw(10) << "L"
                 << left << setw(10) << "C"
                 << left << setw(15) << "WQS"
                 << left << setw(15) << "WQL"
                 << left << setw(15) << "WQC"
                 << left << setw(5)  << "CS"
                 << left << setw(5)  << "CL"
                 << left << setw(5)  << "CC"
                 << "\n";
        }

        int getCurrentTime() 
        {
            return timeCounter;
        }

        void increaseTimeByOne()
        {
            timeCounter++;
        }

        int getPassengerCount(char route)
        {
            switch (route)
            {
                case 'S':
                    return passengerCountS;
                    break;
                case 'L':
                    return passengerCountL;
                    break;
                case 'C':
                    return passengerCountC;
                    break;
            }
        }

        void increasePassengerCount(char route)
        {
            switch (route)
            {
                case 'S':
                    passengerCountS++;
                    break;
                case 'L':
                    passengerCountL++;
                    break;
                case 'C':
                    passengerCountC++;
                    break;
            }
        }

        void sendFullTaxis()
        {
            if (taxiS == 0) { taxiS = MAX_CAPACITY; }
            if (taxiL == 0) { taxiL = MAX_CAPACITY; }
            if (taxiC == 0) { taxiC = MAX_CAPACITY; }
        }

        Passenger getCurrentPassengerBoarding(char route)
        {
            switch (route) 
            {
                case 'S':
                    return boardingS;
                    break;
                case 'L':
                    return boardingL;
                    break;
                case 'C':
                    return boardingC;
                    break;
            }
        }

        void setCurrentPassengerBoarding(Passenger passenger)
        {
            switch (passenger.route) 
            {
                case 'S':
                    boardingS = passenger;
                    break;
                case 'L':
                    boardingL = passenger;
                    break;
                case 'C':
                    boardingC = passenger;
                    break;
            }
        }

        void decreaseAllPassengerBoardingTime()
        {
            if (boardingS.route != '-' && boardingS.boardingTime > 0) { boardingS.boardingTime--; }
            if (boardingL.route != '-' && boardingL.boardingTime > 0) { boardingL.boardingTime--; }
            if (boardingC.route != '-' && boardingC.boardingTime > 0) { boardingC.boardingTime--; }
        }

        void boardValidPassengers() 
        {
            // If a valid passenger is boarding and their boarding time is zero (0) then 
            // board them onto the taxi and decrease the taxi capacity by 1 and
            // initialise boarding passenger variable back to empty indicator ('-')
            if (boardingS.route != '-' && boardingS.boardingTime == 0) 
            {
                boardingS.passengerCount = 0;
                boardingS.timeInst = 0;
                boardingS.route = '-';
                boardingS.boardingTime = 0;
                taxiS--;
            }
            if (boardingL.route != '-' && boardingL.boardingTime == 0)
            {
                boardingL.passengerCount = 0;
                boardingL.timeInst = 0;
                boardingL.route = '-';
                boardingL.boardingTime = 0;
                taxiL--;
            }
            if (boardingC.route != '-' && boardingC.boardingTime == 0)
            {
                boardingC.passengerCount = 0;
                boardingC.timeInst = 0;
                boardingC.route = '-';
                boardingC.boardingTime = 0;
                taxiC--;
            }
        }

        void addPassengerToWaitingQueue(Passenger passenger)
        {
            switch (passenger.route) 
            {
                case 'S':
                    waitingS.push(passenger);
                    break;
                case 'L':
                    waitingL.push(passenger);
                    break;
                case 'C':
                    waitingC.push(passenger);
                    break;
            }
        }

        void startBoardingAnyWaitingPassengers()
        {
            // First check that there is no one already boarding by checking for the empty indicator ('-')
            // If no one is currently boarding and there is an item in the waiting queue then
            // move the passenger in the front of the waiting queue to start boarding
            if (boardingS.route == '-' && waitingS.size() != 0) 
            { 
                boardingS = waitingS.front();
                waitingS.pop();
            }
            if (boardingL.route == '-' && waitingL.size() != 0)
            {
                boardingL = waitingL.front();
                waitingL.pop();
            }
            if (boardingC.route == '-' && waitingC.size() != 0)
            {
                boardingC = waitingC.front();
                waitingC.pop();
            }
        }

        Passenger initialisePassenger(string line)
        {
            // Using the string line from the input file, extrapolate the time instance, route, and boarding time
            // and save it in the correct format for the Passenger struct
            int firstCommaPos = line.find(',');
            int secondCommaPos = line.find(',', firstCommaPos + 1);
            Passenger passenger;

            // Used web page https://stackoverflow.com/questions/7663709/how-can-i-convert-a-stdstring-to-int to learn how to cast string to int
            passenger.timeInst = atoi(line.substr(0, firstCommaPos).c_str());
            passenger.route = line[firstCommaPos + 1];
            passenger.boardingTime = atoi(line.substr(secondCommaPos + 1).c_str());

            increasePassengerCount(passenger.route);
            passenger.passengerCount = getPassengerCount(passenger.route);

            return passenger;
        }

        string passengerString(Passenger passenger)
        {
            if (passenger.route == '-')
            {
                return "-";
            }
            else 
            {
                return passenger.route + to_string(passenger.passengerCount) + "(" + to_string(passenger.boardingTime) + ")";
            }
        }

        string buildQueuePassengerString(queue<Passenger> queue)
        {
            string output = "";
            while (!queue.empty())
            {
                output = output + passengerString(queue.front()) + ",";
                queue.pop();
            }
            return output.substr(0, output.length() == 0 ? 0 : output.length() - 1); // remove trailing ","
        }

        void displayEachColumn() 
        {
            // Using a temporary variable so as not to make changes to actual queues
            queue<Passenger> tempS = waitingS;
            queue<Passenger> tempL = waitingL;
            queue<Passenger> tempC = waitingC;

            string waitingStringS = buildQueuePassengerString(tempS);
            string waitingStringL = buildQueuePassengerString(tempL);
            string waitingStringC = buildQueuePassengerString(tempC);

            cout << left << setw(5)  << timeCounter
                 << left << setw(10) << passengerString(boardingS)
                 << left << setw(10) << passengerString(boardingL)
                 << left << setw(10) << passengerString(boardingC)
                 << left << setw(15) << waitingStringS
                 << left << setw(15) << waitingStringL
                 << left << setw(15) << waitingStringC
                 << left << setw(5)  << taxiS
                 << left << setw(5)  << taxiL
                 << left << setw(5)  << taxiC
                 << "\n";
        }

};


int main()
{
    // Initialise Taxi Rank class
    TaxiRank taxiRank;

    ifstream infile;
    string filePath = "./taxiData.txt";

    // Used textbook: C++ Programming Program Design including Data Structures, 8th Edition, by D.S. Malik page 1224 for file handling
    infile.open(filePath);
    if (!infile)
    {
        cout << "Cannot open the input file. " << "Program terminates!" << "\n";
        return 1;
    }

    queue<Passenger> allPassengers;
    string line;
    while (infile && getline(infile, line))
    {
        // Save all passengers in a queue
        if (!line.empty()) { allPassengers.push(taxiRank.initialisePassenger(line)); }
    }
    infile.close();

    int currentTime = taxiRank.getCurrentTime();

    // Loop through all passengers in the queue
    while (!allPassengers.empty()) {

        Passenger currentPassenger = allPassengers.front();

        // Housekeeping
        if (currentPassenger.timeInst > currentTime) 
        {
            taxiRank.sendFullTaxis();
            taxiRank.boardValidPassengers();
            taxiRank.startBoardingAnyWaitingPassengers();
            taxiRank.displayEachColumn();
            taxiRank.decreaseAllPassengerBoardingTime();
            taxiRank.increaseTimeByOne();
        }

        currentTime = taxiRank.getCurrentTime();

        // Handle current passenger boarding
        if (currentPassenger.timeInst == currentTime) 
        {
            Passenger boardingInCurrentPassengerQueue = taxiRank.getCurrentPassengerBoarding(currentPassenger.route);
            // If no one is currently boarding for this route (if there is an empty indicator) then 
            // start boarding the current passenger, else add them to the waiting queue
            if (boardingInCurrentPassengerQueue.route == '-') 
            {
                taxiRank.setCurrentPassengerBoarding(currentPassenger);
            }
            else 
            {
                taxiRank.addPassengerToWaitingQueue(currentPassenger);
            }
            allPassengers.pop();
        }
      
    }

    // If we reach the end of the queue but there are still passengers busy boarding or
    // waiting to board then wait until all passengers have boarded and left
    while (taxiRank.getCurrentPassengerBoarding('S').route != '-' ||
        taxiRank.getCurrentPassengerBoarding('L').route != '-' ||
        taxiRank.getCurrentPassengerBoarding('C').route != '-')
    {
        taxiRank.sendFullTaxis();
        taxiRank.boardValidPassengers();
        taxiRank.startBoardingAnyWaitingPassengers();
        taxiRank.displayEachColumn();
        taxiRank.decreaseAllPassengerBoardingTime();
        taxiRank.increaseTimeByOne();
    }

    return 0;
}
