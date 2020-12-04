#include <iostream>
#include <bits/stdc++.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <math.h>
using namespace std;

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
  int infected = 0;
  double p = 0.3;
  double Q = 0.0;
  double q = 0.0;
  double t = 0.0;
  int i = 0;
  bool one_percent = false;
  double volume = 0.0;
  bool normal = false;
  bool study_results = false;
  double percentage = 0.01;

  while((opt = getopt(argc, argv, "v:i:p:Q:q:t:-:m")) != -1){
    if (i == 0){
      switch (opt){

        case '-':
          if (!strcmp(optarg, "percentage")) {
            one_percent = true;
          }
          else if (!strcmp(optarg, "normal")) {
            normal = true;
          }
          else if (!strcmp(optarg, "study-results")) {
            study_results = true;
          }
          else{
            cerr << "ERROR: Unknown argument " << optarg << endl;
            return 2;
          }
          break;
        default:
          cerr << "ERROR: incorrect argument usage." << endl;
          return 2;
      }
      i++;
      continue;
    }
    if (one_percent) {
      int random;
      switch (opt) {
        case 'v':
          volume = atof(optarg);
          break;
        case 'i':
          infected = atoi(optarg);
          break;
        case 'm':
          masks = true;
          break;
        case 'p':
          percentage = atof(optarg);
          percentage /= 100;
          cout << percentage << endl;
        case 't':
          t = atof(optarg);
          break;
        case 'q':
          q = atoi(optarg);
          break;
        case ':':
          fprintf(stderr, "Missing option. %s\n", optarg);
          return 2;
        case '?':
          fprintf(stderr, "Unknown argument: %c\n", optopt);
          return 2;
      }
    }
    else if(study_results){
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
        case ':':
          fprintf(stderr, "Missing option. %s\n", optarg);
          return 2;
        case '?':
          fprintf(stderr, "Unknown argument: %c\n", optopt);
          return 2;
      }
    }

    i++;
  }
  if (one_percent) {
    cout << "Calculating ideal (0-1%) conditions of probability of infection risk." << endl;
    cout << "Volume: " << volume << endl;
    cout << "Number of infected (I): " << infected << endl;
    cout << "Pulmonary rate (p): " << p << endl;
    cout << "Quantum generation rate per infecter (q): " << q << endl;
    cout << "Time (t): " << t << endl;

  }
  else if(normal){
    cout << "Calculating for:" << endl;
    cout << "Number of infected (I): " << infected << endl;
    cout << "Pulmonary rate (p): " << p << endl;
    cout << "Room ventilation rate (Q): " << Q << endl;
    cout << "Quantum generation rate per infecter (q): " << q << endl;
    cout << "Time (t): " << t << endl;
  }

  double result = 0.0;

  if (one_percent) {
    if (q == 0.0) {
      cout << "Quantum generation rate not entered, using two values 14 and 48." << endl;
      q = 14.0;
      result = ideal_ventilation(infected, p, q, t, masks, percentage);
      cout << "Result for q = " << q << ", masks are "<< ((masks) ? "on" : "not on")  <<" is: " << result << endl;
      q = 48.0;
      result = ideal_ventilation(infected, p, q, t, masks, percentage);
      cout << "Result for q = " << q << ", masks are "<< ((masks) ? "on" : "not on")  <<" is: " << result << endl;
    }
    else{
      result = ideal_ventilation(infected, p, q, t, masks, percentage);
      cout << "Result for q = " << q << ", masks are "<< ((masks) ? "on" : "not on")  <<" is: " << result << endl;
    }
  }
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
      study_line(perc + "%", to_string(14), study(14, 0.5, false, 75, *i, 0.3, 1), study(14, 0.5, false, 348, *i, 0.3, 1),
                                            study(14, 0.5, false, 100, *i, 0.3, 1), study(14, 0.5, false, 150, *i, 0.3, 1));
      study_line(perc + "%", to_string(48), study(48, 0.5, false, 75, *i, 0.3, 1), study(48, 0.5, false, 348, *i, 0.3, 1),
                                            study(48, 0.5, false, 100, *i, 0.3, 1), study(48, 0.5, false, 150, *i, 0.3, 1));
    }

    cout << endl;
    cout << endl;

    study_header(true);
    for (auto i = percentage.begin(); i != percentage.end(); ++i){
      string perc = (*i == 0.02 ? "2.0" : (*i == 0.015 ? "1.5" : (*i == 0.01 ? "1.0" : (*i == 0.005 ? "0.5" : "0.1"))));
      study_line(perc + "%", to_string(14), study(14, 0.5, true, 75, *i, 0.3, 1), study(14, 0.5, true, 348, *i, 0.3, 1),
                                            study(14, 0.5, true, 100, *i, 0.3, 1), study(14, 0.5, true, 150, *i, 0.3, 1));
      study_line(perc + "%", to_string(48), study(48, 0.5, true, 75, *i, 0.3, 1), study(48, 0.5, true, 348, *i, 0.3, 1),
                                            study(48, 0.5, true, 100, *i, 0.3, 1), study(48, 0.5, true, 150, *i, 0.3, 1));
    }
  }
  else{
    // wellsRiley = wells_riley(1, 0.3, 1200, 14, 3);
    cout << "Without masks." << endl;
    result = wells_riley(infected, p, Q, q, t, masks);
    cout << "Probability of infection is: " << result * 100 << "%"  << endl;
  }


  return 0;
}

double study(double q, double t, bool masks, double volume, double percentage, double p, double infected){
  return ach(ideal_ventilation(infected, p, q, t, masks, percentage), volume);
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
