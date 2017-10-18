#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <sys/stat.h>

using namespace std;

inline bool starts_with(std::string const & value, std::string const & starting)
{
    if (starting.size() > value.size()) return false;
    return std::equal(starting.begin(), starting.end(), value.begin());
}

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline string trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return string("");
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}

inline int split(const string& str, vector<string>& ret_, string sep = ",")
{
    if (str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        ret_.push_back(tmp);
        tmp.clear();
    }
    return 0;
}

inline string replace(const string& str, const string& src, const string& dest)
{
    string ret;

    string::size_type pos_begin = 0;
    string::size_type pos       = str.find(src);
    while (pos != string::npos)
    {
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos       = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}

inline string basename(const string& filename){
    string::size_type pos = filename.find_last_of('/');
    if (pos == string::npos)
        return filename;
    else if(pos == filename.length()-1)
        return ""; // a bad filename
    else
        return filename.substr(pos+1, filename.length() - pos - 1);
}

inline string dirname(const string& filename){
    string::size_type pos = filename.find_last_of('/');
    if (pos == string::npos) {
        return "./";
    } else
        return filename.substr(0, pos+1);
}

inline string joinpath(const string& dirname, const string& basename){
    if(dirname[dirname.length()-1] == '/'){
        return dirname + basename;
    } else {
        return dirname + "/" + basename;
    }
}

inline string int2str(int num){
    char str[20];
    sprintf(str,"%d",num);
    return string(str);
}

//Check if a string is a file or directory
inline bool file_exists(const  string& s)
{
    bool exists = false;
    if(s.length() > 0) {
        struct stat status;
        int result = stat( s.c_str(), &status );
        if(result == 0) {
            exists = true;
        }
    }
    return exists;
}


// check if a string is a directory
inline bool is_directory(const  string& path)
{
    bool isdir = false;
    struct stat status;
    // visual studion use _S_IFDIR instead of S_IFDIR
    // http://msdn.microsoft.com/en-us/library/14h5k7ff.aspx
#ifdef _MSC_VER
#define S_IFDIR _S_IFDIR
#endif
    stat( path.c_str(), &status );
    if ( status.st_mode &  S_IFDIR  ) {
        isdir = true;
    }
// #endif
    return isdir;
}

inline void check_file_valid(const  string& s) {
    if(!file_exists(s)){
        cout << "ERROR: file '" << s << "' doesn't exist, quit now" << endl;
        exit(-1);
    }
    if(is_directory(s)){
        cout << "ERROR: '" << s << "' is a folder, not a file, quit now" << endl;
        exit(-1);
    }
}

#endif /* UTIL_H */
