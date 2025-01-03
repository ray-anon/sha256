#include<bits/stdc++.h>
using namespace std;
void deleting_extra_calculated_prime(vector<int> &prime64)
{
    while(prime64.size() > 64)
    {
        prime64.pop_back();
    }

}
//estimating the range for finding first 64 primes
int estimate_range(int n)
{
    double x0 = n * log(n);
    double x1 = x0;
    while(true)
    {
        x1 = n * log(x0);
        if(x1 == x0)
            break;
        x0 = x1;
    }
    return int(x1);
}

vector<int>  find_first_64_prime()
{
    vector<int> v;
    int n = estimate_range(64);

    bool prime[n + 1];
    memset(prime , true , sizeof(prime));

    for(int i = 2; i < n + 1; i++)
    {
        if(prime[i])
        {
            for(int j = i + i; j < n + 1; j+=i)
            {
                prime[j] = false;
            }
        }
    }
    
    for(int i = 2; i < n + 1; i++)
    {
        if(prime[i])
            v.push_back(i);
    }

    return v;
}
int main()
{
    string password = "";
    cout<< "Enter the password to Hash";
    cin>> password;

    int n = password.size();
    int arr[n];

    //converting string to ascii value
    for(int i = 0; i < n; i++)
    {
        arr[i] = int(password[i]);
    }

    //finding first 64 prime
    vector<int> prime64 = find_first_64_prime();

    //deleting extra calculated primes in the estimated range
    deleting_extra_calculated_prime(prime64);


    vector<double> hexprime64;
    vector<string> hexvalues;
    //finding cuberoot of first 64 primes
    for(int i = 0; i < prime64.size(); i++)
    {
        hexprime64.push_back(cbrt(prime64[i]));
    }
    
    //deleting the integer part and only having the fractional part
    for(int i = 0; i < hexprime64.size(); i++)
    {
        hexprime64[i] = hexprime64[i] - int(hexprime64[i]);
    }
    
    // storing hex values and using stringstreams and unsigned int to convert decimal values like 1.1191e+9 to normal integer 119100000
    for(int i = 0; i < hexprime64.size(); i++)
    {
        unsigned int num =  static_cast<unsigned int>(hexprime64[i] * pow(2 , 32));
        stringstream ss ;
        ss << hex << num;
        hexvalues.push_back("0x" + ss.str());
    }
    
}