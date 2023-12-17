#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
using namespace  std;


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

string getCurrentDate(){
    std::time_t t = std::time(0);   // get time now
    std::tm *now = std::localtime(&t);
    string currentDate = to_string(now->tm_mday) + "/" + to_string(now->tm_mon + 1) + "/"+ to_string(now->tm_year + 1900);
    return currentDate;
}

bool checkDni(string &dni){
    bool isDniCorrect = true;
    //check the length of the string
    if(dni.size() != 9){
        return false;
    }
    //check if the first 8 positions are digits
    for( char ch : dni.substr(0, 7)){
        if(!isdigit(ch)){
            return false;
        }
    }
    //check if the first 8 positions are digits
    for( char ch : dni.substr(8)){
       if(isalpha(ch) == 0){
           return false;
       }
    }
    return isDniCorrect;
}

bool checkDate(string &date, bool isExpireDate){
    bool isValidDate = true;
    if(isExpireDate && date == "never"){
        return true;
    }
    struct tm tm;

    if (!strptime(date.c_str(), "%d/%m/%Y", &tm)){
        isValidDate = false;
    }
    cout<<strptime(date.c_str(), "%d/%m/%Y", &tm);
    return isValidDate;
}

bool lookIfDniExist(std::vector<std::vector<string>> &success, const string &dni){
    bool existDni = false;
    for(auto & succes : success){
        if(std::find(succes.begin(), succes.end(), dni) != succes.end() ){
            existDni = true;
            break;
        }
    }
    return existDni;
}

int compareLowerEqualDate(string &date1, string &date2, bool returnDiferenceInSeconds)
{
    stringstream ss(date1 + "/" + date2);
    int year, month, day;
    char hyphen;

    // Parse the first date into seconds
    ss >> day >> hyphen >> month >> hyphen >> year;
    struct tm starttm = { 0, 0, 0, day,
                          month - 1, year - 1900 };
    time_t start = mktime(&starttm);

    // Parse the second date into seconds
    ss >> hyphen >> day >> hyphen
       >> month >> hyphen >> year;

    struct tm endtm = { 0, 0, 0, day,
                        month - 1, year - 1900 };
    time_t end = mktime(&endtm);

    if(returnDiferenceInSeconds){
        return (end - start);
    }else{
        return (start <= end);
    }
}
int getDateAsMkTime(string &date1)
{
    stringstream ss(date1 + "/");
    int year, month, day;
    char hyphen;

    // Parse the first date into seconds
    ss >> day >> hyphen >> month >> hyphen >> year;
    struct tm starttm = { 0, 0, 0, day,
                          month - 1, year - 1900 };
    return mktime(&starttm);
}
int daysBetweenDates(string &date1, string &date2, bool dayDifference)
{
    int differenceInMscnd = compareLowerEqualDate(date1, date2, true);
    if(dayDifference){
        return abs(differenceInMscnd) / 86400;
    }else{
        return abs(differenceInMscnd) / 31536000;
    }
}



