#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <iomanip>
#include <chrono>
using namespace std;
#define sety 2014//the beginning year of output data 
#define setm 1 //the beginning month of output data 
#define setd 2//the beginning day of output data 
void* new2d(int h, int w, int size) { //the function of creating Dynamic array 
    int i;
    void** p;
    p = (void**)new char[h * sizeof(void*) + h * w * size];
    for (int i = 0; i < h; i++)
        p[i] = ((char*)(p + h)) + i * w * size;
    return p;
}
#define NEW2D(H,W,TYPE) (TYPE **)new2d(H,W,sizeof(TYPE))
#define mnumber 85//how many month of whole data
void cut(int** period, int& begindate) {
    ifstream input;
    input.open("C:/Users/GD/CLionProjects/2018catchYY/GOOG.csv", ios::in);//read stock data
    string s;
    int year, month, day, record = 0;
    int store[2] = { 0 }, mcount = 0, dcount = 0, over = 1766, count = 0;//variable over means how many openning days of whole data
    store[0] = 2012;//initial year of the data
    store[1] = 12;//initial month of the data
    while (getline(input, s)) {
        string tmp, time;
        stringstream ss(s);
        for (int j = 0; j < 3; j++) {
            getline(ss, tmp, ',');
            if (j == 0) {
                stringstream date(tmp);
                getline(date, time, '/');
                stringstream a;
                a << time;
                a >> year;
                getline(date, time, '/');
                stringstream b;
                b << time;
                b >> month;
                getline(date, time, '/');
                stringstream c;
                c << time;
                c >> day;
            }
        }
        if (year == sety && month == setm && day == setd)
            record = count;
        count++;
        if (count == over) {
            period[0][mcount] = store[0];
            period[1][mcount] = store[1];
            period[2][mcount] = dcount + 1;
            break;
        }
        if (month != store[1]) {
            period[0][mcount] = store[0];
            period[1][mcount] = store[1];
            period[2][mcount] = dcount;
            store[0] = year;
            store[1] = month;
            dcount = 1;
            mcount++;
        }
        else
            dcount++;
    }
    input.close();
    begindate = record;
}
vector<int> slidingwindow(int** period, string choosetime) {//distinguish each slidingwindow
    int train = 0, test = 0, count = 1, day = 0, next = 0, begin = 0;//set begin is 2012
    vector<int> vec;
    int A, B, C;
    if (choosetime == "MM") {
        A = 1;
        B = 1;
        C = 0;
    }
    if (choosetime == "QQ") {
        A = 3;
        B = 3;
        C = 0;
    }
    if (choosetime == "HH") {
        A = 6;
        B = 6;
        C = 0;
    }
    if (choosetime == "YY") {
        A = 12;
        B = 12;
        C = 0;
    }
    if (choosetime == "QM") {
        A = 3;
        B = 1;
        C = 0;
    }
    if (choosetime == "HM") {
        A = 6;
        B = 1;
        C = 0;
    }
    if (choosetime == "YM") {
        A = 12;
        B = 1;
        C = 0;
    }
    if (choosetime == "HQ") {
        A = 6;
        B = 3;
        C = 0;
    }
    if (choosetime == "YQ") {
        A = 12;
        B = 3;
        C = 0;
    }
    if (choosetime == "YH") {
        A = 12;
        B = 6;
        C = 0;
    }
    if (choosetime == "M#") {
        A = 1;
        B = 13;
        C = 1;
        next = 1;
    }
    if (choosetime == "Q#") {
        A = 3;
        B = 15;
        C = 3;
        next = 1;
    }
    if (choosetime == "H#") {
        A = 6;
        B = 18;
        C = 6;
        next = 1;
    }
    while (true) {
        if ((count + A + B - C) <= 85) {
            for (int i = 0; i < A; i++)
                day += period[2][count + i];
            if (period[0][count] == sety && period[1][count] == setm) {
                begin = 1;
            }
            cout << period[0][count] << " " << period[1][count] << " " << day << endl;
            if (begin == 1)
                vec.push_back(day);
            day = 0;
        }
        else
            break;
        if (next == 1) {
            if (begin == 1) {
                int temp = 0;
                for (int i = 0; i < B % 12; i++)
                    temp += period[2][count + i];
                vec.push_back(temp);
            }
            for (int i = 0; i < B % 12; i++)
                day += period[2][count + A + 12 - C + i];
            cout << day << endl;
            day = 0;
            count += B % 12;
        }
        else {
            if (begin == 1) {
                int temp = 0;
                for (int i = 0; i < B; i++)
                    temp += period[2][count + i];
                vec.push_back(temp);
            }
            for (int i = 0; i < B; i++)
                day += period[2][count + A + i];
            cout << day << endl;
            day = 0;
            count += B;
        }
    }
    return vec;
}
void measure(double* beta, int size, int** strategy, int num, int** m) {//using randnumber measure each bit
    double x;
    int count = 0;
    for (int i = 0; i < 32; i++) {
        x = rand() / (double)(RAND_MAX + 1);
        if (x > beta[i])
            m[num][i] = 1;
        else
            m[num][i] = 0;
    }
    for (int i = 0; i < 32; i++) {
        count += pow(2, (7 - i % 8)) * m[num][i];
        if (i % 8 == 7) {
            strategy[num][i / 8] = count + 1;
            count = 0;
        }
    }
}
void SMA(vector<double> v, int begin, int day, double** MA) {//SMA formula
    double total = 0.0;
    for (int i = 0; i < 256; i++) {
        double ma = 0.0;
        for (int j = 0; j < i + 1; j++) {
            ma += v[begin + day - j];
        }
        ma /= (i + 1);
        MA[i][day] = ma;
    }
}
void WMA(vector<double> v, int begin, int day, double** MA) {//WMA formula
    int divisor = 0;
    for (int i = 0; i < 256; i++) {
        double ma = 0.0;
        for (int j = 0; j < i + 1; j++) {
            ma += (i + 1 - j) * v[begin + day - j];
        }
        divisor = ((i + 2) * (i + 1)) / 2;
        ma /= divisor;
        MA[i][day] = ma;
    }
}
void RWMA(vector<double> v, int begin, int day, double** MA) {//RWMA formula
    int divisor = 0;
    for (int i = 0; i < 256; i++) {
        double ma = 0.0;
        for (int j = 0; j < i + 1; j++) {
            ma += (1 + j) * v[begin + day - j];
        }
        divisor = ((i + 2) * (i + 1)) / 2;
        ma /= divisor;
        MA[i][day] = ma;
    }
}
void EMA(vector<double> v, int begin, int day, double** MA, double store[256], int count, int stop) {//EMA formula
    if (count < 257) {
        for (int i = 0; i < count; i++) {
            double total = 0.0;
            if (i + 1 == count) {
                for (int j = 0; j < count; j++)
                    total += v[j];
                total /= count;
                store[i] = total;
            }
            else {
                store[i] += 2 * (v[count] - store[i]) / (i + 2);
            }
        }
    }
    else {
        for (int i = 0; i < 256; i++) {
            store[i] += 2 * (v[count] - store[i]) / (i + 2);
        }
    }
    if (begin != 0 && stop != 1) {
        for (int i = 0; i < 256; i++) {
            MA[i][day] = store[i];
        }
    }
}
void file(vector<double> v, double** MA, double* p, int number, int start) {//read file and store stock price
    ifstream input;
    int begin = 0, count = 0, c = 0;
    int y, m, d;
    double store[256] = { 0.0 };
    double value;
    input.open("C:/Users/GD/CLionProjects/2018catchYY/GOOG.csv", ios::in);//read stock data
    string s;
    while (getline(input, s)) {
        string tmp, day;
        stringstream ss(s);
        int stop = 0;
        for (int j = 0; j < 2; j++) {
            getline(ss, tmp, ',');
            if (j == 0) {
                stringstream date(tmp);
                getline(date, day, '/');
                stringstream a;
                a << day;
                a >> y;
                getline(date, day, '/');
                stringstream b;
                b << day;
                b >> m;
                getline(date, day, '/');
                stringstream c;
                c << day;
                c >> d;
            }
            else {
                stringstream a;
                a << tmp;
                a >> value;
                v.push_back(value);
            }
        }
        if (start == count)//the first day of output data
            begin = count;
        else if (begin != 0) {
            if (begin + number == count) {//the last day of output data
                stop += 1;
                cout << count << endl;
            }
        }
        //EMA(v, begin, c, MA, store, count, stop);//EMA mode is here 
        if (begin != 0 && stop == 0) {
            RWMA(v, begin, c, MA);//SMA¡BWMA¡BRWMA mode are here 
            p[c] = v[begin + c];
            c++;
        }
        else if (stop == 1) {
            break;
        }
        count++;
    }
    input.close();
}
void update(int& gbest, int& worst, int money, double* beta, int** m, int num, int gmatrix[32], int wmatrix[32], int& temp, int four[4]) {//update the probability of matrix
    int count = 0;
    if (money > temp)
        temp = money;
    if (money > gbest) {
        gbest = money;
        for (int i = 0; i < 32; i++)
            gmatrix[i] = m[num][i];
    }
    else if (money <= worst) {
        worst = money;
        for (int i = 0; i < 32; i++)
            wmatrix[i] = m[num][i];
    }
    if (num == 299) {
        for (int i = 0; i < 32; i++) {
            if (gmatrix[i] != wmatrix[i] && gmatrix[i] == 0) {
                if (beta[i] + 0.0004 <= 1)//1.0
                    beta[i] += 0.0004;
            }
            else if (gmatrix[i] != wmatrix[i] && gmatrix[i] == 1) {
                if (beta[i] - 0.0004 >= 0)//0.0
                    beta[i] -= 0.0004;
            }
            for (int i = 0; i < 32; i++) {
                count += pow(2, (7 - i % 8)) * gmatrix[i];
                if (i % 8 == 7) {
                    four[i / 8] = count + 1;
                    count = 0;
                }
            }
        }
    }
}

