/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//timestamp.h

#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_

#include "time.h"
#include <string>
#include <stdlib.h>
#include <regex.h>

using std::string;

class Timestamp{
public:
    Timestamp(time_t timestampValue, const string& timezoneValue);
    Timestamp(time_t timestampValue, const string& timezoneValue, const string& afterVal);
    time_t getNumericalTime() const;
    const string& getTimezone() const;
    const string& getAfter() const;
    string to_plain_str(const string& format) const;
    static Timestamp parse(const string& time_str, const string& format);
    bool operator ==(const Timestamp& other) const;
    bool operator !=(const Timestamp& other) const;
    static const Timestamp illegal;
private:
    time_t timestamp;
    string timezone;
    string after;
    /* @return timezone string. "" if do not find a legal timezone
     * */
    static string extractTimezone(const string& time_str, const string& format);
    static string translate2Env(const string& timezone);
    static void setTimezone(const string& timezone);

    static void initRegex();
    static bool regex_initialized;
    static regex_t num_tz_regex;
    static regex_t gmt_tz_regex;
};

#endif
