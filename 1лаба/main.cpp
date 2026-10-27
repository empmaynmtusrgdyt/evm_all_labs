#include <iostream>
#include <bitset>

#define S_LEFT 1
#define S_RIGHT 2

union Convert{
    double d;
    unsigned long long u;
};

using namespace std;

void func_for_long(){
    long input_num; // sizeof(long) = 64 bits
    cin >> input_num;
    unsigned long num_1 = input_num;
    cout << bitset<sizeof(num_1) * 8>(num_1) << endl;

    int side, shift_amount, group_size, high_bit;
    cin >> side >> shift_amount >> group_size >> high_bit;
    if(high_bit >= sizeof(num_1) * 8 || high_bit - group_size + 1 < 0 || shift_amount < 0 || shift_amount >= group_size || group_size <= 0){
        cout << "ERROR" << endl;
        return;
    }

    int low_bit = high_bit - group_size + 1;
    unsigned long mask = (1UL << group_size) - 1;
    unsigned long group = (num_1 >> low_bit) & mask;
    unsigned long new_group;

    if(side == S_LEFT){
        new_group = ((group << shift_amount) | (group >> (group_size - shift_amount))) & mask;
    }
    else if(side == S_RIGHT){
        new_group = ((group >> shift_amount) | (group << (group_size - shift_amount))) & mask;
    }
    else{
        cout << "ERROR" << endl;
        return;
    }

    num_1 = num_1 & ~(mask << low_bit);
    num_1 = num_1 | (new_group << low_bit);
    
    cout << bitset<sizeof(num_1) * 8>(num_1) << endl;
    cout << (long)num_1 << endl; 
}

void func_for_double(){
    union Convert input;
    cin >> input.d;
    cout << bitset<sizeof(input.u) * 8>(input.u) << endl;

    int side, shift_amount, group_size, high_bit;
    cin >> side >> shift_amount >> group_size >> high_bit;
    if(high_bit >= sizeof(input.u) * 8 || high_bit - group_size + 1 < 0 || shift_amount < 0 || shift_amount >= group_size || group_size <= 0){
        cout << "ERROR" << endl;
        return;
    }

    int low_bit = high_bit - group_size + 1;
    unsigned long long mask = (1ULL << group_size) - 1;
    unsigned long long group = (input.u >> low_bit) & mask;
    unsigned long long new_group;

    if(side == S_LEFT){
        new_group = ((group << shift_amount) | (group >> (group_size - shift_amount))) & mask;
    }
    else if(side == S_RIGHT){
        new_group = ((group >> shift_amount) | (group << (group_size - shift_amount))) & mask;
    }
    else{
        cout << "ERROR" << endl;
        return;
    }

    input.u = input.u & ~(mask << low_bit);
    input.u = input.u | (new_group << low_bit);

    cout << bitset<sizeof(input.u) * 8>(input.u) << endl;
    cout << input.d << endl; 
}

int main(){
    cout << "Enter '1' for long, or '2' for double." << endl;

    int menu = 0;
    cin >> menu;
    switch (menu){
    case 1:
        func_for_long();
        break;
    case 2:
        func_for_double();
        break;
    default:
        break;
    }

    return 0;
}