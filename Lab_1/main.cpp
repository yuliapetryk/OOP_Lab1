#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <fstream>

using namespace std;
/// клас,який описує дату і часовий пояс
class DataTime {
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int timezone;

    const int Daysinmonth[12]
        = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    /// перевірка на високосний рік
    static bool Leap(int y) {
        if (y % 4 != 0)
            return false;
        if (y % 100 == 0 && y % 400 != 0)
            return false;

        return true;
    }

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
public:
    DataTime() = default; //конструктор за замовчуванням

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

    //перевірка правильності дати за григоріанським календарем
    bool Gregorian() {
        if (hour > 23 || minute > 59 || second > 59 || month > 12 || timezone > 24)   return false;
        if (day > Daysinmonth[month - 1]) return false;
        if (month == 2 && day >= 29 && Leap(year)) return false;
        return true;
    }

    // визначення дня тижня
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

    //номер тижня в рокові
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
            a += Daysinmonth[i];
        if (Leap(year))
            a++;
        a += day;
        a -= i;
        return a / 7 + 1;
    }

    //Видає характеристику дати у вигляді "Перший понеділок жовтня"
    string dayofmonth(int t) {
        int number;
        number = day / 7 + 1;
        string result;
        result = to_string(number);
        result += Ends(number) + " " + weekday(t) + " " + "of " + monthName(month);
        return result;
    }

    // різниця двох дат
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
            d1.day += d1.Daysinmonth[d1.month];
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
        diff.hour = fabs(d1.hour - d2.hour - (fabs(d1.timezone - d2.timezone))); // підтримка часових зон
        diff.day = d1.day - d2.day;
        diff.month = d1.month - d2.month;
        diff.year = fabs(d1.year - d2.year);

        return diff;
    }

    // сума двох дат або знайденої різниці і іншої дати
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

        if (d1.day + d2.day > d1.Daysinmonth[d1.month + d2.month - 1]) {
            d1.day -= d2.Daysinmonth[d2.month - 1];
            if ((Leap(date1.year)) && d1.Daysinmonth[d1.month] == 28)
                d1.day++;
            d1.month++;
        }

        DataTime sum{ d1.year + d2.year, d1.month + d2.month, d1.day + d2.day, d1.hour + d2.hour, d1.minute + d2.minute, d1.second + d2.second,d2.timezone };

        return sum;
    }

   // вивід об'єкта типу DataTime у файл 
    friend  ostream& operator << (ostream& fout, const DataTime& date) {
        if ((date.year == 0)&& (date.month== 0) && (date.day == 0) && (date.hour == 0) && (date.minute == 0) && (date.second== 0)) fout << 0;
        if (date.year != 0)
            fout << date.year << " years ";
        if (date.month != 0)
            fout << date.month << " months ";
        if (date.day != 0)
            fout << date.day << " days ";
        if (date.hour != 0)
            fout << date.hour << " hours ";
        if (date.minute != 0)
            fout << date.minute << " minutes ";
        if (date.second != 0)
            fout << date.second << " seconds";
        return fout;
    }
    
    // знаходження статистики
    string statistics(int number, const DataTime& date4, const DataTime& diff, int t) {
        DataTime date;
        int Days[7] = { 0 };
        string today;
        int a = diff.month;
        for (int i = 1; i < a + 2; i++) {
            date.setYear(this->year);
            date.setMonth(i);
            date.setDay(number);
            if (number > Daysinmonth[i - 1])
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
     // введення даних з файлу
    bool Inputf(const string& input) {
        if (input[2] != '.' || input[5] != '.' || input[10] != '-' || input[13] != ':' || input[16] != ':' || input[19] != '-')
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
        string timezoneStr = input.substr(20, 2);
        int timezoneInt = stoi(timezoneStr);
        setTimezone(timezoneInt);
        return true;
    }
    
    bool Input(ifstream& fin,DataTime& date) {
        string input;
        fin >> input;
        if (date.Inputf(input)) return true;
        else return false;
    }

   // для закінчення (1st, 2nd ...)
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
    DataTime date1, date2, date3, date4, date5;
    string type;
    int number, i = 0;
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
        while (!fin.eof())
        {
            i++;
            int sum = 0;
            fout << i<<"."<<endl;
            int t = -1;

            if (date1.Input(fin, date1)) sum += 1;
            if (date2.Input(fin, date2)) sum += 1;
            if (date3.Input(fin, date3)) sum += 1;
            fin >> type;
            if (date4.Input(fin, date4)) sum += 1;
            if (date5.Input(fin, date5)) sum += 1;
            fin >> number;

            if (sum == 5) {
                if (date1.Gregorian())
                {
                    fout << "The format is satisfactory" << endl;
                    if (date2.Gregorian())
                    {
                        fout << "Difference between the two dates is: " <<(date1- date2) << endl;
                        if (date3.Gregorian())
                        {
                            fout << "The sum of the found difference and the specified date: " << date2 + (date1 - date2) << endl;
                        }
                        else fout << "The format is not satisfactory" << endl;
                    }
                    else fout<< "The format is not satisfactory" << endl;

                    if (type == "Julian" || type == "julian") t = 1;
                    else if (type == "Gregorian" || type == "gregorian") t = 0;
                    if (t != -1) {
                        fout << "It is: " << endl;
                        fout << date1.weekday(t) << endl;
                        fout << date1.weekNumber(t) << " week of the year " << endl;
                        fout << date1.dayofmonth(t) << endl;

                    }
                    else fout << "Wrong type!" << endl;
                }
                else fout << "The format is not satisfactory" << endl;

                if ((date4.Gregorian()) && (date5.Gregorian()))
                {
                    if (number <= 0 || number > 31) cout << " Wrong number!" << endl;
                    else
                    {

                        fout << number << date3.Ends(number) << " usually falls on a " << date4.statistics(number, date5, operator-(date5, date4), t) << endl;
                    }
                }
                else fout << "The format is not satisfactory" << endl;
            }
            else fout << "Wrong input" << endl;
        }
        
    }
  
    fin.close();
    fout.close();

}