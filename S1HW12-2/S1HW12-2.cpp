#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace::std;

struct MemberRecord
{
	char email[40];
	char password[24];
	char IDNumber[12];
	char name[12];
	char phone[12];
};

struct Movie
{
	int movieCode;
	int prices[4]; // prices[0]:adult, prices[1]:concession, prices[2]:disability, prices[3]:elderly
	bool dates[9];
	bool sessionTimes[17];
	bool occupiedSeats[9][17][8][12]; // occupiedSeats[i][j] is the occupied Seats for all accounts
};                                           // at j-th session time on i-th date

struct BookingInfo
{
	char email[40];
	int movieCode;
	int dateCode;
	int sessionTimeCode;
	int numTickets[4]; // numTickets[0]: the number of adult tickets,
					   // numTickets[1]: the number of concession tickets,
					   // numTickets[2]: the number of disability tickets,
					   // numTickets[3]: the number of elderly tickets
	char selectedSeats[24][4]; // seleted seats for the user with the specified email
};

char hours[17][8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00",
"18:00", "19:00", "20:00", "21:00", "22:00", "23:00", "00:00", "01:00" };




void loadMemberInfo(MemberRecord memberDetails[], int &numMembers);
//int inputAnInteger(int begin, int end);
void signIn(MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][60], int numMovies,
	BookingInfo bookingHistories[], int &numBookings, char availableDates[][12], int numDates);
bool legal(char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber);
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber);
void newMember(MemberRecord memberDetails[], int &numMembers);
bool existingID(char newIDNumber[], MemberRecord memberDetails[], int &numMembers);
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers);

void loadBookingHistories(BookingInfo bookingHistories[], int &numBookings);
void loadAvailableDates(char availableDates[][12], int &numDates);
void loadMovies(Movie movies[], int &numMovies);
void loadMovieNames(char movieNames[][60], int numMovies);
void buyTickets(BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][60],
	int numMovies, char availableDates[][12], int numDates, char email[]);
void selectSeats(BookingInfo bookingHistories[], int numBookings, Movie movies[]);
void display(Movie movies[], BookingInfo bookingHistory);
void ShowSessionTimes(Movie movies[], char movieNames[][60], int numMovies,
	char availableDates[][12], int numDates);
void ShowBookingHistory(BookingInfo bookingHistories[], int numBookings, Movie movies[],
	char movieNames[][60], char availableDates[][12], char email[]);
void saveMemberInfo(MemberRecord memberDetails[], int numMembers);
void saveMovies(Movie movies[], int numMovies);
void saveBookingHistories(BookingInfo bookingHistories[], int numBookings);