void updateExpirationDate(std::vector<std::vector<string>> &success, string &dni){
    bool existDni = false;
    int newExpirationDate;
    bool neverExpire = false;

    string currentDate = getCurrentDate();

    for(int i=0; i< success.size(); i++){
        if(success[i][0] == dni){
            int years = daysBetweenDates(success[i][2], currentDate, false);
            if(years < 5){
                newExpirationDate = 2;
            }else if(years >=5 && years < 30){
                newExpirationDate = 5;
            }else if(years >= 30 && years < 70){
                newExpirationDate = 10;
            }else{
                neverExpire = true;
            }
            string header;
            for(int j = 0; j<=40; j++){
                header += "#";
            }
            cout<<header<<"             DNI Renew              "<<header<<endl;
            if(neverExpire){
                success[i][1] = "never";
                cout<<"DNI with number "<<dni<<" has been renewed to expire never expiry date"<<endl;
                break;
            }else{
                string month, day;
                int year;
                day = currentDate.substr(0, 2);
                month = currentDate.substr(3, 2);
                year = stoi(currentDate.substr(6, 4)) + newExpirationDate;
                success[i][1] = day + "/" + month + "/" + to_string(year);
                cout<<"DNI with number "<<dni<<" has been renewed to expire " + success[i][1] +" expiry date"<<endl;
                break;
            }
        }
    }
}
string center(const string s, const int w) {
    stringstream ss, spaces;
    int padding = w - s.size();                 // count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}
string prd(const string x, const int width) {
    stringstream ss;
    ss << fixed << left;
    ss.fill(' ');        // fill space around displayed #
    ss.width(width);     // set  width around displayed #
    ss << x;
    return ss.str();
}


void printData(vector<vector<string>>(&dniSuccess), vector<string> (&dniErrors)){
    std::sort(dniSuccess.begin(), dniSuccess.end(),
              [](auto& p1, auto& p2) {
                int expireTime1 = getDateAsMkTime(p1[1]);
                int expireTime2 = getDateAsMkTime(p2[1]);
                  return std::tie(expireTime1, p1[0]) < std::tie(expireTime2, p2[0]);
              });
    string currentDate = getCurrentDate();
    cout<<endl<<endl;
    std::cout << std::string(12*3 + 2*3, '-') << "\n";
    std::cout << center("DNI",10)       << " | "
              << center("Expir. Date",12)     << " | "
              << center("Birth Date",10) << "\n";

    std::cout << std::string(12*3 + 2*3, '-') << "\n";

    for(int i = 0; i< dniSuccess.size(); i++){
        string dniExpiration = dniSuccess[i][1];
        if(compareLowerEqualDate(dniExpiration, currentDate, false)){
            dniExpiration +="*";
        }
        std::cout << prd(dniSuccess[i][0],10)       << " | "
                  << prd(dniExpiration,12)      << " | "
                  << prd(dniSuccess[i][2],10)  << "  |\n";
    }

    cout<<endl;
    cout<<"2. The process encountered the errors below:"<<endl;
    for(int i = 0; i< dniErrors.size(); i++){
        cout<<dniErrors[i];
    }
    cout<<endl<<endl<<endl;
}

//template <size_t rows, size_t cols>
//void processData(string (&content)[rows][cols])
void processData(vector<vector<string>>(&content))
{
    vector<string> dniErrors;
    vector<vector<string>> dniSuccess;

    for(int i = 0; i < content.size(); i++){
        //the default value for storing
        int option = 1;
        if(content[i][0] == "expired"){
            option = 2;
        }else if(content[i][0] == "renew"){
            option = 3;
        }else if(content[i][0] != "store"){
            option = 4;
        }
        switch(option) {
            case 1: {
                bool isValidDni, isValidExpireDate, isBirthDayDateValid;
                if (content[i].size() < 4) {
                    cout << "Error with store command: exactly 3 arguments are expected" << endl;
                    exit(1);
                }
                isValidDni = checkDni(content[i][1]);
                if (!isValidDni) {
                    dniErrors.push_back("Invalid DNI entered: " + content[i][1]);
                }
                isValidExpireDate = checkDate(content[i][2], true);

                if (!isValidExpireDate) {
                    dniErrors.push_back("Invalid date entered: " + content[i][2] + " of the DNI " + content[i][1]);
                }
                isBirthDayDateValid = checkDate(content[i][3], false);
                if (!isBirthDayDateValid) {
                    dniErrors.push_back("Invalid date entered: " + content[i][3] + " of the DNI " + content[i][1]);
                }

                if (isValidDni && isValidExpireDate && isBirthDayDateValid) {
                    if (lookIfDniExist(dniSuccess, content[i][1])) {
                        cout << "Cannot store DNIs with duplicate numbers ->" << content[i][1] << endl;
                        exit(1);
                    } else {
                        vector<string> insert = {content[i][1], content[i][2], content[i][3]};
                        cout << "Successfully entered DNI with number " << content[i][1] << endl;
                        dniSuccess.push_back(insert);
                    }
                }
                break;
            }
            case 2: {
                string result;
                result = "";
                bool isValidDate;
                if (content[i].size() < 2) {
                    cout << "Error with expire command: date must be provided" << endl;
                    exit(1);
                }
                isValidDate = checkDate(content[i][1], false);
                if (!isValidDate) {
                    cout << "Error with expire command: invalid date has been entered" << endl;
                    exit(1);
                }
                for (int j = 0; j < dniSuccess.size(); j++) {
                    if (dniSuccess[j][1] != "never" && compareLowerEqualDate(dniSuccess[j][1], content[i][1], false)) {
                        if (result.empty()) {
                            result = "DNI " + dniSuccess[j][0] + " expired for " +
                                      to_string(daysBetweenDates(dniSuccess[j][1], content[i][1], true)) + " days";
                        } else {
                            result += ", DNI " + dniSuccess[j][0] + " expired for " +
                                      to_string(daysBetweenDates(dniSuccess[j][1], content[i][1], true)) + " days";
                        }
                    }
                }
                string header;
                cout.setf(ios::right, ios::adjustfield);

                for(int j=0; j<=40; j++){
                    header += "#";
                }
                cout<<header<<"             Expire DNIs               "<<header<<endl;
                cout << result << endl;
                break;
            }
            case 3:
            {
                bool isValidDni;
                if (content[i].size() < 2) {
                    cout << "Error with expire command: date must be provided" << endl;
                    exit(1);
                }
                isValidDni = checkDni(content[i][1]);
                if (!isValidDni) {
                    cout << "Error with expire command: invalid date has been entered" << content[i][1] << endl;
                    exit(1);
                }

                if (!lookIfDniExist(dniSuccess, content[i][1])) {
                    cout << "Error with renew command: no DNI with number " << content[i][1] << " found" << endl;
                    exit(1);
                }

                updateExpirationDate(dniSuccess, content[i][1]);
                break;
        }
            case 4:
                cout<<"Unrecognised command, program aborted"<<endl;
                exit(1);
                break;
        }
    }
    printData(dniSuccess, dniErrors);
}

int main(int argc, char* argv[]) {
    int allowParameters = 1;

    bool showDescription = false;
    bool correctFileExtension = false;
    int totalArguments = argc - 1;
    string fileName;
    string description = "dnihandler reads and processes commands from an input file in order to perform simple storage, management and querying operations on DNIs. All commands must be provided in the same input file and all the permitted commands are described in this specification. The input file should be a .txt file and should be in the same directory as the dnihandler program.";
    std::vector<std::vector<string>>content;

    for(int i=0; i < argc; i++ ){
        if(std::string(argv[i]) == "-h"){
            showDescription = true;
            totalArguments--;
        }
        if(std::string(argv[i]).find("txt") != std::string::npos ){
            correctFileExtension = true;
            fileName = std::string(argv[i]);
        }
    }
    if( showDescription && totalArguments >=0 && totalArguments<=1 ){
        cout<<description;
    }else if( totalArguments < 0 || totalArguments > 1){
        cout<<"Invalid use of options";
        exit(1);
    }

    if( (!correctFileExtension && totalArguments > 0)
                || (fileName.find("\\") != std::string::npos || fileName.find("/") != std::string::npos)){
        cout<<"File extension must be .txt and it must be in the same folder";
        exit(1);
    }

    if(fileName.empty()){
        cout<<"The file must be provided, run with -h to get the description";
        exit(1);
    }

    int linesCounter = 0;

    try{
        std::ifstream infile(fileName);
        if (!infile.is_open()) {
            throw invalid_argument("Could not open the file");
        }
        //Check the number of lines
        string line;
        while (getline(infile, line))
        {
            vector<string> row_values;
            split(line, '\t', row_values);
            content.push_back( row_values);
            row_values.clear();
            linesCounter++;
        }
        if (infile.bad()) {
            throw invalid_argument("Problem while reading file");
        }
        if(linesCounter < 1 || linesCounter >50){
            cout<<linesCounter<<endl;
            cout<<"file must content at least one command and up to 50";
            exit(1);
        }
        processData(content);
    }catch(const invalid_argument& e){
        cout<<"An error occur trying to read the file. Check the file exist or has the correct permissions"<<endl;
        cout<<e.what();
    }

    return 0;
}
