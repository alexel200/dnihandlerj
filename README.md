# Specification - DNI Handler #
## 1. Name ##
dnihandler - A simple program for managing DNIs, a mandatory identity document for Spanish citizens.
## 2. Usage ##
dnihandler **[option] \<input filename.txt\>**\
[option] does not have to be specified. Possible options are specified in the Options section. \<input filename.txt\> can contain a list of 1 - 50 commands as specified below in Input data:
- store \<dni number\> \<expiry date\> \<birth date\>
- expired \<on date\>
- renew \<dni number\>
## 3. Description ##
dnihandler reads and processes commands from an input file in order to perform simple storage, management and querying operations on DNIs. All commands must be provided in the same input file and all the permitted commands are described in this specification. The input file should be a .txt file and should be in the same directory as the dnihandler program.
## 4. Options ##
• -h: Provides a short description of the program, showing the text in the Description section verbatim.

If any other option is provided or there are not 0 - 1 options provided, the program fails to execute with the error message _Invalid use of options_.
## 5. Input data ##
Only the following commands are allowed in the input files. Each and every command should be on its own line with no other commands. Commands are run in the exact order provided in the file. There must be at least 1 command and at most 50 commands in the .txt input file. Any other command or misspelling will cause the program to terminate with an exit message of _Unrecognised command, program aborted_.

- store \<dni number\> \<expiry date\> \<birth date\>:<p align="justify"> \<dni number\> must be a string of 8 digits followed by a single character such as _12345678A_. If the length or types do not match, an error message of Invalid DNI entered should be displayed while terminating the program. \<expiry date\> and \<birth date\> should be strings of characters in the format **_DD/MM/YYYY_**: if this format is not matched exactly, an error message of _Invalid date entered_ should be displayed while terminating the program. However, there is one exception: \<expiry date\> is allowed to be the string _never_ to reflect the fact that the DNI of an older person never expires. If there are not exactly 3 arguments provided with this command, the program should exit with the error message _Error with store command: exactly 3 arguments are expected_. The system does not allow two DNIs with the same number to be entered because the number is meant to be a unique identification: if trying to enter a duplicate DNI number, the program terminates with the error message _Cannot store DNIs with duplicate numbers_. If successful, this command stores the DNI and its data internally and produces the output _Successfully entered DNI with number \<dni number\>_ on its own line to the console output.</p>
- expired \<on date\>: <p align="justify">This commands lists all of the DNIs which have been stored that will have expired by \<on date\> which should be the string today to represent the current date or a string in the format **DD/MM/YYYY**. A DNI is considered expired if the expiry date is before or equal to the provided date. All expired DNIs are listed in a comma-separated list with the number of days they have been expired for. For example, an output might look like this: DNI 12345768A expired for 143 days, DNI 23456789B expired for 29 days. A DNI with expiry date never will never appear in this list.</p>
- renew \<dni number\>: <p align="justify">This command renews a DNI which can be done whether it is valid or expired. If there is no DNI stored with number \<dni number\>, the command fails and the program terminates with the error message Error with renew command: no DNI with number \<dni number\>found. If there is a matching DNI, its expiry date is updated from the current date according to the following rules: if the person is less than 5 years old, the new validity is 2 years, else if under 30, the new validity is 5 years, else if under 70, the validity is 10 years, else the expiry date is never. The new expiry date is printed on a line to console in the format DNI with number \<dni number\>has been renewed to expire \<new expiry date\>.</p>
<p align="justify">
Once the program has finished a successful execution of all commands, a table of all the data stored is printed where rows are sorted by earliest expiry date first and all never expiries last. If two expiry dates are the same, rows are sorted by ascending alphabetical order. There are 3 columns: DNI number, expiry date and birth date. The expiry date for a DNI is the renewed one if the renew command was run on the DNI, otherwise the original one provided with the store command. All rows with expired DNIs should have an asterisk after the expiry date: for example, 22/03/2022*.
</p>

## 6. Limitations ##
Functionality for reading files is just for testing and therefore may not be especially fault-tolerant. As mentioned previously, the input must be between 1 and 50 lines: smaller or larger inputs will fail. All dates must be provided in the exact format: there is no tolerance for dashes or spaces instead of slashes.
