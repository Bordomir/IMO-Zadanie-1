#include "include/DataLoader.hpp"

#include <filesystem>
#include <string>
#include <fstream>
#include <ranges>
#include <string_view>
#include <charconv>
#include <stdexcept>

using namespace std;

DataLoader::DataLoader(const string &filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        throw runtime_error("Nie można otworzyć pliku z danymi: " + filename);
    }

    auto size = filesystem::file_size(filename); 

    string line;
    vector<array<int, 2>> nodePositions;
    while (getline(file, line))
    {
        if (line.empty()) continue;
        
        auto [x, y, profit] = line 
            | views::split(';') 
            | views::transform([](auto &&part){ return string_view(part); }) 
            | views::transform([](string_view part)
            {
                int val = 0;
                
                auto [ptr, ec] = from_chars(part.data(), part.data() + part.size(), val);
                
                if (ec == errc::result_out_of_range) {
                    throw runtime_error("Zbyt duża liczba w danych");
                } else if (ec == errc::invalid_argument) {
                    throw runtime_error("Pole nie będące liczbą w danych");
                }
                if (ec != errc{}) {
                    throw runtime_error("Nieznany błąd w danych");
                }
                return val; 
            }) 
            | ranges::to<array<int, 3>>();
        
        nodePositions.push_back({x, y});
        nodeProfits.push_back(profit);
    }
}