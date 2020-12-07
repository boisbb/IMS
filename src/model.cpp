#include <iostream>
#include <bits/stdc++.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <math.h>
#include <cmath>
using namespace std;

double cnt_infected(double susceptible, double q, double p, double t, double Q, double infected, bool masks);
double study(double q, double t, bool masks, double volume, double percentage, double p, double infected);
double ach(double Q, double volume);
void line(int count, char character);
void study_line(string percentage, string q, double bus, double classroom, double aircraftc, double office);
void study_header(bool masks);
double ideal_ventilation(int infected, double p, double q, double t, bool masks, double percentage);
double wells_riley(int infected, double p, double Q, double q, double t, bool masks);


int main(int argc, char *argv[]) {
  int opt;
  bool masks = false;
  int infected = 1;
  double p = 0.3;
  double Q = 0.0;
  double q = 14.0;
  double t = 0.25;
  int i = 0;
  bool ventialtion_rate = false;
  bool study_results = false;
  bool count_infected = false;
  bool normal = false;
  double volume = 0.0;
  double percentage = 0.01;
  int susceptible = 4;
  double result = 0.0;

  // Parsing arguments
  while((opt = getopt(argc, argv, "s:v:i:p:Q:q:t:-:m")) != -1){
    if (i == 0){
      switch (opt){
        case '-':
          if (!strcmp(optarg, "ventilation-rate")) {
            ventialtion_rate = true;
          }
          else if (!strcmp(optarg, "normal")) {
            normal = true;
          }
          else if (!strcmp(optarg, "study-results")) {
            study_results = true;
          }
          else if (!strcmp(optarg, "infected-count")){
            count_infected = true;
          }
          else if (!strcmp(optarg, "help")){
            cout << "Model that simulates epidemic spread in confined spaces based on ventialtion rate." << endl;
            cout << "Please run the program like so:" << endl;
            cout << "make run ARGS=\"--ventilation-rate [-i NUMBER | -q NUMBER | -t number | -p NUMBER | -m]\"" << endl;
            cout << "make run ARGS=\"--normal [-i NUMBER | -q NUMBER | -t number | -Q NUMBER | -m]\"" << endl;
            cout << "make run ARGS=\"--infected-count [-i NUMBER | -q NUMBER | -t number | -s NUMBER | -m]\"" << endl;
            cout << "make run ARGS=\"--study-results" << endl << endl;

            cout << "Example runs:" << endl;
            cout << "make run ARGS=\"--normal -i 1 -Q 1310 -q 14 -t 8 -m\"" << endl;
            cout << "make run ARGS=\"--infected-count -s 5 -i 1 -Q 665 -q 14 -t 8\"" << endl;
            cout << "make run ARGS=\"--ventilation-rate -i 1 -q 48 -t 3 -p 1 -m\"" << endl;
            cout << "make run ARGS=\"--study-results\"" << endl << endl;

            cout << "Arguments:" << endl;
            cout << "-i NUMBER - number of infected" << endl;
            cout << "-q NUMBER - quantum generation rate in h^-1" << endl;
            cout << "-t NUMBER - time in hours" << endl;
            cout << "-p NUMBER - percentage" << endl;
            cout << "-Q NUMBER - ventilation rate in m^3/h" << endl;
            cout << "-s NUMBER - number of susceptible" << endl;
            cout << "-m NUMBER - masks switch" << endl;
            return 0;
          }
          else{
            cerr << "ERROR: Unknown argument " << optarg << endl;
            return 1;
          }
          break;
        default:
          cerr << "ERROR: incorrect argument usage." << endl;
          return 1;
      }
      i++;
      continue;
    }

    if(study_results){
      break;
    }
    else{
      switch (opt) {
        case 'i':
          infected = atoi(optarg);
          break;
        case 'Q':
          Q = atof(optarg);
          break;
        case 'q':
          q = atoi(optarg);
          break;
        case 't':
          t = atof(optarg);
          break;
        case 'm':
          masks = true;
          break;
        case 'p':
          if (!ventialtion_rate) {
            fprintf(stderr, "Invalid argument combination.\n");
            return 1;
          }
          percentage = atof(optarg);

          if (percentage >= 100) {
            fprintf(stderr, "Percentage cannot be higher than 100%% (or equal).\n");
            return 1;
          }

          percentage /= 100;
          cout << percentage << endl;
          break;
        case 's':
          if (!count_infected) {
            fprintf(stderr, "Invalid argument combination.\n");
            return 1;
          }
          susceptible = atoi(optarg);
          break;
        case ':':
          fprintf(stderr, "Missing option. %s\n", optarg);
          return 1;
        case '?':
          fprintf(stderr, "Unknown argument: %c\n", optopt);
          return 1;
      }
    }

    i++;
  }

  // In case no argument was entered
  if (!study_results && !ventialtion_rate && !count_infected && !normal) {
    cerr << "ERROR: no arguments." << endl;
    cout << "Pleas run: "<< endl << "make run ARGS=\"--help\"" << endl;
    return 1;
  }

  // For ventilation computing
  if (ventialtion_rate) {
    cout << "Calculating ideal (0-1%) conditions of probability of infection risk." << endl;
    cout << "Number of infected (I): " << infected << endl;
    cout << "Pulmonary rate (p): " << p << endl;
    cout << "Quantum generation rate per infecter (q): " << q << endl;
    cout << "Time (t): " << t << endl;
    cout << "Desired percentage: " << p * 100 << "%" << endl;



    result = ideal_ventilation(infected, p, q, t, masks, percentage);

    line(80, '=');
    printf("|%10s|%10s|%10s|%10s|%10s|%10s|%12s|\n", "infected", "p", "q", "time", "masks", "percentage", "Q");
    line(80, '-');
    printf("|%10d|%10.2f|%10.2f|%10.2f|%10s|%9.4f%%|%12.0f|\n", infected, p, q, t, (masks) ? "on" : "off", percentage * 100, round(result));
    line(80, '=');

    cout << "Result for q = " << q << ", masks are "<< ((masks) ? "on" : "not on")  <<" is: " << result << endl;
  }
  // Compute and output results of the table
  else if(study_results){
    vector<double> percentage;
    percentage.push_back(0.02);
    percentage.push_back(0.015);
    percentage.push_back(0.01);
    percentage.push_back(0.005);
    percentage.push_back(0.001);

    study_header(false);
    for (auto i = percentage.begin(); i != percentage.end(); ++i){
      string perc = (*i == 0.02 ? "2.0" : (*i == 0.015 ? "1.5" : (*i == 0.01 ? "1.0" : (*i == 0.005 ? "0.5" : "0.1"))));
      study_line(perc + "%", to_string(14), study(14, 0.5, false, 75, *i, 0.3, 1), study(14, 2.0, false, 348, *i, 0.3, 1),
                                            study(14, 4.0, false, 100, *i, 0.3, 1), study(14, 8.0, false, 150, *i, 0.3, 1));
    }
    line(81, '=');
    for (auto i = percentage.begin(); i != percentage.end(); ++i){
      string perc = (*i == 0.02 ? "2.0" : (*i == 0.015 ? "1.5" : (*i == 0.01 ? "1.0" : (*i == 0.005 ? "0.5" : "0.1"))));
      study_line(perc + "%", to_string(48), study(48, 0.5, false, 75, *i, 0.3, 1), study(48, 2.0, false, 348, *i, 0.3, 1),
                                            study(48, 4.0, false, 100, *i, 0.3, 1), study(48, 8.0, false, 150, *i, 0.3, 1));
    }

    cout << endl;
    cout << endl;

    study_header(true);
    for (auto i = percentage.begin(); i != percentage.end(); ++i){
      string perc = (*i == 0.02 ? "2.0" : (*i == 0.015 ? "1.5" : (*i == 0.01 ? "1.0" : (*i == 0.005 ? "0.5" : "0.1"))));
      study_line(perc + "%", to_string(14), study(14, 0.5, true, 75, *i, 0.3, 1), study(14, 2.0, true, 348, *i, 0.3, 1),
                                            study(14, 4.0, true, 100, *i, 0.3, 1), study(14, 8.0, true, 150, *i, 0.3, 1));
    }
    line(81, '=');
    for (auto i = percentage.begin(); i != percentage.end(); ++i){
      string perc = (*i == 0.02 ? "2.0" : (*i == 0.015 ? "1.5" : (*i == 0.01 ? "1.0" : (*i == 0.005 ? "0.5" : "0.1"))));
      study_line(perc + "%", to_string(48), study(48, 0.5, true, 75, *i, 0.3, 1), study(48, 2.0, true, 348, *i, 0.3, 1),
                                          study(48, 4.0, true, 100, *i, 0.3, 1), study(48, 8.0, true, 150, *i, 0.3, 1));
    }
  }
  // Count number of infected
  else if (count_infected) {
    cout << "Calculating infection probability:" << endl;
    cout << "Number of infected (I): " << infected << endl;
    cout << "Number of susceptible (S): " << susceptible << endl;
    cout << "Pulmonary rate (p): " << p << endl;
    cout << "Room ventilation rate (Q): " << Q << endl;
    cout << "Quantum generation rate per infecter (q): " << q << endl;
    cout << "Time in hours (t): " << t << endl;
    result = cnt_infected(susceptible, q, 0.3, t, Q, infected, masks);

    line(80, '=');
    printf("|%10s|%10s|%10s|%10s|%10s|%10s|%12s|\n", "infected", "p", "Q", "q", "time", "masks", "new infected");
    line(80, '-');
    printf("|%10d|%10.2f|%10.2f|%10.2f|%10.2f|%10s|%12.0f|\n", infected, p, Q, q, t, (masks) ? "on" : "off", round(result));
    line(80, '=');

    cout << "Number of infected: " << round(result) << endl;
  }
  // For computation of probability
  else{
    cout << "Calculating infection probability:" << endl;
    cout << "Number of infected (I): " << infected << endl;
    cout << "Pulmonary rate (p): " << p << endl;
    cout << "Room ventilation rate (Q): " << Q << endl;
    cout << "Quantum generation rate per infecter (q): " << q << endl;
    cout << "Time in hours (t): " << t << endl;
    // wellsRiley = wells_riley(1, 0.3, 1200, 14, 3);
    result = wells_riley(infected, p, Q, q, t, masks);
    line(78, '=');
    printf("|%10s|%10s|%10s|%10s|%10s|%10s|%10s|\n", "infected", "p", "Q", "q", "time", "masks", "prob");
    line(78, '-');
    printf("|%10d|%10.2f|%10.2f|%10.2f|%10.2f|%10s|%9.3f%%|\n", infected, p, Q, q, t, (masks) ? "on" : "off", result*100);
    line(78, '=');

    cout << "Probability of infection is: " << result * 100 << "%"  << endl;
  }


  return 0;
}

