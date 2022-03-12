/* SeibertProj4.cpp

   Levi Seibert
   00086809
   lseiber1@my.athens.edu

   This program calculates and displays the total travel expenses of a professional development trip. */

#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

//function prototypes
int daysSpent();
void times(double &, double &);
bool isValidTime(double);
double airFare();
double transportation();
double additionalFees();
double vehicle();
double parking();
double registration();
double hotel(int);
double meals(int);
double getBreakfast();
double getLunch();
double getDinner();


//global constants
const double TAX = 0.012;
const double AIRFARE_REIMBURSE = 750.0;
const double TRANSPORTATION_REIMBURSE = 20.0;
const double MILE_REIMBURSE = 0.535;
const double PARKING_DAY = 24.0;
const double PARKING_HOUR = 2.0;
const double HOTEL_REIMBURSE = 125.0;
const double FOOD_FULL = 39.0;
const double FOOD_PARTIAL = 19.5;

//main function 
int main()
{
	ofstream outputFile;
	outputFile.open("trip1.txt");

	char singleDay, fees, personal, parkingFee;
	int days;
	double departureTime, returnTime = 0.0;
	double totalExpenses, airCost, transportationCost, personalVehicle, parkingPrice, taxiPrice, registrationPrice, hotelPrice, food, hoursTraveled,
		reimburseAirfare, reimburseTransportation, reimbursePersonal, reimburseParking, reimburseHotel, reimburseFood, allowableExpenses;

	//check number of days
	cout << "Is this a single day trip?" << endl << "(Enter 'y' or 'n')" << endl;
	cin >> singleDay;

	while ((singleDay != 'y') && (singleDay != 'Y') && (singleDay != 'n') && (singleDay != 'N'))
	{
		cout << "Error!  Please enter 'y' or n': ";
		cin >> singleDay;
	}

	switch (singleDay)
	{
	case 'y':
	case 'Y':
		days = 1;
		break;
	case 'n':
	case 'N':
		days = daysSpent();
		break;
	}

	//get departure time, return time, and airfare cost
	times(departureTime, returnTime);
	airCost = airFare();

	//check for additional fees
	cout << "Were there any additonal fees? " << endl
		<< "(Please enter 'y' or 'n')" << endl;
	cin >> fees;

	while ((fees != 'y') && (fees != 'Y') && (fees != 'n') && (fees != 'N'))
	{
		cout << "Error!  Please enter 'y' or n': ";
		cin >> fees;
	}

	switch (fees)
	{
	case 'y':
	case 'Y':
		airCost += additionalFees();
		break;
	case 'n':
	case 'N':
		airCost += 0.0;
		break;
	}

	//get transportation cost
	transportationCost = transportation();

	//check for personal vehicle miles
	cout << "Was a personal vehicle driven during this trip? " << endl
		<< "(Please enter 'y' or 'n')" << endl;
	cin >> personal;

	while ((personal != 'y') && (personal != 'Y') && (personal != 'n') && (personal != 'N'))
	{
		cout << "Error!  Please enter 'y' or n': ";
		cin >> personal;
	}

	switch (personal)
	{
	case 'y':
	case 'Y':
		personalVehicle = vehicle();
		break;
	case 'n':
	case 'N':
		personalVehicle = 0.0;
		break;
	}

	//check for parking fees
	cout << "Were there any parking fees on this trip? " << endl
		<< "(Please enter 'y' or 'n')" << endl;
	cin >> parkingFee;

	while ((parkingFee != 'y') && (parkingFee != 'Y') && (parkingFee != 'n') && (parkingFee != 'N'))
	{
		cout << "Error!  Please enter 'y' or n': ";
		cin >> parkingFee;
	}

	switch (parkingFee)
	{
	case 'y':
	case 'Y':
		parkingPrice = parking();
		break;
	case 'n':
	case 'N':
		parkingPrice = 0.0;
		break;
	}

	//get registration price, hotel price, and the price of food
	registrationPrice = registration();
	hotelPrice = hotel(days);
	food = meals(days);

	hoursTraveled = returnTime - departureTime;

	//get reimbursement amounts;
	if (airCost == 0.0)
	{
		reimburseAirfare = 0.0;
	}

	else
	{
		reimburseAirfare = AIRFARE_REIMBURSE;
	}
	reimburseTransportation = days * TRANSPORTATION_REIMBURSE;
	reimbursePersonal = personalVehicle * MILE_REIMBURSE;

	if (days == 1)
	{
		if (parkingPrice == 0)
		{
			reimburseParking = 0.0;
		}

		else
		{
			reimburseParking = hoursTraveled * PARKING_HOUR;
		}

		reimburseHotel = 0;

		if ((returnTime - departureTime) < 12.0)
		{
			reimburseFood = FOOD_PARTIAL;
		}

		else
		{
			reimburseFood = FOOD_FULL;
		}
	}

	else
	{
		if (parkingPrice == 0)
		{
			reimburseParking = 0.0;
		}

		else
		{
			reimburseParking = ((23.59 - departureTime + .25) * PARKING_HOUR) + ((days - 2) * PARKING_DAY) + ((23.59 - returnTime - .25) * PARKING_HOUR);
		}

		if (hotelPrice < (HOTEL_REIMBURSE * (days - 1)))
		{
			reimburseHotel = hotelPrice;
		}

		else
		{
			reimburseHotel = (days - 1) * HOTEL_REIMBURSE;
		}
		reimburseFood = FOOD_FULL * days;
	}


	//display results to screen and file

	cout << setprecision(2) << fixed << showpoint << endl << setw(50) << "Professional Development Report" << endl;
	outputFile << setprecision(2) << fixed << showpoint << endl << setw(50) << "Professional Development Report" << endl;
	
	if (days == 1)
	{
		cout << "Hours Traveled: " << hoursTraveled << " hours" << endl;
		outputFile << "Hours Traveled: " << hoursTraveled << " hours" << endl;
	}

	else
	{
		cout << "Trip Length: " << days << endl;
		outputFile << "Trip Length: " << days << endl;
	}

	cout << "Time Departed: " << departureTime << setw(25) << "Time Returned: " << returnTime << endl;
	cout << "*************************************************************" << endl;
	cout << "Expenses" << setw(35) << "Amount" << setw(20) << "Covered" << endl;
	cout << "Airfare Expenses:" << setw(25) << airCost << setw(21) << reimburseAirfare << endl;
	cout << "Ground Transportation Expenses:" << setw(11) << transportationCost << setw(21) << reimburseTransportation << endl;
	cout << "Mileage Reimbursement:" << setw(20) << personalVehicle << " miles" << setw(15) << reimbursePersonal << endl;
	cout << "Parking Expenses:" << setw(25) << parkingPrice << setw(21) << reimburseParking << endl;
	cout << "Registration Expenses:" << setw(20) << registrationPrice << setw(21) << registrationPrice << endl;
	cout << "Hotel Expenses:" << setw(27) << hotelPrice << setw(21) << reimburseHotel << endl;
	cout << "Food Expenses:" << setw(28) << food << setw(21) << reimburseFood << endl;

	outputFile << "Time Departed: " << departureTime << setw(25) << "Time Returned: " << returnTime << endl;
	outputFile << "*************************************************************" << endl;
	outputFile << "Expenses" << setw(35) << "Amount" << setw(20) << "Covered" << endl;
	outputFile << "Airfare Expenses:" << setw(25) << airCost << setw(21) << reimburseAirfare << endl;
	outputFile << "Ground Transportation Expenses:" << setw(11) << transportationCost << setw(21) << reimburseTransportation << endl;
	outputFile << "Mileage Reimbursement:" << setw(20) << personalVehicle << " miles" << setw(15) << reimbursePersonal << endl;
	outputFile << "Parking Expenses:" << setw(25) << parkingPrice << setw(21) << reimburseParking << endl;
	outputFile << "Registration Expenses:" << setw(20) << registrationPrice << setw(21) << registrationPrice << endl;
	outputFile << "Hotel Expenses:" << setw(27) << hotelPrice << setw(21) << reimburseHotel << endl;
	outputFile << "Food Expenses:" << setw(28) << food << setw(21) << reimburseFood << endl;

	totalExpenses = airCost + transportationCost + parkingPrice + registrationPrice + hotelPrice + food;
	allowableExpenses = airCost + reimburseTransportation + reimbursePersonal + registrationPrice + reimburseParking + reimburseHotel + reimburseFood;

	cout << endl << "Your expenses were: $" << totalExpenses << endl;
	cout << "Your total allowed expenses were: $" << allowableExpenses << endl;
	outputFile << endl << "Your expenses were: $" << totalExpenses << endl;
	outputFile << "Your total allowed expenses were: $" << allowableExpenses << endl;

	if (totalExpenses == allowableExpenses)
	{
		cout << "You recieve full reimbursement of $" << allowableExpenses;
		outputFile << "You recieve full reimbursement of $" << allowableExpenses;
	}

	else if (totalExpenses > allowableExpenses)
	{
		cout << "You recive partial reimbursement but you still owe $" << (totalExpenses - allowableExpenses);
		outputFile << "You recive partial reimbursement but you still owe $" << (totalExpenses - allowableExpenses);
	}

	else if (totalExpenses < allowableExpenses)
	{
		cout << "You will recieve a check for $" << (allowableExpenses - totalExpenses);
		outputFile << "You will recieve a check for $" << (allowableExpenses - totalExpenses);
	}

	outputFile.close();
	return 0;
}