int main()
{
	MemberRecord memberDetails[100] = {};
	int numMembers = 0;
	loadMemberInfo(memberDetails, numMembers);

	BookingInfo bookingHistories[1000] = {};
	int numBookings = 0;
	loadBookingHistories(bookingHistories, numBookings);

	Movie movies[30] = {};
	int numMovies = 0;
	loadMovies(movies, numMovies);

	char movieNames[30][60] = {};
	loadMovieNames(movieNames, numMovies);

	char availableDates[10][12];
	int numDates;
	loadAvailableDates(availableDates, numDates);

	cout << "Welcome to Vieshow Cinemas Taipei QSquare system\n\n";

	int choice;

	while (true)
	{
		cout << "Enter your choice:" << endl;
		cout << "1. Sign In\n";
		cout << "2. New Member\n";
		cout << "3. End\n? ";

		cin >> choice;
		cout << endl;

		switch (choice)
		{
		case 1:
			signIn(memberDetails, numMembers, movies, movieNames, numMovies, bookingHistories, numBookings,
				availableDates, numDates);
			break;

		case 2:
			newMember(memberDetails, numMembers);
			break;

		case 3:
			saveMemberInfo(memberDetails, numMembers);
			//saveMovies(movies, numMovies);
			saveBookingHistories(bookingHistories, numBookings);
			cout << "Thank you...\n\n";
			system("pause");
			return 0;

		default:
			cout << "Input Error!\n\n";
			break;
		}
	}

	system("pause");
}
void loadMemberInfo(MemberRecord memberDetails[], int &numMembers)
{
	ifstream inMember("MemberInfo.txt", ios::in);
	if (!inMember)
	{
		cout << "File could not be opened" << endl;
		system("pause");
		exit(1);
	}
	char temp[40];
	while (inMember >> temp)
	{
		strcpy_s(memberDetails[numMembers].email, temp);
		inMember >> memberDetails[numMembers].IDNumber;
		inMember >> memberDetails[numMembers].name;
		inMember >> memberDetails[numMembers].password;
		inMember >> memberDetails[numMembers].phone;
		numMembers++;
	}
	inMember.close();
}
//struct Movie
//{
//	int movieCode;
//	int prices[4]; // prices[0]:adult, prices[1]:concession, prices[2]:disability, prices[3]:elderly
//	bool dates[9];
//	bool sessionTimes[17];
//	bool occupiedSeats[9][17][8][12]; // occupiedSeats[i][j] is the occupied Seats for all accounts
//};
void loadMovies(Movie movies[], int &numMovies)
{
	ifstream inMovies("Movies.txt", ios::in);
	while (inMovies >> movies[numMovies].movieCode)
	{
		for (int i = 0; i < 4; i++)
		{
			inMovies >> movies[numMovies].prices[i];
		}

		for (int i = 0; i < 9; i++)
		{
			inMovies >> movies[numMovies].dates[i];
		}
		for (int i = 0; i < 17; i++)
		{
			inMovies >> movies[numMovies].sessionTimes[i];
		}
		for (int i = 0; i < 9; i++)
		{
			if (movies[numMovies].dates[i] == true)
			{
				for (int k = 0; k < 17; k++)
				{
					if (movies[numMovies].sessionTimes[k] == true)
					{
						for (int j = 0; j < 8; j++)
						{
							for (int g = 0; g < 12; g++)
							{
								movies[numMovies].occupiedSeats[i][k][j][g] = 0;
							}
						}
					}
				}
			}
		}
		numMovies++;
	}
}
void loadBookingHistories(BookingInfo bookingHistories[], int &numBookings)
{
	ifstream inBookingHistory("Booking History.txt", ios::in);
	char temp[40];
	int numTicket;
	while (inBookingHistory >> temp)
	{
		strcpy_s(bookingHistories[numBookings].email, temp);
		inBookingHistory >> bookingHistories[numBookings].movieCode;
		inBookingHistory >> bookingHistories[numBookings].dateCode;
		inBookingHistory >> bookingHistories[numBookings].sessionTimeCode;
		numTicket = 0;
		for (int i = 0; i < 4; i++)
		{
			inBookingHistory >> bookingHistories[numBookings].numTickets[i];
			numTicket += bookingHistories[numBookings].numTickets[i];
		}
		for (int i = 0; i <numTicket; i++)
		{
			inBookingHistory >> bookingHistories[numBookings].selectedSeats[i];
		}
		numBookings++;
	}
	inBookingHistory.close();
}
void loadMovieNames(char movieNames[][60], int numMovies)
{
	ifstream inMovieNames("Movie Names.txt", ios::in);
	char temp[100];
	for (int i = 0; i < numMovies; i++)
	{
		inMovieNames.getline(temp, 256);
		strcpy_s(movieNames[i], temp);
	}
	inMovieNames.close();
}
void loadAvailableDates(char availableDates[][12], int &numDates)
{

}

