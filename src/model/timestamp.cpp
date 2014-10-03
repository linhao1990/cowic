/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//timestamp.cpp

#include <stdio.h>
#include <iostream>

#include "timestamp.h"

const char* num_tz_pattern = "[\\+-][0-9]{4}";
const char* gmt_tz_pattern = "GMT";
const Timestamp Timestamp::illegal(0, "");
bool Timestamp::regex_initialized = false;
const int cflags = REG_EXTENDED;
regex_t Timestamp::num_tz_regex;
regex_t Timestamp::gmt_tz_regex;

Timestamp::Timestamp(time_t timestampValue, const string& timezoneValue)
    : timestamp(timestampValue), timezone(timezoneValue), after(){
}

Timestamp::Timestamp(time_t timestampValue, const string& timezoneValue, const string& afterVal)
    : timestamp(timestampValue), timezone(timezoneValue), after(afterVal){
}

time_t Timestamp::getNumericalTime() const{
    return timestamp;
}

const string& Timestamp::getTimezone() const{
    return timezone;
}

const string& Timestamp::getAfter() const{
    return after;
}

string Timestamp::translate2Env(const string& timezone){
    if(timezone == "GMT"){
        return "GMT";
    } else if(timezone[0] == '+' || timezone[0] == '-'){
        // replace '+' by '-', '-', by '+'
        char sign = timezone[0] == '+' ? '-' : '+';
        string ret = "UTC";
        return ret + sign + timezone.substr(1, 2);
    } else 
        return "";
}

void Timestamp::setTimezone(const string& timezone){
    if(!timezone.empty()){
        string env_tz = Timestamp::translate2Env(timezone);
        setenv("TZ", env_tz.c_str(), 1);
        tzset();
    }
}

//TODO Bug: If format contains str cannot be parsed successfully, 
// the translation may be uncorrect.
// e.g. timeStr = "[03/Feb/2003:03:07:23 +0100] "
//      format = "[%d/%b/%Y:%H:%M:%S %z "  the last space in format is not parsed
//      output = "[03/Feb/2003:03:07:23 +0100 ] " the not parsed space appears uncorrectly
string Timestamp::to_plain_str(const string& format) const{
    setTimezone(timezone);

    struct tm *lt = localtime(&timestamp);
    char szBuf[256] = {0}; 
    strftime(szBuf, sizeof(szBuf), format.c_str(), lt);
    return string(szBuf) + after;
}

void Timestamp::initRegex(){
    if(!Timestamp::regex_initialized){
        Timestamp::regex_initialized = true;
        regcomp(&Timestamp::num_tz_regex, num_tz_pattern, cflags);
        regcomp(&Timestamp::gmt_tz_regex, gmt_tz_pattern, cflags);
    }
}

string Timestamp::extractTimezone(const string& time_str, const string& format){
    //init regex
    initRegex();
    //extract timezone
    regex_t* tz_regex;;
    if(format.find("%z") != string::npos){
        tz_regex = &Timestamp::num_tz_regex;
    }
    else if(format.find("%Z") != string::npos){ 
        tz_regex = &Timestamp::gmt_tz_regex;
    } else{
        return "";
    }
    
    regmatch_t pmatch[1];
    size_t nmatch = 1;
    int status = regexec(tz_regex, time_str.c_str(), nmatch, pmatch, 0);
    if(status == REG_NOMATCH)
        return "";
    else if(status == 0){
        return time_str.substr(pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
    }
    return "";
}

string cutAfterString(char* parsedOffset, const string& time_str){
    unsigned int parsed_bit = (unsigned int)(parsedOffset - time_str.c_str());
    return time_str.substr(parsed_bit);
}

Timestamp Timestamp::parse(const string& time_str, const string& format){
    string timezone = Timestamp::extractTimezone(time_str, format);
    setTimezone(timezone);
    //parse timestamp
    struct tm tm_time;
    char * parsedOffset = strptime(time_str.c_str(), format.c_str(), &tm_time);
    if(parsedOffset == NULL)
        return Timestamp::illegal;
    string after = cutAfterString(parsedOffset, time_str);
    time_t timestamp = mktime(&tm_time);
    return Timestamp(timestamp, timezone, after);
}

bool Timestamp::operator ==(const Timestamp& other) const{
    return getNumericalTime() == other.getNumericalTime() && getTimezone() == other.getTimezone();
}

bool Timestamp::operator !=(const Timestamp& other) const{
    return !(this->operator== (other));
}