// ********************************************************
// The daysSpent function asks for the number of days     *
// spent on the trip and returns the value as an integer. *
// ********************************************************
int daysSpent()
{
	int days;
	cout << "How long is the trip? " << endl
		<< "(enter the number of days including the day of departure)" << endl;
	cin >> days;

	while (days < 1)
	{
		cout << "Error!  You already said it was more than one day.  Please try again: ";
		cin >> days;
	}

	return days;
}

// ********************************************************
// The times function asks for the time of departure and  *
// the time of return. The times are validated and stored *
// in the reference parameter variables: start and end.   *
// ********************************************************
void times(double & start, double & end)
{
	cout << "Enter the time you left: " << endl
		<< "(in military time, hour.min)" << endl;
	cin >> start;

	while (isValidTime(start) == false)
	{
		cout << "Error!  Please enter a valid starting time, per the above instructions: ";
		cin >> start;
	}

	cout << "Enter the time you returned:" << endl
		<< "(in military time, hour.min)" << endl;
	cin >> end;

	while (isValidTime(end) == false)
	{
		cout << "Error!  Please enter a valid return time, per the above instructions: ";
		cin >> end;
	}
}

// ********************************************************
// The isValidTime function accepts a double argument     *
// assumed to be a time in the 24 hour format of HH.MM.   *
// The value is tested, and if it is valid the function   *
// returns true. Otherwise it returns false.              *
// ********************************************************
bool isValidTime(double time)
{
	if ((time < 0.00) || (time > 23.59) || ((time - static_cast<int>(time)) > .59))
	{
		return false;
	}

	else
	{
		return true;
	}
}