// computes ideal ventilation and ACH for values in study
double study(double q, double t, bool masks, double volume, double percentage, double p, double infected){
  return ach(ideal_ventilation(infected, p, q, t, masks, percentage), volume);
}

// computes new infected people - this number is rounded
double cnt_infected(double susceptible, double q, double p, double t, double Q, double infected, bool masks){
  if (masks) {
    return susceptible * (1 - exp(-((infected * q * p * q * t) * (1-0.5) * (1-0.5)) / Q));
  }
  else{
    return susceptible * (1 - exp(-(infected * q * p * q * t) / Q));
  }
}

/*
ACH = 60 Q / V
  - 60 - one hour
  - q = fresh air (m^3/min)
  - V = volume of room
Q = V * ACH
*/
double ach(double Q, double volume){
  return Q/volume;
}

// prints header for study-results
void study_header(bool masks){
  printf("%5s", "");
  line(76, '=');
  if (masks)
    printf("%5s|%31s %s %31s|\n", "", "","With Masks", "");
  else
    printf("%5s|%30s %s %29s|\n", "", "","Without Masks", "");

  printf("%5s", "");
  line(76, '=');
  printf("%5s|%14s|%14s|%14s|%14s|%14s|\n", "","environment", "bus", "classroom", "aircraft cabin", "office");
  printf("%5s", "");
  line(76, '-');
  printf("%5s|%14s|%14s|%14s|%14s|%14s|\n", "","volume", "75", "348", "100", "150");
  printf("%5s", "");
  line(76, '-');
  printf("%5s|%14s|%14s|%14s|%14s|%14s|\n", "","exposure time","0.5", "2.0", "4.0", "8.0");
  printf("%5s", "");
  line(76, '=');
  printf("%5s|%29s %s %28s|\n", "", "","Air Change Rate", "");
  line(81, '-');
}