int trade(int** strategy, double** MA, int num, int section, double* price, double* beta, int** m, int& gbest, int& worst){//phase1
    int origin = 1000000, money = 1000000, stock = 0;
    int remainder;
    int buy = 0, day = 0;
    double rate;
    for (int i = 0; i < section; i++) {
        if (buy == 0) {
            if (MA[strategy[num][0] - 1][i] <= MA[strategy[num][1] - 1][i] &&
                MA[strategy[num][0] - 1][i + 1] > MA[strategy[num][1] - 1][i + 1]) {
                buy += 1;
                stock = int(money / price[1 + i]);
                remainder = int(money - price[1 + i] * stock);
                day = i + 1;
            }
        }
        else {
            if (MA[strategy[num][2] - 1][i] >= MA[strategy[num][3] - 1][i] &&
                MA[strategy[num][2] - 1][i + 1] < MA[strategy[num][3] - 1][i + 1]) {
                buy -= 1;
                day = i + 1;
                money = int(price[1 + i] * stock + remainder);
            }
        }
        if (buy == 1 && i == section - 1) {
            money = int(price[1 + i] * stock + remainder);
        }
    }
    if (money != 1000000) {
        rate = (money - origin);
        return money;
    }
    else
        return origin;
}

int holdstock(int** strategy, double** MA, int num, int section, double* price, double* beta, int** m, int& gbest, int& worst) {//phase2
    int origin = 1000000, money = 0, stock = 0;
    int remainder;
    int buy = 1, day = 0;
    double rate;
    stock = int(origin / price[1]);
    remainder = int(origin - price[1] * stock);
    for (int i = 0; i < section; i++) {
        if (buy == 0) {
            if (MA[strategy[num][0] - 1][i] <= MA[strategy[num][1] - 1][i] &&
                MA[strategy[num][0] - 1][i + 1] > MA[strategy[num][1] - 1][i + 1]) {
                buy += 1;
                stock = int(money / price[1 + i]);
                remainder = int(money - price[1 + i] * stock);
                day = i + 1;
            }
        }
        else {
            if (MA[strategy[num][2] - 1][i] >= MA[strategy[num][3] - 1][i] &&
                MA[strategy[num][2] - 1][i + 1] < MA[strategy[num][3] - 1][i + 1]) {
                buy -= 1;
                day = i + 1;
                money = int(price[1 + i] * stock + remainder);
            }
        }
        if (buy == 1 && i == section - 1) {
            money = int(price[1 + i] * stock + remainder);
        }
    }
    if (money != 0) {
        rate = (money - origin);
        return money;
    }
    else
        return origin;
}
int main() {
    auto sta = chrono::steady_clock::now();
    int size = 32, gbest = 0, start;
    int gmatrix[32] = { 0 }, wmatrix[32] = { 0 };
    int money;
    int** period = NEW2D(3, mnumber, int);
    cut(period, start);
    int** strategy = NEW2D(300, 4, int);//one generation has 300 strategies by decimal
    int** m = NEW2D(300, 32, int);//one generation has 300 strategies by binary
    vector<double> price;
    double* beta;
    int starty = sety - 1;
    beta = new double[size];
    for (int o = 0; o < 13; o++) {//totally number of slidingwindow is 13
        string zone[13] = { "MM","QQ","HH","YY","QM","HM","YM","HQ","YQ","YH","M#","H#","Q#" };
        int sstart = start - 1;
        vector<int> date;
        date = slidingwindow(period, zone[o]);
        ofstream pha1("D:/2018GOOG/0.0004/2018GOOGRWMAphase130" + zone[o] + ".csv");
        ofstream pha2("D:/2018GOOG/0.0004/2018GOOGRWMAphase230" + zone[o] + ".csv");
        ofstream pha3("D:/2018GOOG/0.0004/2018GOOGRWMAcompare30" + zone[o] + ".csv");
        int four[4] = { 0 };
        cout << date.size() / 2 << endl;
        for (int l = 0; l < date.size() / 2; l++) {//date.size()/2
            double* p;
            p = new double[date[2 * l] + 1];
            double** MA = NEW2D(256, date[2 * l] + 1, double);
            file(price, MA, p, date[2 * l] + 1, sstart);
            for (int h = 0; h < 30; h++) {
                int g1, g2;
                int compare = 0, count = 0;
                gbest = 0;
                srand(h + 1);
                for (int i = 0; i < size; i++)//initial
                    beta[i] = 0.5;
                for (int j = 0; j < 10000; j++) {//number of generation
                    for (int i = 0; i < 300; i++) {//number of particle
                        measure(beta, size, strategy, i, m);
                    }
                    int best_tmp = 0;//the lowerbound of valu,you can regard as nagative max_int
                    int worst = 2000000;// the upperbound of value,you can regard as max_int
                    for (int i = 0; i < 300; i++) {//10000
                        money = trade(strategy, MA, i, date[2 * l], p, beta, m, gbest, worst);//hold money
                        update(gbest, worst, money, beta, m, i, gmatrix, wmatrix, best_tmp, four);
                    }
                    if (compare != best_tmp) {//Determine whether the result is the same as gbest
                        compare = best_tmp;//gbest
                        count = 0;
                    }
                    else {
                        count++;
                    }
                    if (count == 30) {//terminatal condition
                        int num = 0;
                        for (int i = 0; i < 32; i++) {
                            num += pow(2, (7 - i % 8)) * gmatrix[i];
                            if (i % 8 == 7) {
                                cout << num + 1 << " ";
                                four[i / 8] = num;
                                num = 0;
                            }
                        }
                        cout << endl;
                        cout << j << endl;
                        break;
                    }
                }
                pha1 << four[0] + 1 << "," << four[1] + 1 << "," << four[2] + 1 << "," << four[3] + 1 << ",";
                pha1 << gbest << endl;
                g1 = gbest;
                //---------------
                //here is phase 2
                srand(h + 1);//10
                gbest = 0;//holdstock
                count = 0;
                compare = 0;
                for (int i = 0; i < size; i++)//initial
                    beta[i] = 0.5;
                for (int j = 0; j < 10000; j++) {//number of generation
                    for (int i = 0; i < 300; i++) {//number of particle
                        measure(beta, size, strategy, i, m);
                    }
                    int best_tmp = 0;//the lowerbound of valu,you can regard as nagative max_int
                    int worst = 2000000;// the upperbound of value,you can regard as max_int
                    for (int i = 0; i < 300; i++) {
                        money = holdstock(strategy, MA, i, date[2 * l], p, beta, m, gbest, worst);//hold share
                        update(gbest, worst, money, beta, m, i, gmatrix, wmatrix, best_tmp, four);
                    }
                    if (compare != best_tmp) {//Determine whether the result is the same as gbest
                        compare = best_tmp;//gbest
                        count = 0;
                    }
                    else {
                        count++;
                    }
                    if (count == 30) {//terminatal condition
                        int num = 0;
                        for (int i = 0; i < 32; i++) {
                            num += pow(2, (7 - i % 8)) * gmatrix[i];
                            if (i % 8 == 7) {
                                cout << num + 1 << " ";
                                four[i / 8] = num;
                                num = 0;
                            }
                        }
                        cout << endl;
                        cout << j << endl;
                        break;
                    }
                }
                pha2 << four[0] + 1 << "," << four[1] + 1 << "," << four[2] + 1 << "," << four[3] + 1 << ",";
                pha2 << gbest << endl;
                g2 = gbest;
                if (g1 >= g2)
                    pha3 << g1 << endl;
                else
                    pha3 << g2 << endl;
            }
            sstart += date[2 * l + 1];
            pha1 << endl;
            pha2 << endl;
            pha3 << endl;
        }
        pha1.close();
        pha2.close();
        pha3.close();
    }
    auto end = chrono::steady_clock::now();
    cout << "time: " << chrono::duration<double>(end - sta).count() << "s" << endl;//record execution time
    system("pause");
    return 0;
}