// ********************************************************
// The airFare function asks the user to enter the amount *
// of any airfare the employee incurred, adds the value   *
// to the totalExpenses, and returns the airfare amount   *
// as a double.                                           *
// ********************************************************
double airFare()
{
	double airFare;
	cout << "What is the cost of airfare?" << endl
		<< "(If this does not apply, put in 0)" << endl;
	cin >> airFare;

	while (airFare < 0)
	{
		cout << "Invalid cost!  Please enter a non-negative number: ";
		cin >> airFare;
	}

	return airFare;
}

// ***************************************************************
// The additionalFees function asks the user to enter the amount *
// of any additional fees the employee incurred, adds the value  *
// to the totalExpenses, and returns the additonal fees amount   *
// as a double.                                                  *
// ***************************************************************
double additionalFees()
{
	double fees;
	cout << "What is the cost of the additional fees?" << endl;
	cin >> fees;

	while (fees < 0)
	{
		cout << "Invalid cost!  Please enter a non-negative number: ";
		cin >> fees;
	}

	return fees;
}
// **********************************************************
// The transportation function asks the user to enter the   *
// amount of transporation fees the employee incurred, adds *
// the value to the totalExpenses, and returns the amount   *
// of transportation fees as a double.                      *
// **********************************************************
double transportation()
{
	double transportation;
	cout << "How much did you spend in transportation?" << endl;
	cin >> transportation;

	while (transportation < 0)
	{
		cout << "Invalid cost!  Please enter a non-negative number: ";
		cin >> transportation;
	}

	return transportation;
}

