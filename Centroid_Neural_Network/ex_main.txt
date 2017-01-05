#include "my_data.hpp"
#include "my_Centroid_Neural_Network.hpp"


int main(int argc, char** argv)
{
    MY_CNN a;
/*
    a.init(4);
    a.read_input("input_data/lena.txt");
    a.learning();
    a.write_weight("weight.txt");
    a.write_setting("setting.txt");
 //   a.write_result("result.txt");
    a.status();
    */
    a.read_input("input_data/camera.txt");
    a.read_weight("weight.txt");
    a.testing();
    a.write_result("result.txt");
    return 0;
}
