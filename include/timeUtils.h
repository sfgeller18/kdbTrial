#include <time.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

inline std::time_t dateToUnix(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    if (ss.fail()) {
        std::cout<<dateStr<<std::endl;
        std::cerr << "Failed to parse the date." << std::endl;
        return -1; // Error indicator
    }

    return std::mktime(&tm)-28800;
}

inline std::string unixToString(std::time_t timestamp) {
    std::tm* tm = std::gmtime(&timestamp); // Convert to UTC time
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline std::string timestamper(std::string date) {
    return(std::to_string(dateToUnix(date)));
}

std::time_t to_time(const std::string& date) {
    std::tm tmDate = {};
    std::istringstream issDate(date);
    issDate >> std::get_time(&tmDate, "%Y-%m-%d");

    if (issDate.fail()) {
        std::cerr << "Error parsing date input." << std::endl;
        return 0;  // Return an invalid time_t value on error
    }

    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(std::mktime(&tmDate)));
}

std::string unixToString(const std::string& unixTimestamp) {
    // Convert string to integer
    std::time_t timestamp = std::stoi(unixTimestamp);

    // Convert timestamp to tm struct in UTC
    std::tm* timeInfo = std::gmtime(&timestamp);

    // Format the date as yyyy-mm-dd
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%d");

    return oss.str();
}

#ifdef __KDB__

#include <KDB/k.h>

F zu(I u){return u/8.64e4-10957;}   // kdb+ datetime from unix
I uz(F f){return 86400*(f+10957);}  // unix from kdb+ datetime
J pu(J u){return 1000000LL*(u-10957LL*86400000LL);} // kdb+ timestamp from unix, use ktj(Kj,n) to create timestamp from n
I up(J f){return (f/8.64e13+10957)*8.64e4;}  // unix from kdb+ timestamp
struct tm* lt(int kd) { time_t t = uz(kd); return localtime(&t); }
struct tm* lt_r(int kd, struct tm* res) { time_t t = uz(kd); return localtime_r(&t, res); }
struct tm* gt(int kd) { time_t t = uz(kd); return gmtime(&t); }
struct tm* gt_r(int kd, struct tm* res) { time_t t = uz(kd); return gmtime_r(&t, res); }
char* fdt(struct tm* ptm, char* d) { strftime(d, 10, "%Y.%m.%d", ptm); return d; }
void tsms(unsigned ts,char*h,char*m,char*s,short*mmm) {*h=ts/3600000;ts-=3600000*(*h);*m=ts/60000;ts-=60000*(*m);*s=ts/1000;ts-=1000*(*s);*mmm=ts;}
char* ftsms(unsigned ts, char* d){char h, m, s; short mmm; tsms(ts, &h, &m, &s, &mmm); sprintf(d, "%02d:%02d:%02d.%03d", h, m, s, mmm); return d;}

#endif