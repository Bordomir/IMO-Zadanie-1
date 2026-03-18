#include "include/DataLoader.hpp"

#include <filesystem>
#include <string>
#include <fstream>
#include <ranges>
#include <string_view>
#include <charconv>
#include <print>

using namespace std;

DataLoader::DataLoader(const string &filename)
{
    // ios::binary pozostawia \r\n które są uzwględniane w rozmiarze pliku
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        throw runtime_error("Nie można otworzyć pliku z danymi: " + filename);
    }

    // pobranie rozmiaru pliku z systemu
    auto size = filesystem::file_size(filename); 
    
    // wczytanie całego pliku do stringa
    string data(size, '\0');
    file.read(data.data(), size);

    // parsowanie danych z użyciem ranges
    auto parsedData = data 
        | views::split('\n') 
        | views::transform([](auto &&line){ return string_view(line); }) 
        | views::filter([](string_view line){ return !(line.empty() || line == "\r"); }) 
        | views::transform([](string_view line)
        {
            // parsowanie linii na liczby
            auto res = line 
                | views::split(';') 
                | views::transform([](auto &&part){ return string_view(part); }) 
                | views::transform([](string_view part)
                {
                    // parsowanie string_view na int
                    int val = 0;
                    // from_chars zignoruje nie usunięte wcześniej możliwie występujące znaki \r na końcu
                    from_chars(part.data(), part.data() + part.size(), val);
                    return val; 
                }) 
                | ranges::to<vector<int>>();
            println("{}", res);
            return res;
        }) 
        | ranges::to<vector<vector<int>>>();
}