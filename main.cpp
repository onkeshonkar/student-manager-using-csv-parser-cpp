#include <iostream>
#include <vector>
#include <unordered_map>

#include "csv_parser.h"
using namespace std;

string ask_year()
{
    vector<string> years{"1", "2", "3", "4"};
    string field_name = "current year";
    return ask_option_string(years, field_name);
}

string ask_branch()
{
    vector<string> branches{"IT", "CSE", "ECE"};
    string field_name = "branch";
    return ask_option_string(branches, field_name);
}

int main()
{

    vector<string> columns = {"name", "gender", "dob", "gmail", "admission_date", "mobile", "branch", "currentyear"};

    unordered_map<string, string (*)()> inputValidators;
    inputValidators["name"] = []()
    { return ask_name(); };

    inputValidators["gender"] = &ask_gender;
    inputValidators["dob"] = []()
    { return ask_date("birth date"); };

    inputValidators["gmail"] = &ask_gmail;
    inputValidators["admission_date"] = []()
    { return ask_date("admission_date"); };

    inputValidators["mobile"] = &ask_mob;
    inputValidators["branch"] = &ask_branch;
    inputValidators["currentyear"] = &ask_year;

    string file_name = "students.csv";

    CSVParser csv(file_name, columns);
    csv.get_validators(inputValidators);

    system("clear");
    bool unsaved = false;
    int option;
    while (true)
    {
        cout << "\nProgram menus" << endl;
        cout << "\n\t1. Add student\n\t2. Show All student\n\t3. Update student\n\t4. Delete student\n\t5. Filter student\n\t6. Save\n\t7. Exit" << endl;
        cout << "\nchoose any: ";
        option = ask_option(7);

        switch (option)
        {
        case 1:
            csv.insert_row();
            unsaved = true;
            break;
        case 2:
            system("clear");
            csv.print_all();
            break;
        case 3:
        {
            int uid;
            cout << "Enter uid to be updated : ";
            uid = ask_option(INT32_MAX);
            unsaved = csv.update_row(uid);
            break;
        }
        case 4:
        {
            int uid;
            cout << "Enter uid to be deleted : ";
            uid = ask_option(INT32_MAX);
            unsaved = csv.delete_row(uid);
            break;
        }
        case 5:
        {
            system("clear");
            string field_name = "column to filter by";
            string column = ask_option_string(columns, field_name);
            string value = ask_string(column + " to be searced", 50);
            csv.filter_by_column(column, value, true);
            break;
        }
        case 6:
        {
            cout << "Override to original file?[Y/N]: ";
            string res = ask_YN();
            if (res == "Y")
            {
                csv.save_modified(file_name);
                unsaved = false;
                break;
            }

            string name;
            cout << "Enter new file name: ";
            cin >> name;
            cout << "\nSave column name?[Y/N]: ";
            string with_column = ask_YN();
            csv.save_modified(name, (with_column == "Y"));
            unsaved = false;
            break;
        }
        case 7:
        {
            if (unsaved)
            {
                cout << "Warning: changes unsaved!!!" << endl;
                cout << "Exit without saving[Y/N]: ";
                string res = ask_YN();
                if (res == "N")
                {
                    break;
                }
            }

            exit(0);
            break;
        }
        default:
            break;
        }
    }

    cout << endl;
    return 0;
}