void study_line(string percentage, string q, double bus, double classroom, double aircraftc, double office){
  string bus_str = to_string(bus);
  string classroom_str = to_string(classroom);
  string aircraftc_str = to_string(aircraftc);
  string office_str = to_string(office);
  printf("|%4s|%14s|%14s|%14s|%14s|%14s|\n", percentage.c_str(), ("q = " + q).c_str(), bus_str.c_str(), classroom_str.c_str(), aircraftc_str.c_str(), office_str.c_str());
  line(81, '-');
}

void line(int count, char character){
  for (int i = 0; i < count; i++) {
    cout << character;
  }
  cout << endl;
}

/*
Ideal ventilation for 1% probability of infection spread for t amount of time
  Q = -((I * q * p * t)/ln(1 - 0.1))
*/
double ideal_ventilation(int infected, double p, double q, double t, bool masks, double percentage){
  if(masks){
    return -((infected*q*p*t)*(1-0.5)*(1-0.5))/log(1 - percentage);
  }
  else{
    return -((infected * q * p * t)/log(1 - percentage));
  }
}


/*
Wells-Riley equation - without masks
  P = C/S = 1 - e^((-I*q*p*t)/Q)
  P - probability of infection risk
  C - number of cases that develop infection
  S - number of susceptible people
  I - number of source patients
  p - pulmonary ventilation rate of each susceptible person per hour
    - 0.3 m^3/h - when people are sitting or practicing light activity indoors
    - idea - we could do that user can set which environment it is computed for (so changing pulmonary rate)
  Q - room ventilation rate
  q - quantum generation rate produced by infector (h^-1)
    - for covid 18-48 h^-1
  t - exposure time


  Modified Wells-Riley equation - with masks
    P = C/S = 1 - e^((-I*q*p*t)*(1-n_I)*(1-n_S)/Q)
    n_I = 50% - exhalation filtration efficiency
    n_S = 50% - respiratory filtration efficiency


*/

double wells_riley(int infected, double p, double Q, double q, double t, bool masks){
  if(!masks){
    return 1 - exp(-(infected*q*p*t)/Q);
  }
  else{
    return 1 - exp(-((infected*q*p*t)*(1-0.5)*(1-0.5))/Q);
  }
}