void newMember(MemberRecord memberDetails[], int &numMembers)
{
	char newEmail[20], newId[20];
	cout << "Enter your Email: ";
	cin >> newEmail;
	while (existingEmail(newEmail, memberDetails, numMembers))
	{
		cout << "Already existed Email.";
		cout << "Enter your Email: ";
		cin >> newEmail;
	}
	strcpy_s(memberDetails[numMembers].email, newEmail);
	cout << "Enter your Id: ";
	cin >> newId;
	while (existingID(newId, memberDetails, numMembers))
	{
		cout << "Already existed ID.";
		cout << "Enter your Id: ";
		cin >> newId;
	}
	strcpy_s(memberDetails[numMembers].IDNumber, newId);
	cout << "Enter your password: ";
	cin >> memberDetails[numMembers].password;
	cout << "Enter your name: ";
	cin >> memberDetails[numMembers].name;
	cout << "Enter your phone number: ";
	cin >> memberDetails[numMembers].phone;
	numMembers++;
}
//struct MemberRecord
//{
//	char email[40];
//	char password[24];
//	char IDNumber[12];
//	char name[12];
//	char phone[12];
//};
bool existingID(char newIDNumber[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)
	{
		if (strcmp(newIDNumber, memberDetails[i].IDNumber) == 0)
		{
			return true;
		}
	}
	return false;
}
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)
	{
		if (strcmp(newEmail, memberDetails[i].email) == 0)
		{
			return true;
		}
	}
	return false;
}
void saveMemberInfo(MemberRecord memberDetails[], int numMembers)
{
	ofstream outMemberDetails("MemberInfo.txt", ios::out);
	for (int i = 0; i < numMembers; i++)
	{
		outMemberDetails << memberDetails[i].email << endl;
		outMemberDetails << memberDetails[i].name << endl;
		outMemberDetails << memberDetails[i].IDNumber << endl;
		outMemberDetails << memberDetails[i].password << endl;
		outMemberDetails << memberDetails[i].phone << endl;
	}
	outMemberDetails.close();
}
void signIn(MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][60], int numMovies,
	BookingInfo bookingHistories[], int &numBookings, char availableDates[][12], int numDates)
{
	char inputEmail[20];
	char inputPassword[20];
	int recordNumber;
	cout << "Enter your Email: ";
	cin >> inputEmail;
	cout << "Enter your password: ";
	cin >> inputPassword;
	
	while (!legal(inputEmail,inputPassword,memberDetails,numMembers,recordNumber))
	{
		cout << "Sorry, unrecognized email or password." << endl;
		cout << "Enter your Email: ";
		cin >> inputEmail;
		cout << "Enter your password: ";
		cin >> inputPassword;
	}
	while (true)
	{
		cout << "Enter your choice:" << endl;
		cout << "1. Account Information." << endl;
		cout << "2. Buy Ticket." << endl;
		cout << "3. My Booking." << endl;
		cout << "4. Sign Out." << endl;
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			accountInfor(memberDetails, numMembers, recordNumber);
			break;
		case 2:
			ShowSessionTimes(movies, movieNames,numMovies,
				availableDates,numDates);
			buyTickets(bookingHistories, numBookings, movies, movieNames,
				numMovies, availableDates, numDates, inputEmail);
			break;

		case 3:
			ShowBookingHistory(bookingHistories, numBookings, movies,
				movieNames, availableDates, inputEmail);
			break;

		case 4:
			saveBookingHistories(bookingHistories, numBookings);
			saveMemberInfo(memberDetails, numMembers);
			return;
			break;
		default:
			cout << "Error Input!";
			break;
		}
	}
}
bool legal(char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber)
{
	bool isSuccess = false;
	for (int i = 0; i < numMembers; i++)
	{
		if(strcmp(email,memberDetails[i].email)==0)
		{
			isSuccess = true;
			recordNumber = i;
			break;
		}
	}
	if (isSuccess == true)
	{
		if (strcmp(password, memberDetails[recordNumber].password) != 0)
		{
			isSuccess = false;
		}
	}
	return isSuccess;
}
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber)
{
	int modify;
	char temp[40];
	cout << "1. Email: " << memberDetails[recordNumber].email << endl;
	cout << "2. Password: " << memberDetails[recordNumber].password << endl;
	cout << "3. IDName: " << memberDetails[recordNumber].IDNumber << endl;
	cout << "4. Name: " << memberDetails[recordNumber].name << endl;
	cout << "5. phone: " << memberDetails[recordNumber].phone << endl;
	while (true)
	{
		cout << "\nWhich do you want to modify? (0~5) 0 --> break\n?";
		cin >> modify;
		switch (modify)
		{
		case 0:
			return;
			break;
		case 1:
			cout << "\nPlease enter new email address: ";
			cin >> temp;
			strcpy_s(memberDetails[recordNumber].email, temp);
			cout << "Success!";
			break;
		case 2:
			cout << "\nPlease enter new password: ";
			cin >> temp;
			strcpy_s(memberDetails[recordNumber].password, temp);
			cout << "Success!";
			break;
		case 3:
			cout << "\nPlease enter new ID: ";
			cin >> temp;
			strcpy_s(memberDetails[recordNumber].IDNumber, temp);
			cout << "Success!";
			break;
		case 4:
			cout << "\nPlease enter new Name: ";
			cin >> temp;
			strcpy_s(memberDetails[recordNumber].name, temp);
			cout << "Success!";
			break;
		case 5:
			cout << "\nPlease enter new phone number: ";
			cin >> temp;
			strcpy_s(memberDetails[recordNumber].phone, temp);
			cout << "Success!";
			break;
		default:
			cout << "Enter Error";
			break;
		}
		
	}
}
void ShowSessionTimes(Movie movies[], char movieNames[][60], int numMovies,
	char availableDates[][12], int numDates)
{
	char Dates[9][20] = { "Dec 13 Wed", "Dec 14 Thur", "Dec 15 Fri", "Dec 16 Sat", "Dec 17 Sun", "Dec 18 Mon", "Dec 19 Tue", "Dec 20 Wed", "Dec 21 Thur" };
	int TicketNumber[4] = {};
	for (int i = 0; i < numMovies; i++)
	{
		cout << i << ". Movie: " << movieNames[i] << endl;
		cout << "   " << "Date: ";
		for (int k = 0; k < 9; k++)
		{
			if (movies[i].dates[k] == true)
			{
				cout << k << ". " << Dates[k] << " ";
			}
		}
		cout << endl;
		cout << "   Session Time: ";
		for (int k = 0; k < 17; k++)
		{
			if (movies[i].sessionTimes[k] == true)
			{
				cout << k << ". " << hours[k] << " ";
			}
		}
		cout << endl;
	}
}
void buyTickets(BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][60],
	int numMovies, char availableDates[][12], int numDates, char email[])
{
	char Dates[9][20] = { "Dec 13 Wed", "Dec 14 Thur", "Dec 15 Fri", "Dec 16 Sat", "Dec 17 Sun", "Dec 18 Mon", "Dec 19 Tue", "Dec 20 Wed", "Dec 21 Thur" };
	int chosenMovie, chosenDate, chosenSessionTime;
	int TicketNumber[4] = {};
	cout << "Please enter the movie code(0~" << numMovies - 1 << "): ";
	cin >> chosenMovie;
	while (chosenMovie < 0 || chosenMovie >= numMovies)
	{
		cout << "Please enter the movie code(0~" << numMovies - 1 << "): ";
		cin >> chosenMovie;
	}
	cout << "Please enter the date code(0~8)";
	cin >> chosenDate;
	while (movies[chosenMovie].dates[chosenDate] != true)
	{
		cout << "Please enter the date code(0~8)";
		cin >> chosenDate;
	}
	cout << "Please enter the session time(0~16)";
	cin >> chosenSessionTime;
	while (movies[chosenMovie].sessionTimes[chosenSessionTime] != true)
	{
		cout << "Please enter the session time(0~16)";
		cin >> chosenSessionTime;
	}
	cout << "Movie: " << movieNames[chosenMovie] << endl;
	cout << "Date: " << Dates[chosenDate] << endl;
	cout << "Sessin Time: " << hours[chosenSessionTime] << endl;
	cout << "Price: Adualt- " << movies[chosenMovie].prices[0] << 
	" Concession- "	<< movies[chosenMovie].prices[1] <<
	" Disability- " << movies[chosenMovie].prices[2] <<
	" Ederly-	" << movies[chosenMovie].prices[3] << endl;
	cout << "Enter the number of Adualt Ticet(0~6): ";
	cin >> TicketNumber[0];
	while (TicketNumber[0] < 0 || TicketNumber[0]>6)
	{
		cout << "Enter the number of Adualt Ticet(0~6): ";
		cin >> TicketNumber[0];
	}
	cout << "Enter the number of Concession Ticet(0~6): ";
	cin >> TicketNumber[1];
	while (TicketNumber[1] < 0 || TicketNumber[1]>6)
	{
		cout << "Enter the number of Concession Ticet(0~6): ";
		cin >> TicketNumber[1];
	}
	cout << "Enter the number of Disability Ticet(0~6): ";
	cin >> TicketNumber[2];
	while (TicketNumber[2] < 0 || TicketNumber[2]>6)
	{
		cout << "Enter the number of Disability Ticet(0~6): ";
		cin >> TicketNumber[2];
	}
	cout << "Enter the number of Elderly Ticet(0~6): ";
	cin >> TicketNumber[3];
	while (TicketNumber[3] < 0 || TicketNumber[3]>6)
	{
		cout << "Enter the number of Elderly Ticet(0~6): ";
		cin >> TicketNumber[3];
	}
	strcpy_s(bookingHistories[numBookings].email, email);
	bookingHistories[numBookings].movieCode = chosenMovie;
	bookingHistories[numBookings].dateCode = chosenDate;
	bookingHistories[numBookings].sessionTimeCode = chosenSessionTime;
	for (int i = 0; i < 4; i++)
	{
		bookingHistories[numBookings].numTickets[i] = TicketNumber[i];
	}
	display(movies, bookingHistories[numBookings]);
	selectSeats(bookingHistories, numBookings, movies);
	cout << "Success!\n\n";
	numBookings++;
}
void selectSeats(BookingInfo bookingHistories[], int numBookings, Movie movies[])
{
	int row, column;
	int MovieCodeRecord = bookingHistories[numBookings].movieCode;
	int MovieDateRecord = bookingHistories[numBookings].dateCode;
	int MovieSessionTimeRecord = bookingHistories[numBookings].sessionTimeCode;
	int numTickets = 0;
	char temp[3], temp2[3];
    char Columns[12][3] = {"A","B","C","D","E","F","G","H","I","J","K","L"};
	for (int i = 0; i < numBookings; i ++)
	{
		if (bookingHistories[i].movieCode == bookingHistories[numBookings].movieCode
			&& bookingHistories[i].dateCode == bookingHistories[numBookings].dateCode
			&& bookingHistories[i].sessionTimeCode == bookingHistories[numBookings].sessionTimeCode)
		{ 
			for (int k = 0; k < 24; k++)
			{
				
				strcpy_s(temp,bookingHistories[i].selectedSeats[k]);
				for (int j = 0; j < 12; j++)
				{
					if (temp[1]==Columns[j][0])
					{
						row = (int)temp[0]-48;
						column = j;
						movies[MovieCodeRecord].occupiedSeats[MovieDateRecord]
							                                 [MovieSessionTimeRecord]
						                                     [row]
						                                     [column] = 1;
						break;
					}
				}
			}
			
		}
	}
	cout << endl;
	cout << "  ";
	for (int i = 0; i < 12; i++)
	{
		cout << Columns[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << i << " ";
		for (int k = 0; k < 12; k++)
		{
			if (movies[MovieCodeRecord].occupiedSeats[MovieDateRecord]
				                                     [MovieSessionTimeRecord]
			                                         [i][k] == true)
			{
				cout << 1 << " ";
			}
			else 
			{
				cout << 0 << " ";
			}
		}
		cout << endl;
	}
	for (int i = 0; i < 4; i++)
	{
		numTickets += bookingHistories[numBookings].numTickets[i];
	}

	cout << "Please chose " << numTickets << " seats\n?";
	int n;
	for (int i = 0; i < numTickets; i++)
	{
		n = 1;
		do
		{
			if (n != 1)
			{
				cout << temp2 << " is occupied, pleaes select another seat.\n";
			}
			cin >> temp2;
			for (int k = 0; k < 12; k++)
			{
				if (temp2[1] == Columns[k][0])
				{
					row = (int)temp2[0] - 48;
					column = k;
					break;
				}
			}
			n++;
		} while (movies[MovieCodeRecord].occupiedSeats[MovieDateRecord][MovieSessionTimeRecord][row][column]==true);
		strcpy_s(bookingHistories[numBookings].selectedSeats[i], temp2);
		movies[MovieCodeRecord].occupiedSeats[MovieDateRecord][MovieSessionTimeRecord][row][column] = 1;
	}
	
	
}
//struct BookingInfo
//{
//	char email[40];
//	int movieCode;
//	int dateCode;
//	int sessionTimeCode;
//	int numTickets[4]; // numTickets[0]: the number of adult tickets,
//					   // numTickets[1]: the number of concession tickets,
//					   // numTickets[2]: the number of disability tickets,
//					   // numTickets[3]: the number of elderly tickets
//	char seletedSeats[24][4]; // seleted seats for the user with the specified email
//};
void display(Movie movies[], BookingInfo bookingHistory)
{
	int total = 0;
	int MovieCode = bookingHistory.movieCode;
	cout << "\n               Tickets   Price    Subtotal";
	for (int i = 0; i < 4; i++)
	{
		if (bookingHistory.numTickets[i] > 0)
		{
			switch (i)
			{
			case 0:
				cout << "\nAdualt      ";
				break;
			case 1:
				cout << "\nConcession   ";
				break;
			case 2:
				cout << "\nDisability   ";
				break;
			case 3:
				cout << "\nElderly      ";
				break;
			default:
				break;
			}
			total += bookingHistory.numTickets[i] * movies[MovieCode].prices[i];
			cout << "      " << bookingHistory.numTickets[i] << "       " <<  movies[MovieCode].prices[i] << "       " << bookingHistory.numTickets[i] * movies[MovieCode].prices[i];
		}
	}
	cout << "\nThe Total Amount for Tickets is: " << total << endl;

}

void ShowBookingHistory(BookingInfo bookingHistories[], int numBookings, Movie movies[],
	char movieNames[][60], char availableDates[][12], char email[])
{
	char Dates[9][20] = { "Dec 13 Wed", "Dec 14 Thur", "Dec 15 Fri", "Dec 16 Sat", "Dec 17 Sun", "Dec 18 Mon", "Dec 19 Tue", "Dec 20 Wed", "Dec 21 Thur" };
	bool Bookingexisted = false;
	int numTicket;
	cout << "\nBooking History:";
	for (int i = 0; i < numBookings; i++)
	{
		if (strcmp(email, bookingHistories[i].email) == 0)
		{
			Bookingexisted = true;
			cout << "\nMovie: " << movieNames[bookingHistories[i].movieCode];
			cout << "\nDate: " << Dates[bookingHistories[i].dateCode];
			cout << "\nShow Time: " <<hours[bookingHistories[i].sessionTimeCode];
			cout << "\nSeat: ";
			numTicket = 0;
			for (int k = 0; k < 4; k++)
			{
				numTicket += bookingHistories[i].numTickets[k];
			}
			for (int k = 0; k <numTicket; k++)
			{
				cout << bookingHistories[i].selectedSeats[k] << ", ";
			}
			display(movies, bookingHistories[i]);
		}
	}
	if (Bookingexisted == false)
	{
		cout << "No Booking Record.";
	}
	
}
void saveBookingHistories(BookingInfo bookingHistories[], int numBookings)
{
	ofstream saveBookingHistories("Booking History.txt", ios::out);
	for (int i = 0; i < numBookings; i++)
	{
		saveBookingHistories << bookingHistories[i].email << endl;
		saveBookingHistories << bookingHistories[i].movieCode << endl;
		saveBookingHistories << bookingHistories[i].dateCode << endl;
		saveBookingHistories << bookingHistories[i].sessionTimeCode << endl;
		for (int k = 0; k < 4; k++)
		{
			saveBookingHistories << bookingHistories[i].numTickets[k] << endl;
		}
		for (int k = 0; k < 24; k ++)
		{
		    saveBookingHistories << bookingHistories[i].selectedSeats[k] << " ";
		}
		saveBookingHistories << endl;
	}

	saveBookingHistories.close();
}
void saveMovies(Movie movies[], int numMovies)
{

}
