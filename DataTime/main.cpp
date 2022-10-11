#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <fstream>



using namespace std;

class DataTime {
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int timezone;

    const int monthDays[12]
        = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

public:
    DataTime() = default;
    DataTime(int Year, int Month, int Day, int Hour, int Minute, int Second, int Timezone) {
        this->year = Year;
        this->month = Month;
        this->day = Day;
        this->hour = Hour;
        this->minute = Minute;
        this->second = Second;
        this->timezone = Timezone;

    }

    void setYear(int Year) { year = Year; }
    void setMonth(int Month) { month = Month; }
    void setDay(int Day) { day = Day; }
    void setHour(int Hour) { hour = Hour; }
    void setMinute(int Minute) { minute = Minute; }
    void setSecond(int Second) { second = Second; }
    void setTimezone(int Timezone) { timezone = Timezone; }

    string monthName(int month) {
        switch (month) {
        case 1: return "January";
        case 2: return "February";
        case 3: return "March";
        case 4: return "April";
        case 5: return "May";
        case 6: return "June";
        case 7: return "July";
        case 8: return "August";
        case 9: return "September";
        case 10: return "October";
        case 11: return "November";
        case 12: return "December";

        default: return "";
        }
    }

    static bool Leap(int y) {
        if (y % 4 != 0)
            return false;
        if (y % 100 == 0 && y % 400 != 0)
            return false;

        return true;
    }

    bool Gregorian() {
        if (hour > 23 || minute > 59 || second > 59 || month > 12 ||timezone>24)
        {
            return false;
        }
        if (day > monthDays[month-1])
        if (month == 2 && day != 29 && Leap(year))
        {
            return false;
        }

        return true;
    }
    

    string weekday(int t) {
        int a, y, m, h;
        a = (14 - month) / 12;
        y = year - a;
        m = month + 12 * a - 2;
        h = (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12);
        h = (h % 7) - t;
        switch (h) {
        case -1: return "Saturday";
        case 0: return "Sunday";
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";

        default: return "";
        }
    }

    int weekNumber(int t) {
        string today;
        today = weekday(t);
        int i;
        if (today == "Monday") i = 1;
        else if (today == "Tuesday") i = 2;
        else if (today == "Wednesday") i = 3;
        else if (today == "Thursday") i = 4;
        else if (today == "Friday")i = 5;
        else if (today == "Saturday") i = 6;
        else i = 7;
        long a = 0;
        for (int i = 1; i < month; i++)
            a += monthDays[i];
        if (Leap(year))
            a++;
        a += day;
        a -= i;
        return a / 7 + 1;
    }


    string dayofmonth(int t) {
        int number;
        number = day / 7 + 1;
        string result;
        result= to_string(number);
        result +=  Ends(number)+" " + weekday(t) + " " + "of " + monthName(month);
        return result;
    }

    friend bool operator > (const DataTime& date1, const DataTime& date2) {
        return ((date1.year >= date2.year) && (date1.month >= date2.month) && (date1.day >= date2.day) && (date1.hour >= date2.hour) && (date1.minute >= date2.minute) && (date1.second > date2.second));

    }

    friend bool operator < (const DataTime& date1, const DataTime& date2) {
        return !(date1 > date2);
    }

    friend bool operator == (const DataTime& date1, const DataTime& date2) {
        return ((date1.year == date2.year) && (date1.month == date2.month) && (date1.day == date2.day) && (date1.hour == date2.hour) && (date1.minute == date2.minute) && (date1.second == date2.second));
    }

    friend bool operator != (const DataTime& date1, const DataTime& date2) {
        return !(date1 == date2);
    }

    friend bool operator >= (const DataTime& date1, const DataTime& date2) {
        return !(date1 < date2);
    }

    friend bool operator <= (const DataTime& date1, const DataTime& date2) {
        return !(date1 > date2);
    }

    friend DataTime operator - (const DataTime& date1, const DataTime& date2) {
        DataTime diff;
        DataTime d1 = date1, d2 = date2;
        if (d1.second < d2.second) {
            d1.second += 60;
            d1.minute--;
        }
        if (d1.minute < d2.minute) {
            d1.minute += 60;
            d1.hour--;
        }
        if (d1.hour < d2.hour) {
            d1.hour += 24;
            d1.day--;
        }

        if (d1.day < d2.day) {
            d1.day += d1.monthDays[d1.month];
            if (Leap(date1.year))
                d1.day++;
            d1.month--;
        }
        if (d1.month < d2.month) {
            d1.month += 12;
            d1.year--;
        }
        diff.second = d1.second - d2.second;
        diff.minute = d1.minute - d2.minute;
        diff.hour = abs(d1.hour - d2.hour - (fabs(d1.timezone - d2.timezone)));
        diff.day = d1.day - d2.day;
        diff.month = d1.month - d2.month;
        diff.year = d1.year - d2.year;

        return diff;
    }



    friend DataTime operator + (const DataTime& date1, const DataTime& date2) {
        DataTime d1 = date1, d2 = date2;
        if (d1.second + d2.second >= 60) {
            d1.second -= 60;
            d1.minute++;
        }
        if (d1.minute + d2.minute >= 60) {
            d1.minute -= 60;
            d1.hour++;
        }
        if (d1.hour + d2.hour >= 24) {
            d1.hour -= 24;
            d1.day++;
        }
        if (d1.month + d2.month > 12) {
            d1.month -= 12;
            d1.year++;
        }

        if (d1.day + d2.day > d1.monthDays[d1.month + d2.month - 1]) {
            d1.day -= d2.monthDays[d2.month - 1];
            if ((Leap(date1.year)) && d1.monthDays[d1.month] == 28)
                d1.day++;
            d1.month++;
        }

        DataTime sum{ d1.year + d2.year, d1.month + d2.month, d1.day + d2.day, d1.hour + d2.hour, d1.minute + d2.minute, d1.second + d2.second,d2.timezone };

        return sum;
    }
    DataTime& operator = (const DataTime& date1) {
        if (this == &date1)
            return *this;
        year = date1.year;
        month = date1.month;
        day = date1.day;
        hour = date1.hour;
        minute = date1.minute;
        second = date1.second;
        return *this;
    }

