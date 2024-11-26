/* 
    Creator: TORRYANA_TANIS
    Project:Temperature sensor

*/

#include <iostream>

using namespace std;

int main ()
{
    //<declaring varaibles
    int adc = 0;
    const float tmax = 150;
    const float tmin = -50;
    const float adc_max = 4095;
    const float adc_min = 0;
    //<intializing variables for formula
    float t_sub;
    float t_divide;
    float t_mult;
    float t_result;

    cout << "Enter ADC reading: "; //<prompts user to enter adc
    cin>> adc;                     //< takes adc from user

    
        //<ERROR messages for number over 4095
        if(adc>adc_max)
        {
            cout<<"ERROR: Invalid Sensor Value."; //<outputs error meassge on terminal
            return 0;
        }
        //<ERROR message for number below 0
        if (adc<adc_min)
        {
            cout<<"ERROR: Invalid Sensor Value."; //<outputs error message on terminal
            return 0;
        }
    
    //<formula to find temp
    t_sub= tmax - tmin;
    t_divide = t_sub / adc_max;
    t_mult = adc * t_divide;
    t_result = t_mult + tmin;

    cout<<"Temperature : "<<  t_result<<"°C"<<endl; //<outputs what the temperature should be in ˚c
}
