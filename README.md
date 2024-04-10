# Overview
A simple tool to print CSV in a table format.

Sample CSV files borrowed from [here](https://people.sc.fsu.edu/~jburkardt/data/csv/csv.html)

# Example
```sh
$ cat csv/test.csv | ./app
username   email                   id    first name  last name
booker12   rachel@yourcompany.com  9012  Rachel      Booker
grey07     laura@yourcompany.com   2070  Laura       Grey
johnson81  craig@yourcompany.com   4081  Craig       Johnson
jenkins46  mary@yourcompany.com    9346  Mary        Jenkins
smith79    jamie@yourcompany.com   5079  Jamie       Smith

$ ./app csv/addresses.csv
John                   Doe       120 jefferson st.                 Riverside    NJ  08075
Jack                   McGinnis  220 hobo Av.                      Phila        PA  09119
John "Da Man"          Repici    120 Jefferson St.                 Riverside    NJ  08075
Stephen                Tyler     7452 Terrace "At the Plaza" road  SomeTown     SD  91234
                       Blankman                                    SomeTown     SD  00298
Joan "the bone", Anne  Jet       9th, at Terrace plc               Desert City  CO  00123

$ ./app < csv/airtravel.csv
Month  1958  1959  1960
JAN    340   360   417
FEB    318   342   391
MAR    362   406   419
APR    348   396   461
MAY    363   420   472
JUN    435   472   535
JUL    491   548   622
AUG    505   559   606
SEP    404   463   508
OCT    359   407   461
NOV    310   362   390
DEC    337   405   432
```