    friend  ostream& operator << (ostream& fout, const DataTime& date1) {
        if (date1.year != 0)
            fout << date1.year << " years ";
        if (date1.month != 0)
            fout << date1.month << " months ";
        if (date1.day != 0)
            fout << date1.day << " days ";
        if (date1.hour != 0)
            fout << date1.hour << " hours ";
        if (date1.minute != 0)
            fout << date1.minute << " minutes ";
        if (date1.second != 0)
            fout << date1.second << " seconds";
        return fout;
    }

    bool parseInput(const string& input) {
        if (input[2] != '.' || input[5] != '.' || input[10] != '-' || input[13] != ':' || input[16] != ':'|| input[19]!='-')
           return false;
        string dayStr = input.substr(0, 2);
        int dayInt = stoi(dayStr);
        setDay(dayInt);
        string monthStr = input.substr(3, 2);
        int monthInt = stoi(monthStr);
        setMonth(monthInt);
        string yearStr = input.substr(6, 4);
        int yearInt = stoi(yearStr);
        setYear(yearInt);
        string hourStr = input.substr(11, 2);
        int hourInt = stoi(hourStr);
        setHour(hourInt);
        string minuteStr = input.substr(14, 2);
        int minuteInt = stoi(minuteStr);
        setMinute(minuteInt);
        string secondStr = input.substr(17, 2);
        int secondInt = stoi(secondStr);
        setSecond(secondInt);
        string timezoneStr = input.substr(20, 1);
        int timezoneInt = stoi(timezoneStr);
        setTimezone(timezoneInt);
        return true;
    }


    string statistics(int number, const DataTime& date4, const DataTime& diff, int t) {
        DataTime date;
        int Days[7] = { 0 };
        string today;
        int a = diff.month;
        for (int i = 1; i < a + 2; i++) {
            date.setYear(this->year);
            date.setMonth(i);
            date.setDay(number);
            if (number > monthDays[i - 1])
                today = " ";
            else today = date.weekday(t);
            if (today == "Monday") Days[0]++;
            else if (today == "Tuesday") Days[1]++;
            else if (today == "Wednesday") Days[2]++;
            else if (today == "Thursday") Days[3]++;
            else if (today == "Friday")Days[4]++;
            else if (today == "Saturday") Days[5]++;
            else if (today == "Sunday") Days[6]++;
        }
        int max = Days[0];
        int maxi = 0;
        for (int i = 1; i < 7; i++) {
            if (Days[i] > max) {
                max = Days[i];
                maxi = i;
            }
        }

        switch (maxi) {

        case 0: return "Monday";
        case 1: return "Tuesday";
        case 2: return "Wednesday";
        case 3: return "Thursday";
        case 4: return "Friday";
        case 5: return "Saturday";
        case 6: return "Sunday";

        default: return "";
        }

    }

    bool Input(ifstream& fin,DataTime& date) {
        string input;
        fin >> input;
        if (date.parseInput(input)) return true;
        else return false;
    }
   
    string Ends(int value) {
        string ends;
        if (value == 1) ends = "st";
        else if (value == 2) ends = "nd";
        else if (value == 3) ends = "rd";
        else ends = "th";
        return ends;
    }
};



int main() {
     DataTime date1,date2,date3,date4,date5;
    ofstream fout;
    fout.open("Out.txt");
    ifstream fin;
    fin.open("In.txt");
    if ( (!fin.is_open()) || (!fout.is_open()))
    {
        cout << "No" << endl;
    }
    else
    {
        cout << "Yes" << endl;
       
        if (date1.Input(fin, date1) ) {
            if (date1.Gregorian())
            {
                fout << "The format is satisfactory" << endl;
                //зчитуєм другу дату для обчислення різниці
                if ( (date2.Input(fin, date2))&& (date2.Gregorian()))
                {
                    fout << "Difference between the two dates is: " << operator-(date1, date2) << endl;
                }
                //зчитуєм третю дату для суми
                if ((date3.Input(fin, date3))  &&(date3.Gregorian()))
                {
                    fout << "The sum of the found difference and the specified date: " << operator+ (date2, operator-(date1, date2))<<endl;
                }
                string type;
                fin >> type;
                int t=-1;
                if (type == "Julian" || type == "julian") t = 1;
                else if (type == "Gregorian" || type=="gregorian") t = 0;
                if (t != -1) {
                    fout << "It is: " << endl;
                    fout << date1.weekday(t) << endl;
                    fout << date1.weekNumber(t) << " week of the year " << endl;
                    fout<<date1.dayofmonth(t)<<endl;
                    //зчитуєм проміжок часу для обчислення статистики
                    if ( (date4.Input(fin, date4)) && (date5.Input(fin, date5)) ){
                            int number;
                            fin >> number;
                            if (number <= 0 || number > 31) cout << " Wrong number!" << endl;
                            else
                            {

                                fout << number << date3.Ends(number) << " usually falls on a " << date4.statistics(number, date5, operator-(date5, date4), t) << endl;
                            }
                    }
                }
                else fout << "Wrong type!" << endl;
            }
            else fout << "The format is not satisfactory" << endl;
        }
            else fout << "Incorrect input" << endl;
        
    }
  
    fin.close();
    fout.close();

}