// ********************************************************
// The vehicle function asks the user to enter the number *
// of miles a private vehicle was driven.The expenses are *
// added to the totalExpenses global variable. Allowable  *
// expenses are returned as a double.                     *
// ********************************************************
double vehicle()
{
	double miles;
	cout << "How many miles were driven in a private vehicle? " << endl
		<< "(Enter the round-trip mileage)" << endl;
	cin >> miles;

	while (miles < 0)
	{
		cout << "Invalid number of miles.  Please enter a non-negative number: ";
		cin >> miles;
	}

	return miles;
}

// ********************************************************
// The parking function asks the user to enter the amount *
// of any parking fees. The expenses are added to the     *
// totalExpenses global variable. Allowable expenses are  *
// returned as a double.                                  *
// ********************************************************
double parking()
{
	double parking;
	cout << "What is the cost of parking fees?" << endl
		<< "(If this does not apply, put in 0)" << endl;
	cin >> parking;

	while (parking < 0)
	{
		cout << "Invalid cost!  Plese enter a non-negative number: ";
		cin >> parking;
	}

	return parking;
}

// ********************************************************
// The registration function asks the user to enter the   *
// amount of conference registration fees incurred by the *
// employee, adds the amount to totalExpenses, and        *
// returns the amount spent as a double.                  *
// ********************************************************
double registration()
{
	double registration;
	cout << "What is the cost of registration?" << endl
		<< "(If this does not apply, put in 0)" << endl;
	cin >> registration;

	while (registration < 0)
	{
		cout << "Invalid cost!  Plese enter a non-negative number: ";
		cin >> registration;
	}

	return registration;
}

// ********************************************************
// The hotel function asks the user to enter the amount   *
// of hotel expenses. The expenses are added to the       *
// totalExpenses global variable. Allowable expenses are  * 
// returned as a double.                                  *
// ********************************************************
double hotel(int days)
{
	double hotel, hotelTotal;
	cout << "What is the cost of hotel per night?" << endl
		<< "(If this does not apply, put in 0)" << endl;
	cin >> hotel;

	while (hotel < 0)
	{
		cout << "Invalid cost!  Plese enter a non-negative number: ";
		cin >> hotel;
	}

	hotelTotal = hotel * (days - 1);
	hotelTotal += hotelTotal * TAX;

	return hotelTotal;
}

// ********************************************************
// The meals function calls the getBreakfast, getLunch,   *
// and getDinner functions to calculate the employee's    *
// meal expenses. The expenses are added to the           *
// totalExpenses global variable. Allowable expenses are  *
// returned as a double.                                  *
// ********************************************************
double meals(int days)
{
	double foodTotal = 0.0;
	double breakfast, lunch, dinner, count;

	for (count = 1; count <= days; count++)
	{
		cout << "Day " << count << endl;
		breakfast = getBreakfast();
		cout << endl;
		lunch = getLunch();
		cout << endl;
		dinner = getDinner();
		cout << endl << endl;

		foodTotal += (breakfast + lunch + dinner);
	}

	return foodTotal;
}

// ********************************************************
// The getBreakfast function asks the user for the amount *
// spent on a breakfast meal. The expenses are added to   *
// the totalExpenses global variable. Allowable expenses  *
// are returned as a double.                              *
// ********************************************************
double getBreakfast()
{
	double breakfast;
	cout << "Enter the amount spend on breakfast: ";
	cin >> breakfast;

	while (breakfast < 0)
	{
		cout << "Invalid price!  Please enter a non-negative number: ";
		cin >> breakfast;
	}

	return breakfast;
}

// ********************************************************
// The getLunch function asks the user for the amount     *
// spent on a lunch meal. The expenses are added to the   *
// totalExpenses global variable. Allowable expenses are  *
// returned as a double.                                  *
// ********************************************************
double getLunch()
{
	double lunch;
	cout  << "Enter the amount spend on lunch: ";
	cin >> lunch;

	while (lunch < 0)
	{
		cout << "Invalid price!  Please enter a non-negative number: ";
		cin >> lunch;
	}

	return lunch;
}

// ********************************************************
// The getDinner function asks the user for the amount    *
// spent on a dinner meal. The expenses are added to the  *
// totalExpenses global variable. Allowable expenses are  *
// returned as a double.                                  *
// ********************************************************
double getDinner()
{
	double dinner;
	cout << "Enter the amount spend on dinner: ";
	cin >> dinner;

	while (dinner < 0)
	{
		cout << "Invalid price!  Please enter a non-negative number: ";
		cin >> dinner;
	}

	return dinner;
}