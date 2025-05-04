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
    double x0 = n * log(n);  // prev
    double x1 = x0; // curr
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
void reverse(bitset<32> &a , int j , int i)
{
    while(i < j)
    {
        bool bit_i = a[i];
        bool bit_j = a[j];
        a[j] = bit_i;
        a[i] = bit_j;
        i++;
        j--;
    }
}
bitset<32> RightShift(bitset<32> a , int k)
{
    return a >> k;
}
// bits are reprenset from n - 1 to 0 in bitset
bitset<32> CircularRightShift(bitset<32> a , int k)
{
    reverse(a ,  31 , k);
    reverse(a ,  k - 1 , 0);
    reverse(a ,  31 , 0);
    return a;
}
bitset<32> sigma0(bitset<32> a)
{
    return CircularRightShift(a , 7) ^ CircularRightShift(a , 18) ^ RightShift(a , 3);
}
bitset<32> sigma1(bitset<32> a)
{
    return CircularRightShift(a , 17) ^ CircularRightShift(a , 19) ^ RightShift(a , 10);
}
bitset<32> summation0(bitset<32> a)
{
    return CircularRightShift(a , 2) ^ CircularRightShift(a , 13) ^ CircularRightShift(a , 22);
}
bitset<32> summation1(bitset<32> a)
{
    return CircularRightShift(a , 6) ^ CircularRightShift(a , 11) ^ CircularRightShift(a , 25);
}
bitset<32> Choose(bitset<32> x , bitset<32> y , bitset<32> z)
{
    return (x & y) ^ (~x & z);
}
bitset<32> Majority(bitset<32> x , bitset<32> y , bitset<32> z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}
int main()
{
    string password = "";
    cout<< "Enter the password to Hash";
    cin>> password;

    int n = password.size();
    int arr[n];
    vector<bitset<8>> b1;

    //converting string to ascii value
    for(int i = 0; i < n; i++)
    {
        arr[i] = int(password[i]);
    }
    
    //padding the string
    for(int i = 0; i < n; i++)
    {
        b1.push_back(password[i]);
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
        ss << hex << showbase <<  num  ;
        hexvalues.push_back(ss.str());
    }
    // 2 phase starting padding
    b1.push_back(128);
    size_t original_bit_length = (b1.size() * 8) - 8;
    size_t target_padding_length = ((original_bit_length + 8 + 64 + 511) / 512) * 512;
    size_t zero_padding_bits = (target_padding_length - original_bit_length - 8 - 64);
    size_t zero_padding_bytes = zero_padding_bits/8;
    
    for(int i = 0; i < zero_padding_bytes + 7; i++){
        b1.push_back(0);
    }
    b1.push_back(original_bit_length);

    // for(int i = 0; i < b.size(); i++){
    //     cout<< b[i] << " ";
    //     if((i + 1) % 4 == 0) cout<< endl;
    // }

    //3 phase block decomposition
    vector<bitset<32>> block;
    // 16 blocks are divided into 32 bits from b and are inserted to block 
    for(size_t i = 0; i < b1.size(); i += 4){
        bitset<32> combined  = (bitset<32>(b1[i].to_ulong())  << 24) |
                               (bitset<32>(b1[i + 1].to_ulong())  << 16) |
                               (bitset<32>(b1[i + 2].to_ulong())  << 8) |
                                bitset<32>(b1[i + 3].to_ulong());
        block.push_back(combined);
    }
    //next 48 blocks
    for(int i = 16; i < 64; i++){
        bitset<32> combined = (block[i - 7].to_ulong()) + 
                              (block[i - 16].to_ulong()) + 
                              (sigma0(block[i - 15]).to_ulong()) +
                              (sigma1(block[i - 2]).to_ulong());
        block.push_back(combined);                    
    } 
    // 4 phase  hash computation;
    vector<int> prime8 = {2 , 3 , 5 , 7 , 11 , 13 , 17 , 19};
    vector<double> hexprime8;
    vector<string> hexvalues8;

    for(int i = 0; i < prime8.size(); i++){
        hexprime8.push_back(sqrt(prime8[i]));
    }

    for(int i = 0; i < hexprime8.size(); i++){
        hexprime8[i] = hexprime8[i] - int(hexprime8[i]);
    }

    for(int i = 0; i < hexprime8.size(); i++){
        unsigned int num  = static_cast<unsigned int>(hexprime8[i] * pow(2 , 32));
        stringstream ss;

        ss <<  hex << showbase << num;
        hexvalues8.push_back(ss.str());
    }

    //assing abcdefg values
    vector<bitset<32>> abcdefg;
    for(int i = 0; i < 8; i++)
    {
        unsigned int  value;
        stringstream convert;
        convert << hex << hexvalues8[i];
        convert >> value;

        abcdefg.push_back(value);
    }

    bitset<32> a = abcdefg[0];
    bitset<32> b = abcdefg[1];
    bitset<32> c = abcdefg[2];
    bitset<32> d = abcdefg[3];
    bitset<32> e = abcdefg[4];
    bitset<32> f = abcdefg[5];
    bitset<32> g = abcdefg[6];
    bitset<32> h = abcdefg[7];

    bitset<32> H1 = abcdefg[0]; 
    bitset<32> H2 = abcdefg[1]; 
    bitset<32> H3 = abcdefg[2]; 
    bitset<32> H4 = abcdefg[3]; 
    bitset<32> H5 = abcdefg[4]; 
    bitset<32> H6 = abcdefg[5]; 
    bitset<32> H7 = abcdefg[6]; 
    bitset<32> H8 = abcdefg[7]; 

    //  do 64 rounds  operations
    bitset<32> T1 , T2;

    for(int i = 0; i < 64; i++)
    {
        unsigned int  value1 , value2;
        stringstream convert1 , convert2;

        convert1 << hex << hexvalues[i];
        convert2 << hex << block[i];

        convert1 >> value1;
        convert2 >> value2;

        bitset<32> k = value1;
        bitset<32> w = value2;

        T1 = bitset<32>(
            h.to_ulong() +
            summation1(e).to_ulong() +
            Choose(e, f, g).to_ulong() +
            k.to_ulong() +
            w.to_ulong()
        );
        T2 = bitset<32>(
            summation0(a).to_ulong() +
            Majority(a, b, c).to_ulong()
        );
        h = g;
        g = f;
        f = e;
        e = bitset<32>(
            d.to_ulong() +
            T1.to_ulong()
        );
        d = c;
        c = b;
        b = a;
        a = bitset<32>(
            T1.to_ulong() +
            T2.to_ulong()
        );
    }
    H1 = bitset<32>(
        H1.to_ulong() +
        a.to_ulong()
    );
    H2 = bitset<32>(
        H2.to_ulong() +
        b.to_ulong()
    );
    H3 = bitset<32>(
        H3.to_ulong() +
        c.to_ulong()
    );
    H4 = bitset<32>(
        H4.to_ulong() +
        d.to_ulong()
    );
    H5 = bitset<32>(
        H5.to_ulong() +
        e.to_ulong()
    );
    H6 = bitset<32>(
        H6.to_ulong() +
        f.to_ulong()
    );
    H7 = bitset<32>(
        H7.to_ulong() +
        g.to_ulong()
    );
    H8 = bitset<32>(
        H8.to_ulong() +
        h.to_ulong()
    );
    cout << "Final Hash: ";
    cout << hex << H1.to_ulong()
        << H2.to_ulong()
        << H3.to_ulong()
        << H4.to_ulong()
        << H5.to_ulong()
        << H6.to_ulong()
        << H7.to_ulong()
        << H8.to_ulong() << endl;

}