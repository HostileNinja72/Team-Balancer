#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

double CONVERGENCE_THRESHOLD = 0.19;

using namespace std;

double avg(const vector<pair<string, string>> &team);
vector<vector<pair<string, string>>> assign_teams(const vector<pair<string, string>> &players);
void adjust_teams(vector<vector<pair<string, string>>> &teams, double target_avg);

map<string, int> rank_categories = {
    {"Iron", 1},
    {"Bronze", 2},
    {"Silver", 3},
    {"Gold", 4},
    {"platinium", 5},
    {"Diamond", 6},
    {"Ascendant", 7},
    {"Immortal", 8},
};

int main(int argc, char **argv)
{

    vector<pair<string, string>> players = {
      };
        random_device rd;
        mt19937 gen(rd());

        shuffle(players.begin(), players.end(), gen);

    double target = 0;
    for (int i = 0; i < players.size(); i++)
    {
        target += rank_categories[players[i].second];
    }
    target /= players.size();
    cout << "target is " << target << endl;

    vector<vector<pair<string, string>>> teams = assign_teams(players);

    adjust_teams(teams, target);
    int i = 0;
    for (const auto &team : teams)
    {
        cout << endl
             << endl;
        cout << "team number " << ++i << " has an avg of " << avg(team) << endl;
        for (const auto &person : team)
        {
            cout << person.first << " : " << person.second << endl;
        }
    }
}

double avg(const vector<pair<string, string>> &team)
{

    double average = 0;
    for (const auto &person : team)
    {
        average += rank_categories[person.second];
    }
    if (team.size() == 0)
        return 0;

    return average / team.size();
}
vector<vector<pair<string, string>>> assign_teams(const vector<pair<string, string>> &players)
{
    int people_per_team = 5;
    int number_of_teams = (players.size()) / 5;
    vector<vector<pair<string, string>>> teams(number_of_teams);
    for (const auto &person : players)
    {
        double min_avg = INT_MAX;
        int min_index = 0;
        for (int i = 0; i < teams.size(); i++)
        {
            double av = avg(teams[i]);
            if (av < min_avg && teams[i].size() < people_per_team)
            {
                min_avg = av;
                min_index = i;
            }
        }
        if (teams[min_index].size() < people_per_team)
            teams[min_index].push_back(person);
    }
    return teams;
}

void adjust_teams(vector<vector<pair<string, string>>> &teams, double target_avg)
{

    while (true)
    {
        double max_avg = INT_MIN;
        int max_index = -1;
        double min_avg = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < teams.size(); i++)
        {
            double av = avg(teams[i]);
            if (av > max_avg)
            {
                max_avg = av;
                max_index = i;
            }
            if (av < min_avg)
            {
                min_avg = av;
                min_index = i;
            }
        }
        if (max_avg - target_avg < CONVERGENCE_THRESHOLD && target_avg - min_avg < CONVERGENCE_THRESHOLD)
            break;

        int r1 = rand() % teams[max_index].size();
        int r2 = rand() % teams[min_index].size();
        swap(teams[max_index][r1], teams[min_index][r2]);
    }
}
